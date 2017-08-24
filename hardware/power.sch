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
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 8
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
L SPX3819 U5
U 1 1 5909DD63
P 6200 3700
F 0 "U5" H 5950 4050 50  0000 C CNN
F 1 "SPX3819" H 6350 4050 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 6200 3300 50  0001 C CIN
F 3 "https://www.exar.com/content/document.ashx?id=22106" H 6250 3200 50  0001 C CNN
F 4 "1016-1873-1-ND" H 6200 3700 60  0001 C CNN "Part Number"
	1    6200 3700
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5909DD64
P 4650 3950
F 0 "C3" H 4675 4050 50  0000 L CNN
F 1 "1uF" H 4675 3850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 4688 3800 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/UPY-GPHC_Y5V_6.3V-to-50V_6.pdf" H 4650 3950 50  0001 C CNN
F 4 "CC0603ZRY5V7BB105" H 4650 3950 60  0001 C CNN "Part Number"
	1    4650 3950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 5909DD65
P 5200 4250
F 0 "#PWR015" H 5200 4000 50  0001 C CNN
F 1 "GND" H 5200 4100 50  0000 C CNN
F 2 "" H 5200 4250 50  0000 C CNN
F 3 "" H 5200 4250 50  0000 C CNN
	1    5200 4250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 5909DD66
P 4650 4300
F 0 "#PWR016" H 4650 4050 50  0001 C CNN
F 1 "GND" H 4650 4150 50  0000 C CNN
F 2 "" H 4650 4300 50  0000 C CNN
F 3 "" H 4650 4300 50  0000 C CNN
	1    4650 4300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 5909DD67
P 6700 4050
F 0 "#PWR017" H 6700 3800 50  0001 C CNN
F 1 "GND" H 6700 3900 50  0000 C CNN
F 2 "" H 6700 4050 50  0000 C CNN
F 3 "" H 6700 4050 50  0000 C CNN
	1    6700 4050
	1    0    0    -1  
$EndComp
Text Label 5550 3000 0    60   ~ 0
Regulator
$Comp
L R R11
U 1 1 5909DD71
P 5050 3950
F 0 "R11" V 5130 3950 50  0000 C CNN
F 1 "10K" V 5050 3950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4980 3950 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 5050 3950 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 5050 3950 60  0001 C CNN "Part Number"
	1    5050 3950
	-1   0    0    1   
$EndComp
$Comp
L C C4
U 1 1 5909DD72
P 6700 3700
F 0 "C4" H 6725 3800 50  0000 L CNN
F 1 "1uF" H 6725 3600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 6738 3550 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/UPY-GPHC_Y5V_6.3V-to-50V_6.pdf" H 6700 3700 50  0001 C CNN
F 4 "CC0603ZRY5V7BB105" H 6700 3700 60  0001 C CNN "Part Number"
	1    6700 3700
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P4
U 1 1 5909DD7C
P 6200 4500
F 0 "P4" H 6100 4700 50  0000 L CNN
F 1 "CONN_01X03" V 6300 4250 50  0001 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 6200 4500 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/hirose-electric-co-ltd/DF13C-3P-1.25V(21)/H121960CT-ND/3907735" H 6200 4500 50  0001 C CNN
F 4 "A04KR04DA28W51B" H 6200 4500 60  0001 C CNN "Part Number"
	1    6200 4500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 5909DD7D
P 5950 4350
F 0 "#PWR018" H 5950 4100 50  0001 C CNN
F 1 "GND" H 5950 4200 50  0000 C CNN
F 2 "" H 5950 4350 50  0000 C CNN
F 3 "" H 5950 4350 50  0000 C CNN
	1    5950 4350
	-1   0    0    1   
$EndComp
Text GLabel 5900 4500 0    60   Input ~ 0
IO15
Wire Wire Line
	6700 3850 6700 4050
Wire Wire Line
	4650 4100 4650 4300
Connection ~ 5650 3500
Wire Wire Line
	5650 3500 5650 3900
Wire Wire Line
	5650 3900 5800 3900
Connection ~ 5200 4200
Wire Wire Line
	5200 4200 5200 4250
Wire Wire Line
	5050 4200 5050 4100
Wire Wire Line
	5050 4200 5400 4200
Wire Wire Line
	5400 4200 5400 3700
Wire Wire Line
	5400 3700 5800 3700
Connection ~ 4650 3500
Connection ~ 5050 3500
Wire Wire Line
	5050 3800 5050 3500
Wire Wire Line
	4650 3500 5800 3500
Connection ~ 6700 3500
Wire Wire Line
	6700 3300 6700 3550
Wire Wire Line
	4650 3250 4650 3800
Wire Wire Line
	5950 4350 5950 4400
Wire Wire Line
	5950 4400 6000 4400
Wire Wire Line
	5950 4650 5950 4600
Wire Wire Line
	5950 4600 6000 4600
Wire Wire Line
	5900 4500 6000 4500
Wire Wire Line
	6600 3500 6700 3500
Text GLabel 4650 3250 1    60   Input ~ 0
5V
Text GLabel 6700 3300 1    60   Input ~ 0
3V3
Text GLabel 5950 4650 3    60   Input ~ 0
5V
$EndSCHEMATC
