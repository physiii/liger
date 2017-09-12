EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:open-automation
LIBS:liger-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L GND #PWR01
U 1 1 59097974
P 5050 2400
F 0 "#PWR01" H 5050 2150 50  0001 C CNN
F 1 "GND" H 5050 2250 50  0000 C CNN
F 2 "" H 5050 2400 50  0000 C CNN
F 3 "" H 5050 2400 50  0000 C CNN
	1    5050 2400
	-1   0    0    1   
$EndComp
Text GLabel 5600 4400 3    60   Input ~ 0
IO13
Text GLabel 5000 3850 0    60   Input ~ 0
IO12
Text GLabel 5000 3750 0    60   Input ~ 0
T_DOWN
Text GLabel 6950 2700 2    60   Input ~ 0
RXD
Text GLabel 6950 3350 2    60   Input ~ 0
LED_G
Text GLabel 6950 3650 2    60   Input ~ 0
IR_RX
Text GLabel 6950 2600 2    60   Input ~ 0
TXD
Text GLabel 6950 3550 2    60   Input ~ 0
IR_TX
Text GLabel 6950 3750 2    60   Input ~ 0
IO0
Text GLabel 6400 4400 3    60   Input ~ 0
IO2
Text GLabel 5000 2550 0    60   Input ~ 0
3V3
Text GLabel 6300 4400 3    60   Input ~ 0
RELAY
$Comp
L ESP-WROOM-32 U1
U 1 1 590979AF
P 6000 3100
F 0 "U1" H 6550 2000 60  0000 C CNN
F 1 "ESP-WROOM-32" H 6000 3900 60  0000 C CNN
F 2 "footprints:ESP-WROOM-32" H 5650 3700 60  0001 C CNN
F 3 "http://espressif.com/sites/default/files/documentation/esp-wroom-32_datasheet_en.pdf" H 5650 3700 60  0001 C CNN
	1    6000 3100
	1    0    0    -1  
$EndComp
Text GLabel 5000 3650 0    60   Input ~ 0
T_UP
Text GLabel 4500 3550 0    60   Input ~ 0
I2S_BCLK
Text GLabel 7400 2850 2    60   Input ~ 0
I2S_WS
Text GLabel 5000 3350 0    60   Input ~ 0
T_RIGHT
Text GLabel 5000 3250 0    60   Input ~ 0
T_LEFT
Text GLabel 5000 3150 0    60   Input ~ 0
PIR2_ADC
Text GLabel 5000 2900 0    60   Input ~ 0
SENSOR_VN
Text GLabel 5000 2800 0    60   Input ~ 0
SENSOR_VP
Text GLabel 5000 3050 0    60   Input ~ 0
PIR1_ADC
Text GLabel 5700 4400 3    60   Input ~ 0
SD2
Text GLabel 5800 4400 3    60   Input ~ 0
SD3
Text GLabel 5900 4400 3    60   Input ~ 0
CMD
Text GLabel 6000 4400 3    60   Input ~ 0
CLK
Text GLabel 6100 4400 3    60   Input ~ 0
SDA
Text GLabel 6200 4400 3    60   Input ~ 0
SCL
Text GLabel 6950 3150 2    60   Input ~ 0
PIR_LO
Text GLabel 6950 3250 2    60   Input ~ 0
LED_R
Text GLabel 6950 2950 2    60   Input ~ 0
I2S_DATA
Text GLabel 6950 3050 2    60   Input ~ 0
PIR_HI
Text GLabel 6950 3450 2    60   Input ~ 0
LED_B
Text Label 1800 2350 0    60   ~ 0
Microcontroller
Text GLabel 5000 2650 0    60   Input ~ 0
EN
Wire Wire Line
	5000 3750 5100 3750
Wire Wire Line
	5000 3850 5100 3850
Wire Wire Line
	5050 2400 5050 2450
Wire Wire Line
	5050 2450 5100 2450
Wire Wire Line
	5100 2550 5000 2550
Wire Wire Line
	5000 2650 5100 2650
Wire Wire Line
	5600 4400 5600 4300
Wire Wire Line
	6400 4300 6400 4400
Wire Wire Line
	6850 3750 6950 3750
Wire Wire Line
	6850 2600 6950 2600
