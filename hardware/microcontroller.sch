EESchema Schematic File Version 4
LIBS:liger-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 9
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
L power:GND #PWR01
U 1 1 59097974
P 5000 1800
F 0 "#PWR01" H 5000 1550 50  0001 C CNN
F 1 "GND" H 5000 1650 50  0000 C CNN
F 2 "" H 5000 1800 50  0000 C CNN
F 3 "" H 5000 1800 50  0000 C CNN
	1    5000 1800
	-1   0    0    1   
$EndComp
$Comp
L liger-rescue:ESP-WROOM-32 U1
U 1 1 590979AF
P 5950 2500
F 0 "U1" H 6500 1400 60  0000 C CNN
F 1 "ESP-WROOM-32" H 5950 3300 60  0000 C CNN
F 2 "open-automation:ESP-WROOM-32" H 5600 3100 60  0001 C CNN
F 3 "http://espressif.com/sites/default/files/documentation/esp-wroom-32_datasheet_en.pdf" H 5600 3100 60  0001 C CNN
	1    5950 2500
	1    0    0    -1  
$EndComp
Text Label 4800 1075 0    197  ~ 0
Microcontroller
$Comp
L power:GND #PWR02
U 1 1 59C84AFB
P 5950 5425
F 0 "#PWR02" H 5950 5175 50  0001 C CNN
F 1 "GND" H 5950 5275 50  0000 C CNN
F 2 "" H 5950 5425 50  0000 C CNN
F 3 "" H 5950 5425 50  0000 C CNN
	1    5950 5425
	1    0    0    -1  
$EndComp
Text Notes 5575 5725 0    60   ~ 0
I2C ByPass CAPs
Wire Wire Line
	4950 3150 5050 3150
Wire Wire Line
	4950 3250 5050 3250
Wire Wire Line
	6350 3700 6350 3800
Connection ~ 5950 5375
Wire Wire Line
	5950 5425 5950 5375
Wire Wire Line
	6150 5375 6150 5325
Wire Wire Line
	5750 5375 5950 5375
Wire Wire Line
	5750 5325 5750 5375
Wire Wire Line
	4250 2950 4350 2950
Wire Wire Line
	4650 2950 5050 2950
Wire Wire Line
	7550 2250 7450 2250
Wire Wire Line
	7150 2250 6800 2250
Wire Wire Line
	7050 5025 7050 4875
Wire Wire Line
	6800 4875 6800 5025
Wire Wire Line
	6800 5325 6800 5475
Wire Wire Line
	7050 5325 7050 5475
Wire Wire Line
	6900 2350 6800 2350
Wire Wire Line
	6900 2550 6800 2550
Wire Wire Line
	6800 2450 6900 2450
Wire Wire Line
	6800 2650 6900 2650
Wire Wire Line
	6250 3700 6250 3800
Wire Wire Line
	4950 2650 5050 2650
Wire Wire Line
	4950 2850 5050 2850
Wire Wire Line
	4950 2750 5050 2750
Wire Wire Line
	4950 3050 5050 3050
Wire Wire Line
	6900 3050 6800 3050
Wire Wire Line
	6800 2950 6900 2950
Wire Wire Line
	6900 2100 6800 2100
Wire Wire Line
	6800 2000 6900 2000
Wire Wire Line
	6800 3150 6900 3150
Wire Wire Line
	5050 1950 5025 1950
Wire Wire Line
	5000 1850 5050 1850
Wire Wire Line
	5000 1800 5000 1850
$Comp
L power:GND #PWR03
U 1 1 5A1695EE
P 4850 5425
F 0 "#PWR03" H 4850 5175 50  0001 C CNN
F 1 "GND" H 4850 5275 50  0000 C CNN
F 2 "" H 4850 5425 50  0000 C CNN
F 3 "" H 4850 5425 50  0000 C CNN
	1    4850 5425
	1    0    0    -1  
$EndComp
Text Notes 4475 5775 0    60   ~ 0
I2S ByPass CAPs
Connection ~ 4850 5400
Wire Wire Line
	4850 5425 4850 5400
