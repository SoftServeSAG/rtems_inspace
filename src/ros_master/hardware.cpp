#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "hardware.h"
#include "ros_handler.h"

static int read_func(int socket_fd, char *ptr, size_t bytes) {
    return read(socket_fd, ptr, bytes);
}

static int write_func(int socket_fd, char *ptr, size_t bytes) {
    return write(socket_fd, ptr, bytes);
}

Stm32Hardware::Stm32Hardware() {
    printf("[Stm32Hardware::Stm32Hardware]\n");
}

void Stm32Hardware::init(void) {
    printf("[Stm32Hardware::init]\n");
}

int Stm32Hardware::read(void) {
    printf("[Stm32Hardware::read]: ");

    int socket_fd = ros_get_socket_fd();
    char msg;

    int num_bytes_read = read_func(socket_fd, &msg, 1);
    
    printf("%c\n", msg);

    if (num_bytes_read < 0) {
        rtems_panic("[Stm32Hardware:read] Worker couldn't read message from client: %s\n", strerror(errno));
        return 0;
    }
    if (num_bytes_read == 0) {
        rtems_panic("[Stm32Hardware:read] Zero bytes received: %s\n", strerror(errno));
        return 0;
    }
    
    return msg;
}

void Stm32Hardware::write(unsigned char* data, int length) {
    printf("[Stm32Hardware::write]\n");
    int socket_fd = ros_get_socket_fd();
    int num_bytes_writen = write_func(socket_fd, (char *)data, length);
    if (num_bytes_writen < 0)
        rtems_panic("[Stm32Hardware::write] Server couldn't write message to client: %s\n", strerror(errno));
}

unsigned long Stm32Hardware::time(void) {
    printf("[Stm32Hardware::time]: ");
    auto time = rtems_clock_get_uptime_seconds();
    printf("%d\n", time);
    return time;
}
