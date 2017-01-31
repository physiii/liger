EESchema Schematic File Version 2
LIBS:liger-rescue
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
LIBS:liger-cache
EELAYER 26 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
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
L SPX3819 U11
U 1 1 5768217D
P 5550 1750
F 0 "U11" H 5300 2100 50  0000 C CNN
F 1 "SPX3819" H 5700 2100 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 5550 1350 50  0001 C CIN
F 3 "http://www.mouser.com/ds/2/146/SPX3819_DS_R200_082312-17072.pdf" H 5600 1250 50  0001 C CNN
F 4 "SPX3819" H 5550 1750 60  0001 C CNN "Part Number"
	1    5550 1750
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 5768271E
P 4000 2000
F 0 "C4" H 4025 2100 50  0000 L CNN
F 1 "1uf" H 4025 1900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4038 1850 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/UPY-GPHC_Y5V_6.3V-to-50V_6.pdf" H 4000 2000 50  0001 C CNN
F 4 "CC0603ZRY5V7BB105" H 4000 2000 60  0001 C CNN "Part Number"
	1    4000 2000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 57682AFF
P 4550 2300
F 0 "#PWR01" H 4550 2050 50  0001 C CNN
F 1 "GND" H 4550 2150 50  0000 C CNN
F 2 "" H 4550 2300 50  0000 C CNN
F 3 "" H 4550 2300 50  0000 C CNN
	1    4550 2300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 57682B37
P 4000 2350
F 0 "#PWR02" H 4000 2100 50  0001 C CNN
F 1 "GND" H 4000 2200 50  0000 C CNN
F 2 "" H 4000 2350 50  0000 C CNN
F 3 "" H 4000 2350 50  0000 C CNN
	1    4000 2350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 57682CA4
P 6050 2100
F 0 "#PWR03" H 6050 1850 50  0001 C CNN
F 1 "GND" H 6050 1950 50  0000 C CNN
F 2 "" H 6050 2100 50  0000 C CNN
F 3 "" H 6050 2100 50  0000 C CNN
	1    6050 2100
	1    0    0    -1  
$EndComp
Text Label 4300 750  0    60   ~ 0
Regulator
Text Label 550  700  0    60   ~ 0
Mic_pre-amp
$Comp
L GND #PWR04
U 1 1 57687E42
P 6200 3600
F 0 "#PWR04" H 6200 3350 50  0001 C CNN
F 1 "GND" H 6200 3450 50  0000 C CNN
F 2 "" H 6200 3600 50  0000 C CNN
F 3 "" H 6200 3600 50  0000 C CNN
	1    6200 3600
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X10 P1
U 1 1 57688107
P 5100 5500
F 0 "P1" H 5100 6050 50  0000 C CNN
F 1 "CONN_01X10" V 5200 5500 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x10" H 5100 5500 50  0001 C CNN
F 3 "http://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=5164713&DocType=Customer+Drawing&DocLang=English" H 5100 5500 50  0001 C CNN
F 4 "1-5164713-0" H 5100 5500 60  0001 C CNN "Part Number"
	1    5100 5500
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X10 P2
U 1 1 576884EF
P 5900 5450
F 0 "P2" H 5900 6000 50  0000 C CNN
F 1 "CONN_01X10" V 6000 5450 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x10" H 5900 5450 50  0001 C CNN
F 3 "http://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=5164713&DocType=Customer+Drawing&DocLang=English" H 5900 5450 50  0001 C CNN
F 4 "1-5164713-0" H 5900 5450 60  0001 C CNN "Part Number"
	1    5900 5450
	-1   0    0    1   
$EndComp
$Comp
L R R1
U 1 1 57689507
P 700 4600
F 0 "R1" V 780 4600 50  0000 C CNN
F 1 "1K" V 700 4600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 630 4600 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 700 4600 50  0001 C CNN
F 4 "RC0603JR-071KL" V 700 4600 60  0001 C CNN "Part Number"
	1    700  4600
	-1   0    0    1   
$EndComp
Text GLabel 1750 4400 3    60   Input ~ 0
IO5
Text GLabel 600  4300 1    60   Input ~ 0
IO13
$Comp
L SW_PUSH PROG1
U 1 1 5768B31A
P 2700 4650
F 0 "PROG1" H 2850 4760 50  0000 C CNN
F 1 "SW_PUSH" H 2700 4570 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_KMR2" H 2700 4650 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/60/KMR2_9aug12-25127.pdf" H 2700 4650 50  0001 C CNN
F 4 "KMR211NG LFS" H 2700 4650 60  0001 C CNN "Part Number"
	1    2700 4650
	0    1    1    0   
$EndComp
Text GLabel 3300 5300 2    60   Input ~ 0
IO0
Text GLabel 2950 4250 2    60   Input ~ 0
RESET
$Comp
L GND #PWR05
U 1 1 5768D6FF
P 2700 5450
F 0 "#PWR05" H 2700 5200 50  0001 C CNN
F 1 "GND" H 2700 5300 50  0000 C CNN
F 2 "" H 2700 5450 50  0000 C CNN
F 3 "" H 2700 5450 50  0000 C CNN
	1    2700 5450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 5768DA26
P 2000 5450
F 0 "#PWR06" H 2000 5200 50  0001 C CNN
F 1 "GND" H 2000 5300 50  0000 C CNN
F 2 "" H 2000 5450 50  0000 C CNN
F 3 "" H 2000 5450 50  0000 C CNN
	1    2000 5450
	1    0    0    -1  
