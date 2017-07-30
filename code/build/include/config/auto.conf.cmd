deps_config := \
	/home/physiii/esp/esp-idf/components/app_trace/Kconfig \
	/home/physiii/esp/esp-idf/components/aws_iot/Kconfig \
	/home/physiii/esp/esp-idf/components/bt/Kconfig \
	/home/physiii/esp/esp-idf/components/esp32/Kconfig \
	/home/physiii/esp/esp-idf/components/ethernet/Kconfig \
	/home/physiii/esp/esp-idf/components/fatfs/Kconfig \
	/home/physiii/esp/esp-idf/components/freertos/Kconfig \
	/home/physiii/circuits/liger/code/components/libwebsockets/Kconfig \
	/home/physiii/esp/esp-idf/components/log/Kconfig \
	/home/physiii/esp/esp-idf/components/lwip/Kconfig \
	/home/physiii/esp/esp-idf/components/mbedtls/Kconfig \
	/home/physiii/esp/esp-idf/components/openssl/Kconfig \
	/home/physiii/esp/esp-idf/components/spi_flash/Kconfig \
	/home/physiii/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/physiii/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/physiii/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/physiii/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