Wire Wire Line
	6950 2700 6850 2700
Wire Wire Line
	6950 3350 6850 3350
Wire Wire Line
	6850 3550 6950 3550
Wire Wire Line
	6950 3650 6850 3650
Wire Wire Line
	5000 3650 5100 3650
Wire Wire Line
	5000 3350 5100 3350
Wire Wire Line
	5000 3450 5100 3450
Wire Wire Line
	5000 3150 5100 3150
Wire Wire Line
	5000 3250 5100 3250
Wire Wire Line
	5000 2800 5100 2800
Wire Wire Line
	5000 2900 5100 2900
Wire Wire Line
	5000 3050 5100 3050
Wire Wire Line
	5700 4400 5700 4300
Wire Wire Line
	6300 4300 6300 4400
Wire Wire Line
	5800 4400 5800 4300
Wire Wire Line
	5900 4400 5900 4300
Wire Wire Line
	6100 4400 6100 4300
Wire Wire Line
	6200 4400 6200 4300
Wire Wire Line
	6000 4300 6000 4400
Wire Wire Line
	6850 3250 6950 3250
Wire Wire Line
	6850 3050 6950 3050
Wire Wire Line
	6950 3150 6850 3150
Wire Wire Line
	6850 2850 6950 2850
Wire Wire Line
	6950 2950 6850 2950
Wire Wire Line
	6850 3450 6950 3450
Text GLabel 5000 3450 0    60   Input ~ 0
SPEAKER_IN
$Comp
L R_10k R1
U 1 1 59B92E27
P 9100 3350
F 0 "R1" V 9100 3350 50  0000 C CNN
F 1 "R_10k" V 9025 3350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9030 3350 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 9180 3350 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 9280 3450 60  0001 C CNN "Part Number"
	1    9100 3350
	0    1    1    0   
$EndComp
$Comp
L R_10k R2
U 1 1 59B92EA0
P 4750 3550
F 0 "R2" V 4750 3550 50  0000 C CNN
F 1 "R_10k" V 4800 3350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4680 3550 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 4830 3550 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 4930 3650 60  0001 C CNN "Part Number"
	1    4750 3550
	0    1    1    0   
$EndComp
$Comp
L R_10k R6
U 1 1 59B92EE3
P 9100 3600
F 0 "R6" V 9100 3600 50  0000 C CNN
F 1 "R_10k" V 9025 3600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9030 3600 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 9180 3600 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 9280 3700 60  0001 C CNN "Part Number"
	1    9100 3600
	0    1    1    0   
$EndComp
$Comp
L R_10k R7
U 1 1 59B92EEA
P 9100 3850
F 0 "R7" V 9100 3850 50  0000 C CNN
F 1 "R_10k" V 9025 3850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9030 3850 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 9180 3850 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 9280 3950 60  0001 C CNN "Part Number"
	1    9100 3850
	0    1    1    0   
$EndComp
Text GLabel 8800 3200 1    60   Input ~ 0
3V3
Text GLabel 9400 3350 2    60   Input ~ 0
I2S_WS
Text GLabel 9400 3600 2    60   Input ~ 0
SDA
Wire Wire Line
	9250 3350 9400 3350
Wire Wire Line
	9250 3600 9400 3600
Wire Wire Line
	9250 3850 9400 3850
Text GLabel 9400 3850 2    60   Input ~ 0
SCL
Wire Wire Line
	8950 3850 8800 3850
Wire Wire Line
	8800 3850 8800 3200
Wire Wire Line
	8950 3350 8800 3350
Connection ~ 8800 3350
Wire Wire Line
	8950 3600 8800 3600
Connection ~ 8800 3600
$Comp
L R_10k R47
U 1 1 59BA04C3
P 7100 2850
F 0 "R47" V 7100 2850 50  0000 C CNN
F 1 "R_10k" V 7050 3050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7030 2850 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 7180 2850 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 7280 2950 60  0001 C CNN "Part Number"
	1    7100 2850
	0    1    1    0   
$EndComp
Wire Wire Line
	7250 2850 7400 2850
Wire Wire Line
	4900 3550 5100 3550
Wire Wire Line
	4500 3550 4600 3550
$EndSCHEMATC
