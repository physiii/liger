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
Sheet 3 10
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
L PT5529B/L2/H2-F U2
U 1 1 59098AF0
P 4900 3000
F 0 "U2" H 4600 2900 60  0000 C CNN
F 1 "PT5529B/L2/H2-F" H 4950 3200 60  0000 C CNN
F 2 "footprints:PT5529" H 4900 3000 60  0001 C CNN
F 3 "http://www.everlight.com/file/ProductFile/PT5529B-L2-H2-F.pdf" H 4900 3000 60  0001 C CNN
F 4 "1080-1161-ND" H 4900 3000 60  0001 C CNN "Part Number"
	1    4900 3000
	1    0    0    -1  
$EndComp
Text Label 4650 2600 0    60   ~ 0
IR-Receive
$Comp
L GND #PWR02
U 1 1 59098AF2
P 4900 3450
F 0 "#PWR02" H 4900 3200 50  0001 C CNN
F 1 "GND" H 4900 3300 50  0000 C CNN
F 2 "" H 4900 3450 50  0000 C CNN
F 3 "" H 4900 3450 50  0000 C CNN
	1    4900 3450
	1    0    0    -1  
$EndComp
Text GLabel 4200 3400 0    60   Input ~ 0
IR_RX
$Comp
L LED D1
U 1 1 59098AF4
P 7500 3050
F 0 "D1" H 7500 3150 50  0000 C CNN
F 1 "LED" H 7500 2950 50  0000 C CNN
F 2 "LEDs:LED_D3.0mm" H 7500 3050 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/149/QED123-890063.pdf" H 7500 3050 50  0001 C CNN
F 4 "QED123" H 7500 3050 60  0001 C CNN "Part Number"
	1    7500 3050
	0    -1   -1   0   
$EndComp
$Comp
L R R5
U 1 1 59098AF5
P 7500 2600
F 0 "R5" V 7580 2600 50  0000 C CNN
F 1 "36" V 7500 2600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 7430 2600 50  0001 C CNN
F 3 "http://www.yageo.com.tw/exep/pages/download/literatures/PYu-R_INT-thick_7.pdf" H 7500 2600 50  0001 C CNN
F 4 "RC0603JR-0736RL" V 7500 2600 60  0001 C CNN "Part Number"
	1    7500 2600
	1    0    0    -1  
$EndComp
$Comp
L Q_NPN_BEC Q1
U 1 1 59098AF8
P 7400 3550
F 0 "Q1" H 7700 3600 50  0000 R CNN
F 1 "Q_NPN_BEC" H 8000 3500 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 7600 3650 50  0001 C CNN
F 3 "http://www.onsemi.com/pub/Collateral/MMBT3904LT1-D.PDF" H 7400 3550 50  0001 C CNN
F 4 "MMBT3904LT1GOSCT-ND" H 7400 3550 60  0001 C CNN "Part Number"
	1    7400 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 59098AF9
P 7500 3950
F 0 "#PWR03" H 7500 3700 50  0001 C CNN
F 1 "GND" H 7500 3800 50  0000 C CNN
F 2 "" H 7500 3950 50  0000 C CNN
F 3 "" H 7500 3950 50  0000 C CNN
	1    7500 3950
	1    0    0    -1  
$EndComp
Text GLabel 6350 3550 0    60   Input ~ 0
IR_TX
Text Label 6600 3000 0    60   ~ 0
IR-Transmit
Wire Wire Line
	6650 3550 6350 3550
Wire Wire Line
	7500 3750 7500 3950
Wire Wire Line
	6950 3550 7200 3550
Wire Wire Line
	7500 2300 7500 2450
Wire Wire Line
	4300 3400 4200 3400
Wire Wire Line
	5200 3400 5200 3300
Wire Wire Line
	5000 3400 5200 3400
Wire Wire Line
	5000 3300 5000 3400
Wire Wire Line
	4900 3300 4900 3450
Wire Wire Line
	4800 3400 4600 3400
Wire Wire Line
	4800 3300 4800 3400
Wire Wire Line
	7500 2750 7500 2900
Wire Wire Line
	7500 3200 7500 3350
Text GLabel 7500 2300 1    60   Input ~ 0
3V3
Text GLabel 5200 3300 1    60   Input ~ 0
V_USB
$Comp
L R_1k R3
U 1 1 59BA28B3
P 4450 3400
F 0 "R3" V 4450 3400 50  0000 C CNN
F 1 "R_1k" V 4375 3400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4380 3400 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" V 4530 3400 50  0001 C CNN
F 4 "RC0603JR-071KL" V 4630 3500 60  0001 C CNN "Part Number"
	1    4450 3400
	0    1    1    0   
$EndComp
$Comp
L R_1k R4
U 1 1 59BA28E7
P 6800 3550
F 0 "R4" V 6800 3550 50  0000 C CNN
F 1 "R_1k" V 6725 3550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6730 3550 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" V 6880 3550 50  0001 C CNN
F 4 "RC0603JR-071KL" V 6980 3650 60  0001 C CNN "Part Number"
	1    6800 3550
	0    1    1    0   
$EndComp
$EndSCHEMATC