Wire Wire Line
	5075 5400 4850 5400
Wire Wire Line
	5075 5325 5075 5400
Wire Wire Line
	4675 5400 4675 5325
Wire Wire Line
	5075 5025 5075 4950
Wire Wire Line
	5075 4950 4875 4950
Wire Wire Line
	4675 4950 4675 5025
Wire Wire Line
	4875 4875 4875 4950
Connection ~ 4875 4950
Wire Notes Line
	5350 5825 4350 5825
Wire Notes Line
	4350 5825 4350 4625
Wire Notes Line
	4350 4625 5350 4625
Wire Notes Line
	5350 4625 5350 5825
Wire Wire Line
	5750 5025 5750 4950
Wire Wire Line
	5750 4950 5975 4950
Wire Wire Line
	6150 4950 6150 5025
Wire Wire Line
	5975 4875 5975 4950
Connection ~ 5975 4950
Wire Notes Line
	5450 5750 6400 5750
Wire Notes Line
	5450 5750 5450 4625
Wire Notes Line
	5450 4625 6400 4625
Wire Notes Line
	6400 4625 6400 5750
Text Notes 6600 5825 0    60   ~ 0
I2C PULL-UP
Wire Notes Line
	7300 5850 6525 5850
Wire Notes Line
	6525 5850 6525 4625
Wire Notes Line
	6525 4625 7300 4625
Wire Notes Line
	7300 4625 7300 5850
Wire Wire Line
	5950 5375 6150 5375
Wire Wire Line
	4850 5400 4675 5400
Wire Wire Line
	4875 4950 4675 4950
Wire Wire Line
	5975 4950 6150 4950
Text HLabel 4950 1950 0    60   Input ~ 0
3V3
Wire Wire Line
	5050 2050 5025 2050
Wire Wire Line
	5025 2050 5025 1950
Connection ~ 5025 1950
Wire Wire Line
	5025 1950 4950 1950
Text HLabel 4950 2650 0    60   Input ~ 0
T_LEFT
Text HLabel 4950 2750 0    60   Input ~ 0
T_RIGHT
Text HLabel 4950 2850 0    60   Input ~ 0
I2S_DATA_OUT
Text HLabel 4250 2950 0    60   Input ~ 0
I2S_BCLK
Text HLabel 4950 3050 0    60   Input ~ 0
T_UP
Text HLabel 4950 3150 0    60   Input ~ 0
FACTORY_RST
Text HLabel 4950 3250 0    60   Input ~ 0
T_DOWN
Text HLabel 6250 3800 3    60   Input ~ 0
PIR
Text HLabel 6350 3800 3    60   Input ~ 0
RELAY
Text HLabel 6900 3150 2    60   Input ~ 0
PROG
Text HLabel 6900 3050 2    60   Output ~ 0
IR_TX
Text HLabel 6900 2950 2    60   Input ~ 0
IR_RX
Text HLabel 6900 2650 2    60   Output ~ 0
BUTTON_LED
Text HLabel 6900 2550 2    60   BiDi ~ 0
SDA
Text HLabel 6900 2450 2    60   Output ~ 0
SCL
Text HLabel 6900 2350 2    60   Input ~ 0
I2S_DATA_IN
Text HLabel 7550 2250 2    60   Input ~ 0
I2S_WS
Text HLabel 6900 2100 2    60   Input ~ 0
RXD
Text HLabel 6900 2000 2    60   Input ~ 0
TXD
Text HLabel 4875 4875 1    60   Input ~ 0
3V3
Text HLabel 5975 4875 1    60   Input ~ 0
3V3
Text HLabel 6800 4875 1    60   Input ~ 0
3V3
Text HLabel 7050 4875 1    60   Input ~ 0
3V3
Text HLabel 6800 5475 3    60   Output ~ 0
SCL
Text HLabel 7050 5475 3    60   BiDi ~ 0
SDA
$Comp
L open-automation:C_1uF C4
U 1 1 5B1F1AAD
P 6150 5175
F 0 "C4" H 6265 5221 50  0000 L CNN
F 1 "C_1uF" H 6265 5130 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6250 5475 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/tdk-corporation/CGA3E1X7R1C105M080AC/445-12540-1-ND/3954206" H 6150 4925 50  0001 C CNN
F 4 "CGA3E1X7R1C105M080AC" H 6275 5375 60  0001 C CNN "Part Number"
	1    6150 5175
	1    0    0    -1  
