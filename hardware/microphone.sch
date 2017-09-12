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
Sheet 4 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 4500 950  0    197  ~ 0
Microphone
$Comp
L SPH0645LM4H-B U3
U 1 1 59AB3A7D
P 5250 1650
F 0 "U3" H 5450 1350 60  0000 C CNN
F 1 "SPH0645LM4H-B" H 5250 1850 60  0000 C CNN
F 2 "open-automation:SPH0645LM4H-B" H 5250 1250 60  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/knowles/SPH0645LM4H-B/423-1405-6-ND/5332436" H 5200 1150 60  0001 C CNN
F 4 "SPH0645LM4H-B" H 5250 1950 60  0001 C CNN "Part Number"
	1    5250 1650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR5
U 1 1 59AB3AE9
P 4800 1900
F 0 "#PWR5" H 4800 1650 50  0001 C CNN
F 1 "GND" H 4800 1750 50  0000 C CNN
F 2 "" H 4800 1900 50  0000 C CNN
F 3 "" H 4800 1900 50  0000 C CNN
	1    4800 1900
	1    0    0    -1  
$EndComp
Text GLabel 4400 1600 0    60   Input ~ 0
VCC_I2S
Text GLabel 5750 1800 2    60   Input ~ 0
I2S_BCLK
Text GLabel 6200 1500 2    60   Input ~ 0
I2S_DATA
Wire Wire Line
	5650 1800 5750 1800
Wire Wire Line
	5750 1700 5650 1700
Wire Wire Line
	4850 1850 4800 1850
Wire Wire Line
	4800 1700 4800 1900
Wire Wire Line
	4850 1700 4800 1700
Connection ~ 4800 1850
$Comp
L R_10k R9
U 1 1 59B93093
P 5900 1500
F 0 "R9" V 5900 1500 50  0000 C CNN
F 1 "R_10k" V 5800 1500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5830 1500 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-0710KL/311-10KGRCT-ND/729647" V 5980 1500 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 6080 1600 60  0001 C CNN "Part Number"
	1    5900 1500
	0    1    1    0   
$EndComp
Text GLabel 5750 1700 2    60   Input ~ 0
I2S_WS
Wire Wire Line
	4400 1600 4850 1600
$Comp
L C_0.1uF C1
U 1 1 59B9ACE4
P 4550 1900
F 0 "C1" H 4575 2000 50  0000 L CNN
F 1 "C_0.1uF" H 4225 1800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4650 2200 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/kemet/C0603C104K5RACTU/399-5089-1-ND/1465623" H 4550 1650 50  0001 C CNN
F 4 "C0603C104K5RACTU" H 4675 2100 60  0001 C CNN "Part Number"
	1    4550 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 1750 4550 1600
Connection ~ 4550 1600
$Comp
L GND #PWR4
U 1 1 59B9AD4C
P 4550 2100
F 0 "#PWR4" H 4550 1850 50  0001 C CNN
F 1 "GND" H 4550 1950 50  0000 C CNN
F 2 "" H 4550 2100 50  0000 C CNN
F 3 "" H 4550 2100 50  0000 C CNN
	1    4550 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 2050 4550 2100
Wire Wire Line
	5650 1600 5750 1600
Wire Wire Line
	6050 1500 6200 1500
Wire Wire Line
	5750 1600 5750 1500
Text GLabel 6000 3500 2    60   Input ~ 0
VCC_PIR1
$Comp
L R_10 R11
U 1 1 59B80153
P 4200 3500
F 0 "R11" V 4280 3500 50  0000 C CNN
F 1 "R_10" V 4200 3500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4130 3500 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/te-connectivity-passive-product/CPF0603B10RE/A119908CT-ND/4966506" V 4280 3500 50  0001 C CNN
F 4 "CPF0603B10RE" V 4380 3600 60  0001 C CNN "Part Number"
	1    4200 3500
	0    1    1    0   