$EndComp
$Comp
L Q_PMOS_GSD Q2
U 1 1 576D6D77
P 2350 4250
F 0 "Q2" H 2650 4300 50  0000 R CNN
F 1 "Q_PMOS_GSD" H 2650 4050 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 2550 4350 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/308/BSS84LT1-D-105913.pdf" H 2350 4250 50  0001 C CNN
F 4 "BSS84LT1G" H 2350 4250 60  0001 C CNN "Part Number"
	1    2350 4250
	-1   0    0    1   
$EndComp
$Comp
L Q_NMOS_GSD Q1
U 1 1 576D6F9D
P 2250 5200
F 0 "Q1" V 2150 5350 50  0000 R CNN
F 1 "Q_NMOS_GSD" V 2450 5450 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 2450 5300 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/308/2N7002L-D-105484.pdf" H 2250 5200 50  0001 C CNN
F 4 "2N7002L" V 2250 5200 60  0001 C CNN "Part Number"
	1    2250 5200
	0    1    1    0   
$EndComp
$Comp
L R R6
U 1 1 576DEB45
P 2250 4750
F 0 "R6" V 2330 4750 50  0000 C CNN
F 1 "1K" V 2250 4750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2180 4750 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 2250 4750 50  0001 C CNN
F 4 "RC0603JR-071KL" V 2250 4750 60  0001 C CNN "Part Number"
	1    2250 4750
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 576DEF74
P 2700 4000
F 0 "R9" V 2780 4000 50  0000 C CNN
F 1 "1K" V 2700 4000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2630 4000 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 2700 4000 50  0001 C CNN
F 4 "RC0603JR-071KL" V 2700 4000 60  0001 C CNN "Part Number"
	1    2700 4000
	1    0    0    -1  
$EndComp
$Comp
L R R10
U 1 1 576DF049
P 3000 5300
F 0 "R10" V 3080 5300 50  0000 C CNN
F 1 "1K" V 3000 5300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2930 5300 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 3000 5300 50  0001 C CNN
F 4 "RC0603JR-071KL" V 3000 5300 60  0001 C CNN "Part Number"
	1    3000 5300
	0    1    1    0   
$EndComp
$Comp
L R R12
U 1 1 576E4AC5
P 4400 2000
F 0 "R12" V 4480 2000 50  0000 C CNN
F 1 "10K" V 4400 2000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4330 2000 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 4400 2000 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 4400 2000 60  0001 C CNN "Part Number"
	1    4400 2000
	-1   0    0    1   
$EndComp
$Comp
L C C5
U 1 1 576FFA59
P 6050 1750
F 0 "C5" H 6075 1850 50  0000 L CNN
F 1 "1uf" H 6075 1650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6088 1600 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/UPY-GPHC_Y5V_6.3V-to-50V_6.pdf" H 6050 1750 50  0001 C CNN
F 4 "CC0603ZRY5V7BB105" H 6050 1750 60  0001 C CNN "Part Number"
	1    6050 1750
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 577082C5
P 2000 5100
F 0 "C2" H 2025 5200 50  0000 L CNN
F 1 "4.7uF" H 2000 5000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2038 4950 50  0001 C CNN
F 3 "http://media.digikey.com/pdf/Data%20Sheets/Samsung%20PDFs/CL_Series_MLCC_ds.pdf" H 2000 5100 50  0001 C CNN
F 4 "CL10B475KQ8NQNC" H 2000 5100 60  0001 C CNN "Part Number"
	1    2000 5100
	-1   0    0    1   
$EndComp
$Comp
L LED-RESCUE-liger D2
U 1 1 577107D2
P 2700 3550
F 0 "D2" H 2700 3650 50  0000 C CNN
F 1 "LED" H 2850 3500 50  0000 C CNN
F 2 "LEDs:LED_0603" H 2700 3550 50  0001 C CNN
F 3 "http://www.osram-os.com/Graphics/XPic9/00128792_0.pdf" H 2700 3550 50  0001 C CNN
F 4 "LS Q976-NR-1" H 2700 3550 60  0001 C CNN "Part Number"
	1    2700 3550
	0    -1   -1   0   
$EndComp
$Comp
L C C6
U 1 1 576F4515
P 6300 1750
F 0 "C6" H 6325 1850 50  0000 L CNN
F 1 "0.1uF" H 6300 1650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6338 1600 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/UPY-GPHC_Y5V_6.3V-to-50V_6.pdf" H 6300 1750 50  0001 C CNN
F 4 "CC0603ZRY5V9BB104" H 6300 1750 60  0001 C CNN "Part Number"
	1    6300 1750
	1    0    0    -1  
$EndComp
Text GLabel 6400 3850 2    60   Input ~ 0
ADC
Text GLabel 3500 1600 2    60   Input ~ 0
ADC
Text GLabel 3750 3900 1    60   Input ~ 0
IO15
Text GLabel 4800 4050 0    60   Input ~ 0
IO13
Text GLabel 4800 3950 0    60   Input ~ 0
IO12
Text GLabel 4800 3850 0    60   Input ~ 0
IO14
Text GLabel 6400 4250 2    60   Input ~ 0
TXD
Text GLabel 6400 5400 2    60   Input ~ 0
RXD
Text GLabel 6400 4450 2    60   Input ~ 0
IO4
Text GLabel 6400 3950 2    60   Input ~ 0
RESET
Text GLabel 6400 5600 2    60   Input ~ 0
IO5
Text GLabel 4800 4350 0    60   Input ~ 0
IO0
Text GLabel 4800 4250 0    60   Input ~ 0
IO2
Text GLabel 6400 3750 2    60   Input ~ 0
IO16
$Comp
L GND #PWR07
U 1 1 57704A85
P 4800 4500
F 0 "#PWR07" H 4800 4250 50  0001 C CNN
F 1 "GND" H 4800 4350 50  0000 C CNN
F 2 "" H 4800 4500 50  0000 C CNN
F 3 "" H 4800 4500 50  0000 C CNN
	1    4800 4500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR08
