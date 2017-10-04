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
P 4875 2850
F 0 "#PWR01" H 4875 2600 50  0001 C CNN
F 1 "GND" H 4875 2700 50  0000 C CNN
F 2 "" H 4875 2850 50  0000 C CNN
F 3 "" H 4875 2850 50  0000 C CNN
	1    4875 2850
	-1   0    0    1   
$EndComp
Text GLabel 4825 4300 0    60   Input ~ 0
T_DOWN
Text GLabel 6775 3150 2    60   Input ~ 0
RXD
Text GLabel 6775 3800 2    60   Input ~ 0
LED_G
Text GLabel 6775 4100 2    60   Input ~ 0
IR_RX
Text GLabel 6775 3050 2    60   Input ~ 0
TXD
Text GLabel 6775 4000 2    60   Input ~ 0
IR_TX
Text GLabel 4825 3000 0    60   Input ~ 0
3V3
Text GLabel 6125 4850 3    60   Input ~ 0
RELAY
$Comp
L ESP-WROOM-32 U1
U 1 1 590979AF
P 5825 3550
F 0 "U1" H 6375 2450 60  0000 C CNN
F 1 "ESP-WROOM-32" H 5825 4350 60  0000 C CNN
F 2 "open-automation:ESP-WROOM-32" H 5475 4150 60  0001 C CNN
F 3 "http://espressif.com/sites/default/files/documentation/esp-wroom-32_datasheet_en.pdf" H 5475 4150 60  0001 C CNN
	1    5825 3550
	1    0    0    -1  
$EndComp
Text GLabel 4825 4100 0    60   Input ~ 0
T_UP
Text GLabel 4125 4000 0    60   Input ~ 0
I2S_BCLK
Text GLabel 7425 3300 2    60   Input ~ 0
I2S_WS
Text GLabel 4825 3800 0    60   Input ~ 0
T_RIGHT
Text GLabel 4825 3700 0    60   Input ~ 0
T_LEFT
Text GLabel 4825 3600 0    60   Input ~ 0
PIR2_ADC
Text GLabel 4825 3500 0    60   Input ~ 0
PIR1_ADC
Text GLabel 5925 4850 3    60   Input ~ 0
SDA
Text GLabel 6025 4850 3    60   Input ~ 0
SCL
Text GLabel 6775 3600 2    60   Input ~ 0
PIR_LO
Text GLabel 6775 3700 2    60   Input ~ 0
LED_R
Text GLabel 6775 3400 2    60   Input ~ 0
I2S_DATA
Text GLabel 6775 3500 2    60   Input ~ 0
PIR_HI
Text GLabel 6775 3900 2    60   Input ~ 0
LED_B
Text GLabel 4825 3100 0    60   Input ~ 0
EN
Text GLabel 4825 3900 0    60   Input ~ 0
SPEAKER_IN
$Comp
L R_10k R6
U 1 1 59B92EE3
P 2750 5025
F 0 "R6" V 2750 5025 50  0000 C CNN
F 1 "R_10k" V 2675 5025 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2680 5025 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 2830 5025 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 2930 5125 60  0001 C CNN "Part Number"
	1    2750 5025
	0    1    1    0   
$EndComp
$Comp
L R_10k R7
U 1 1 59B92EEA
P 2750 5275
F 0 "R7" V 2750 5275 50  0000 C CNN
F 1 "R_10k" V 2675 5275 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2680 5275 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 2830 5275 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 2930 5375 60  0001 C CNN "Part Number"
	1    2750 5275
	0    1    1    0   
$EndComp
Text GLabel 2450 4875 1    60   Input ~ 0
3V3
Text GLabel 3050 5025 2    60   Input ~ 0
SDA
Text GLabel 3050 5275 2    60   Input ~ 0
SCL
$Comp
L R_47.5 R2
U 1 1 59B81C1C
P 7175 3300
F 0 "R2" V 7175 3300 50  0000 C CNN
F 1 "R_47.5" V 7100 3300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7105 3300 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603FR-0747R5L/311-47.5HRCT-ND/730201" V 7255 3300 50  0001 C CNN
F 4 "RC0603FR-0747R5L" V 7355 3400 60  0001 C CNN "Part Number"
	1    7175 3300
	0    1    1    0   
