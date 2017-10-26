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
L SW_PUSH REBOOT1
U 1 1 59092D1B
P 3900 4200
F 0 "REBOOT1" V 4125 4375 50  0000 C CNN
F 1 "SW_PUSH" H 3900 4120 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_KMR2" H 3900 4200 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/60/KMR2_9aug12-25127.pdf" H 3900 4200 50  0001 C CNN
F 4 "KMR211NG LFS" H 3900 4200 60  0001 C CNN "Part Number"
	1    3900 4200
	0    1    1    0   
$EndComp
Text GLabel 3250 3800 2    60   Input ~ 0
PROG
Text GLabel 4150 3800 2    60   Input ~ 0
EN
$Comp
L GND #PWR018
U 1 1 59092D1C
P 3900 4575
F 0 "#PWR018" H 3900 4325 50  0001 C CNN
F 1 "GND" H 3900 4425 50  0000 C CNN
F 2 "" H 3900 4575 50  0000 C CNN
F 3 "" H 3900 4575 50  0000 C CNN
	1    3900 4575
	1    0    0    -1  
$EndComp
$Comp
L LED D2
U 1 1 59092D24
P 3900 3100
F 0 "D2" H 3900 3200 50  0000 C CNN
F 1 "LED" V 4025 3025 50  0000 C CNN
F 2 "LEDs:LED_0603_HandSoldering" H 3900 3100 50  0001 C CNN
F 3 "http://www.osram-os.com/Graphics/XPic9/00128792_0.pdf" H 3900 3100 50  0001 C CNN
F 4 "475-2512-2-ND" H 3900 3100 60  0001 C CNN "Part Number"
	1    3900 3100
	0    -1   -1   0   
$EndComp
Text Label 800  850  0    60   ~ 0
Switches_N_Stuff
Text GLabel 3900 2800 1    60   Input ~ 0
3V3
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
Connection ~ 3900 3800
Wire Wire Line
	3900 3700 3900 3900
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
F 2 "Resistors_SMD:R_0603_HandSoldering" V 7830 4550 50  0001 C CNN
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
F 2 "Resistors_SMD:R_0603_HandSoldering" V 7830 4700 50  0001 C CNN
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
F 2 "Resistors_SMD:R_0603_HandSoldering" V 7830 4850 50  0001 C CNN
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
L R_1k R15
U 1 1 59BA2DC7
P 3900 3550
F 0 "R15" V 3900 3550 50  0000 C CNN
F 1 "R_1k" V 3825 3550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3830 3550 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" V 3980 3550 50  0001 C CNN
F 4 "RC0603JR-071KL" V 4080 3650 60  0001 C CNN "Part Number"
	1    3900 3550
	-1   0    0    1   
$EndComp
Wire Wire Line
	4150 3800 3900 3800
$Comp
L SW_PUSH PROG1
U 1 1 59CA58CF
P 3000 4200
F 0 "PROG1" V 3225 4400 50  0000 C CNN
F 1 "SW_PUSH" H 3000 4120 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_KMR2" H 3000 4200 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/60/KMR2_9aug12-25127.pdf" H 3000 4200 50  0001 C CNN
F 4 "KMR211NG LFS" H 3000 4200 60  0001 C CNN "Part Number"
	1    3000 4200
	0    1    1    0   
$EndComp
$Comp
L GND #PWR019
U 1 1 59CA58D6
P 3000 4575
F 0 "#PWR019" H 3000 4325 50  0001 C CNN
F 1 "GND" H 3000 4425 50  0000 C CNN
F 2 "" H 3000 4575 50  0000 C CNN
F 3 "" H 3000 4575 50  0000 C CNN
	1    3000 4575
	1    0    0    -1  
$EndComp
$Comp
L LED D5
U 1 1 59CA58DD
P 3000 3100
F 0 "D5" H 3000 3200 50  0000 C CNN
F 1 "LED" V 3125 3025 50  0000 C CNN
F 2 "LEDs:LED_0603_HandSoldering" H 3000 3100 50  0001 C CNN
F 3 "http://www.osram-os.com/Graphics/XPic9/00128792_0.pdf" H 3000 3100 50  0001 C CNN
F 4 "475-2512-2-ND" H 3000 3100 60  0001 C CNN "Part Number"
	1    3000 3100
	0    -1   -1   0   
$EndComp
Text GLabel 3000 2800 1    60   Input ~ 0
3V3
Wire Wire Line
	3000 2800 3000 2950
Connection ~ 3000 3800
Wire Wire Line
	3000 3700 3000 3900
Wire Wire Line
	3000 3250 3000 3400
$Comp
L R_1k R16
U 1 1 59CA58E9
P 3000 3550
F 0 "R16" V 3000 3550 50  0000 C CNN
F 1 "R_1k" V 2925 3550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2930 3550 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" V 3080 3550 50  0001 C CNN
F 4 "RC0603JR-071KL" V 3180 3650 60  0001 C CNN "Part Number"
	1    3000 3550
	-1   0    0    1   
$EndComp
Wire Wire Line
	3250 3800 3000 3800
Wire Wire Line
	3000 4500 3000 4575
Wire Wire Line
	3900 4500 3900 4575
Text GLabel 2375 3800 2    60   Input ~ 0
FACTORY
$Comp
L SW_PUSH FACTORY1
U 1 1 59CA6153
P 2125 4200
F 0 "FACTORY1" V 2350 4400 50  0000 C CNN
F 1 "SW_PUSH" H 2125 4120 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_KMR2" H 2125 4200 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/60/KMR2_9aug12-25127.pdf" H 2125 4200 50  0001 C CNN
F 4 "KMR211NG LFS" H 2125 4200 60  0001 C CNN "Part Number"
	1    2125 4200
	0    1    1    0   
$EndComp
$Comp
L GND #PWR020
U 1 1 59CA6159
P 2125 4575
F 0 "#PWR020" H 2125 4325 50  0001 C CNN
F 1 "GND" H 2125 4425 50  0000 C CNN
F 2 "" H 2125 4575 50  0000 C CNN
F 3 "" H 2125 4575 50  0000 C CNN
	1    2125 4575
	1    0    0    -1  
$EndComp
$Comp
L LED D4
U 1 1 59CA6160
P 2125 3100
F 0 "D4" H 2125 3200 50  0000 C CNN
F 1 "LED" V 2250 3025 50  0000 C CNN
F 2 "LEDs:LED_0603_HandSoldering" H 2125 3100 50  0001 C CNN
F 3 "http://www.osram-os.com/Graphics/XPic9/00128792_0.pdf" H 2125 3100 50  0001 C CNN
F 4 "475-2512-2-ND" H 2125 3100 60  0001 C CNN "Part Number"
	1    2125 3100
	0    -1   -1   0   
$EndComp
Text GLabel 2125 2800 1    60   Input ~ 0
3V3
Wire Wire Line
	2125 2800 2125 2950
Connection ~ 2125 3800
Wire Wire Line
	2125 3700 2125 3900
Wire Wire Line
	2125 3250 2125 3400
$Comp
L R_1k R14
U 1 1 59CA616C
P 2125 3550
F 0 "R14" V 2125 3550 50  0000 C CNN
F 1 "R_1k" V 2050 3550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2055 3550 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" V 2205 3550 50  0001 C CNN
F 4 "RC0603JR-071KL" V 2305 3650 60  0001 C CNN "Part Number"
	1    2125 3550
	-1   0    0    1   
$EndComp
Wire Wire Line
	2375 3800 2125 3800
Wire Wire Line
	2125 4500 2125 4575
$EndSCHEMATC
