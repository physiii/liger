EESchema Schematic File Version 4
LIBS:liger-cache
EELAYER 26 0
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
Text Label 2750 1225 0    394  ~ 0
PIR_MOTION_SENSOR
$Comp
L power:GND #PWR029
U 1 1 59B7E3DB
P 5100 3250
F 0 "#PWR029" H 5100 3000 50  0001 C CNN
F 1 "GND" H 5100 3100 50  0000 C CNN
F 2 "" H 5100 3250 50  0000 C CNN
F 3 "" H 5100 3250 50  0000 C CNN
	1    5100 3250
	1    0    0    -1  
$EndComp
$Comp
L liger-rescue:R_100k R20
U 1 1 5A1610DC
P 6450 3150
F 0 "R20" V 6450 3150 50  0000 C CNN
F 1 "R_100k" V 6350 3150 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6380 3150 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603JR-07100KL/311-100KGRCT-ND/729645" V 6530 3150 50  0001 C CNN
F 4 "ERA-3AEB104V" V 6630 3250 60  0001 C CNN "Part Number"
	1    6450 3150
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR030
U 1 1 5A1624EE
P 6450 3350
F 0 "#PWR030" H 6450 3100 50  0001 C CNN
F 1 "GND" H 6450 3200 50  0000 C CNN
F 2 "" H 6450 3350 50  0000 C CNN
F 3 "" H 6450 3350 50  0000 C CNN
	1    6450 3350
	1    0    0    -1  
$EndComp
Text HLabel 4850 2850 0    60   Input ~ 0
3V3
Text HLabel 6600 2950 2    60   Input ~ 0
PIR
$Comp
L open-automation:PYQ2898 U7
U 1 1 5BA828EE
P 5450 2950
F 0 "U7" H 5650 2750 60  0000 C CNN
F 1 "PYQ2898" H 5500 3191 60  0000 C CNN
F 2 "open-automation:PYQ2898" H 5500 2550 60  0001 C CNN
F 3 "https://datasheet.octopart.com/PYQ2898-Excelitas-Technology-datasheet-9789873.pdf" H 5600 2650 60  0001 C CNN
F 4 "PYQ2898" H 5450 3250 60  0001 C CNN "Part Number"
	1    5450 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 2950 6450 2950
Wire Wire Line
	6450 3000 6450 2950
Connection ~ 6450 2950
Wire Wire Line
	6450 2950 6600 2950
Wire Wire Line
	6450 3350 6450 3300
$Comp
L open-automation:C_0.1uF C3
U 1 1 5BA837C7
P 5000 3050
F 0 "C3" H 4900 3150 50  0000 L CNN
F 1 "C_0.1uF" H 4700 2950 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5100 3350 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/tdk-corporation/C1608X8R1H104K080AB/445-8818-1-ND/3248223" H 5000 2800 50  0001 C CNN
F 4 "CL10F104ZO8NNNC" H 5125 3250 60  0001 C CNN "Part Number"
	1    5000 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 2850 5000 2850
Wire Wire Line
	5000 2900 5000 2850
Connection ~ 5000 2850
Wire Wire Line
	5000 2850 5200 2850
Wire Wire Line
	5000 3200 5100 3200
Wire Wire Line
	5200 3200 5200 3050
Wire Wire Line
	5100 3250 5100 3200
Connection ~ 5100 3200
Wire Wire Line
	5100 3200 5200 3200
$Comp
L open-automation:C_100pF C4
U 1 1 5BA83E1B
P 5900 3200
F 0 "C4" H 5950 3300 50  0000 L CNN
F 1 "C_100pF" H 5950 3100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6000 3500 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/avx-corporation/06035A101FAT2A/478-6202-1-ND/2391401" H 5900 2950 50  0001 C CNN
F 4 "06035A101FAT2A" H 6025 3400 60  0001 C CNN "Part Number"
	1    5900 3200
	1    0    0    -1  
$EndComp
$Comp
L open-automation:R_47.5 R8
U 1 1 5BA85345
P 6150 2950
F 0 "R8" V 6150 2950 50  0000 C CNN
F 1 "R_47.5" V 6050 2950 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6080 2950 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0603FR-0747R5L/311-47.5HRCT-ND/730201" V 6230 2950 50  0001 C CNN
F 4 "ERJ-3EKF47R5V" V 6330 3050 60  0001 C CNN "Part Number"
	1    6150 2950
	0    1    1    0   
$EndComp
Wire Wire Line
	5800 2950 5900 2950
Wire Wire Line
	5900 3050 5900 2950
Connection ~ 5900 2950
Wire Wire Line
	5900 2950 6000 2950
$Comp
L power:GND #PWR0103
U 1 1 5BA8569A
P 5900 3400
F 0 "#PWR0103" H 5900 3150 50  0001 C CNN
F 1 "GND" H 5900 3250 50  0000 C CNN
F 2 "" H 5900 3400 50  0000 C CNN
F 3 "" H 5900 3400 50  0000 C CNN
	1    5900 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 3350 5900 3400
$EndSCHEMATC