$EndComp
$Comp
L C_1uF C8
U 1 1 59B80154
P 4900 3700
F 0 "C8" H 4900 3800 50  0000 L CNN
F 1 "C_1uF" H 4650 3600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5000 4000 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/CC0603ZRY5V7BB105/311-1372-1-ND/2103156" H 4900 3450 50  0001 C CNN
F 4 "CC0603ZRY5V7BB105" H 5025 3900 60  0001 C CNN "Part Number"
	1    4900 3700
	1    0    0    -1  
$EndComp
$Comp
L C_0.1uF C9
U 1 1 59B80155
P 5300 3700
F 0 "C9" H 5300 3800 50  0000 L CNN
F 1 "C_0.1uF" H 5000 3600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5400 4000 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/kemet/C0603C104K5RACTU/399-5089-1-ND/1465623" H 5300 3450 50  0001 C CNN
F 4 "C0603C104K5RACTU" H 5425 3900 60  0001 C CNN "Part Number"
	1    5300 3700
	1    0    0    -1  
$EndComp
$Comp
L C_100uF C2
U 1 1 59B80156
P 4500 3700
F 0 "C2" H 4525 3800 50  0000 L CNN
F 1 "C_100uF" H 4150 3600 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H 4600 4000 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/kemet/C0603C104K5RACTU/399-5089-1-ND/1465623" H 4500 3450 50  0001 C CNN
F 4 "C3216X5R1A107M160AC" H 4625 3900 60  0001 C CNN "Part Number"
	1    4500 3700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR6
U 1 1 59B80157
P 4900 3950
F 0 "#PWR6" H 4900 3700 50  0001 C CNN
F 1 "GND" H 4900 3800 50  0000 C CNN
F 2 "" H 4900 3950 50  0000 C CNN
F 3 "" H 4900 3950 50  0000 C CNN
	1    4900 3950
	1    0    0    -1  
$EndComp
Text GLabel 3950 3500 0    60   Input ~ 0
3V3
$Comp
L C_0.1uF C10
U 1 1 59B80158
P 5850 3700
F 0 "C10" H 5850 3800 50  0000 L CNN
F 1 "C_0.1uF" H 5550 3600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5950 4000 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/kemet/C0603C104K5RACTU/399-5089-1-ND/1465623" H 5850 3450 50  0001 C CNN
F 4 "C0603C104K5RACTU" H 5975 3900 60  0001 C CNN "Part Number"
	1    5850 3700
	1    0    0    -1  
$EndComp
Text Notes 4300 4300 0    60   ~ 0
PIR1 ByPass CAPs
Text Notes 5650 4300 0    60   ~ 0
PIR1 OPAMP
Wire Wire Line
	4500 3850 4500 3900
Wire Wire Line
	4500 3900 5850 3900
Wire Wire Line
	5300 3900 5300 3850
Wire Wire Line
	4900 3850 4900 3950
Connection ~ 4900 3900
Wire Wire Line
	4350 3500 6000 3500
Wire Wire Line
	5300 3550 5300 3500
Connection ~ 5300 3500
Wire Wire Line
	4900 3550 4900 3500
Connection ~ 4900 3500
Wire Wire Line
	4500 3550 4500 3500
Connection ~ 4500 3500
Wire Wire Line
	3950 3500 4050 3500
Wire Wire Line
	5850 3550 5850 3500
Connection ~ 5850 3500
Wire Wire Line
	5850 3900 5850 3850
Connection ~ 5300 3900
Wire Notes Line
	3650 4200 5550 4200
Wire Notes Line
	5550 4200 5550 3350
Wire Notes Line
	5550 3350 3650 3350
Wire Notes Line
	5750 3350 5750 4200
Wire Notes Line
	5750 4200 6550 4200
Wire Notes Line
	5750 3350 6550 3350
Wire Notes Line
	3650 3350 3650 4200
Wire Notes Line
	6550 3350 6550 4200
$EndSCHEMATC