$EndComp
$Comp
L R_47.5 R1
U 1 1 59B81C70
P 4375 4000
F 0 "R1" V 4375 4000 50  0000 C CNN
F 1 "R_47.5" V 4475 4000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4305 4000 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603FR-0747R5L/311-47.5HRCT-ND/730201" V 4455 4000 50  0001 C CNN
F 4 "RC0603FR-0747R5L" V 4555 4100 60  0001 C CNN "Part Number"
	1    4375 4000
	0    1    1    0   
$EndComp
Text GLabel 4825 4200 0    60   Input ~ 0
FACTORY
Text GLabel 6775 4200 2    60   Input ~ 0
PROG
$Comp
L GND #PWR02
U 1 1 59C84AF6
P 8700 3625
F 0 "#PWR02" H 8700 3375 50  0001 C CNN
F 1 "GND" H 8700 3475 50  0000 C CNN
F 2 "" H 8700 3625 50  0000 C CNN
F 3 "" H 8700 3625 50  0000 C CNN
	1    8700 3625
	-1   0    0    1   
$EndComp
Text GLabel 8650 3775 0    60   Input ~ 0
3V3
Text GLabel 9350 3975 2    60   Input ~ 0
RXD
Text GLabel 9350 3875 2    60   Input ~ 0
TXD
Text GLabel 9350 3775 2    60   Input ~ 0
V_USB
$Comp
L GND #PWR03
U 1 1 59C84AF7
P 9300 3625
F 0 "#PWR03" H 9300 3375 50  0001 C CNN
F 1 "GND" H 9300 3475 50  0000 C CNN
F 2 "" H 9300 3625 50  0000 C CNN
F 3 "" H 9300 3625 50  0000 C CNN
	1    9300 3625
	-1   0    0    1   
$EndComp
Text Label 4200 1025 0    197  ~ 0
Microcontroller
$Comp
L R_10 R48
U 1 1 59C84AF8
P 2125 3575
F 0 "R48" V 2205 3575 50  0000 C CNN
F 1 "R_10" V 2125 3575 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2055 3575 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/te-connectivity-passive-product/CPF0603B10RE/A119908CT-ND/4966506" V 2205 3575 50  0001 C CNN
F 4 "CPF0603B10RE" V 2305 3675 60  0001 C CNN "Part Number"
	1    2125 3575
	0    1    1    0   
$EndComp
$Comp
L C_1uF C42
U 1 1 59C84AF9
P 2825 3775
F 0 "C42" H 2825 3875 50  0000 L CNN
F 1 "C_1uF" H 2575 3675 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2925 4075 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/CC0603ZRY5V7BB105/311-1372-1-ND/2103156" H 2825 3525 50  0001 C CNN
F 4 "CC0603ZRY5V7BB105" H 2950 3975 60  0001 C CNN "Part Number"
	1    2825 3775
	1    0    0    -1  
$EndComp
$Comp
L C_100uF C41
U 1 1 59C84AFA
P 2425 3775
F 0 "C41" H 2450 3875 50  0000 L CNN
F 1 "C_100uF" H 2075 3675 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 2525 4075 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/tdk-corporation/C3216X5R1A107M160AC/445-6007-1-ND/2444048" H 2425 3525 50  0001 C CNN
F 4 "C3216X5R1A107M160AC" H 2550 3975 60  0001 C CNN "Part Number"
	1    2425 3775
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 59C84AFB
P 2625 4025
F 0 "#PWR04" H 2625 3775 50  0001 C CNN
F 1 "GND" H 2625 3875 50  0000 C CNN
F 2 "" H 2625 4025 50  0000 C CNN
F 3 "" H 2625 4025 50  0000 C CNN
	1    2625 4025
	1    0    0    -1  
$EndComp
Text GLabel 1875 3575 0    60   Input ~ 0
3V3
Text GLabel 2975 3575 2    60   Input ~ 0
VCC_I2C
Text Notes 2175 4425 0    60   ~ 0
I2C ByPass CAPs
Text GLabel 8650 3975 0    60   Input ~ 0
PROG
Text GLabel 8650 3875 0    60   Input ~ 0
FACTORY
$Comp
L R_10k R49
U 1 1 59C84AFC
P 6425 4800
F 0 "R49" V 6425 4800 50  0000 C CNN
F 1 "R_10k" V 6500 4800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6355 4800 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 6505 4800 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 6605 4900 60  0001 C CNN "Part Number"
	1    6425 4800
	0    1    1    0   
