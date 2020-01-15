/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "ros.h"
#include "std_msgs/String.h"

static void ros_message_received_callback(const std_msgs::String &msg);

static ros::NodeHandle kNodeHandle;
static std_msgs::String kMessage;
static ros::Publisher kPublisher("/rtems/state", &kMessage);
static ros::Subscriber<std_msgs::String> kSubscriber("/rtems/cmd", ros_message_received_callback);

static void ros_message_received_callback(const std_msgs::String &msg) {
}

static void ros_init() {
    kNodeHandle.subscribe(kSubscriber);
    kNodeHandle.advertise(kPublisher);
}

static void ros_update() {
    printf("[ros_update]");

    kNodeHandle.spinOnce();

    kMessage.data = "RTEMS publishing message";
    kPublisher.publish(&kMessage);
}

extern "C" {

#include <bsp.h>
#include "../testmacros.h"

rtems_id   Task_id[ 4 ];         /* array of task ids */
rtems_name Task_name[ 4 ];       /* array of task names */

rtems_task Test_task( rtems_task_argument task_index ) {
    rtems_time_of_day time;
    rtems_interval    ticks;

    ticks = task_index * 5 * rtems_clock_get_ticks_per_second();

    for ( ; ; ) {
        (void) rtems_clock_get_tod( &time );
        if ( time.second >= 35 ) {
            puts( "*** END OF CLOCK TICK TEST ***" );
            exit( 0 );
        }
        ros_update();
        put_name( Task_name[ task_index ], FALSE );
        (void) rtems_task_wake_after( ticks );
        print_time( " - rtems_clock_get_tod - ", &time, "\n" );
    }
}

rtems_task Init(rtems_task_argument argument) {
    printf("[Init]");

    ros_init();
    
    rtems_time_of_day time;

    time.year   = 1988;
    time.month  = 12;
    time.day    = 31;
    time.hour   = 9;
    time.minute = 0;
    time.second = 0;
    time.ticks  = 0;

    (void) rtems_clock_set( &time );

    Task_name[ 1 ] = rtems_build_name( 'T', 'A', '1', ' ' );
    (void) rtems_task_create(
            Task_name[ 1 ], 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
            RTEMS_DEFAULT_ATTRIBUTES, &Task_id[ 1 ] );
    (void) rtems_task_start( Task_id[ 1 ], Test_task, 1 );
    (void) rtems_task_delete( RTEMS_SELF );
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             4

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_INIT_TASK_STACK_SIZE    (2 * RTEMS_MINIMUM_STACK_SIZE)

#define CONFIGURE_EXTRA_TASK_STACKS       (4 * RTEMS_MINIMUM_STACK_SIZE)

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/

}