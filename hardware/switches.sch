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
Sheet 6 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 6550 4700 0    60   Input ~ 0
LED_R
$Comp
L SW_PUSH PROG1
U 1 1 59092D1B
P 3900 4200
F 0 "PROG1" H 4050 4310 50  0000 C CNN
F 1 "SW_PUSH" H 3900 4120 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_KMR2" H 3900 4200 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/60/KMR2_9aug12-25127.pdf" H 3900 4200 50  0001 C CNN
F 4 "KMR211NG LFS" H 3900 4200 60  0001 C CNN "Part Number"
	1    3900 4200
	0    1    1    0   
$EndComp
Text GLabel 4500 4850 2    60   Input ~ 0
IO0
Text GLabel 4150 3800 2    60   Input ~ 0
EN
$Comp
L GND #PWR014
U 1 1 59092D1C
P 3900 5000
F 0 "#PWR014" H 3900 4750 50  0001 C CNN
F 1 "GND" H 3900 4850 50  0000 C CNN
F 2 "" H 3900 5000 50  0000 C CNN
F 3 "" H 3900 5000 50  0000 C CNN
	1    3900 5000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 59092D1D
P 3200 5000
F 0 "#PWR015" H 3200 4750 50  0001 C CNN
F 1 "GND" H 3200 4850 50  0000 C CNN
F 2 "" H 3200 5000 50  0000 C CNN
F 3 "" H 3200 5000 50  0000 C CNN
	1    3200 5000
	1    0    0    -1  
$EndComp
$Comp
L Q_PMOS_GSD Q3
U 1 1 59092D1E
P 3550 3800
F 0 "Q3" H 3850 3850 50  0000 R CNN
F 1 "Q_PMOS_GSD" H 3850 3600 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3750 3900 50  0001 C CNN
F 3 "https://www.fairchildsemi.com/datasheets/BS/BSS84.pdf" H 3550 3800 50  0001 C CNN
F 4 "BSS84" H 3550 3800 60  0001 C CNN "Part Number"
	1    3550 3800
	-1   0    0    1   
$EndComp
$Comp
L Q_NMOS_GSD Q2
U 1 1 59092D1F
P 3450 4750
F 0 "Q2" V 3350 4900 50  0000 R CNN
F 1 "Q_NMOS_GSD" V 3650 5000 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 3650 4850 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/308/2N7002L-D-105484.pdf" H 3450 4750 50  0001 C CNN
F 4 "2N7002L" V 3450 4750 60  0001 C CNN "Part Number"
	1    3450 4750
	0    1    1    0   
$EndComp
$Comp
L C C5
U 1 1 59092D23
P 3200 4650
F 0 "C5" H 3225 4750 50  0000 L CNN
F 1 "4.7uF" H 3200 4550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 3238 4500 50  0001 C CNN
F 3 "http://media.digikey.com/pdf/Data%20Sheets/Samsung%20PDFs/CL_Series_MLCC_ds.pdf" H 3200 4650 50  0001 C CNN
F 4 "CL10B475KQ8NQNC" H 3200 4650 60  0001 C CNN "Part Number"
	1    3200 4650
	-1   0    0    1   
$EndComp
$Comp
L LED D2
U 1 1 59092D24
P 3900 3100
F 0 "D2" H 3900 3200 50  0000 C CNN
F 1 "LED" H 4050 3050 50  0000 C CNN
F 2 "LEDs:LED_0603" H 3900 3100 50  0001 C CNN
F 3 "http://www.osram-os.com/Graphics/XPic9/00128792_0.pdf" H 3900 3100 50  0001 C CNN
F 4 "475-2512-2-ND" H 3900 3100 60  0001 C CNN "Part Number"
	1    3900 3100
	0    -1   -1   0   
$EndComp
Text Label 800  850  0    60   ~ 0
Switches_N_Stuff
Text GLabel 3900 2800 1    60   Input ~ 0
V_USB
$Comp
L soft-button-4x U12
U 1 1 597D230D
P 7100 3100
F 0 "U12" H 7450 3350 60  0000 C CNN
F 1 "soft-button-4x" H 7550 2650 60  0000 C CNN
F 2 "open-automation:soft-button-4" H 7100 3100 60  0001 C CNN
F 3 "" H 7100 3100 60  0001 C CNN
	1    7100 3100
	1    0    0    -1  
$EndComp
Text GLabel 6650 3200 0    60   Input ~ 0
T_LEFT
Text GLabel 7750 3200 2    60   Input ~ 0
T_RIGHT
Text GLabel 7200 2650 1    60   Input ~ 0
T_UP
Text GLabel 7200 3750 3    60   Input ~ 0
T_DOWN
$Comp
L RGB-LED U7
U 1 1 5980E7D1
P 7150 4700
F 0 "U7" H 7400 4400 60  0000 C CNN
F 1 "RGB-LED" H 7150 5000 60  0000 C CNN
F 2 "open-automation:RGB-LED-J-Lead-SMD-6" H 7300 4700 60  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/osram-opto-semiconductors-inc/LTRBGFSF-ABCB-QKYO-Z/475-2899-6-ND/2415092" H 7300 4700 60  0001 C CNN
F 4 "LTRBGFSF-ABCB-QKYO-Z" H 7150 4700 60  0001 C CNN "Part Number"
	1    7150 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 2800 3900 2950