$EndComp
$Comp
L GND #PWR05
U 1 1 59C84AFD
P 6625 4800
F 0 "#PWR05" H 6625 4550 50  0001 C CNN
F 1 "GND" H 6625 4650 50  0000 C CNN
F 2 "" H 6625 4800 50  0000 C CNN
F 3 "" H 6625 4800 50  0000 C CNN
	1    6625 4800
	0    -1   -1   0   
$EndComp
Text GLabel 8650 4075 0    60   Input ~ 0
GND
Text GLabel 9350 4075 2    60   Input ~ 0
V_BAT
$Comp
L Conn_02x05_Odd_Even J1
U 1 1 59C84AFE
P 8950 3875
F 0 "J1" H 9000 4175 50  0000 C CNN
F 1 "Conn_02x05_Odd_Even" H 9000 3575 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x05_Pitch2.54mm" H 8950 3875 50  0001 C CNN
F 3 "" H 8950 3875 50  0001 C CNN
	1    8950 3875
	1    0    0    -1  
$EndComp
Wire Wire Line
	4875 2850 4875 2900
Wire Wire Line
	4875 2900 4925 2900
Wire Wire Line
	4925 3000 4825 3000
Wire Wire Line
	4825 3100 4925 3100
Wire Wire Line
	6675 4200 6775 4200
Wire Wire Line
	6675 3050 6775 3050
Wire Wire Line
	6775 3150 6675 3150
Wire Wire Line
	6775 3800 6675 3800
Wire Wire Line
	6675 4000 6775 4000
Wire Wire Line
	6775 4100 6675 4100
Wire Wire Line
	4825 4100 4925 4100
Wire Wire Line
	4825 3800 4925 3800
Wire Wire Line
	4825 3900 4925 3900
Wire Wire Line
	4825 3600 4925 3600
Wire Wire Line
	4825 3700 4925 3700
Wire Wire Line
	4825 3500 4925 3500
Wire Wire Line
	6125 4750 6125 4850
Wire Wire Line
	5925 4850 5925 4750
Wire Wire Line
	6025 4850 6025 4750
Wire Wire Line
	6675 3700 6775 3700
Wire Wire Line
	6675 3500 6775 3500
Wire Wire Line
	6775 3600 6675 3600
Wire Wire Line
	6775 3400 6675 3400
Wire Wire Line
	6675 3900 6775 3900
Wire Wire Line
	2900 5025 3050 5025
Wire Wire Line
	2900 5275 3050 5275
Wire Wire Line
	2450 5275 2600 5275
Wire Wire Line
	2600 5025 2450 5025
Connection ~ 2450 5025
Wire Wire Line
	7025 3300 6675 3300
Wire Wire Line
	7425 3300 7325 3300
Wire Wire Line
	4525 4000 4925 4000
Wire Wire Line
	4125 4000 4225 4000
Wire Wire Line
	2450 4875 2450 5275
Wire Wire Line
	9300 3675 9300 3625
Wire Wire Line
	9250 3675 9300 3675
Wire Wire Line
	8700 3675 8750 3675
Wire Wire Line
	8700 3625 8700 3675
Wire Wire Line
	9250 3775 9350 3775
Wire Wire Line
	9350 3975 9250 3975
Wire Wire Line
	9250 3875 9350 3875
Wire Wire Line
	8650 3775 8750 3775
Wire Wire Line
	1875 3575 1975 3575
Wire Wire Line
	2275 3575 2975 3575
Wire Wire Line
	2425 3575 2425 3625
Wire Wire Line
	2825 3575 2825 3625
Connection ~ 2425 3575
Connection ~ 2825 3575
Wire Wire Line
	2425 3925 2425 3975
Wire Wire Line
	2425 3975 2825 3975
Wire Wire Line
	2825 3975 2825 3925
Wire Wire Line
	2625 4025 2625 3975
Connection ~ 2625 3975
Wire Notes Line
	3425 3475 3425 4325
Wire Notes Line
	3425 4325 1575 4325
Wire Notes Line
	1575 4325 1575 3475
Wire Notes Line
	1575 3475 3425 3475
Wire Wire Line
	8650 3975 8750 3975
Wire Wire Line
	8650 3875 8750 3875
Wire Wire Line
	6625 4800 6575 4800
Wire Wire Line
	8650 4075 8750 4075
Wire Wire Line
	9250 4075 9350 4075
Wire Wire Line
	6275 4800 6225 4800
Wire Wire Line
	6225 4800 6225 4750
Wire Wire Line
	4825 4300 4925 4300
Wire Wire Line
	4825 4200 4925 4200
$EndSCHEMATC