$EndComp
$Comp
L open-automation:C_1uF C2
U 1 1 5B1F1B60
P 5075 5175
F 0 "C2" H 5125 5275 50  0000 L CNN
F 1 "C_1uF" H 5190 5130 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5175 5475 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/tdk-corporation/CGA3E1X7R1C105M080AC/445-12540-1-ND/3954206" H 5075 4925 50  0001 C CNN
F 4 "CGA3E1X7R1C105M080AC" H 5200 5375 60  0001 C CNN "Part Number"
	1    5075 5175
	1    0    0    -1  
$EndComp
$Comp
L open-automation:C_10uF C3
U 1 1 5B1F217F
P 5750 5175
F 0 "C3" H 5625 5275 50  0000 L CNN
F 1 "C_10uF" H 5475 5100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5850 5475 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/samsung-electro-mechanics/CL10A106MP8NNNC/1276-1871-1-ND/3889957" H 5750 4925 50  0001 C CNN
F 4 "CL10A106MP8NNNC" H 5875 5375 60  0001 C CNN "Part Number"
	1    5750 5175
	1    0    0    -1  
$EndComp
$Comp
L open-automation:C_10uF C1
U 1 1 5B1F22B0
P 4675 5175
F 0 "C1" H 4550 5250 50  0000 L CNN
F 1 "C_10uF" H 4400 5075 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4775 5475 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/samsung-electro-mechanics/CL10A106MP8NNNC/1276-1871-1-ND/3889957" H 4675 4925 50  0001 C CNN
F 4 "CL10A106MP8NNNC" H 4800 5375 60  0001 C CNN "Part Number"
	1    4675 5175
	1    0    0    -1  
$EndComp
$Comp
L open-automation:R_10k R6
U 1 1 5B31EE0D
P 7050 5175
F 0 "R6" H 7120 5221 50  0000 L CNN
F 1 "R_10k" H 7120 5130 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6980 5175 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 7130 5175 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 7230 5275 60  0001 C CNN "Part Number"
	1    7050 5175
	1    0    0    -1  
$EndComp
$Comp
L open-automation:R_10k R2
U 1 1 5B31EE98
P 6800 5175
F 0 "R2" H 6870 5221 50  0000 L CNN
F 1 "R_10k" H 6870 5130 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6730 5175 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 6880 5175 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 6980 5275 60  0001 C CNN "Part Number"
	1    6800 5175
	1    0    0    -1  
$EndComp
$Comp
L open-automation:R_47.5 R7
U 1 1 5B321D4E
P 7300 2250
F 0 "R7" V 7093 2250 50  0000 C CNN
F 1 "R_47.5" V 7184 2250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7230 2250 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603FR-0747R5L/311-47.5HRCT-ND/730201" V 7380 2250 50  0001 C CNN
F 4 "RC0603FR-0747R5L" V 7480 2350 60  0001 C CNN "Part Number"
	1    7300 2250
	0    1    1    0   
$EndComp
$Comp
L open-automation:R_47.5 R1
U 1 1 5B322166
P 4500 2950
F 0 "R1" V 4293 2950 50  0000 C CNN
F 1 "R_47.5" V 4600 2950 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4430 2950 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603FR-0747R5L/311-47.5HRCT-ND/730201" V 4580 2950 50  0001 C CNN
F 4 "RC0603FR-0747R5L" V 4680 3050 60  0001 C CNN "Part Number"
	1    4500 2950
	0    1    1    0   
$EndComp
Wire Wire Line
	4950 2550 5050 2550
Text HLabel 4950 2550 0    60   Input ~ 0
ZERO_DETECT
$EndSCHEMATC
