#include "ros.h"
#include "roslib/std_msgs/String.h"
#include "ros_handler.h"

static void ros_message_received_callback(const std_msgs::String &msg);

static ros::NodeHandle kNodeHandle;
static std_msgs::String kMessage;
static ros::Publisher kPublisher("/rtems/state", &kMessage);
static ros::Subscriber<std_msgs::String> kSubscriber("/rtems/cmd", ros_message_received_callback);

static int kClientSocketFD = -1;

static void ros_message_received_callback(const std_msgs::String &msg) {
    printf("[ros_message_received_callback]");
}

void ros_init() {
    printf("[ros_init]");

    kNodeHandle.subscribe(kSubscriber);
    kNodeHandle.advertise(kPublisher);
}

void ros_register_socket_fd(int socket_fd) {
    kClientSocketFD = socket_fd;
}

int ros_get_socket_fd() {
    return kClientSocketFD;
}

void ros_spin_once() {
    printf("[ros_spin_once]");

    kNodeHandle.spinOnce();

    kMessage.data = "RTEMS publishing message";
    kPublisher.publish(&kMessage);
}