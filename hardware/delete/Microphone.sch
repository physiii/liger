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
Sheet 1 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	6750 4750 6950 4750
Connection ~ 6950 3800
Wire Wire Line
	6950 4750 6950 3800
Wire Wire Line
	6900 3800 7000 3800
Connection ~ 6250 3900
Wire Wire Line
	6250 4750 6450 4750
Wire Wire Line
	6250 3900 6250 4750
Wire Wire Line
	6200 3900 6300 3900
Wire Wire Line
	5550 4100 5550 4250
Wire Wire Line
	5550 4550 5550 4600
Wire Wire Line
	6500 4100 6500 4300
Wire Wire Line
	6200 3700 6300 3700
Wire Wire Line
	6200 3350 6200 3700
Wire Wire Line
	6050 3350 6200 3350
Wire Wire Line
	5400 4100 5550 4100
Wire Wire Line
	5800 3900 5900 3900
Wire Wire Line
	5400 3900 5500 3900
Wire Wire Line
	4250 4100 4300 4100
Wire Wire Line
	4250 4150 4250 4100
Wire Wire Line
	5450 3150 5400 3150
Wire Wire Line
	5450 3500 5400 3500
Connection ~ 5750 3350
Wire Wire Line
	5750 3150 5750 3500
Wire Wire Line
	6500 3500 6500 3400
Wire Wire Line
	4250 3900 4300 3900
Wire Wire Line
	4250 3850 4250 3900
$Comp
L OPA344 U?
U 1 1 59091960
P 6600 3800
F 0 "U?" H 6600 3950 50  0000 L CNN
F 1 "OPA344" H 6550 3600 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 6500 3850 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/texas-instruments/OPA344NA-3K/296-41475-1-ND/5222851" H 6600 3950 50  0001 C CNN
	1    6600 3800
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 59091961
P 5600 3500
F 0 "R?" V 5680 3500 50  0000 C CNN
F 1 "1K" V 5600 3500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5530 3500 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" H 5600 3500 50  0001 C CNN
F 4 "RC0603JR-071KL" V 5600 3500 60  0001 C CNN "Part Number"
	1    5600 3500
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 59091962
P 6050 3900
F 0 "R?" V 6130 3900 50  0000 C CNN
F 1 "1K" V 6050 3900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5980 3900 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" H 6050 3900 50  0001 C CNN
F 4 "RC0603JR-071KL" V 6050 3900 60  0001 C CNN "Part Number"
	1    6050 3900
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 59091963
P 5550 4400
F 0 "C?" V 5500 4450 50  0000 L CNN
F 1 "4.7uf" V 5700 4300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 5665 4355 50  0001 L CNN
F 3 "http://www.digikey.com/product-detail/en/samsung-electro-mechanics-america-inc/CL10B475KQ8NQNC/1276-2087-1-ND/3890173" H 5665 4264 50  0001 L CNN
F 4 "CL10B475KQ8NQNC" H 5550 4400 60  0001 C CNN "Part Number"
	1    5550 4400
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 59091964
P 5900 3350
F 0 "R?" V 5980 3350 50  0000 C CNN
F 1 "1K" V 5900 3350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5830 3350 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" H 5900 3350 50  0001 C CNN
F 4 "RC0603JR-071KL" V 5900 3350 60  0001 C CNN "Part Number"
	1    5900 3350
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 59091965
P 6600 4750
F 0 "R?" V 6680 4750 50  0000 C CNN
F 1 "10K" V 6600 4750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 6530 4750 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 6600 4750 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 6600 4750 60  0001 C CNN "Part Number"
	1    6600 4750
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 59091966
P 5550 4600
F 0 "#PWR?" H 5550 4350 50  0001 C CNN
F 1 "GND" H 5550 4450 50  0000 C CNN
F 2 "" H 5550 4600 50  0000 C CNN
F 3 "" H 5550 4600 50  0000 C CNN
	1    5550 4600
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 59091967
P 5400 3150
F 0 "#PWR?" H 5400 3000 50  0001 C CNN
F 1 "+3.3V" H 5400 3290 50  0000 C CNN
F 2 "" H 5400 3150 50  0000 C CNN
F 3 "" H 5400 3150 50  0000 C CNN
	1    5400 3150
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 59091968
P 5400 3500
F 0 "#PWR?" H 5400 3250 50  0001 C CNN
F 1 "GND" H 5400 3350 50  0000 C CNN
F 2 "" H 5400 3500 50  0000 C CNN
F 3 "" H 5400 3500 50  0000 C CNN
	1    5400 3500
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 59091969
P 5600 3150
F 0 "R?" V 5680 3150 50  0000 C CNN
F 1 "6.65K" V 5600 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5530 3150 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/yageo/RC0603FR-076K65L/311-6.65KHRCT-ND/730269" H 5600 3150 50  0001 C CNN
F 4 "RC0603FR-076K65L" V 5600 3150 60  0001 C CNN "Part Number"
	1    5600 3150
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5909196A
P 6500 4300
F 0 "#PWR?" H 6500 4050 50  0001 C CNN
F 1 "GND" H 6500 4150 50  0000 C CNN
F 2 "" H 6500 4300 50  0000 C CNN
F 3 "" H 6500 4300 50  0000 C CNN
	1    6500 4300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 5909196B
P 6500 3400
F 0 "#PWR?" H 6500 3250 50  0001 C CNN
F 1 "+3.3V" H 6500 3540 50  0000 C CNN
F 2 "" H 6500 3400 50  0000 C CNN
F 3 "" H 6500 3400 50  0000 C CNN
	1    6500 3400
	1    0    0    -1  
$EndComp
$Comp
L MEMS-SPM0408LE5H U?
U 1 1 5909196C
P 4850 4050
F 0 "U?" H 5150 4350 60  0000 C CNN
F 1 "MEMS-SPM0408LE5H" H 4950 3900 60  0000 C CNN
F 2 "footprints:memes-mic" H 4850 4050 60  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/knowles/SPM0408LE5H-TB-6/423-1129-6-ND/2242368" H 4850 4050 60  0001 C CNN
F 4 "SPM0408LE5H-TB-6" H 4850 4050 60  0001 C CNN "Part Number"
	1    4850 4050
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 5909196D
P 4250 3850
F 0 "#PWR?" H 4250 3700 50  0001 C CNN
F 1 "+3.3V" H 4250 3990 50  0000 C CNN
F 2 "" H 4250 3850 50  0000 C CNN
F 3 "" H 4250 3850 50  0000 C CNN
	1    4250 3850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5909196E
P 4250 4150
F 0 "#PWR?" H 4250 3900 50  0001 C CNN
F 1 "GND" H 4250 4000 50  0000 C CNN
F 2 "" H 4250 4150 50  0000 C CNN
F 3 "" H 4250 4150 50  0000 C CNN
	1    4250 4150
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5909196F
P 5650 3900
F 0 "C?" V 5600 3950 50  0000 L CNN
F 1 "220nF" V 5800 3800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 5765 3855 50  0001 L CNN
F 3 "http://www.digikey.com/product-detail/en/tdk-corporation/C1608X7R1C224K080AC/445-1318-1-ND/567690" H 5765 3764 50  0001 L CNN
F 4 "C1608X7R1C224K080AC" H 5650 3900 60  0001 C CNN "Part Number"
	1    5650 3900
	0    1    1    0   
$EndComp
Text GLabel 7000 3800 2    60   Input ~ 0
ADC
Text Label 4050 2900 0    60   ~ 0
Mic_pre-amp
$EndSCHEMATC
