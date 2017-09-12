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
Sheet 8 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 5000 1050 0    197  ~ 0
Environment
Text GLabel 5150 2300 0    60   Input ~ 0
VCC_I2C
$Comp
L GND #PWR021
U 1 1 5962DDD8
P 5600 2450
F 0 "#PWR021" H 5600 2200 50  0001 C CNN
F 1 "GND" H 5600 2300 50  0000 C CNN
F 2 "" H 5600 2450 50  0000 C CNN
F 3 "" H 5600 2450 50  0000 C CNN
	1    5600 2450
	1    0    0    -1  
$EndComp
Text GLabel 6600 2300 2    60   Input ~ 0
SCL
Text GLabel 6600 2400 2    60   Input ~ 0
SDA
$Comp
L SI7020-A20 U6
U 1 1 59AB3352
P 6100 2400
F 0 "U6" H 6300 2300 60  0000 C CNN
F 1 "SI7020-A20" H 6100 2600 60  0000 C CNN
F 2 "open-automation:SI7020-A20" H 6100 2100 60  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/silicon-labs/SI7020-A20-GM1R/336-3596-1-ND/5823452" H 6100 2200 60  0001 C CNN
F 4 "SI7020-A20-GM1R" H 6200 2200 60  0001 C CNN "Part Number"
	1    6100 2400
	1    0    0    -1  
$EndComp
$Comp
L C_0.1uF C23
U 1 1 59B8F8FE
P 5300 2500
F 0 "C23" H 5325 2600 50  0000 L CNN
F 1 "C_0.1uF" H 5000 2400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5400 2800 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/kemet/C0603C104K5RACTU/399-5089-1-ND/1465623" H 5300 2250 50  0001 C CNN
F 4 "C0603C104K5RACTU" H 5425 2700 60  0001 C CNN "Part Number"
	1    5300 2500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR022
U 1 1 59B8FA16
P 5300 2700
F 0 "#PWR022" H 5300 2450 50  0001 C CNN
F 1 "GND" H 5300 2550 50  0000 C CNN
F 2 "" H 5300 2700 50  0000 C CNN
F 3 "" H 5300 2700 50  0000 C CNN
	1    5300 2700
	1    0    0    -1  
$EndComp
Text Label 5650 3500 0    60   ~ 0
Light-Level
$Comp
L OPT3001 U5
U 1 1 59B90C13
P 6000 4000
F 0 "U5" H 6150 3750 60  0000 C CNN
F 1 "OPT3001" H 6000 4150 60  0000 C CNN
F 2 "Housings_DFN_QFN:DFN-6-1EP_2x2mm_Pitch0.65mm" H 6050 3650 60  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/texas-instruments/OPT3001DNPR/296-40474-1-ND/5178351" H 6150 3750 60  0001 C CNN
F 4 "OPT3001" H 6250 3850 60  0001 C CNN "Part Number"
	1    6000 4000
	1    0    0    -1  
$EndComp
Text GLabel 6400 3900 1    60   Input ~ 0
SDA
Text GLabel 6400 4200 3    60   Input ~ 0
SCL
$Comp
L GND #PWR023
U 1 1 59B90C14
P 5650 4200
F 0 "#PWR023" H 5650 3950 50  0001 C CNN
F 1 "GND" H 5650 4050 50  0000 C CNN
F 2 "" H 5650 4200 50  0000 C CNN
F 3 "" H 5650 4200 50  0000 C CNN
	1    5650 4200
	1    0    0    -1  
$EndComp
$Comp
L R_10k R25
U 1 1 59B90C15
P 6850 3800
F 0 "R25" V 6850 3800 50  0000 C CNN
F 1 "R_10k" V 6750 3800 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6780 3800 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 6930 3800 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 7030 3900 60  0001 C CNN "Part Number"
	1    6850 3800
	-1   0    0    1   
$EndComp
Text GLabel 5200 3950 0    60   Input ~ 0
VCC_I2C
$Comp
L C_0.1uF C24
U 1 1 59B90DE4
P 5350 4150
F 0 "C24" H 5375 4250 50  0000 L CNN
F 1 "C_0.1uF" H 5050 4050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5450 4450 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/kemet/C0603C104K5RACTU/399-5089-1-ND/1465623" H 5350 3900 50  0001 C CNN
F 4 "C0603C104K5RACTU" H 5475 4350 60  0001 C CNN "Part Number"
	1    5350 4150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR024
U 1 1 59B90DEC
P 5350 4350
F 0 "#PWR024" H 5350 4100 50  0001 C CNN
F 1 "GND" H 5350 4200 50  0000 C CNN
F 2 "" H 5350 4350 50  0000 C CNN
F 3 "" H 5350 4350 50  0000 C CNN
	1    5350 4350
	1    0    0    -1  
$EndComp
Text GLabel 6850 3600 1    60   Input ~ 0
3V3
Text Label 5600 2000 0    60   ~ 0
Temp-Humidty
Wire Wire Line
	5150 2300 5650 2300
Wire Wire Line
	5650 2400 5600 2400
Wire Wire Line
	6600 2400 6500 2400
Wire Wire Line
	6500 2300 6600 2300
Wire Wire Line
	5300 2350 5300 2300
Connection ~ 5300 2300
Wire Wire Line
	5300 2700 5300 2650
Wire Wire Line
	5600 2400 5600 2450
Wire Wire Line
	6400 3900 6400 3950
Wire Wire Line
	6400 3950 6350 3950
Wire Wire Line
	6400 4200 6400 4150
Wire Wire Line
	6400 4150 6350 4150
Wire Wire Line
	5650 4050 5650 4200
Wire Wire Line
	5650 4150 5700 4150
Wire Wire Line
	5700 4050 5650 4050
Connection ~ 5650 4150
Wire Wire Line
	6350 4050 7000 4050
Wire Wire Line
	5200 3950 5700 3950
Wire Wire Line
	5350 4000 5350 3950
Connection ~ 5350 3950
Wire Wire Line
	5350 4350 5350 4300
Wire Wire Line
	6850 3650 6850 3600
Text GLabel 7000 4050 2    60   Input ~ 0
LIGHT_INT
Wire Wire Line
	6850 3950 6850 4050
Connection ~ 6850 4050
$EndSCHEMATC
