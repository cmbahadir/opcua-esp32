idf_component_register(SRCS "ethernet_connect.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "driver" "esp_wifi" "esp_eth" "lwip" "bootloader_support" "spi_flash" "nvs_flash" "esp_netif")