U 1 1 577051B4
P 4800 3250
F 0 "#PWR08" H 4800 3100 50  0001 C CNN
F 1 "+3.3V" H 4800 3390 50  0000 C CNN
F 2 "" H 4800 3250 50  0000 C CNN
F 3 "" H 4800 3250 50  0000 C CNN
	1    4800 3250
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P5
U 1 1 57706983
P 9400 2300
F 0 "P5" H 9400 2550 50  0000 C CNN
F 1 "CONN_01X04" V 9500 2300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04" H 9400 2300 50  0001 C CNN
F 3 "http://sullinscorp.com/catalogs/101_PAGE114-115_.100_FEMALE_HDR.pdf" H 9400 2300 50  0001 C CNN
F 4 "PPTC041LGBN-RC" H 9400 2300 60  0001 C CNN "Part Number"
	1    9400 2300
	0    1    1    0   
$EndComp
Text Label 8950 1150 0    60   ~ 0
Ultra-Sonic_Sensor
Text Label 650  3250 0    60   ~ 0
Switches_N_Stuff
Text Label 3650 3200 0    60   ~ 0
wifi-microcontroller
$Comp
L PT5529B/L2/H2-F U5
U 1 1 57717626
P 9200 3950
F 0 "U5" H 8900 3850 60  0000 C CNN
F 1 "PT5529B/L2/H2-F" H 9250 4150 60  0000 C CNN
F 2 "footprints:PT5529" H 9200 3950 60  0001 C CNN
F 3 "http://www.everlight.com/file/ProductFile/PT5529B-L2-H2-F.pdf" H 9200 3950 60  0001 C CNN
F 4 "PT5529B/L2/H2-F" H 9200 3950 60  0001 C CNN "Part Number"
	1    9200 3950
	1    0    0    -1  
$EndComp
Text Label 8150 3750 0    60   ~ 0
IR-Receive
$Comp
L +5V #PWR09
U 1 1 5771C30D
P 9500 4250
F 0 "#PWR09" H 9500 4100 50  0001 C CNN
F 1 "+5V" H 9500 4390 50  0000 C CNN
F 2 "" H 9500 4250 50  0000 C CNN
F 3 "" H 9500 4250 50  0000 C CNN
	1    9500 4250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 5771C669
P 9200 4400
F 0 "#PWR010" H 9200 4150 50  0001 C CNN
F 1 "GND" H 9200 4250 50  0000 C CNN
F 2 "" H 9200 4400 50  0000 C CNN
F 3 "" H 9200 4400 50  0000 C CNN
	1    9200 4400
	1    0    0    -1  
$EndComp
Text GLabel 8500 4350 0    60   Input ~ 0
IO4
$Comp
L R R17
U 1 1 5771E483
P 8750 4350
F 0 "R17" V 8830 4350 50  0000 C CNN
F 1 "1K" V 8750 4350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 8680 4350 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 8750 4350 50  0001 C CNN
F 4 "RC0603JR-071KL" V 8750 4350 60  0001 C CNN "Part Number"
	1    8750 4350
	0    1    1    0   
$EndComp
$Comp
L LED-RESCUE-liger D8
U 1 1 57722A1E
P 10150 4950
F 0 "D8" H 10150 5050 50  0000 C CNN
F 1 "LED" H 10150 4850 50  0000 C CNN
F 2 "LEDs:LED-3MM" H 10150 4950 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/149/QED123-890063.pdf" H 10150 4950 50  0001 C CNN
F 4 "QED123" H 10150 4950 60  0001 C CNN "Part Number"
	1    10150 4950
	0    -1   -1   0   
$EndComp
$Comp
L R R25
U 1 1 577249CF
P 10150 4500
F 0 "R25" V 10230 4500 50  0000 C CNN
F 1 "36" V 10150 4500 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 10080 4500 50  0001 C CNN
F 3 "http://www.yageo.com.tw/exep/pages/download/literatures/PYu-R_INT-thick_7.pdf" H 10150 4500 50  0001 C CNN
F 4 "RC0603JR-0736RL" V 10150 4500 60  0001 C CNN "Part Number"
	1    10150 4500
	1    0    0    -1  
$EndComp
$Comp
L R R24
U 1 1 5772613E
P 9450 5450
F 0 "R24" V 9530 5450 50  0000 C CNN
F 1 "1K" V 9450 5450 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 9380 5450 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 9450 5450 50  0001 C CNN
F 4 "RC0603JR-071KL" V 9450 5450 60  0001 C CNN "Part Number"
	1    9450 5450
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR011
U 1 1 57726F3B
P 9600 2050
F 0 "#PWR011" H 9600 1900 50  0001 C CNN
F 1 "+5V" H 9600 2190 50  0000 C CNN
F 2 "" H 9600 2050 50  0000 C CNN
F 3 "" H 9600 2050 50  0000 C CNN
	1    9600 2050
	0    1    1    0   
$EndComp
$Comp
L Q_NPN_BEC Q4
U 1 1 577276B5
P 10050 5450
F 0 "Q4" H 10350 5500 50  0000 R CNN
F 1 "Q_NPN_BEC" H 10650 5400 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 10250 5550 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/308/MMBT3904LT1-D-113169.pdf" H 10050 5450 50  0001 C CNN
F 4 "MMBT3904LT1G" H 10050 5450 60  0001 C CNN "Part Number"
	1    10050 5450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 57727DF2
