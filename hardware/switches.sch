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
Sheet 7 9
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
L R R12
U 1 1 59092D1A
P 2850 4500
F 0 "R12" V 2930 4500 50  0000 C CNN
F 1 "1K" V 2850 4500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2780 4500 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 2850 4500 50  0001 C CNN
F 4 "RC0603JR-071KL" V 2850 4500 60  0001 C CNN "Part Number"
	1    2850 4500
	-1   0    0    1   
$EndComp
Text GLabel 3900 4300 3    60   Input ~ 0
IO5
$Comp
L SW_PUSH PROG1
U 1 1 59092D1B
P 6200 4300
F 0 "PROG1" H 6350 4410 50  0000 C CNN
F 1 "SW_PUSH" H 6200 4220 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_KMR2" H 6200 4300 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/60/KMR2_9aug12-25127.pdf" H 6200 4300 50  0001 C CNN
F 4 "KMR211NG LFS" H 6200 4300 60  0001 C CNN "Part Number"
	1    6200 4300
	0    1    1    0   
$EndComp
Text GLabel 6800 4950 2    60   Input ~ 0
IO0
Text GLabel 6450 3900 2    60   Input ~ 0
EN
$Comp
L GND #PWR22
U 1 1 59092D1C
P 6200 5100
F 0 "#PWR22" H 6200 4850 50  0001 C CNN
F 1 "GND" H 6200 4950 50  0000 C CNN
F 2 "" H 6200 5100 50  0000 C CNN
F 3 "" H 6200 5100 50  0000 C CNN
	1    6200 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR21
U 1 1 59092D1D
P 5500 5100
F 0 "#PWR21" H 5500 4850 50  0001 C CNN
F 1 "GND" H 5500 4950 50  0000 C CNN
F 2 "" H 5500 5100 50  0000 C CNN
F 3 "" H 5500 5100 50  0000 C CNN
	1    5500 5100
	1    0    0    -1  
$EndComp
$Comp
L Q_PMOS_GSD Q3
U 1 1 59092D1E
P 5850 3900
F 0 "Q3" H 6150 3950 50  0000 R CNN
F 1 "Q_PMOS_GSD" H 6150 3700 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 6050 4000 50  0001 C CNN
F 3 "https://www.fairchildsemi.com/datasheets/BS/BSS84.pdf" H 5850 3900 50  0001 C CNN
F 4 "BSS84" H 5850 3900 60  0001 C CNN "Part Number"
	1    5850 3900
	-1   0    0    1   
$EndComp
$Comp
L Q_NMOS_GSD Q2
U 1 1 59092D1F
P 5750 4850
F 0 "Q2" V 5650 5000 50  0000 R CNN
F 1 "Q_NMOS_GSD" V 5950 5100 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 5950 4950 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/308/2N7002L-D-105484.pdf" H 5750 4850 50  0001 C CNN
F 4 "2N7002L" V 5750 4850 60  0001 C CNN "Part Number"
	1    5750 4850
	0    1    1    0   
$EndComp
$Comp
L R R14
U 1 1 59092D20
P 5750 4400
F 0 "R14" V 5830 4400 50  0000 C CNN
F 1 "1K" V 5750 4400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5680 4400 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 5750 4400 50  0001 C CNN
F 4 "RC0603JR-071KL" V 5750 4400 60  0001 C CNN "Part Number"
	1    5750 4400
	1    0    0    -1  
$EndComp
$Comp
L R R15
U 1 1 59092D21
P 6200 3650
F 0 "R15" V 6280 3650 50  0000 C CNN
F 1 "1K" V 6200 3650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6130 3650 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 6200 3650 50  0001 C CNN
F 4 "RC0603JR-071KL" V 6200 3650 60  0001 C CNN "Part Number"
	1    6200 3650
	1    0    0    -1  
$EndComp
$Comp
L R R16
U 1 1 59092D22
P 6500 4950
F 0 "R16" V 6580 4950 50  0000 C CNN
F 1 "1K" V 6500 4950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6430 4950 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 6500 4950 50  0001 C CNN
F 4 "RC0603JR-071KL" V 6500 4950 60  0001 C CNN "Part Number"
	1    6500 4950
	0    1    1    0   
