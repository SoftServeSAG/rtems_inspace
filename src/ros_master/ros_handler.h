#ifndef _ROS_HANDLER_H_
#define _ROS_HANDLER_H_

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void ros_init();
EXTERNC void ros_register_socket_fd(int socket_id);
EXTERNC int ros_get_socket_fd();
EXTERNC void ros_spin_once();

#endif // _ROS_HANDLER_H_