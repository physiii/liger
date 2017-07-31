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
P 2700 4250
F 0 "R12" V 2780 4250 50  0000 C CNN
F 1 "1K" V 2700 4250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2630 4250 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 2700 4250 50  0001 C CNN
F 4 "RC0603JR-071KL" V 2700 4250 60  0001 C CNN "Part Number"
	1    2700 4250
	-1   0    0    1   
$EndComp
Text GLabel 3750 4050 3    60   Input ~ 0
IO5
$Comp
L SW_PUSH PROG1
U 1 1 59092D1B
P 6050 4050
F 0 "PROG1" H 6200 4160 50  0000 C CNN
F 1 "SW_PUSH" H 6050 3970 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_KMR2" H 6050 4050 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/60/KMR2_9aug12-25127.pdf" H 6050 4050 50  0001 C CNN
F 4 "KMR211NG LFS" H 6050 4050 60  0001 C CNN "Part Number"
	1    6050 4050
	0    1    1    0   
$EndComp
Text GLabel 6650 4700 2    60   Input ~ 0
IO0
Text GLabel 6300 3650 2    60   Input ~ 0
EN
$Comp
L GND #PWR22
U 1 1 59092D1C
P 6050 4850
F 0 "#PWR22" H 6050 4600 50  0001 C CNN
F 1 "GND" H 6050 4700 50  0000 C CNN
F 2 "" H 6050 4850 50  0000 C CNN
F 3 "" H 6050 4850 50  0000 C CNN
	1    6050 4850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR21
U 1 1 59092D1D
P 5350 4850
F 0 "#PWR21" H 5350 4600 50  0001 C CNN
F 1 "GND" H 5350 4700 50  0000 C CNN
F 2 "" H 5350 4850 50  0000 C CNN
F 3 "" H 5350 4850 50  0000 C CNN
	1    5350 4850
	1    0    0    -1  
$EndComp
$Comp
L Q_PMOS_GSD Q3
U 1 1 59092D1E
P 5700 3650
F 0 "Q3" H 6000 3700 50  0000 R CNN
F 1 "Q_PMOS_GSD" H 6000 3450 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 5900 3750 50  0001 C CNN
F 3 "https://www.fairchildsemi.com/datasheets/BS/BSS84.pdf" H 5700 3650 50  0001 C CNN
F 4 "BSS84" H 5700 3650 60  0001 C CNN "Part Number"
	1    5700 3650
	-1   0    0    1   
$EndComp
$Comp
L Q_NMOS_GSD Q2
U 1 1 59092D1F
P 5600 4600
F 0 "Q2" V 5500 4750 50  0000 R CNN
F 1 "Q_NMOS_GSD" V 5800 4850 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 5800 4700 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/308/2N7002L-D-105484.pdf" H 5600 4600 50  0001 C CNN
F 4 "2N7002L" V 5600 4600 60  0001 C CNN "Part Number"
	1    5600 4600
	0    1    1    0   
$EndComp
$Comp
L R R14
U 1 1 59092D20
P 5600 4150
F 0 "R14" V 5680 4150 50  0000 C CNN
F 1 "1K" V 5600 4150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5530 4150 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 5600 4150 50  0001 C CNN
F 4 "RC0603JR-071KL" V 5600 4150 60  0001 C CNN "Part Number"
	1    5600 4150
	1    0    0    -1  
$EndComp
$Comp
L R R15
U 1 1 59092D21
P 6050 3400
F 0 "R15" V 6130 3400 50  0000 C CNN
F 1 "1K" V 6050 3400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5980 3400 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 6050 3400 50  0001 C CNN
F 4 "RC0603JR-071KL" V 6050 3400 60  0001 C CNN "Part Number"
	1    6050 3400
	1    0    0    -1  
$EndComp
$Comp
L R R16
U 1 1 59092D22
P 6350 4700
F 0 "R16" V 6430 4700 50  0000 C CNN
F 1 "1K" V 6350 4700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6280 4700 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 6350 4700 50  0001 C CNN
F 4 "RC0603JR-071KL" V 6350 4700 60  0001 C CNN "Part Number"
	1    6350 4700
	0    1    1    0   
$EndComp
$Comp
L C C5
U 1 1 59092D23
P 5350 4500
F 0 "C5" H 5375 4600 50  0000 L CNN
F 1 "4.7uF" H 5350 4400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 5388 4350 50  0001 C CNN
F 3 "http://media.digikey.com/pdf/Data%20Sheets/Samsung%20PDFs/CL_Series_MLCC_ds.pdf" H 5350 4500 50  0001 C CNN
F 4 "CL10B475KQ8NQNC" H 5350 4500 60  0001 C CNN "Part Number"
	1    5350 4500
	-1   0    0    1   