$EndComp
$Comp
L C C5
U 1 1 59092D23
P 5500 4750
F 0 "C5" H 5525 4850 50  0000 L CNN
F 1 "4.7uF" H 5500 4650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 5538 4600 50  0001 C CNN
F 3 "http://media.digikey.com/pdf/Data%20Sheets/Samsung%20PDFs/CL_Series_MLCC_ds.pdf" H 5500 4750 50  0001 C CNN
F 4 "CL10B475KQ8NQNC" H 5500 4750 60  0001 C CNN "Part Number"
	1    5500 4750
	-1   0    0    1   
$EndComp
$Comp
L LED D2
U 1 1 59092D24
P 6200 3200
F 0 "D2" H 6200 3300 50  0000 C CNN
F 1 "LED" H 6350 3150 50  0000 C CNN
F 2 "LEDs:LED_0603" H 6200 3200 50  0001 C CNN
F 3 "http://www.osram-os.com/Graphics/XPic9/00128792_0.pdf" H 6200 3200 50  0001 C CNN
F 4 "475-2512-2-ND" H 6200 3200 60  0001 C CNN "Part Number"
	1    6200 3200
	0    -1   -1   0   
$EndComp
Text Label 2800 3150 0    60   ~ 0
Switches_N_Stuff
$Comp
L LED-switch SW1
U 1 1 59092D25
P 3400 4050
F 0 "SW1" H 3500 4250 60  0000 C CNN
F 1 "LED-switch" H 3350 3750 60  0000 C CNN
F 2 "footprints:SMT-led-switch" H 3400 4050 60  0001 C CNN
F 3 "http://www.ckswitches.com/media/1481/k8.pdf" H 3400 4050 60  0001 C CNN
F 4 "CKN10530TR-ND" H 3400 4050 60  0001 C CNN "Part Number"
	1    3400 4050
	1    0    0    -1  
$EndComp
$Comp
L R R13
U 1 1 59092D26
P 3900 3900
F 0 "R13" V 3980 3900 50  0000 C CNN
F 1 "36" V 3900 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3830 3900 50  0001 C CNN
F 3 "http://www.yageo.com.tw/exep/pages/download/literatures/PYu-R_INT-thick_7.pdf" H 3900 3900 50  0001 C CNN
F 4 "RC0603JR-0736RL" V 3900 3900 60  0001 C CNN "Part Number"
	1    3900 3900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR20
U 1 1 59092D28
P 2850 4750
F 0 "#PWR20" H 2850 4500 50  0001 C CNN
F 1 "GND" H 2850 4600 50  0000 C CNN
F 2 "" H 2850 4750 50  0000 C CNN
F 3 "" H 2850 4750 50  0000 C CNN
	1    2850 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 3700 3900 3700
Connection ~ 3900 3700
Connection ~ 2900 4000
Wire Wire Line
	2850 4000 2900 4000
Wire Wire Line
	2850 3700 2850 4000
Wire Wire Line
	3900 4250 3900 4300
Wire Wire Line
	3800 4250 3900 4250
Wire Wire Line
	2900 3950 2900 4050
Wire Wire Line
	2750 4250 2900 4250
Wire Wire Line
	2850 4250 2850 4350
Wire Wire Line
	3900 3650 3900 3750
Wire Wire Line
	3900 4150 3900 4050
Wire Wire Line
	3800 4150 3900 4150
Wire Wire Line
	6200 2900 6200 3050
Wire Wire Line
	6650 4950 6800 4950
Wire Wire Line
	6200 4600 6200 5100
Wire Wire Line
	5950 4950 6350 4950
Wire Wire Line
	5500 4600 5750 4600
Connection ~ 5500 4950
Wire Wire Line
	5500 4900 5500 5100
Wire Wire Line
	5550 4950 5500 4950
