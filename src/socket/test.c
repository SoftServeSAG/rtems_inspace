#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXECUTIVE_RAM_SIZE        (512*1024)
#define CONFIGURE_MAXIMUM_SEMAPHORES        20
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES    20
#define CONFIGURE_MAXIMUM_TASKS            20

#define CONFIGURE_MICROSECONDS_PER_TICK    10000
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 50

#define CONFIGURE_INIT_TASK_STACK_SIZE    (10*1024)
#define CONFIGURE_INIT_TASK_PRIORITY    50
#define CONFIGURE_INIT_TASK_INITIAL_MODES (RTEMS_PREEMPT | \
                                           RTEMS_NO_TIMESLICE | \
                                           RTEMS_NO_ASR | \
                                           RTEMS_INTERRUPT_LEVEL(0))
#define CONFIGURE_INIT_TASK_ATTRIBUTES RTEMS_FLOATING_POINT

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
/****************  END OF CONFIGURATION INFORMATION  ****************/

#include <rtems/inttypes.h>
#include <rtems/rtems_bsdnet.h>
#include <rtems/error.h>
#include <rtems/rtems_dhcp_failsafe.h>

const char *kServerIP = "10.0.0.42";
const int16_t kServerPort = 11311;

int rtems_fxp_attach(struct rtems_bsdnet_ifconfig *config, int attaching);

static char ethernet_address[6] = {0x00, 0x04, 0x9F, 0x00, 0x27, 0x50 };

static struct rtems_bsdnet_ifconfig netdriver_config = {
        .name = "fxp1" /*RTEMS_BSP_NETWORK_DRIVER_NAME*/,
        .attach = rtems_fxp_attach /*RTEMS_BSP_NETWORK_DRIVER_ATTACH*/,
        .next = NULL,
        .ip_address = "10.0.0.42",
        .ip_netmask = "255.255.255.0",
        .hardware_address = ethernet_address
        /* more options can follow */
};

struct rtems_bsdnet_config rtems_bsdnet_config = {
        .ifconfig = &netdriver_config,
        .bootp = rtems_bsdnet_do_dhcp_failsafe, /* fill if DHCP is used*/
};

static void spawn_task(rtems_task_entry entryPoint, rtems_task_priority priority, rtems_task_argument arg) {
    rtems_status_code sc;
    rtems_id tid;

    sc = rtems_task_create(rtems_build_name('t','a','s','k'),
            priority,
            RTEMS_MINIMUM_STACK_SIZE+(8*1024),
            RTEMS_PREEMPT|RTEMS_TIMESLICE|RTEMS_NO_ASR|RTEMS_INTERRUPT_LEVEL(0),
            RTEMS_FLOATING_POINT|RTEMS_LOCAL,
            &tid);
    if (sc != RTEMS_SUCCESSFUL)
        rtems_panic("Can't create task: %s", rtems_status_text(sc));
    sc = rtems_task_start(tid, entryPoint, arg);
    if (sc != RTEMS_SUCCESSFUL)
        rtems_panic("Can't start task: %s", rtems_status_text(sc));
}

static rtems_task worker_task(rtems_task_argument arg) {
    int socket_fd = arg;
    char msg[80];
    char reply[120];
    int num_bytes_read, num_bytes_writen;
    int num_bytes_formatted;

    printf("[worker_task] Begin\n");

    for (;;) {
        printf("[worker_task] Read\n");
        num_bytes_read = read(socket_fd, msg, sizeof msg);
        if (num_bytes_read < 0) {
            printf("[worker_task] Worker couldn't read message from client: %s\n", strerror(errno));
            break;
        }
        if (num_bytes_read == 0) {
            printf("[worker_task] Received zero bytes\n");
            break;
        }

        msg[num_bytes_read]='\0';
        num_bytes_formatted = snprintf(reply, sizeof(reply), "Server received %d bytes: %s", num_bytes_read, msg);
        
        printf("[worker_task] Write\n");
        num_bytes_writen = write(socket_fd, reply, num_bytes_formatted);
        if (num_bytes_writen < 0) {
            printf("[worker_task] Server couldn't write message to client: %s\n", strerror(errno));
            break;
        }
    }
    if (close(socket_fd) < 0)
        printf("[worker_task] Can't close worker task socket: %s\n", strerror(errno));

    printf("[worker_task] End\n");
    rtems_task_exit();
}

static rtems_task server_task(rtems_task_argument arg) {
    int socket_fd, accepted_socket_fd;
    socklen_t accepted_socket_addr_len;
	struct sockaddr_in server_addr, accepted_socket_addr;
    rtems_task_priority server_task_priority;

    printf("[server_task] Begin\n");
    rtems_task_set_priority(RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &server_task_priority);

	printf("[server_task] Create socket\n");
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
		rtems_panic("[server_task] Can't create socket: %s\n", strerror(errno));

    memset(&server_addr, 0, sizeof server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(kServerPort);
    inet_aton(kServerIP, &server_addr.sin_addr);

	printf("[server_task] Bind socket\n");
	if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof server_addr) < 0)
		rtems_panic("[server_task] Can't bind socket: %s\n", strerror(errno));

    if (listen(socket_fd, 5) < 0)
		printf("[server_task] Can't listen on socket: %s\n", strerror(errno));

    printf("[server_task] Server address: [ %s ]\n", inet_ntoa(server_addr.sin_addr));

    for(;;) {
        accepted_socket_addr_len = sizeof accepted_socket_addr;
        accepted_socket_fd = accept(socket_fd, (struct sockaddr *)&accepted_socket_addr, &accepted_socket_addr_len);
        if (accepted_socket_fd < 0)
            if (errno == ENXIO)
                rtems_task_exit();
            else
                rtems_panic("[server_task] Can't accept connection: %s", strerror(errno));
        else
            printf("[server_task] Accepted: [ %s ]\n", inet_ntoa(accepted_socket_addr.sin_addr));
        spawn_task(worker_task, server_task_priority, accepted_socket_fd);
    }

    printf("[server_task] End\n");
    rtems_task_exit();
}

rtems_task Init (rtems_task_argument ignored) { 
    if (rtems_bsdnet_initialize_network() != 0)
        rtems_panic("Failed to init network: %s\n", strerror(errno));

    rtems_bsdnet_show_inet_routes ();

    spawn_task(server_task, 150, 0);

    (void) rtems_task_delete( RTEMS_SELF );
}