P 10150 5850
F 0 "#PWR012" H 10150 5600 50  0001 C CNN
F 1 "GND" H 10150 5700 50  0000 C CNN
F 2 "" H 10150 5850 50  0000 C CNN
F 3 "" H 10150 5850 50  0000 C CNN
	1    10150 5850
	1    0    0    -1  
$EndComp
Text GLabel 9000 5450 0    60   Input ~ 0
IO14
Text GLabel 6400 5300 2    60   Input ~ 0
IO4
Text GLabel 6400 4350 2    60   Input ~ 0
RXD
Text GLabel 6400 5500 2    60   Input ~ 0
TXD
Text GLabel 6400 4050 2    60   Input ~ 0
IO5
Text GLabel 6400 5800 2    60   Input ~ 0
ADC
Text GLabel 6400 5700 2    60   Input ~ 0
RESET
Text GLabel 6400 5900 2    60   Input ~ 0
IO16
$Comp
L GND #PWR013
U 1 1 57744532
P 6400 5000
F 0 "#PWR013" H 6400 4750 50  0001 C CNN
F 1 "GND" H 6400 4850 50  0000 C CNN
F 2 "" H 6400 5000 50  0000 C CNN
F 3 "" H 6400 5000 50  0000 C CNN
	1    6400 5000
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR014
U 1 1 57744A7C
P 6750 5050
F 0 "#PWR014" H 6750 4900 50  0001 C CNN
F 1 "+5V" H 6750 5190 50  0000 C CNN
F 2 "" H 6750 5050 50  0000 C CNN
F 3 "" H 6750 5050 50  0000 C CNN
	1    6750 5050
	1    0    0    -1  
$EndComp
Text GLabel 4600 5450 0    60   Input ~ 0
IO15
Text GLabel 4600 5550 0    60   Input ~ 0
IO13
Text GLabel 4600 5650 0    60   Input ~ 0
IO12
Text GLabel 4600 5750 0    60   Input ~ 0
IO14
Text GLabel 4600 5250 0    60   Input ~ 0
IO0
Text GLabel 4600 5350 0    60   Input ~ 0
IO2
Text GLabel 4650 3750 0    60   Input ~ 0
3v3
Text GLabel 4600 5850 0    60   Input ~ 0
EN
$Comp
L GND #PWR015
U 1 1 57746D84
P 4550 5050
F 0 "#PWR015" H 4550 4800 50  0001 C CNN
F 1 "GND" H 4550 4900 50  0000 C CNN
F 2 "" H 4550 5050 50  0000 C CNN
F 3 "" H 4550 5050 50  0000 C CNN
	1    4550 5050
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR016
U 1 1 57746FCF
P 4600 5950
F 0 "#PWR016" H 4600 5800 50  0001 C CNN
F 1 "+3.3V" H 4600 6090 50  0000 C CNN
F 2 "" H 4600 5950 50  0000 C CNN
F 3 "" H 4600 5950 50  0000 C CNN
	1    4600 5950
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR017
U 1 1 57771C8C
P 6750 4200
F 0 "#PWR017" H 6750 3950 50  0001 C CNN
F 1 "GND" H 6750 4050 50  0000 C CNN
F 2 "" H 6750 4200 50  0000 C CNN
F 3 "" H 6750 4200 50  0000 C CNN
	1    6750 4200
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR018
U 1 1 577743ED
P 6900 5150
F 0 "#PWR018" H 6900 5000 50  0001 C CNN
F 1 "+3.3V" H 6900 5290 50  0000 C CNN
F 2 "" H 6900 5150 50  0000 C CNN
F 3 "" H 6900 5150 50  0000 C CNN
	1    6900 5150
	1    0    0    -1  
$EndComp
$Comp
L ESP-WROOM-02 U3
U 1 1 57775BA4
P 4850 3300
F 0 "U3" H 5900 2000 60  0000 C CNN
F 1 "ESP-WROOM-02" H 5550 3150 60  0000 C CNN
F 2 "libraries:ESP-WROOM-02" H 4850 3300 60  0001 C CNN
F 3 "http://espressif.com/sites/default/files/documentation/0c-esp-wroom-02_datasheet_en.pdf" H 4850 3300 60  0001 C CNN
F 4 "ESP-WROOM-02" H 4850 3300 60  0001 C CNN "Part Number"
	1    4850 3300
	1    0    0    -1  
$EndComp
$Comp
L C C11
U 1 1 57C8DD11
P 2150 1700
F 0 "C11" V 2100 1750 50  0000 L CNN
F 1 "220nF" V 2300 1600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 2265 1655 50  0001 L CNN
F 3 "http://www.digikey.com/product-detail/en/tdk-corporation/C1608X7R1C224K080AC/445-1318-1-ND/567690" H 2265 1564 50  0001 L CNN
F 4 "C1608X7R1C224K080AC" H 2150 1700 60  0001 C CNN "Part Number"
	1    2150 1700
	0    1    1    0   
$EndComp
$Comp
L GND #PWR019
U 1 1 57D0735A
P 750 1950
F 0 "#PWR019" H 750 1700 50  0001 C CNN
F 1 "GND" H 750 1800 50  0000 C CNN
F 2 "" H 750 1950 50  0000 C CNN
F 3 "" H 750 1950 50  0000 C CNN
	1    750  1950
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR020
U 1 1 57D0788E
P 750 1650
F 0 "#PWR020" H 750 1500 50  0001 C CNN
F 1 "+3.3V" H 750 1790 50  0000 C CNN
F 2 "" H 750 1650 50  0000 C CNN
F 3 "" H 750 1650 50  0000 C CNN
	1    750  1650
	1    0    0    -1  