Connection ~ 5750 4600
Wire Wire Line
	5750 4550 5750 4650
Connection ~ 6200 3900
Wire Wire Line
	6200 3800 6200 4000
Wire Wire Line
	6050 3900 6450 3900
Wire Wire Line
	5750 4250 5750 4100
Connection ~ 6200 3450
Wire Wire Line
	5750 3450 5750 3700
Wire Wire Line
	6200 3450 5750 3450
Wire Wire Line
	6200 3350 6200 3500
Wire Wire Line
	2850 4750 2850 4650
Wire Wire Line
	2750 4200 2750 4250
Connection ~ 2850 4250
Text GLabel 8100 3400 0    60   Input ~ 0
IO32
Text GLabel 8100 3900 0    60   Input ~ 0
IO33
Text GLabel 8100 4400 0    60   Input ~ 0
IO27
Text GLabel 8100 4900 0    60   Input ~ 0
IO14
Wire Wire Line
	8100 3900 8200 3900
Wire Wire Line
	8200 4400 8100 4400
Wire Wire Line
	8100 4900 8200 4900
Text GLabel 2750 4200 1    60   Input ~ 0
IO21
Wire Wire Line
	8100 3400 8200 3400
$Comp
L touchpad U7
U 1 1 590F873E
P 8600 3400
F 0 "U7" H 8750 3200 60  0000 C CNN
F 1 "touchpad" H 8600 3600 60  0000 C CNN
F 2 "footprints:touch-pad" H 8600 3400 60  0001 C CNN
F 3 "" H 8600 3400 60  0001 C CNN
	1    8600 3400
	1    0    0    -1  
$EndComp
$Comp
L touchpad U8
U 1 1 590F8CCA
P 8600 3900
F 0 "U8" H 8750 3700 60  0000 C CNN
F 1 "touchpad" H 8600 4100 60  0000 C CNN
F 2 "footprints:touch-pad" H 8600 3900 60  0001 C CNN
F 3 "" H 8600 3900 60  0001 C CNN
	1    8600 3900
	1    0    0    -1  
$EndComp
$Comp
L touchpad U9
U 1 1 590F8D35
P 8600 4400
F 0 "U9" H 8750 4200 60  0000 C CNN
F 1 "touchpad" H 8600 4600 60  0000 C CNN
F 2 "footprints:touch-pad" H 8600 4400 60  0001 C CNN
F 3 "" H 8600 4400 60  0001 C CNN
	1    8600 4400
	1    0    0    -1  
$EndComp
$Comp
L touchpad U10
U 1 1 590F8D3B
P 8600 4900
F 0 "U10" H 8750 4700 60  0000 C CNN
F 1 "touchpad" H 8600 5100 60  0000 C CNN
F 2 "footprints:touch-pad" H 8600 4900 60  0001 C CNN
F 3 "" H 8600 4900 60  0001 C CNN
	1    8600 4900
	1    0    0    -1  
$EndComp
Text GLabel 6200 2900 1    60   Input ~ 0
5V
Text GLabel 3900 3650 1    60   Input ~ 0
3V3
$Comp
L soft-button-4x U12
U 1 1 597D230D
P 9200 1900
F 0 "U12" H 9550 2150 60  0000 C CNN
F 1 "soft-button-4x" H 9650 1450 60  0000 C CNN
F 2 "footprints:soft-button-4" H 9200 1900 60  0001 C CNN
F 3 "" H 9200 1900 60  0001 C CNN
	1    9200 1900
	1    0    0    -1  
$EndComp
Text GLabel 8750 2000 0    60   Input ~ 0
IO32
Text GLabel 9850 2000 2    60   Input ~ 0
IO33
Text GLabel 9300 1450 1    60   Input ~ 0
IO27
Text GLabel 9300 2550 3    60   Input ~ 0
IO14
Wire Wire Line
	9300 1450 9300 1500
Wire Wire Line
	8750 2000 8800 2000
Wire Wire Line
	9300 2500 9300 2550
Wire Wire Line
	9850 2000 9800 2000
$EndSCHEMATC
