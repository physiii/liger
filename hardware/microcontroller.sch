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
P 4500 1950
F 0 "#PWR01" H 4500 1700 50  0001 C CNN
F 1 "GND" H 4500 1800 50  0000 C CNN
F 2 "" H 4500 1950 50  0000 C CNN
F 3 "" H 4500 1950 50  0000 C CNN
	1    4500 1950
	-1   0    0    1   
$EndComp
Text GLabel 5050 3950 3    60   Input ~ 0
IO13
Text GLabel 4450 3400 0    60   Input ~ 0
IO12
Text GLabel 4450 3300 0    60   Input ~ 0
T_DOWN
Text GLabel 6400 2250 2    60   Input ~ 0
RXD
Text GLabel 6400 2900 2    60   Input ~ 0
LED_G
Text GLabel 6400 3200 2    60   Input ~ 0
IR_RX
Text GLabel 6400 2150 2    60   Input ~ 0
TXD
Text GLabel 6400 3100 2    60   Input ~ 0
IR_TX
Text GLabel 6400 3300 2    60   Input ~ 0
IO0
Text GLabel 5850 3950 3    60   Input ~ 0
IO2
Text GLabel 4450 2100 0    60   Input ~ 0
3V3
Text GLabel 5750 3950 3    60   Input ~ 0
RELAY
$Comp
L ESP-WROOM-32 U1
U 1 1 590979AF
P 5450 2650
F 0 "U1" H 6000 1550 60  0000 C CNN
F 1 "ESP-WROOM-32" H 5450 3450 60  0000 C CNN
F 2 "open-automation:ESP-WROOM-32" H 5100 3250 60  0001 C CNN
F 3 "http://espressif.com/sites/default/files/documentation/esp-wroom-32_datasheet_en.pdf" H 5100 3250 60  0001 C CNN
	1    5450 2650
	1    0    0    -1  
$EndComp
Text GLabel 4450 3200 0    60   Input ~ 0
T_UP
Text GLabel 3750 3100 0    60   Input ~ 0
I2S_BCLK
Text GLabel 7050 2400 2    60   Input ~ 0
I2S_WS
Text GLabel 4450 2900 0    60   Input ~ 0
T_RIGHT
Text GLabel 4450 2800 0    60   Input ~ 0
T_LEFT
Text GLabel 4450 2700 0    60   Input ~ 0
PIR2_ADC
Text GLabel 4450 2450 0    60   Input ~ 0
SENSOR_VN
Text GLabel 4450 2350 0    60   Input ~ 0
SENSOR_VP
Text GLabel 4450 2600 0    60   Input ~ 0
PIR1_ADC
Text GLabel 5150 3950 3    60   Input ~ 0
SD2
Text GLabel 5250 3950 3    60   Input ~ 0
SD3
Text GLabel 5350 3950 3    60   Input ~ 0
CMD
Text GLabel 5450 3950 3    60   Input ~ 0
CLK
Text GLabel 5550 3950 3    60   Input ~ 0
SDA
Text GLabel 5650 3950 3    60   Input ~ 0
SCL
Text GLabel 6400 2700 2    60   Input ~ 0
PIR_LO
Text GLabel 6400 2800 2    60   Input ~ 0
LED_R
Text GLabel 6400 2500 2    60   Input ~ 0
I2S_DATA
Text GLabel 6400 2600 2    60   Input ~ 0
PIR_HI
Text GLabel 6400 3000 2    60   Input ~ 0
LED_B
Text Label 5600 700  0    60   ~ 0
Microcontroller
Text GLabel 4450 2200 0    60   Input ~ 0
EN
Wire Wire Line
	4450 3300 4550 3300
Wire Wire Line
	4450 3400 4550 3400
Wire Wire Line
	4500 1950 4500 2000
Wire Wire Line
	4500 2000 4550 2000
Wire Wire Line
	4550 2100 4450 2100
Wire Wire Line
	4450 2200 4550 2200
Wire Wire Line
	5050 3950 5050 3850
Wire Wire Line
	5850 3850 5850 3950