$EndComp
$Comp
L MEMS-SPM0408LE5H U1
U 1 1 57D08DA5
P 1350 1850
F 0 "U1" H 1650 2150 60  0000 C CNN
F 1 "MEMS-SPM0408LE5H" H 1450 1700 60  0000 C CNN
F 2 "footprints:memes-mic" H 1350 1850 60  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/knowles/SPM0408LE5H-TB-6/423-1129-6-ND/2242368" H 1350 1850 60  0001 C CNN
F 4 "SPM0408LE5H-TB-6" H 1350 1850 60  0001 C CNN "Part Number"
	1    1350 1850
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR021
U 1 1 57D0CF6C
P 3000 1200
F 0 "#PWR021" H 3000 1050 50  0001 C CNN
F 1 "+3.3V" H 3000 1340 50  0000 C CNN
F 2 "" H 3000 1200 50  0000 C CNN
F 3 "" H 3000 1200 50  0000 C CNN
	1    3000 1200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR022
U 1 1 57D0CF72
P 3000 2100
F 0 "#PWR022" H 3000 1850 50  0001 C CNN
F 1 "GND" H 3000 1950 50  0000 C CNN
F 2 "" H 3000 2100 50  0000 C CNN
F 3 "" H 3000 2100 50  0000 C CNN
	1    3000 2100
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 57D0E6CD
P 2100 950
F 0 "R3" V 2180 950 50  0000 C CNN
F 1 "6.65K" V 2100 950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2030 950 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/yageo/RC0603FR-076K65L/311-6.65KHRCT-ND/730269" H 2100 950 50  0001 C CNN
F 4 "RC0603FR-076K65L" V 2100 950 60  0001 C CNN "Part Number"
	1    2100 950 
	0    1    1    0   
$EndComp
$Comp
L GND #PWR023
U 1 1 57D10918
P 1900 1300
F 0 "#PWR023" H 1900 1050 50  0001 C CNN
F 1 "GND" H 1900 1150 50  0000 C CNN
F 2 "" H 1900 1300 50  0000 C CNN
F 3 "" H 1900 1300 50  0000 C CNN
	1    1900 1300
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR024
U 1 1 57D10AE9
P 1900 950
F 0 "#PWR024" H 1900 800 50  0001 C CNN
F 1 "+3.3V" H 1900 1090 50  0000 C CNN
F 2 "" H 1900 950 50  0000 C CNN
F 3 "" H 1900 950 50  0000 C CNN
	1    1900 950 
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR025
U 1 1 57D13092
P 2050 2400
F 0 "#PWR025" H 2050 2150 50  0001 C CNN
F 1 "GND" H 2050 2250 50  0000 C CNN
F 2 "" H 2050 2400 50  0000 C CNN
F 3 "" H 2050 2400 50  0000 C CNN
	1    2050 2400
	1    0    0    -1  
$EndComp
$Comp
L R R11
U 1 1 57D1D84F
P 3100 2550
F 0 "R11" V 3180 2550 50  0000 C CNN
F 1 "10K" V 3100 2550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3030 2550 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 3100 2550 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 3100 2550 60  0001 C CNN "Part Number"
	1    3100 2550
	0    -1   -1   0   
$EndComp
$Comp
L R R7
U 1 1 57D1D9BA
P 2400 1150
F 0 "R7" V 2480 1150 50  0000 C CNN
F 1 "1K" V 2400 1150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2330 1150 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" H 2400 1150 50  0001 C CNN
F 4 "RC0603JR-071KL" V 2400 1150 60  0001 C CNN "Part Number"
	1    2400 1150
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR026
U 1 1 57D12FB5
P 4000 1300
F 0 "#PWR026" H 4000 1150 50  0001 C CNN
F 1 "+5V" H 4015 1473 50  0000 C CNN
F 2 "" H 4000 1300 50  0000 C CNN
F 3 "" H 4000 1300 50  0000 C CNN
	1    4000 1300
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 57D31D9B
P 2050 2200
F 0 "C1" V 2000 2250 50  0000 L CNN
F 1 "4.7uf" V 2200 2100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 2165 2155 50  0001 L CNN
F 3 "http://www.digikey.com/product-detail/en/samsung-electro-mechanics-america-inc/CL10B475KQ8NQNC/1276-2087-1-ND/3890173" H 2165 2064 50  0001 L CNN
F 4 "CL10B475KQ8NQNC" H 2050 2200 60  0001 C CNN "Part Number"
	1    2050 2200
	-1   0    0    1   
$EndComp
$Comp
L R R8
U 1 1 57D32F11
P 2550 1700
F 0 "R8" V 2630 1700 50  0000 C CNN
F 1 "1K" V 2550 1700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2480 1700 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" H 2550 1700 50  0001 C CNN
F 4 "RC0603JR-071KL" V 2550 1700 60  0001 C CNN "Part Number"
	1    2550 1700
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 57D3513B
P 2100 1300
F 0 "R5" V 2180 1300 50  0000 C CNN
F 1 "1K" V 2100 1300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2030 1300 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/yageo/RC0603JR-071KL/311-1.0KGRCT-ND/729624" H 2100 1300 50  0001 C CNN
F 4 "RC0603JR-071KL" V 2100 1300 60  0001 C CNN "Part Number"
	1    2100 1300
	0    1    1    0   
