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
Sheet 9 9
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 4950 3700 2    60   Input ~ 0
IO34
Text Label 4450 2900 0    60   ~ 0
thermometer
$Comp
L R R24
U 1 1 590D2A99
P 4750 3450
F 0 "R24" V 4830 3450 50  0000 C CNN
F 1 "100K" V 4750 3450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4680 3450 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/PYu-RC_Group_51_RoHS_L_7.pdf" H 4750 3450 50  0001 C CNN
F 4 "RC0603JR-07100KL" V 4750 3450 60  0001 C CNN "Part Number"
	1    4750 3450
	1    0    0    -1  
$EndComp
$Comp
L R R25
U 1 1 590D2A9A
P 4750 3950
F 0 "R25" V 4830 3950 50  0000 C CNN
F 1 "10K" V 4750 3950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4680 3950 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 4750 3950 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 4750 3950 60  0001 C CNN "Part Number"
	1    4750 3950
	-1   0    0    1   
$EndComp
Wire Wire Line
	4750 3600 4750 3800
Wire Wire Line
	4950 3700 4750 3700
Connection ~ 4750 3700
$Comp
L GND #PWR026
U 1 1 590D2C98
P 4750 4200
F 0 "#PWR026" H 4750 3950 50  0001 C CNN
F 1 "GND" H 4750 4050 50  0000 C CNN
F 2 "" H 4750 4200 50  0000 C CNN
F 3 "" H 4750 4200 50  0000 C CNN
	1    4750 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 4100 4750 4200
Wire Wire Line
	4750 3200 4750 3300
Text GLabel 4750 3200 1    60   Input ~ 0
3v3
$EndSCHEMATC