Wire Wire Line
	6300 3300 6400 3300
Wire Wire Line
	6300 2150 6400 2150
Wire Wire Line
	6400 2250 6300 2250
Wire Wire Line
	6400 2900 6300 2900
Wire Wire Line
	6300 3100 6400 3100
Wire Wire Line
	6400 3200 6300 3200
Wire Wire Line
	4450 3200 4550 3200
Wire Wire Line
	4450 2900 4550 2900
Wire Wire Line
	4450 3000 4550 3000
Wire Wire Line
	4450 2700 4550 2700
Wire Wire Line
	4450 2800 4550 2800
Wire Wire Line
	4450 2350 4550 2350
Wire Wire Line
	4450 2450 4550 2450
Wire Wire Line
	4450 2600 4550 2600
Wire Wire Line
	5150 3950 5150 3850
Wire Wire Line
	5750 3850 5750 3950
Wire Wire Line
	5250 3950 5250 3850
Wire Wire Line
	5350 3950 5350 3850
Wire Wire Line
	5550 3950 5550 3850
Wire Wire Line
	5650 3950 5650 3850
Wire Wire Line
	5450 3850 5450 3950
Wire Wire Line
	6300 2800 6400 2800
Wire Wire Line
	6300 2600 6400 2600
Wire Wire Line
	6400 2700 6300 2700
Wire Wire Line
	6400 2500 6300 2500
Wire Wire Line
	6300 3000 6400 3000
Text GLabel 4450 3000 0    60   Input ~ 0
SPEAKER_IN
$Comp
L R_10k R6
U 1 1 59B92EE3
P 8550 3150
F 0 "R6" V 8550 3150 50  0000 C CNN
F 1 "R_10k" V 8475 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8480 3150 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 8630 3150 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 8730 3250 60  0001 C CNN "Part Number"
	1    8550 3150
	0    1    1    0   
$EndComp
$Comp
L R_10k R7
U 1 1 59B92EEA
P 8550 3400
F 0 "R7" V 8550 3400 50  0000 C CNN
F 1 "R_10k" V 8475 3400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8480 3400 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 8630 3400 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 8730 3500 60  0001 C CNN "Part Number"
	1    8550 3400
	0    1    1    0   
$EndComp
Text GLabel 8250 3000 1    60   Input ~ 0
3V3
Text GLabel 8850 3150 2    60   Input ~ 0
SDA
Wire Wire Line
	8700 3150 8850 3150
Wire Wire Line
	8700 3400 8850 3400
Text GLabel 8850 3400 2    60   Input ~ 0
SCL
Wire Wire Line
	8250 3400 8400 3400
Wire Wire Line
	8400 3150 8250 3150
Connection ~ 8250 3150
Wire Wire Line
	6650 2400 6300 2400
Wire Wire Line
	7050 2400 6950 2400
Wire Wire Line
	4150 3100 4550 3100
Wire Wire Line
	3750 3100 3850 3100
Wire Wire Line
	8250 3000 8250 3400
$Comp
L R_47.5 R2
U 1 1 59B81C1C
P 6800 2400
F 0 "R2" V 6800 2400 50  0000 C CNN
F 1 "R_47.5" V 6725 2400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6730 2400 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603FR-0747R5L/311-47.5HRCT-ND/730201" V 6880 2400 50  0001 C CNN
F 4 "RC0603FR-0747R5L" V 6980 2500 60  0001 C CNN "Part Number"
	1    6800 2400
	0    1    1    0   
$EndComp
$Comp
L R_47.5 R1
U 1 1 59B81C70
P 4000 3100
F 0 "R1" V 4000 3100 50  0000 C CNN
F 1 "R_47.5" V 4100 3100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3930 3100 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603FR-0747R5L/311-47.5HRCT-ND/730201" V 4080 3100 50  0001 C CNN
F 4 "RC0603FR-0747R5L" V 4180 3200 60  0001 C CNN "Part Number"
	1    4000 3100
	0    1    1    0   
$EndComp
$EndSCHEMATC