$EndComp
$Comp
L LED-switch SW2
U 1 1 57DA217C
P 1250 4150
F 0 "SW2" H 1350 4350 60  0000 C CNN
F 1 "LED-switch" H 1200 3850 60  0000 C CNN
F 2 "footprints:SMT-led-switch" H 1250 4150 60  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/c-k-components/K8WH41GLFS/CKN10530TR-ND/4176636" H 1250 4150 60  0001 C CNN
	1    1250 4150
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 57DA5412
P 1750 4000
F 0 "R4" V 1830 4000 50  0000 C CNN
F 1 "36" V 1750 4000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 1680 4000 50  0001 C CNN
F 3 "http://www.yageo.com.tw/exep/pages/download/literatures/PYu-R_INT-thick_7.pdf" H 1750 4000 50  0001 C CNN
F 4 "RC0603JR-0736RL" V 1750 4000 60  0001 C CNN "Part Number"
	1    1750 4000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR027
U 1 1 57DAD5B2
P 1750 3750
F 0 "#PWR027" H 1750 3600 50  0001 C CNN
F 1 "+3.3V" H 1750 3890 50  0000 C CNN
F 2 "" H 1750 3750 50  0000 C CNN
F 3 "" H 1750 3750 50  0000 C CNN
	1    1750 3750
	1    0    0    -1  
$EndComp
Text GLabel 9450 1900 1    60   Input ~ 0
IO12
Text GLabel 9350 1900 1    60   Input ~ 0
IO14
$Comp
L GND #PWR028
U 1 1 57DBB421
P 9200 2050
F 0 "#PWR028" H 9200 1800 50  0001 C CNN
F 1 "GND" H 9200 1900 50  0000 C CNN
F 2 "" H 9200 2050 50  0000 C CNN
F 3 "" H 9200 2050 50  0000 C CNN
	1    9200 2050
	0    1    1    0   
$EndComp
$Comp
L OPA344 U2
U 1 1 57DBFA97
P 3100 1600
F 0 "U2" H 3100 1750 50  0000 L CNN
F 1 "OPA344" H 3050 1400 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 3000 1650 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/texas-instruments/OPA344NA-3K/296-41475-1-ND/5222851" H 3100 1750 50  0001 C CNN
	1    3100 1600
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P6
U 1 1 57DDC8F3
P 5550 2550
F 0 "P6" H 5450 2750 50  0000 L CNN
F 1 "CONN_01X03" V 5650 2300 50  0001 L CNN
F 2 "footprints:connector_3pos_KR" H 5550 2550 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/hirose-electric-co-ltd/DF13C-3P-1.25V(21)/H121960CT-ND/3907735" H 5550 2550 50  0001 C CNN
F 4 "A04KR04DA28W51B" H 5550 2550 60  0001 C CNN "Part Number"
	1    5550 2550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR029
U 1 1 57DDC8F9
P 5300 2400
F 0 "#PWR029" H 5300 2150 50  0001 C CNN
F 1 "GND" H 5300 2250 50  0000 C CNN
F 2 "" H 5300 2400 50  0000 C CNN
F 3 "" H 5300 2400 50  0000 C CNN
	1    5300 2400
	-1   0    0    1   
$EndComp
$Comp
L +5V #PWR030
U 1 1 57DDC8FF
P 5300 2700
F 0 "#PWR030" H 5300 2550 50  0001 C CNN
F 1 "+5V" H 5300 2840 50  0000 C CNN
F 2 "" H 5300 2700 50  0000 C CNN
F 3 "" H 5300 2700 50  0000 C CNN
	1    5300 2700
	-1   0    0    1   
$EndComp
Text GLabel 5250 2550 0    60   Input ~ 0
IO15
$Comp
L +3.3V #PWR031
U 1 1 57E248C2
P 6050 1350
F 0 "#PWR031" H 6050 1200 50  0001 C CNN
F 1 "+3.3V" H 6050 1490 50  0000 C CNN
F 2 "" H 6050 1350 50  0000 C CNN
F 3 "" H 6050 1350 50  0000 C CNN
	1    6050 1350
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR032
U 1 1 57E1BD58
P 10150 4200
F 0 "#PWR032" H 10150 4050 50  0001 C CNN
F 1 "+3.3V" H 10150 4340 50  0000 C CNN
F 2 "" H 10150 4200 50  0000 C CNN
F 3 "" H 10150 4200 50  0000 C CNN
	1    10150 4200
	1    0    0    -1  
$EndComp
Text Label 7600 1250 0    60   ~ 0
PIR
$Comp
L CONN_01X03 P8
U 1 1 57E1EB19
P 7900 2000
F 0 "P8" H 7850 2200 50  0000 L CNN
F 1 "CONN_01X03" H 7850 1800 50  0001 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03" H 7977 1904 50  0001 L CNN
F 3 "http://sullinscorp.com/catalogs/101_PAGE114-115_.100_FEMALE_HDR.pdf" H 7900 2000 50  0001 C CNN
	1    7900 2000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR033
U 1 1 57E1FCBC
P 7650 1850
F 0 "#PWR033" H 7650 1600 50  0001 C CNN
F 1 "GND" H 7650 1700 50  0000 C CNN
F 2 "" H 7650 1850 50  0000 C CNN
F 3 "" H 7650 1850 50  0000 C CNN
	1    7650 1850
	-1   0    0    1   
$EndComp
$Comp
L +5V #PWR034
U 1 1 57E20222
P 7650 2150
F 0 "#PWR034" H 7650 2000 50  0001 C CNN
F 1 "+5V" H 7650 2290 50  0000 C CNN
F 2 "" H 7650 2150 50  0000 C CNN
F 3 "" H 7650 2150 50  0000 C CNN
	1    7650 2150
	-1   0    0    1   
