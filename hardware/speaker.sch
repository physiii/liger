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
Sheet 8 9
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
L C C7
U 1 1 590C9F1F
P 5000 3100
F 0 "C7" V 4950 3150 50  0000 L CNN
F 1 "100nF" V 5150 3000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 5115 3055 50  0001 L CNN
F 3 "http://www.digikey.com/product-detail/en/tdk-corporation/C1608X7R1C224K080AC/445-1318-1-ND/567690" H 5115 2964 50  0001 L CNN
F 4 "GRM188R71C104KA01D" H 5000 3100 60  0001 C CNN "Part Number"
	1    5000 3100
	0    1    1    0   
$EndComp
Text GLabel 4700 3100 0    60   Input ~ 0
IO25
Text Label 4400 2750 0    60   ~ 0
speaker_driver
$Comp
L R R23
U 1 1 590C74FA
P 5800 3900
F 0 "R23" V 5880 3900 50  0000 C CNN
F 1 "100K" V 5800 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5730 3900 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/PYu-RC_Group_51_RoHS_L_7.pdf" H 5800 3900 50  0001 C CNN
F 4 "RC0603JR-07100KL" V 5800 3900 60  0001 C CNN "Part Number"
	1    5800 3900
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR022
U 1 1 590CB30F
P 6600 3350
F 0 "#PWR022" H 6600 3100 50  0001 C CNN
F 1 "GND" H 6600 3200 50  0000 C CNN
F 2 "" H 6600 3350 50  0000 C CNN
F 3 "" H 6600 3350 50  0000 C CNN
	1    6600 3350
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 590CBCB3
P 6400 3200
F 0 "C8" V 6350 3250 50  0000 L CNN
F 1 "100nF" V 6550 3100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 6515 3155 50  0001 L CNN
F 3 "http://www.digikey.com/product-detail/en/tdk-corporation/C1608X7R1C224K080AC/445-1318-1-ND/567690" H 6515 3064 50  0001 L CNN
F 4 "GRM188R71C104KA01D" H 6400 3200 60  0001 C CNN "Part Number"
	1    6400 3200
	0    1    1    0   
$EndComp
$Comp
L R R21
U 1 1 590CC974
P 5250 3900
F 0 "R21" V 5330 3900 50  0000 C CNN
F 1 "100K" V 5250 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5180 3900 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/PYu-RC_Group_51_RoHS_L_7.pdf" H 5250 3900 50  0001 C CNN
F 4 "RC0603JR-07100KL" V 5250 3900 60  0001 C CNN "Part Number"
	1    5250 3900
	0    1    1    0   
$EndComp
$Comp
L R R22
U 1 1 590D0A06
P 5450 2850
F 0 "R22" V 5530 2850 50  0000 C CNN
F 1 "100K" V 5450 2850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5380 2850 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/PYu-RC_Group_51_RoHS_L_7.pdf" H 5450 2850 50  0001 C CNN
F 4 "RC0603JR-07100KL" V 5450 2850 60  0001 C CNN "Part Number"
	1    5450 2850
	0    -1   -1   0   
$EndComp
$Comp
L R R20
U 1 1 590D0B81
P 5250 3350
F 0 "R20" V 5330 3350 50  0000 C CNN
F 1 "100K" V 5250 3350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5180 3350 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/PYu-RC_Group_51_RoHS_L_7.pdf" H 5250 3350 50  0001 C CNN
F 4 "RC0603JR-07100KL" V 5250 3350 60  0001 C CNN "Part Number"
	1    5250 3350
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR023
U 1 1 590D0C30
P 5250 3550
F 0 "#PWR023" H 5250 3300 50  0001 C CNN
F 1 "GND" H 5250 3400 50  0000 C CNN
F 2 "" H 5250 3550 50  0000 C CNN
F 3 "" H 5250 3550 50  0000 C CNN
	1    5250 3550
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 590D0C50
P 4800 3900
F 0 "C6" V 4750 3950 50  0000 L CNN
F 1 "100nF" V 4950 3800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 4915 3855 50  0001 L CNN
F 3 "http://www.digikey.com/product-detail/en/tdk-corporation/C1608X7R1C224K080AC/445-1318-1-ND/567690" H 4915 3764 50  0001 L CNN
F 4 "GRM188R71C104KA01D" H 4800 3900 60  0001 C CNN "Part Number"
	1    4800 3900
	0    1    1    0   
$EndComp
$Comp
L GND #PWR024
U 1 1 590D1231
P 5700 3550
F 0 "#PWR024" H 5700 3300 50  0001 C CNN
F 1 "GND" H 5700 3400 50  0000 C CNN
F 2 "" H 5700 3550 50  0000 C CNN
F 3 "" H 5700 3550 50  0000 C CNN
	1    5700 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 2800 5700 2900
Wire Wire Line
	5500 3300 5500 3900
Wire Wire Line
	5400 3900 5650 3900
Wire Wire Line
	6100 3200 6250 3200
Wire Wire Line
	5950 3900 6200 3900
Wire Wire Line
	6200 3900 6200 3200
Connection ~ 6200 3200
Connection ~ 5500 3900
Wire Wire Line
	5600 2850 5700 2850
Connection ~ 5700 2850
Wire Wire Line
	5150 3100 5500 3100
Wire Wire Line
	5300 2850 5250 2850
Wire Wire Line
	5250 2850 5250 3200
Connection ~ 5250 3100
Wire Wire Line
	5250 3500 5250 3550
Wire Wire Line
	4950 3900 5100 3900
Wire Wire Line
	4700 3100 4850 3100
Wire Wire Line
	4500 3900 4650 3900
Wire Wire Line
	6650 3200 6550 3200
Wire Wire Line
	5700 3500 5700 3550
$Comp
L Speaker LS1
U 1 1 590F705D
P 6850 3200
F 0 "LS1" H 6900 3425 50  0000 R CNN
F 1 "Speaker" H 6900 3350 50  0000 R CNN
F 2 "footprints:speaker-1508" H 6850 3000 50  0001 C CNN
F 3 "http://www.cui.com/product/resource/cvs-1508.pdf" H 6840 3150 50  0001 C CNN
F 4 "CVS-1508" H 6850 3200 60  0001 C CNN "Part Number"
	1    6850 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 3300 6600 3300
Wire Wire Line
	6600 3300 6600 3350
$Comp
L GND #PWR025
U 1 1 590F73ED
P 4500 3900
F 0 "#PWR025" H 4500 3650 50  0001 C CNN
F 1 "GND" H 4500 3750 50  0000 C CNN
F 2 "" H 4500 3900 50  0000 C CNN
F 3 "" H 4500 3900 50  0000 C CNN
	1    4500 3900
	0    1    1    0   
$EndComp
$Comp
L OPA344 U11
U 1 1 59108FEA
P 5800 3200
AR Path="/590C715D/59108FEA" Ref="U11"  Part="1" 
AR Path="/59092AD4/59108FEA" Ref="U11"  Part="1" 
F 0 "U11" H 5800 3350 50  0000 L CNN
F 1 "OPA344" H 5800 3050 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 5700 3250 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/opa344.pdf" H 5800 3350 50  0001 C CNN
F 4 "296-41475-1-ND" H 5800 3200 60  0001 C CNN "Part Number"
	1    5800 3200
	1    0    0    -1  
$EndComp
Text GLabel 5700 2800 1    60   Input ~ 0
5V
$EndSCHEMATC
