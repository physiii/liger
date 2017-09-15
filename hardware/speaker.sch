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
Sheet 7 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 6700 4300 3    60   Input ~ 0
SPEAKER_IN
Text Label 4050 2200 0    60   ~ 0
speaker_driver
$Comp
L Speaker LS1
U 1 1 590F705D
P 6850 3150
F 0 "LS1" H 6900 3375 50  0000 R CNN
F 1 "Speaker" H 6900 3300 50  0000 R CNN
F 2 "open-automation:speaker-1508" H 6850 2950 50  0001 C CNN
F 3 "http://www.cui.com/product/resource/cvs-1508.pdf" H 6840 3100 50  0001 C CNN
F 4 "CVS-1508" H 6850 3150 60  0001 C CNN "Part Number"
	1    6850 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 590F73ED
P 5400 3400
F 0 "#PWR016" H 5400 3150 50  0001 C CNN
F 1 "GND" H 5400 3250 50  0000 C CNN
F 2 "" H 5400 3400 50  0000 C CNN
F 3 "" H 5400 3400 50  0000 C CNN
	1    5400 3400
	1    0    0    -1  
$EndComp
Text GLabel 5350 3250 0    60   Input ~ 0
V_USB
$Comp
L Amplifier-D-TS4962IQT U8
U 1 1 59812510
P 5900 3250
F 0 "U8" H 6150 2950 60  0000 C CNN
F 1 "Amplifier-D-TS4962IQT" H 5900 3500 60  0000 C CNN
F 2 "open-automation:Amplifier-D-TS4962IQT" H 5600 2800 60  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/stmicroelectronics/TS4962IQT/497-5993-1-ND/1578476" H 5600 2800 60  0001 C CNN
F 4 "TS4962IQT" H 5900 3250 60  0001 C CNN "Part Number"
	1    5900 3250
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 59812A7F
P 6500 4050
F 0 "C6" H 6500 4150 50  0000 L CNN
F 1 "4.7uF" H 6550 3950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6615 4005 50  0001 L CNN
F 3 "https://www.digikey.com/product-detail/en/samsung-electro-mechanics-america-inc/CL10B475KQ8NQNC/1276-2087-1-ND/3890173" H 6615 3914 50  0001 L CNN
F 4 "CL10B475KQ8NQNC" H 6500 4050 60  0001 C CNN "Part Number"
	1    6500 4050
	-1   0    0    1   
$EndComp
$Comp
L C C7
U 1 1 59812D76
P 6700 4050
F 0 "C7" H 6600 4150 50  0000 L CNN
F 1 "4.7uF" H 6500 3950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6815 4005 50  0001 L CNN
F 3 "https://www.digikey.com/product-detail/en/samsung-electro-mechanics-america-inc/CL10B475KQ8NQNC/1276-2087-1-ND/3890173" H 6815 3914 50  0001 L CNN
F 4 "CL10B475KQ8NQNC" H 6700 4050 60  0001 C CNN "Part Number"
	1    6700 4050
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR017
U 1 1 59812E44
P 6500 4300
F 0 "#PWR017" H 6500 4050 50  0001 C CNN
F 1 "GND" H 6500 4150 50  0000 C CNN
F 2 "" H 6500 4300 50  0000 C CNN
F 3 "" H 6500 4300 50  0000 C CNN
	1    6500 4300
	1    0    0    -1  
$EndComp
Text GLabel 6450 2650 1    60   Input ~ 0
SPEAKER-
Text GLabel 6600 2650 1    60   Input ~ 0
SPEAKER+
Wire Wire Line
	5450 3350 5400 3350
Wire Wire Line
	5400 3350 5400 3400
Wire Wire Line
	5350 3250 5450 3250
Wire Wire Line
	5450 3150 5400 3150
Wire Wire Line
	5400 3150 5400 3250
Connection ~ 5400 3250
Wire Wire Line
	6400 3150 6650 3150
Wire Wire Line
	6400 3250 6650 3250
Wire Wire Line
	6400 3450 6500 3450
Wire Wire Line
	6400 3350 6700 3350
Wire Wire Line
	6700 3800 6700 3900
Wire Wire Line
	6500 3800 6500 3900
Wire Wire Line
	6700 4200 6700 4300
Wire Wire Line
	6500 4200 6500 4300
Wire Wire Line
	6500 3450 6500 3500
Wire Wire Line
	6700 3350 6700 3500
Wire Wire Line
	6450 2650 6450 3150
Connection ~ 6450 3150
Wire Wire Line
	6600 2650 6600 3250
Connection ~ 6600 3250
$Comp
L R_100k R21
U 1 1 59BA2110
P 6700 3650
F 0 "R21" V 6700 3650 50  0000 C CNN
F 1 "R_100k" V 6625 3650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6630 3650 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-07100KL/311-100KGRCT-ND/729645" V 6780 3650 50  0001 C CNN
F 4 "RC0603JR-07100KL" V 6880 3750 60  0001 C CNN "Part Number"
	1    6700 3650
	1    0    0    -1  
$EndComp
$Comp
L R_100k R20
U 1 1 59BA214E
P 6500 3650
F 0 "R20" V 6500 3650 50  0000 C CNN
F 1 "R_100k" V 6425 3650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6430 3650 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-07100KL/311-100KGRCT-ND/729645" V 6580 3650 50  0001 C CNN
F 4 "RC0603JR-07100KL" V 6680 3750 60  0001 C CNN "Part Number"
	1    6500 3650
	1    0    0    -1  
$EndComp
$EndSCHEMATC
