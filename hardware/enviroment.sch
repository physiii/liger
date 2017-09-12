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
Text Label 4800 1300 0    197  ~ 0
Environment
Text GLabel 3050 3200 0    60   Input ~ 0
VCC_I2C
$Comp
L GND #PWR18
U 1 1 5962DDD8
P 3500 3350
F 0 "#PWR18" H 3500 3100 50  0001 C CNN
F 1 "GND" H 3500 3200 50  0000 C CNN
F 2 "" H 3500 3350 50  0000 C CNN
F 3 "" H 3500 3350 50  0000 C CNN
	1    3500 3350
	1    0    0    -1  
$EndComp
Text GLabel 4500 3200 2    60   Input ~ 0
SCL
Text GLabel 4500 3300 2    60   Input ~ 0
SDA
$Comp
L SI7020-A20 U6
U 1 1 59AB3352
P 4000 3300
F 0 "U6" H 4200 3200 60  0000 C CNN
F 1 "SI7020-A20" H 4000 3500 60  0000 C CNN
F 2 "open-automation:SI7020-A20" H 4000 3000 60  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/silicon-labs/SI7020-A20-GM1R/336-3596-1-ND/5823452" H 4000 3100 60  0001 C CNN
F 4 "SI7020-A20-GM1R" H 4100 3100 60  0001 C CNN "Part Number"
	1    4000 3300
	1    0    0    -1  
$EndComp
$Comp
L C_0.1uF C23
U 1 1 59B8F8FE
P 3200 3400
F 0 "C23" H 3225 3500 50  0000 L CNN
F 1 "C_0.1uF" H 2900 3300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 3300 3700 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/kemet/C0603C104K5RACTU/399-5089-1-ND/1465623" H 3200 3150 50  0001 C CNN
F 4 "C0603C104K5RACTU" H 3325 3600 60  0001 C CNN "Part Number"
	1    3200 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR17
U 1 1 59B8FA16
P 3200 3600
F 0 "#PWR17" H 3200 3350 50  0001 C CNN
F 1 "GND" H 3200 3450 50  0000 C CNN
F 2 "" H 3200 3600 50  0000 C CNN
F 3 "" H 3200 3600 50  0000 C CNN
	1    3200 3600
	1    0    0    -1  
$EndComp
Text Label 7150 2700 0    60   ~ 0
Light-Level
$Comp
L OPT3001 U5
U 1 1 59B90C13
P 7500 3200
F 0 "U5" H 7650 2950 60  0000 C CNN
F 1 "OPT3001" H 7500 3350 60  0000 C CNN
F 2 "Housings_DFN_QFN:DFN-6-1EP_2x2mm_Pitch0.65mm" H 7550 2850 60  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/texas-instruments/OPT3001DNPR/296-40474-1-ND/5178351" H 7650 2950 60  0001 C CNN
F 4 "OPT3001" H 7750 3050 60  0001 C CNN "Part Number"
	1    7500 3200
	1    0    0    -1  
$EndComp
Text GLabel 7900 3100 1    60   Input ~ 0
SDA
Text GLabel 7900 3400 3    60   Input ~ 0
SCL
$Comp
L GND #PWR21
U 1 1 59B90C14
P 7150 3400
F 0 "#PWR21" H 7150 3150 50  0001 C CNN
F 1 "GND" H 7150 3250 50  0000 C CNN
F 2 "" H 7150 3400 50  0000 C CNN
F 3 "" H 7150 3400 50  0000 C CNN
	1    7150 3400
	1    0    0    -1  
$EndComp
$Comp
L R_10k R25
U 1 1 59B90C15
P 8350 3000
F 0 "R25" V 8350 3000 50  0000 C CNN
F 1 "R_10k" V 8250 3000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8280 3000 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 8430 3000 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 8530 3100 60  0001 C CNN "Part Number"
	1    8350 3000
	-1   0    0    1   
$EndComp
Text GLabel 6700 3150 0    60   Input ~ 0
VCC_I2C
$Comp
L C_0.1uF C24
U 1 1 59B90DE4
P 6850 3350
F 0 "C24" H 6875 3450 50  0000 L CNN
F 1 "C_0.1uF" H 6550 3250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6950 3650 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/kemet/C0603C104K5RACTU/399-5089-1-ND/1465623" H 6850 3100 50  0001 C CNN
F 4 "C0603C104K5RACTU" H 6975 3550 60  0001 C CNN "Part Number"
	1    6850 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR20
