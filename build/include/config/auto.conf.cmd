deps_config := \
	/home/cmb/esp/esp-idf/components/app_trace/Kconfig \
	/home/cmb/esp/esp-idf/components/aws_iot/Kconfig \
	/home/cmb/esp/esp-idf/components/bt/Kconfig \
	/home/cmb/esp/esp-idf/components/driver/Kconfig \
	/home/cmb/esp/esp-idf/components/esp32/Kconfig \
	/home/cmb/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/cmb/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/cmb/esp/esp-idf/components/ethernet/Kconfig \
	/home/cmb/esp/esp-idf/components/fatfs/Kconfig \
	/home/cmb/esp/esp-idf/examples/open62541/components/freertos/Kconfig \
	/home/cmb/esp/esp-idf/components/heap/Kconfig \
	/home/cmb/esp/esp-idf/components/libsodium/Kconfig \
	/home/cmb/esp/esp-idf/components/log/Kconfig \
	/home/cmb/esp/esp-idf/examples/open62541/components/lwip/Kconfig \
	/home/cmb/esp/esp-idf/examples/open62541/components/mbedtls/Kconfig \
	/home/cmb/esp/esp-idf/components/mdns/Kconfig \
	/home/cmb/esp/esp-idf/components/openssl/Kconfig \
	/home/cmb/esp/esp-idf/components/pthread/Kconfig \
	/home/cmb/esp/esp-idf/components/spi_flash/Kconfig \
	/home/cmb/esp/esp-idf/components/spiffs/Kconfig \
	/home/cmb/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/cmb/esp/esp-idf/components/vfs/Kconfig \
	/home/cmb/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/cmb/esp/esp-idf/Kconfig.compiler \
	/home/cmb/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/cmb/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/cmb/esp/esp-idf/examples/open62541/main/Kconfig.projbuild \
	/home/cmb/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/cmb/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
