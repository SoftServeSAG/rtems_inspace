#include <rtems/inttypes.h>
#include <rtems/rtems_bsdnet.h>
#include <rtems/error.h>
#include <rtems/rtems_dhcp_failsafe.h>
#include <bsp.h>

static char ethernet_address[6] = {0x00, 0x04, 0x9F, 0x00, 0x27, 0x50 };

static struct rtems_bsdnet_ifconfig netdriver_config = {
        BSP_NE2000_NETWORK_DRIVER_NAME,
        BSP_NE2000_NETWORK_DRIVER_ATTACH,
        NULL,
        .ip_address = "10.0.0.42",
        .ip_netmask = "255.255.255.0",
        .hardware_address = ethernet_address
};

struct rtems_bsdnet_config rtems_bsdnet_config = {
        .ifconfig = &netdriver_config,
};