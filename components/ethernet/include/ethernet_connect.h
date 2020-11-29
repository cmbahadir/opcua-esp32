/* Common functions for protocol examples, to establish Wi-Fi or Ethernet connection.
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"
#include "esp_netif.h"

#define CONFIG_ESP32_SPIRAM_SUPPORT 1
#define CONFIG_MBEDTLS_EXTERNAL_MEM_ALLOC 1
#define CONFIG_EXAMPLE_CONNECT_ETHERNET 1
#define CONFIG_EXAMPLE_USE_INTERNAL_ETHERNET 1
#define CONFIG_EXAMPLE_ETH_PHY_LAN8720 1
#define CONFIG_EXAMPLE_ETH_MDC_GPIO 23
#define CONFIG_EXAMPLE_ETH_MDIO_GPIO 18
#define CONFIG_EXAMPLE_ETH_PHY_ADDR 0

#define CONFIG_USE_STATIC_IP 1

#ifdef CONFIG_USE_STATIC_IP
#define CONFIG_ETHERNET_STATIC_IP4_ADDRESS "192.168.74.253"
#define CONFIG_ETHERNET_STATIC_IP4_GATEWAY "192.168.74.1"
#define CONFIG_ETHERNET_STATIC_IP4_NETMASK "255.255.255.0"
#endif //CONFIG_USE_STATIC_IP

#ifdef CONFIG_EXAMPLE_CONNECT_ETHERNET
// #define EXAMPLE_INTERFACE get_example_netif()
#define EXAMPLE_INTERFACE TCPIP_ADAPTER_IF_ETH
#endif


esp_err_t ethernet_connect(void);
esp_err_t ethernet_disconnect(void);
esp_netif_t *get_example_netif(void);

#ifdef __cplusplus
}
#endif