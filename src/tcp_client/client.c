#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#include <rtems/inttypes.h>
#include <rtems/rtems_bsdnet.h>

const char *kServerIP = "10.0.0.42";
const int16_t kServerPort = 11311;

void spawn_task(rtems_task_entry entryPoint, rtems_task_priority priority, rtems_task_argument arg) {
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

rtems_task client_task(rtems_task_argument arg) {
    int socket_fd, accepted_socket_fd;
    socklen_t accepted_socket_addr_len;
	struct sockaddr_in server_addr, my_addr;
    fd_set fdset;
    struct timeval tv;

	printf("[client_task] Create socket\n");
	socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fd < 0)
		rtems_panic("[client_task] Can't create socket: %s\n", strerror(errno));

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    // my_addr.sin_port = htons(0);
    // my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = kServerPort;
    inet_aton(kServerIP, &my_addr.sin_addr);

    if (bind(socket_fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0)
        rtems_panic("[client_task] Can't bind socket: %s\n", strerror(errno));
        
    printf("[client_task] Client address: %s\n", inet_ntoa(my_addr.sin_addr));

    memset(&server_addr, 0, sizeof server_addr);
	server_addr.sin_len = sizeof(server_addr);
    server_addr.sin_family = AF_INET;
    
    // 10.0.0.1:11411
    server_addr.sin_port = htons(11411);
    inet_aton("10.0.0.1", &server_addr.sin_addr);

    // 10.0.2.100:1234
    // server_addr.sin_port = htons(1234);
    // inet_aton("10.0.2.100", &server_addr.sin_addr);
    

    // long fd_flags; 
    // if( (fd_flags = fcntl(socket_fd, F_GETFL, NULL)) < 0)
    //     rtems_panic("[client_task] Error fcntl(..., F_GETFL): %s\n", strerror(errno));

    // fd_flags |= SOCK_NONBLOCK; 
    // if( fcntl(socket_fd, F_SETFL, fd_flags) < 0)
    //     rtems_panic("[client_task] Error fcntl(..., F_SETFL): %s\n", strerror(errno));

    // fcntl(socket_fd, F_SETFL, O_NONBLOCK);

    // struct timeval timeout;
    // timeout.tv_sec  = 7;  // after 7 seconds connect() will timeout
    // timeout.tv_usec = 0;
    // setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    printf("[client_task] Connecting to server: %s\n", inet_ntoa(server_addr.sin_addr));

    if (connect(socket_fd, &server_addr, sizeof(server_addr)) != 0)
        rtems_panic("[client_task] Can't connect to server: %s\n", strerror(errno));
    
    printf("[client_task] Connected to server\n");
}