Wire Wire Line
	4350 4850 4500 4850
Wire Wire Line
	3900 4500 3900 5000
Wire Wire Line
	3650 4850 4050 4850
Wire Wire Line
	3200 4500 3450 4500
Connection ~ 3200 4850
Wire Wire Line
	3200 4800 3200 5000
Wire Wire Line
	3250 4850 3200 4850
Connection ~ 3450 4500
Wire Wire Line
	3450 4450 3450 4550
Connection ~ 3900 3800
Wire Wire Line
	3900 3700 3900 3900
Wire Wire Line
	3750 3800 4150 3800
Wire Wire Line
	3450 4150 3450 4000
Connection ~ 3900 3350
Wire Wire Line
	3450 3350 3450 3600
Wire Wire Line
	3900 3350 3450 3350
Wire Wire Line
	3900 3250 3900 3400
Wire Wire Line
	7200 2650 7200 2700
Wire Wire Line
	6650 3200 6700 3200
Wire Wire Line
	7200 3700 7200 3750
Wire Wire Line
	7750 3200 7700 3200
Text GLabel 6550 4850 0    60   Input ~ 0
LED_G
Text GLabel 6550 4550 0    60   Input ~ 0
LED_B
Wire Wire Line
	6550 4550 6650 4550
Wire Wire Line
	6550 4700 6650 4700
Wire Wire Line
	6550 4850 6650 4850
$Comp
L R R8
U 1 1 5980F245
P 7900 4550
F 0 "R8" V 7980 4550 50  0000 C CNN
F 1 "36" V 7900 4550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7830 4550 50  0001 C CNN
F 3 "http://www.yageo.com.tw/exep/pages/download/literatures/PYu-R_INT-thick_7.pdf" H 7900 4550 50  0001 C CNN
F 4 "RC0603JR-0736RL" V 7900 4550 60  0001 C CNN "Part Number"
	1    7900 4550
	0    1    1    0   
$EndComp
Text GLabel 8150 4450 1    60   Input ~ 0
3V3
$Comp
L R R12
U 1 1 5980F487
P 7900 4700
F 0 "R12" V 7980 4700 50  0000 C CNN
F 1 "36" V 7900 4700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7830 4700 50  0001 C CNN
F 3 "http://www.yageo.com.tw/exep/pages/download/literatures/PYu-R_INT-thick_7.pdf" H 7900 4700 50  0001 C CNN
F 4 "RC0603JR-0736RL" V 7900 4700 60  0001 C CNN "Part Number"
	1    7900 4700
	0    1    1    0   
$EndComp
$Comp
L R R13
U 1 1 5980F4D4
P 7900 4850
F 0 "R13" V 7980 4850 50  0000 C CNN
F 1 "36" V 7900 4850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7830 4850 50  0001 C CNN
F 3 "http://www.yageo.com.tw/exep/pages/download/literatures/PYu-R_INT-thick_7.pdf" H 7900 4850 50  0001 C CNN
F 4 "RC0603JR-0736RL" V 7900 4850 60  0001 C CNN "Part Number"
	1    7900 4850
	0    1    1    0   
$EndComp
Wire Wire Line
	7650 4550 7750 4550
Wire Wire Line
	7650 4700 7750 4700
Wire Wire Line
	7650 4850 7750 4850
Wire Wire Line
	8150 4450 8150 4850
Wire Wire Line
	8150 4550 8050 4550
Wire Wire Line
	8150 4700 8050 4700
Connection ~ 8150 4550
Wire Wire Line
	8150 4850 8050 4850
Connection ~ 8150 4700
$Comp
L R_1k R14
U 1 1 59BA2D07
P 3450 4300
F 0 "R14" V 3450 4300 50  0000 C CNN
F 1 "R_1k" V 3375 4300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3380 4300 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" V 3530 4300 50  0001 C CNN
F 4 "RC0603JR-071KL" V 3630 4400 60  0001 C CNN "Part Number"
	1    3450 4300
	1    0    0    -1  
$EndComp
$Comp
L R_1k R16
U 1 1 59BA2D49
P 4200 4850
F 0 "R16" V 4200 4850 50  0000 C CNN
F 1 "R_1k" V 4125 4850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4130 4850 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" V 4280 4850 50  0001 C CNN
F 4 "RC0603JR-071KL" V 4380 4950 60  0001 C CNN "Part Number"
	1    4200 4850
	0    1    1    0   
$EndComp
$Comp
L R_1k R15
U 1 1 59BA2DC7
P 3900 3550
F 0 "R15" V 3900 3550 50  0000 C CNN
F 1 "R_1k" V 3825 3550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3830 3550 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" V 3980 3550 50  0001 C CNN
F 4 "RC0603JR-071KL" V 4080 3650 60  0001 C CNN "Part Number"
	1    3900 3550
	-1   0    0    1   
$EndComp
$EndSCHEMATC