$EndComp
$Comp
L LED D2
U 1 1 59092D24
P 6050 2950
F 0 "D2" H 6050 3050 50  0000 C CNN
F 1 "LED" H 6200 2900 50  0000 C CNN
F 2 "LEDs:LED_0603" H 6050 2950 50  0001 C CNN
F 3 "http://www.osram-os.com/Graphics/XPic9/00128792_0.pdf" H 6050 2950 50  0001 C CNN
F 4 "475-2512-2-ND" H 6050 2950 60  0001 C CNN "Part Number"
	1    6050 2950
	0    -1   -1   0   
$EndComp
Text Label 2650 2900 0    60   ~ 0
Switches_N_Stuff
$Comp
L LED-switch SW1
U 1 1 59092D25
P 3250 3800
F 0 "SW1" H 3350 4000 60  0000 C CNN
F 1 "LED-switch" H 3200 3500 60  0000 C CNN
F 2 "footprints:SMT-led-switch" H 3250 3800 60  0001 C CNN
F 3 "http://www.ckswitches.com/media/1481/k8.pdf" H 3250 3800 60  0001 C CNN
F 4 "CKN10530TR-ND" H 3250 3800 60  0001 C CNN "Part Number"
	1    3250 3800
	1    0    0    -1  
$EndComp
$Comp
L R R13
U 1 1 59092D26
P 3750 3650
F 0 "R13" V 3830 3650 50  0000 C CNN
F 1 "36" V 3750 3650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3680 3650 50  0001 C CNN
F 3 "http://www.yageo.com.tw/exep/pages/download/literatures/PYu-R_INT-thick_7.pdf" H 3750 3650 50  0001 C CNN
F 4 "RC0603JR-0736RL" V 3750 3650 60  0001 C CNN "Part Number"
	1    3750 3650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR20
U 1 1 59092D28
P 2700 4500
F 0 "#PWR20" H 2700 4250 50  0001 C CNN
F 1 "GND" H 2700 4350 50  0000 C CNN
F 2 "" H 2700 4500 50  0000 C CNN
F 3 "" H 2700 4500 50  0000 C CNN
	1    2700 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 3450 3750 3450
Connection ~ 3750 3450
Connection ~ 2750 3750
Wire Wire Line
	2700 3750 2750 3750
Wire Wire Line
	2700 3450 2700 3750
Wire Wire Line
	3750 4000 3750 4050
Wire Wire Line
	3650 4000 3750 4000
Wire Wire Line
	2750 3700 2750 3800
Wire Wire Line
	2600 4000 2750 4000
Wire Wire Line
	2700 4000 2700 4100
Wire Wire Line
	3750 3400 3750 3500
Wire Wire Line
	3750 3900 3750 3800
Wire Wire Line
	3650 3900 3750 3900
Wire Wire Line
	6050 2650 6050 2800
Wire Wire Line
	6500 4700 6650 4700
Wire Wire Line
	6050 4350 6050 4850
Wire Wire Line
	5800 4700 6200 4700
Wire Wire Line
	5350 4350 5600 4350
Connection ~ 5350 4700
Wire Wire Line
	5350 4650 5350 4850
Wire Wire Line
	5400 4700 5350 4700
Connection ~ 5600 4350
Wire Wire Line
	5600 4300 5600 4400
Connection ~ 6050 3650
Wire Wire Line
	6050 3550 6050 3750
Wire Wire Line
	5900 3650 6300 3650
Wire Wire Line
	5600 4000 5600 3850
Connection ~ 6050 3200
Wire Wire Line
	5600 3200 5600 3450
Wire Wire Line
	6050 3200 5600 3200
Wire Wire Line
	6050 3100 6050 3250
Wire Wire Line
	2700 4500 2700 4400
Wire Wire Line
	2600 3950 2600 4000
Connection ~ 2700 4000
Text GLabel 2600 3950 1    60   Input ~ 0
IO21
Text GLabel 6050 2650 1    60   Input ~ 0
5V
Text GLabel 3750 3400 1    60   Input ~ 0
3V3
$Comp
L soft-button-4x U12
U 1 1 597D230D
P 8700 3600
F 0 "U12" H 9050 3850 60  0000 C CNN
F 1 "soft-button-4x" H 9150 3150 60  0000 C CNN
F 2 "footprints:soft-button-4" H 8700 3600 60  0001 C CNN
F 3 "" H 8700 3600 60  0001 C CNN
	1    8700 3600
	1    0    0    -1  
$EndComp
Text GLabel 8250 3700 0    60   Input ~ 0
IO32
Text GLabel 9350 3700 2    60   Input ~ 0
IO33
Text GLabel 8800 3150 1    60   Input ~ 0
IO27
Text GLabel 8800 4250 3    60   Input ~ 0
IO0
Wire Wire Line
	8800 3150 8800 3200
Wire Wire Line
	8250 3700 8300 3700
Wire Wire Line
	8800 4200 8800 4250
Wire Wire Line
	9350 3700 9300 3700
$EndSCHEMATC