U 1 1 59B90DEC
P 6850 3550
F 0 "#PWR20" H 6850 3300 50  0001 C CNN
F 1 "GND" H 6850 3400 50  0000 C CNN
F 2 "" H 6850 3550 50  0000 C CNN
F 3 "" H 6850 3550 50  0000 C CNN
	1    6850 3550
	1    0    0    -1  
$EndComp
Text GLabel 8350 2800 1    60   Input ~ 0
3V3
Text Label 3500 2900 0    60   ~ 0
Temp-Humidty
Wire Wire Line
	3050 3200 3550 3200
Wire Wire Line
	3550 3300 3500 3300
Wire Wire Line
	4500 3300 4400 3300
Wire Wire Line
	4400 3200 4500 3200
Wire Wire Line
	3200 3250 3200 3200
Connection ~ 3200 3200
Wire Wire Line
	3200 3600 3200 3550
Wire Wire Line
	3500 3300 3500 3350
Wire Wire Line
	7900 3100 7900 3150
Wire Wire Line
	7900 3150 7850 3150
Wire Wire Line
	7900 3400 7900 3350
Wire Wire Line
	7900 3350 7850 3350
Wire Wire Line
	7150 3250 7150 3400
Wire Wire Line
	7150 3350 7200 3350
Wire Wire Line
	7200 3250 7150 3250
Connection ~ 7150 3350
Wire Wire Line
	7850 3250 8500 3250
Wire Wire Line
	6700 3150 7200 3150
Wire Wire Line
	6850 3200 6850 3150
Connection ~ 6850 3150
Wire Wire Line
	6850 3550 6850 3500
Wire Wire Line
	8350 2850 8350 2800
Text GLabel 8500 3250 2    60   Input ~ 0
LIGHT_INT
Wire Wire Line
	8350 3150 8350 3250
Connection ~ 8350 3250
$Comp
L R_10 R17
U 1 1 59B8099B
P 5100 4900
F 0 "R17" V 5180 4900 50  0000 C CNN
F 1 "R_10" V 5100 4900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5030 4900 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/te-connectivity-passive-product/CPF0603B10RE/A119908CT-ND/4966506" V 5180 4900 50  0001 C CNN
F 4 "CPF0603B10RE" V 5280 5000 60  0001 C CNN "Part Number"
	1    5100 4900
	0    1    1    0   
$EndComp
$Comp
L C_1uF C12
U 1 1 59B8099C
P 5800 5100
F 0 "C12" H 5800 5200 50  0000 L CNN
F 1 "C_1uF" H 5550 5000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5900 5400 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/CC0603ZRY5V7BB105/311-1372-1-ND/2103156" H 5800 4850 50  0001 C CNN
F 4 "CC0603ZRY5V7BB105" H 5925 5300 60  0001 C CNN "Part Number"
	1    5800 5100
	1    0    0    -1  
$EndComp
$Comp
L C_100uF C11
U 1 1 59B8099D
P 5400 5100
F 0 "C11" H 5425 5200 50  0000 L CNN
F 1 "C_100uF" H 5050 5000 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 5500 5400 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/kemet/C0603C104K5RACTU/399-5089-1-ND/1465623" H 5400 4850 50  0001 C CNN
F 4 "C3216X5R1A107M160AC" H 5525 5300 60  0001 C CNN "Part Number"
	1    5400 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR19
U 1 1 59B8099E
P 5600 5350
F 0 "#PWR19" H 5600 5100 50  0001 C CNN
F 1 "GND" H 5600 5200 50  0000 C CNN
F 2 "" H 5600 5350 50  0000 C CNN
F 3 "" H 5600 5350 50  0000 C CNN
	1    5600 5350
	1    0    0    -1  
$EndComp
Text GLabel 4850 4900 0    60   Input ~ 0
3V3
Text GLabel 5950 4900 2    60   Input ~ 0
VCC_I2C
Wire Wire Line
	4850 4900 4950 4900
Wire Wire Line
	5250 4900 5950 4900
Wire Wire Line
	5400 4900 5400 4950
Wire Wire Line
	5800 4900 5800 4950
Connection ~ 5400 4900
Connection ~ 5800 4900
Wire Wire Line
	5400 5250 5400 5300
Wire Wire Line
	5400 5300 5800 5300
Wire Wire Line
	5800 5300 5800 5250
Wire Wire Line
	5600 5350 5600 5300
Connection ~ 5600 5300
Text Notes 5150 5750 0    60   ~ 0
I2C ByPass CAPs
Wire Notes Line
	6400 4800 6400 5650
Wire Notes Line
	6400 5650 4550 5650
Wire Notes Line
	4550 4800 6400 4800
Wire Notes Line
	4550 5650 4550 4800
$EndSCHEMATC
