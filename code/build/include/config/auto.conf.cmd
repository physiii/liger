deps_config := \
	/home/physiii/code/esp-idf/components/aws_iot/Kconfig \
	/home/physiii/code/esp-idf/components/bt/Kconfig \
	/home/physiii/code/esp-idf/components/esp32/Kconfig \
	/home/physiii/code/esp-idf/components/ethernet/Kconfig \
	/home/physiii/code/esp-idf/components/fatfs/Kconfig \
	/home/physiii/code/esp-idf/components/freertos/Kconfig \
	/home/physiii/circuits/liger/code/components/libwebsockets/Kconfig \
	/home/physiii/code/esp-idf/components/log/Kconfig \
	/home/physiii/code/esp-idf/components/lwip/Kconfig \
	/home/physiii/code/esp-idf/components/mbedtls/Kconfig \
	/home/physiii/code/esp-idf/components/openssl/Kconfig \
	/home/physiii/code/esp-idf/components/spi_flash/Kconfig \
	/home/physiii/code/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/physiii/code/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/physiii/code/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/physiii/code/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
