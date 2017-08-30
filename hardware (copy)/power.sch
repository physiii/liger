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
L C C3
U 1 1 5909DD64
P 4950 3150
F 0 "C3" H 4975 3250 50  0000 L CNN
F 1 "1uF" H 4975 3050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 4988 3000 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/UPY-GPHC_Y5V_6.3V-to-50V_6.pdf" H 4950 3150 50  0001 C CNN
F 4 "CC0603ZRY5V7BB105" H 4950 3150 60  0001 C CNN "Part Number"
	1    4950 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 5909DD67
P 6150 3400
F 0 "#PWR015" H 6150 3150 50  0001 C CNN
F 1 "GND" H 6150 3250 50  0000 C CNN
F 2 "" H 6150 3400 50  0000 C CNN
F 3 "" H 6150 3400 50  0000 C CNN
	1    6150 3400
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 5909DD72
P 6150 3200
F 0 "C4" H 6175 3300 50  0000 L CNN
F 1 "1uF" H 6175 3100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 6188 3050 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/UPY-GPHC_Y5V_6.3V-to-50V_6.pdf" H 6150 3200 50  0001 C CNN
F 4 "CC0603ZRY5V7BB105" H 6150 3200 60  0001 C CNN "Part Number"
	1    6150 3200
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P4
U 1 1 5909DD7C
P 6200 4500
F 0 "P4" H 6100 4700 50  0000 L CNN
F 1 "CONN_01X03" V 6300 4250 50  0001 L CNN
F 2 "open-automation:Pin_Header_Straight_1x03_Pitch2.54mm_CORRECTED" H 6200 4500 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/hirose-electric-co-ltd/DF13C-3P-1.25V(21)/H121960CT-ND/3907735" H 6200 4500 50  0001 C CNN
F 4 "A04KR04DA28W51B" H 6200 4500 60  0001 C CNN "Part Number"
	1    6200 4500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 5909DD7D
P 5950 4350
F 0 "#PWR016" H 5950 4100 50  0001 C CNN
F 1 "GND" H 5950 4200 50  0000 C CNN
F 2 "" H 5950 4350 50  0000 C CNN
F 3 "" H 5950 4350 50  0000 C CNN
	1    5950 4350
	-1   0    0    1   
$EndComp
Text GLabel 5900 4500 0    60   Input ~ 0
IO15
Text GLabel 6350 3050 2    60   Input ~ 0
3V3
Text GLabel 5950 4650 3    60   Input ~ 0
5V
$Comp
L SPX1117M3-L-3-3/TR U11
U 1 1 59A4B87F
P 5650 3050
F 0 "U11" H 5750 2750 50  0000 C CNN
F 1 "SPX1117M3-L-3-3/TR" H 5650 3300 50  0000 C CNN
F 2 "open-automation:SOT-223_CORRECTED" H 5650 2650 50  0001 C CIN
F 3 "https://www.digikey.com/product-detail/en/exar-corporation/SPX1117M3-L-3-3-TR/1016-1848-1-ND/3586565" H 5700 2550 50  0001 C CNN
F 4 "SPX1117M3-L-3-3/TR" H 5850 2850 60  0001 C CNN "Part Number"
	1    5650 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 59A4BA1C
P 5200 3200
F 0 "#PWR017" H 5200 2950 50  0001 C CNN
F 1 "GND" H 5200 3050 50  0000 C CNN
F 2 "" H 5200 3200 50  0000 C CNN
F 3 "" H 5200 3200 50  0000 C CNN
	1    5200 3200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 59A4BAA9
P 4950 3350
F 0 "#PWR018" H 4950 3100 50  0001 C CNN
F 1 "GND" H 4950 3200 50  0000 C CNN
F 2 "" H 4950 3350 50  0000 C CNN
F 3 "" H 4950 3350 50  0000 C CNN
	1    4950 3350
	1    0    0    -1  
$EndComp
Text Label 4600 2450 0    60   ~ 0
Regulator
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
	6000 3050 6350 3050
Wire Wire Line
	6150 3400 6150 3350
Wire Wire Line
	5200 3200 5200 3150
Wire Wire Line
	5200 3150 5250 3150
Wire Wire Line
	4950 3000 4950 2950
Wire Wire Line
	4800 2950 5250 2950
Wire Wire Line
	4950 3350 4950 3300
Connection ~ 6150 3050
Text GLabel 4800 2950 0    60   Input ~ 0
5V
Connection ~ 4950 2950
$EndSCHEMATC
