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
P 5250 2550
F 0 "U2" H 4950 2450 60  0000 C CNN
F 1 "PT5529B/L2/H2-F" H 5300 2750 60  0000 C CNN
F 2 "footprints:PT5529" H 5250 2550 60  0001 C CNN
F 3 "http://www.everlight.com/file/ProductFile/PT5529B-L2-H2-F.pdf" H 5250 2550 60  0001 C CNN
F 4 "1080-1161-ND" H 5250 2550 60  0001 C CNN "Part Number"
	1    5250 2550
	1    0    0    -1  
$EndComp
Text Label 5000 2150 0    60   ~ 0
IR-Receive
$Comp
L GND #PWR02
U 1 1 59098AF2
P 5250 3000
F 0 "#PWR02" H 5250 2750 50  0001 C CNN
F 1 "GND" H 5250 2850 50  0000 C CNN
F 2 "" H 5250 3000 50  0000 C CNN
F 3 "" H 5250 3000 50  0000 C CNN
	1    5250 3000
	1    0    0    -1  
$EndComp
Text GLabel 4550 2950 0    60   Input ~ 0
IR_RX
$Comp
L LED D1
U 1 1 59098AF4
P 5650 4550
F 0 "D1" H 5650 4650 50  0000 C CNN
F 1 "LED" H 5650 4450 50  0000 C CNN
F 2 "LEDs:LED_D3.0mm" H 5650 4550 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/149/QED123-890063.pdf" H 5650 4550 50  0001 C CNN
F 4 "QED123" H 5650 4550 60  0001 C CNN "Part Number"
	1    5650 4550
	0    -1   -1   0   
$EndComp
$Comp
L R R5
U 1 1 59098AF5
P 5650 4100
F 0 "R5" V 5730 4100 50  0000 C CNN
F 1 "36" V 5650 4100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5580 4100 50  0001 C CNN
F 3 "http://www.yageo.com.tw/exep/pages/download/literatures/PYu-R_INT-thick_7.pdf" H 5650 4100 50  0001 C CNN
F 4 "RC0603JR-0736RL" V 5650 4100 60  0001 C CNN "Part Number"
	1    5650 4100
	1    0    0    -1  
$EndComp
$Comp
L Q_NPN_BEC Q1
U 1 1 59098AF8
P 5550 5050
F 0 "Q1" H 5850 5100 50  0000 R CNN
F 1 "Q_NPN_BEC" H 6150 5000 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 5750 5150 50  0001 C CNN
F 3 "http://www.onsemi.com/pub/Collateral/MMBT3904LT1-D.PDF" H 5550 5050 50  0001 C CNN
F 4 "MMBT3904LT1GOSCT-ND" H 5550 5050 60  0001 C CNN "Part Number"
	1    5550 5050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 59098AF9
P 5650 5450
F 0 "#PWR03" H 5650 5200 50  0001 C CNN
F 1 "GND" H 5650 5300 50  0000 C CNN
F 2 "" H 5650 5450 50  0000 C CNN
F 3 "" H 5650 5450 50  0000 C CNN
	1    5650 5450
	1    0    0    -1  
$EndComp
Text GLabel 4500 5050 0    60   Input ~ 0
IR_TX
Text Label 4750 4500 0    60   ~ 0
IR-Transmit
Wire Wire Line
	4800 5050 4500 5050
Wire Wire Line
	5650 5250 5650 5450
Wire Wire Line
	5100 5050 5350 5050
Wire Wire Line
	5650 3800 5650 3950
Wire Wire Line
	4650 2950 4550 2950
Wire Wire Line
	5550 2950 5550 2850
Wire Wire Line
	5350 2950 5550 2950
Wire Wire Line
	5350 2850 5350 2950
Wire Wire Line
	5250 2850 5250 3000
Wire Wire Line
	5150 2950 4950 2950
Wire Wire Line
	5150 2850 5150 2950
Wire Wire Line
	5650 4250 5650 4400
Wire Wire Line
	5650 4700 5650 4850
Text GLabel 5650 3800 1    60   Input ~ 0
3V3
Text GLabel 5550 2850 1    60   Input ~ 0
V_USB
$Comp
L R_1k R3
U 1 1 59BA28B3
P 4800 2950
F 0 "R3" V 4800 2950 50  0000 C CNN
F 1 "R_1k" V 4725 2950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4730 2950 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" V 4880 2950 50  0001 C CNN
F 4 "RC0603JR-071KL" V 4980 3050 60  0001 C CNN "Part Number"
	1    4800 2950
	0    1    1    0   
$EndComp
$Comp
L R_1k R4
U 1 1 59BA28E7
P 4950 5050
F 0 "R4" V 4950 5050 50  0000 C CNN
F 1 "R_1k" V 4875 5050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4880 5050 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" V 5030 5050 50  0001 C CNN
F 4 "RC0603JR-071KL" V 5130 5150 60  0001 C CNN "Part Number"
	1    4950 5050
	0    1    1    0   
$EndComp
$EndSCHEMATC
