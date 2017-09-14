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
Sheet 9 10
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
L CONN_01X03 P1
U 1 1 59B5D7B6
P 4800 3750
F 0 "P1" H 4700 3950 50  0000 L CNN
F 1 "CONN_01X03" V 4900 3500 50  0001 L CNN
F 2 "open-automation:Pin_Header_Straight_1x03_Pitch2.54mm_CORRECTED" H 4800 3750 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/hirose-electric-co-ltd/DF13C-3P-1.25V(21)/H121960CT-ND/3907735" H 4800 3750 50  0001 C CNN
F 4 "A04KR04DA28W51B" H 4800 3750 60  0001 C CNN "Part Number"
	1    4800 3750
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR023
U 1 1 59B5D7B7
P 5050 3900
F 0 "#PWR023" H 5050 3650 50  0001 C CNN
F 1 "GND" H 5050 3750 50  0000 C CNN
F 2 "" H 5050 3900 50  0000 C CNN
F 3 "" H 5050 3900 50  0000 C CNN
	1    5050 3900
	1    0    0    -1  
$EndComp
Text GLabel 5600 3750 2    60   Input ~ 0
RELAY
Text Label 5100 900  0    60   ~ 0
Relay
Text GLabel 5050 3600 1    60   Input ~ 0
V_USB
Wire Wire Line
	5050 3900 5050 3850
Wire Wire Line
	5050 3850 5000 3850
Wire Wire Line
	5050 3600 5050 3650
Wire Wire Line
	5050 3650 5000 3650
Wire Wire Line
	5000 3750 5600 3750
$Comp
L R_10k R26
U 1 1 59B9995F
P 5350 3900
F 0 "R26" V 5350 3900 50  0000 C CNN
F 1 "R_10k" V 5275 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5280 3900 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 5430 3900 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 5530 4000 60  0001 C CNN "Part Number"
	1    5350 3900
	1    0    0    -1  
$EndComp
Connection ~ 5350 3750
$Comp
L GND #PWR024
U 1 1 59B99A98
P 5350 4100
F 0 "#PWR024" H 5350 3850 50  0001 C CNN
F 1 "GND" H 5350 3950 50  0000 C CNN
F 2 "" H 5350 4100 50  0000 C CNN
F 3 "" H 5350 4100 50  0000 C CNN
	1    5350 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 4100 5350 4050
$EndSCHEMATC
