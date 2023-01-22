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
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/ip_addr.h"
#include <lwip/sockets.h>
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_netif.h"

#define CONFIG_ESP32_SPIRAM_SUPPORT 1
#define CONFIG_MBEDTLS_EXTERNAL_MEM_ALLOC 1

#ifdef CONFIG_EXAMPLE_CONNECT_ETHERNET
#include "esp_eth.h"
#define CONFIG_EXAMPLE_USE_INTERNAL_ETHERNET 1
#define CONFIG_EXAMPLE_ETH_PHY_LAN8720 1
#define CONFIG_EXAMPLE_ETH_MDC_GPIO 23
#define CONFIG_EXAMPLE_ETH_MDIO_GPIO 18
#define CONFIG_EXAMPLE_ETH_PHY_ADDR 0
#define EXAMPLE_INTERFACE TCPIP_ADAPTER_IF_ETH
#define BASE_IP_EVENT ETH_EVENT
#define GOT_IP_EVENT IP_EVENT_ETH_GOT_IP
#define DISCONNECT_EVENT ETHERNET_EVENT_DISCONNECTED
#endif

#ifdef CONFIG_EXAMPLE_CONNECT_WIFI
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#define BASE_IP_EVENT WIFI_EVENT
#define GOT_IP_EVENT IP_EVENT_STA_GOT_IP
#define DISCONNECT_EVENT WIFI_EVENT_STA_DISCONNECTED
#define EXAMPLE_INTERFACE TCPIP_ADAPTER_IF_STA
#endif

esp_err_t example_connect(void);
esp_err_t example_disconnect(void);
esp_netif_t *get_example_netif(void);
esp_err_t set_dns_server(esp_netif_t *netif, uint32_t addr, esp_netif_dns_type_t type);

#ifdef __cplusplus
}
#endif
