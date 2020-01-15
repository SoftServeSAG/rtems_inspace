#include <errno.h>

// #include "network_config.h"
#include "network_config_simple.h"

void spawn_task(rtems_task_entry entryPoint, rtems_task_priority priority, rtems_task_argument arg);
rtems_task client_task(rtems_task_argument arg);

rtems_task Init (rtems_task_argument ignored) { 
    if (rtems_bsdnet_initialize_network() != 0)
        rtems_panic("Failed to init network: %s\n", strerror(errno));

    rtems_bsdnet_show_inet_routes ();

    spawn_task(client_task, 150, 0);

    (void) rtems_task_delete( RTEMS_SELF );
}

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