$EndComp
Text GLabel 7600 2000 0    60   Input ~ 0
IO16
$Comp
L GND #PWR035
U 1 1 58029261
P 700 4850
F 0 "#PWR035" H 700 4600 50  0001 C CNN
F 1 "GND" H 700 4700 50  0000 C CNN
F 2 "" H 700 4850 50  0000 C CNN
F 3 "" H 700 4850 50  0000 C CNN
	1    700  4850
	1    0    0    -1  
$EndComp
Text Label 9200 5050 0    60   ~ 0
IR-Transmit
$Comp
L +5V #PWR036
U 1 1 582A1DED
P 2700 3250
F 0 "#PWR036" H 2700 3100 50  0001 C CNN
F 1 "+5V" H 2700 3390 50  0000 C CNN
F 2 "" H 2700 3250 50  0000 C CNN
F 3 "" H 2700 3250 50  0000 C CNN
	1    2700 3250
	1    0    0    -1  
$EndComp
$Comp
L R R13
U 1 1 576E9D8A
P 3750 4200
F 0 "R13" V 3830 4200 50  0000 C CNN
F 1 "10K" V 3750 4200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3680 4200 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 3750 4200 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 3750 4200 60  0001 C CNN "Part Number"
	1    3750 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 1900 9450 2100
Wire Wire Line
	9350 1900 9350 2100
Wire Wire Line
	9550 2050 9550 2100
Wire Wire Line
	9600 2050 9550 2050
Wire Wire Line
	9250 2050 9250 2100
Wire Wire Line
	9200 2050 9250 2050
Wire Wire Line
	700  3800 1750 3800
Connection ~ 1750 3800
Connection ~ 750  4100
Wire Wire Line
	700  4100 750  4100
Wire Wire Line
	700  3800 700  4100
Wire Wire Line
	1750 4350 1750 4400
Wire Wire Line
	1650 4350 1750 4350
Wire Wire Line
	750  4050 750  4150
Wire Wire Line
	600  4350 750  4350
Wire Wire Line
	700  4350 700  4450
Wire Wire Line
	1750 3750 1750 3850
Wire Wire Line
	1750 4250 1750 4150
Wire Wire Line
	1650 4250 1750 4250
Wire Notes Line
	500  9350 10750 9350
Wire Notes Line
	500  6250 10750 6250
Wire Notes Line
	500  2950 10750 2950
Wire Wire Line
	6900 5200 6900 5150
Wire Wire Line
	6750 5100 6750 5050
Wire Wire Line
	6150 4350 6400 4350
Wire Wire Line
	6750 4150 6750 4200
Wire Wire Line
	6150 4150 6750 4150
Connection ~ 4600 5050
Wire Wire Line
	4600 5150 4600 5050
Connection ~ 4750 3750
Wire Wire Line
	6100 5200 6900 5200
Wire Wire Line
	4600 5950 4900 5950
Wire Wire Line
	4600 5850 4900 5850
Wire Wire Line
	4600 5750 4900 5750
Wire Wire Line
	4600 5650 4900 5650
Wire Wire Line
	4600 5550 4900 5550
Wire Wire Line
	4600 5450 4900 5450
Wire Wire Line
	4600 5350 4900 5350
Wire Wire Line
	4600 5250 4900 5250
Wire Wire Line
	4600 5150 4900 5150
Wire Wire Line
	4550 5050 4900 5050
Wire Wire Line
	6100 5900 6400 5900
Wire Wire Line
	6100 5800 6400 5800
Wire Wire Line
	6100 5700 6400 5700
Wire Wire Line
	6100 5600 6400 5600
Wire Wire Line
	6100 5500 6400 5500
Wire Wire Line
	6100 5400 6400 5400
Wire Wire Line
	6100 5300 6400 5300
Wire Wire Line
	6100 5100 6750 5100
Wire Wire Line
	6100 5000 6400 5000
Wire Wire Line
	4650 3750 4900 3750
Wire Wire Line
	9300 5450 9000 5450
Wire Wire Line
	10150 5650 10150 5850
Wire Wire Line
	9600 5450 9850 5450
Wire Wire Line
	10150 5250 10150 5150
Wire Wire Line
	10150 4650 10150 4750
Wire Wire Line
	10150 4200 10150 4350
Wire Wire Line
	8600 4350 8500 4350
Wire Wire Line
	9500 4350 9500 4250
Wire Wire Line
	9300 4350 9500 4350
Wire Wire Line
	9300 4250 9300 4350
Wire Wire Line
	9200 4250 9200 4400
Wire Wire Line
	9100 4350 8900 4350
Wire Wire Line
	9100 4250 9100 4350
Wire Notes Line
	10750 9350 10750 550 
Wire Wire Line
	6150 3750 6400 3750
Wire Wire Line
	4800 3850 4900 3850
Wire Wire Line
	4800 3950 4900 3950
Wire Wire Line
	4800 4050 4900 4050
Wire Wire Line
	4800 4250 4900 4250
Wire Wire Line
	4800 4350 4900 4350
Wire Wire Line
	6150 4050 6400 4050
Wire Wire Line
	6150 3950 6400 3950
Wire Wire Line
	6150 4250 6400 4250
Wire Wire Line
	6150 4450 6400 4450
Wire Wire Line
	6150 3850 6400 3850
Wire Notes Line
	500  550  13000 550 
Wire Notes Line
	500  550  500  9350
Wire Notes Line
	7900 2950 7900 6250
Wire Notes Line
	3550 2950 3550 6250
Wire Wire Line
	2700 3350 2700 3250
Wire Wire Line
	3150 5300 3300 5300
Wire Wire Line
	2700 4950 2700 5450
Wire Wire Line
	2450 5300 2850 5300
