deps_config := \
	/usr/local/src/esp/esp-idf/components/app_trace/Kconfig \
	/usr/local/src/esp/esp-idf/components/aws_iot/Kconfig \
	/usr/local/src/esp/esp-idf/components/bt/Kconfig \
	/usr/local/src/esp/esp-idf/components/driver/Kconfig \
	/usr/local/src/esp/esp-idf/components/esp32/Kconfig \
	/usr/local/src/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/usr/local/src/esp/esp-idf/components/esp_http_client/Kconfig \
	/usr/local/src/esp/esp-idf/components/ethernet/Kconfig \
	/usr/local/src/esp/esp-idf/components/fatfs/Kconfig \
	/usr/local/src/esp/esp-idf/components/freertos/Kconfig \
	/usr/local/src/esp/esp-idf/components/heap/Kconfig \
	/usr/local/src/esp/esp-idf/components/libsodium/Kconfig \
	/usr/local/src/esp/esp-idf/components/log/Kconfig \
	/usr/local/src/esp/esp-idf/components/lwip/Kconfig \
	/usr/local/src/esp/esp-idf/components/mbedtls/Kconfig \
	/usr/local/src/esp/esp-idf/components/mdns/Kconfig \
	/usr/local/src/esp/esp-idf/components/openssl/Kconfig \
	/usr/local/src/esp/esp-idf/components/pthread/Kconfig \
	/usr/local/src/esp/esp-idf/components/spi_flash/Kconfig \
	/usr/local/src/esp/esp-idf/components/spiffs/Kconfig \
	/usr/local/src/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/usr/local/src/esp/esp-idf/components/vfs/Kconfig \
	/usr/local/src/esp/esp-idf/components/wear_levelling/Kconfig \
	/usr/local/src/esp/esp-idf/Kconfig.compiler \
	/usr/local/src/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/usr/local/src/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/usr/local/src/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/usr/local/src/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
