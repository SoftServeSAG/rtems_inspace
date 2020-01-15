#include <rtems/inttypes.h>
#include <rtems/rtems_bsdnet.h>
#include <rtems/error.h>
#include <rtems/rtems_dhcp_failsafe.h>

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
        .ifconfig = &netdriver_config, /* fill if DHCP is used*/
};