Wire Wire Line
	2000 4950 2250 4950
Connection ~ 2000 5300
Wire Wire Line
	2000 5250 2000 5450
Wire Wire Line
	2050 5300 2000 5300
Connection ~ 2250 4950
Wire Wire Line
	2250 4900 2250 5000
Connection ~ 2700 4250
Wire Wire Line
	2700 4150 2700 4350
Wire Wire Line
	2550 4250 2950 4250
Wire Wire Line
	2250 4600 2250 4450
Connection ~ 2700 3800
Wire Wire Line
	2250 3800 2250 4050
Wire Wire Line
	2700 3800 2250 3800
Wire Wire Line
	2700 3750 2700 3850
Wire Wire Line
	6200 3650 6200 3600
Wire Wire Line
	6150 3650 6200 3650
Wire Wire Line
	4750 3400 4750 3750
Wire Wire Line
	4800 3400 4800 3250
Wire Wire Line
	4750 3400 4900 3400
Wire Wire Line
	4900 3400 4900 3650
Wire Notes Line
	6500 2950 6500 550 
Wire Notes Line
	3850 2950 3850 550 
Connection ~ 6050 2000
Wire Wire Line
	6050 1900 6050 2100
Wire Wire Line
	6300 2000 6050 2000
Wire Wire Line
	6300 1900 6300 2000
Wire Wire Line
	4000 2150 4000 2350
Connection ~ 5000 1550
Wire Wire Line
	5000 1550 5000 1950
Wire Wire Line
	5000 1950 5150 1950
Connection ~ 4550 2250
Wire Wire Line
	4550 2250 4550 2300
Wire Wire Line
	4400 2250 4400 2150
Wire Wire Line
	4400 2250 4750 2250
Wire Wire Line
	4750 2250 4750 1750
Wire Wire Line
	4750 1750 5150 1750
Connection ~ 4000 1550
Connection ~ 4400 1550
Wire Wire Line
	4400 1850 4400 1550
Wire Wire Line
	4000 1550 5150 1550
Connection ~ 6050 1550
Wire Wire Line
	6300 1550 6300 1600
Wire Wire Line
	6050 1350 6050 1600
Wire Wire Line
	5950 1550 6300 1550
Wire Wire Line
	750  1650 750  1700
Wire Wire Line
	750  1700 800  1700
Wire Wire Line
	3000 1300 3000 1200
Wire Wire Line
	2250 950  2250 1300
Connection ~ 2250 1150
Wire Wire Line
	1950 1300 1900 1300
Wire Wire Line
	1950 950  1900 950 
Wire Wire Line
	750  1950 750  1900
Wire Wire Line
	750  1900 800  1900
Wire Wire Line
	1900 1700 2000 1700
Wire Wire Line
	2300 1700 2400 1700
Wire Wire Line
	1900 1900 2050 1900
Wire Wire Line
	2550 1150 2700 1150
Wire Wire Line
	2700 1150 2700 1500
Wire Wire Line
	2700 1500 2800 1500
Wire Wire Line
	3000 1900 3000 2100
Wire Wire Line
	2050 2350 2050 2400
Wire Wire Line
	2050 1900 2050 2050
Wire Wire Line
	2700 1700 2800 1700
Wire Wire Line
	2750 1700 2750 2550
Wire Wire Line
	2750 2550 2950 2550
Connection ~ 2750 1700
Wire Wire Line
	3400 1600 3500 1600
Wire Wire Line
	3450 2550 3450 1600
Connection ~ 3450 1600
Wire Wire Line
	3250 2550 3450 2550
Wire Wire Line
	4000 1300 4000 1850
Wire Wire Line
	5300 2400 5300 2450
Wire Wire Line
	5300 2450 5350 2450
Wire Wire Line
	5300 2700 5300 2650
Wire Wire Line
	5300 2650 5350 2650
Wire Wire Line
	5250 2550 5350 2550
Wire Wire Line
	7650 1850 7650 1900
Wire Wire Line
	7650 1900 7700 1900
Wire Wire Line
	7700 2100 7650 2100
Wire Wire Line
	7650 2100 7650 2150
Wire Wire Line
	7600 2000 7700 2000
Wire Wire Line
	700  4850 700  4750
Wire Wire Line
	600  4300 600  4350
Connection ~ 700  4350
Connection ~ 4800 3400
$Comp
L GND #PWR?
U 1 1 583E8B95
P 3750 4450
F 0 "#PWR?" H 3750 4200 50  0001 C CNN
F 1 "GND" H 3750 4300 50  0000 C CNN
F 2 "" H 3750 4450 50  0000 C CNN
F 3 "" H 3750 4450 50  0000 C CNN
	1    3750 4450
	1    0    0    -1  
$EndComp
Text GLabel 4800 4150 0    60   Input ~ 0
IO15
Wire Wire Line
	4800 4150 4900 4150
Wire Wire Line
	4800 4500 4800 4450
Wire Wire Line
	4800 4450 4900 4450
Wire Wire Line
	3750 3900 3750 4050
Wire Wire Line
	3750 4350 3750 4450
Text GLabel 4000 4450 3    60   Input ~ 0
IO2
$Comp
L R R?
U 1 1 583E9909
P 4000 4200
F 0 "R?" V 4080 4200 50  0000 C CNN
F 1 "10K" V 4000 4200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3930 4200 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 4000 4200 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 4000 4200 60  0001 C CNN "Part Number"
	1    4000 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 4450 4000 4350
Text GLabel 4000 3850 1    60   Input ~ 0
3v3
Wire Wire Line
	4000 3850 4000 4050
$EndSCHEMATC
