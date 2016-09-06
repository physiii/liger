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
LIBS:liger-cache
EELAYER 25 0
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
L V23105 Relay1
U 1 1 57633D5A
P 8400 1550
F 0 "Relay1" H 7750 1750 50  0000 L CNN
F 1 "V23105" H 8850 1400 50  0000 L CNN
F 2 "libraries:V23105" H 8400 1550 50  0001 C CNN
F 3 "file:///home/chris/Downloads/ENG_SS_108-98007_L%20(3).pdf" H 8400 1550 50  0001 C CNN
F 4 "V23105A5001A201" H 8400 1550 60  0001 C CNN "Part Number"
	1    8400 1550
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 57633FAF
P 1450 1300
F 0 "R3" V 1530 1300 50  0000 C CNN
F 1 "2.2K" V 1450 1300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1380 1300 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 1450 1300 50  0001 C CNN
F 4 "RC0603FR-072K2L" V 1450 1300 60  0001 C CNN "Part Number"
	1    1450 1300
	1    0    0    -1  
$EndComp
$Comp
L R R11
U 1 1 57634014
P 3050 950
F 0 "R11" V 3130 950 50  0000 C CNN
F 1 "1M" V 3050 950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2980 950 50  0001 C CNN
F 3 "http://www.yageo.com/documents/recent/PYu-RC_Group_51_RoHS_L_04.pdf" H 3050 950 50  0001 C CNN
F 4 "RC0603FR-071ML" V 3050 950 60  0001 C CNN "Part Number"
	1    3050 950 
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 5763405B
P 2200 1650
F 0 "R5" V 2280 1650 50  0000 C CNN
F 1 "10K" V 2200 1650 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2130 1650 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 2200 1650 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 2200 1650 60  0001 C CNN "Part Number"
	1    2200 1650
	0    1    1    0   
$EndComp
$Comp
L C C1
U 1 1 576341A5
P 1700 1650
F 0 "C1" V 1750 1700 50  0000 L CNN
F 1 "4.7uF" V 1750 1400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1738 1500 50  0001 C CNN
F 3 "http://media.digikey.com/pdf/Data%20Sheets/Samsung%20PDFs/CL_Series_MLCC_ds.pdf" H 1700 1650 50  0001 C CNN
F 4 "CL10B475KQ8NQNC" H 1700 1650 60  0001 C CNN "Part Number"
	1    1700 1650
	0    -1   -1   0   
$EndComp
$Comp
L C C3
U 1 1 57634222
P 2250 2150
F 0 "C3" H 2275 2250 50  0000 L CNN
F 1 "10uF" H 2050 2050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2288 2000 50  0001 C CNN
F 3 "http://www.murata.com/~/media/webrenewal/support/library/catalog/products/capacitor/mlcc/c02e.pdf" H 2250 2150 50  0001 C CNN
F 4 "GRM188R60G106ME47D" H 2250 2150 60  0001 C CNN "Part Number"
	1    2250 2150
	1    0    0    -1  
$EndComp
$Comp
L C C10
U 1 1 57634242
P 3050 750
F 0 "C10" V 3100 800 50  0000 L CNN
F 1 "12pF" V 3100 500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 3088 600 50  0001 C CNN
F 3 "http://www.kemet.com/Lists/ProductCatalog/Attachments/46/KEM_C1003_C0G_SMD.pdf" H 3050 750 50  0001 C CNN
F 4 "C0402C120J5GACTU" H 3050 750 60  0001 C CNN "Part Number"
	1    3050 750 
	0    -1   -1   0   
$EndComp
$Comp
L ADA4075 U2
U 1 1 576343F3
P 3050 1750
F 0 "U2" H 3050 1900 50  0000 L CNN
F 1 "ADA4075" H 3050 1600 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2950 1800 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/609/OP275-879536.pdf" H 3050 1900 50  0001 C CNN
F 4 "OPA344NA/3K" H 3050 1750 60  0001 C CNN "Part Number"
	1    3050 1750
	1    0    0    -1  
$EndComp
$Comp
L electret_microphone U1
U 1 1 57634A59
P 750 1850
F 0 "U1" H 900 1600 60  0000 C CNN
F 1 "electret_microphone" H 750 2350 60  0000 C CNN
F 2 "libraries:Electret" H 650 2000 60  0001 C CNN
F 3 "http://www.mouser.com/ds/2/670/cmc-5044pf-a-515902.pdf" H 650 2000 60  0001 C CNN
F 4 "CMC-5044PF-A" H 750 1850 60  0001 C CNN "Part Number"
	1    750  1850
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR01
U 1 1 57635FF7
P 1450 1000
F 0 "#PWR01" H 1450 850 50  0001 C CNN
F 1 "+3.3V" H 1450 1140 50  0000 C CNN
F 2 "" H 1450 1000 50  0000 C CNN
F 3 "" H 1450 1000 50  0000 C CNN
	1    1450 1000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR02
U 1 1 57636057
P 2450 1050
F 0 "#PWR02" H 2450 900 50  0001 C CNN
F 1 "+3.3V" H 2450 1190 50  0000 C CNN
F 2 "" H 2450 1050 50  0000 C CNN
F 3 "" H 2450 1050 50  0000 C CNN
	1    2450 1050
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR03
U 1 1 57636091
P 2950 1350
F 0 "#PWR03" H 2950 1200 50  0001 C CNN
F 1 "+3.3V" H 2950 1490 50  0000 C CNN
F 2 "" H 2950 1350 50  0000 C CNN
F 3 "" H 2950 1350 50  0000 C CNN
	1    2950 1350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 57636188
P 1150 2150
F 0 "#PWR04" H 1150 1900 50  0001 C CNN
F 1 "GND" H 1150 2000 50  0000 C CNN
F 2 "" H 1150 2150 50  0000 C CNN
F 3 "" H 1150 2150 50  0000 C CNN
	1    1150 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 576361F5
P 2450 2700
F 0 "#PWR05" H 2450 2450 50  0001 C CNN
F 1 "GND" H 2450 2550 50  0000 C CNN
F 2 "" H 2450 2700 50  0000 C CNN
F 3 "" H 2450 2700 50  0000 C CNN
	1    2450 2700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 57636208
P 2950 2300
F 0 "#PWR06" H 2950 2050 50  0001 C CNN
F 1 "GND" H 2950 2150 50  0000 C CNN
F 2 "" H 2950 2300 50  0000 C CNN
F 3 "" H 2950 2300 50  0000 C CNN
	1    2950 2300
	1    0    0    -1  
$EndComp
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
L ZENER D3
U 1 1 57682607
P 4000 1150
F 0 "D3" V 4100 1200 50  0000 C CNN
F 1 "ZENER" V 4100 1000 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 4000 1150 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/308/MBR120VLSFT1-D-107081.pdf" H 4000 1150 50  0001 C CNN
F 4 "MBR120VLSFT1G" H 4000 1150 60  0001 C CNN "Part Number"
	1    4000 1150
	0    -1   -1   0   
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
L GND #PWR07
U 1 1 57682AFF
P 4550 2300
F 0 "#PWR07" H 4550 2050 50  0001 C CNN
F 1 "GND" H 4550 2150 50  0000 C CNN
F 2 "" H 4550 2300 50  0000 C CNN
F 3 "" H 4550 2300 50  0000 C CNN
	1    4550 2300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 57682B37
P 4000 2350
F 0 "#PWR08" H 4000 2100 50  0001 C CNN
F 1 "GND" H 4000 2200 50  0000 C CNN
F 2 "" H 4000 2350 50  0000 C CNN
F 3 "" H 4000 2350 50  0000 C CNN
	1    4000 2350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 57682CA4
P 6050 2100
F 0 "#PWR09" H 6050 1850 50  0001 C CNN
F 1 "GND" H 6050 1950 50  0000 C CNN
F 2 "" H 6050 2100 50  0000 C CNN
F 3 "" H 6050 2100 50  0000 C CNN
	1    6050 2100
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR010
U 1 1 57682E51
P 4000 850
F 0 "#PWR010" H 4000 700 50  0001 C CNN
F 1 "+5V" H 4000 990 50  0000 C CNN
F 2 "" H 4000 850 50  0000 C CNN
F 3 "" H 4000 850 50  0000 C CNN
	1    4000 850 
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR011
U 1 1 5768313C
P 6050 1350
F 0 "#PWR011" H 6050 1200 50  0001 C CNN
F 1 "+3.3V" H 6050 1490 50  0000 C CNN
F 2 "" H 6050 1350 50  0000 C CNN
F 3 "" H 6050 1350 50  0000 C CNN
	1    6050 1350
	1    0    0    -1  
$EndComp
Text Label 4300 750  0    60   ~ 0
Regulator
Text Label 650  750  0    60   ~ 0
Mic_pre-amp
$Comp
L D D4
U 1 1 57684FDE
P 7600 1600
F 0 "D4" H 7600 1700 50  0000 C CNN
F 1 "D" H 7600 1500 50  0000 C CNN
F 2 "Diodes_SMD:SMA_Standard" H 7600 1600 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/149/BAT54-888471.pdf" H 7600 1600 50  0001 C CNN
F 4 "SS14" H 7600 1600 60  0001 C CNN "Part Number"
	1    7600 1600
	0    1    1    0   
$EndComp
$Comp
L Q_NPN_BEC Q3
U 1 1 576851E0
P 7500 2250
F 0 "Q3" H 7800 2300 50  0000 R CNN
F 1 "Q_NPN_BEC" H 8100 2200 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 7700 2350 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/308/MMBT3904LT1-D-113169.pdf" H 7500 2250 50  0001 C CNN
F 4 "MMBT3904LT1G" H 7500 2250 60  0001 C CNN "Part Number"
	1    7500 2250
	1    0    0    -1  
$EndComp
Text GLabel 6800 2250 0    60   Input ~ 0
IO15
$Comp
L CONN_01X02 P4
U 1 1 57685A47
P 9250 1700
F 0 "P4" H 9250 1850 50  0000 C CNN
F 1 "CONN_01X02" V 9350 1700 50  0000 C CNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_MKDS1.5-2pol" H 9250 1700 50  0001 C CNN
F 3 "http://media.digikey.com/pdf/Data%20Sheets/Phoenix%20Contact%20PDFs/1935161.pdf" H 9250 1700 50  0001 C CNN
F 4 "1935161" H 9250 1700 60  0001 C CNN "Part Number"
	1    9250 1700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 57685FFE
P 7600 2550
F 0 "#PWR012" H 7600 2300 50  0001 C CNN
F 1 "GND" H 7600 2400 50  0000 C CNN
F 2 "" H 7600 2550 50  0000 C CNN
F 3 "" H 7600 2550 50  0000 C CNN
	1    7600 2550
	1    0    0    -1  
$EndComp
Text Label 6750 750  0    60   ~ 0
Relay
$Comp
L GND #PWR013
U 1 1 57687E42
P 6200 3600
F 0 "#PWR013" H 6200 3350 50  0001 C CNN
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
P 650 4900
F 0 "R1" V 730 4900 50  0000 C CNN
F 1 "1K" V 650 4900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 580 4900 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 650 4900 50  0001 C CNN
F 4 "RC0603JR-071KL" V 650 4900 60  0001 C CNN "Part Number"
	1    650  4900
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR014
U 1 1 57689636
P 1500 4100
F 0 "#PWR014" H 1500 3950 50  0001 C CNN
F 1 "+3.3V" H 1500 4240 50  0000 C CNN
F 2 "" H 1500 4100 50  0000 C CNN
F 3 "" H 1500 4100 50  0000 C CNN
	1    1500 4100
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR015
U 1 1 57689EA7
P 650 4500
F 0 "#PWR015" H 650 4350 50  0001 C CNN
F 1 "+3.3V" H 650 4640 50  0000 C CNN
F 2 "" H 650 4500 50  0000 C CNN
F 3 "" H 650 4500 50  0000 C CNN
	1    650  4500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 57689F0C
P 1050 5300
F 0 "#PWR016" H 1050 5050 50  0001 C CNN
F 1 "GND" H 1050 5150 50  0000 C CNN
F 2 "" H 1050 5300 50  0000 C CNN
F 3 "" H 1050 5300 50  0000 C CNN
	1    1050 5300
	1    0    0    -1  
$EndComp
Text GLabel 1050 4400 1    60   Input ~ 0
IO15
Text GLabel 1500 5550 3    60   Input ~ 0
IO5
Text GLabel 650  5250 3    60   Input ~ 0
IO2
$Comp
L SW_PUSH SW1
U 1 1 5768B31A
P 2700 4750
F 0 "SW1" H 2850 4860 50  0000 C CNN
F 1 "SW_PUSH" H 2700 4670 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_KMR2" H 2700 4750 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/60/KMR2_9aug12-25127.pdf" H 2700 4750 50  0001 C CNN
F 4 "KMR211NG LFS" H 2700 4750 60  0001 C CNN "Part Number"
	1    2700 4750
	0    1    1    0   
$EndComp
Text GLabel 3300 5400 2    60   Input ~ 0
IO0
Text GLabel 2950 4350 2    60   Input ~ 0
RESET
$Comp
L GND #PWR017
U 1 1 5768D6FF
P 2700 5550
F 0 "#PWR017" H 2700 5300 50  0001 C CNN
F 1 "GND" H 2700 5400 50  0000 C CNN
F 2 "" H 2700 5550 50  0000 C CNN
F 3 "" H 2700 5550 50  0000 C CNN
	1    2700 5550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 5768DA26
P 2000 5550
F 0 "#PWR018" H 2000 5300 50  0001 C CNN
F 1 "GND" H 2000 5400 50  0000 C CNN
F 2 "" H 2000 5550 50  0000 C CNN
F 3 "" H 2000 5550 50  0000 C CNN
	1    2000 5550
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR019
U 1 1 5768DCF5
P 2700 3350
F 0 "#PWR019" H 2700 3200 50  0001 C CNN
F 1 "+3.3V" H 2700 3490 50  0000 C CNN
F 2 "" H 2700 3350 50  0000 C CNN
F 3 "" H 2700 3350 50  0000 C CNN
	1    2700 3350
	1    0    0    -1  
$EndComp
$Comp
L Q_PMOS_GSD Q2
U 1 1 576D6D77
P 2350 4350
F 0 "Q2" H 2650 4400 50  0000 R CNN
F 1 "Q_PMOS_GSD" H 3000 4300 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 2550 4450 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/308/BSS84LT1-D-105913.pdf" H 2350 4350 50  0001 C CNN
F 4 "BSS84LT1G" H 2350 4350 60  0001 C CNN "Part Number"
	1    2350 4350
	-1   0    0    1   
$EndComp
$Comp
L Q_NMOS_GSD Q1
U 1 1 576D6F9D
P 2250 5300
F 0 "Q1" V 2150 5450 50  0000 R CNN
F 1 "Q_NMOS_GSD" V 2450 5550 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 2450 5400 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/308/2N7002L-D-105484.pdf" H 2250 5300 50  0001 C CNN
F 4 "2N7002L" V 2250 5300 60  0001 C CNN "Part Number"
	1    2250 5300
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 576DE8EB
P 1050 4900
F 0 "R2" V 1130 4900 50  0000 C CNN
F 1 "1K" V 1050 4900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 980 4900 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 1050 4900 50  0001 C CNN
F 4 "RC0603JR-071KL" V 1050 4900 60  0001 C CNN "Part Number"
	1    1050 4900
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 576DE941
P 1500 5150
F 0 "R4" V 1580 5150 50  0000 C CNN
F 1 "1K" V 1500 5150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 1430 5150 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 1500 5150 50  0001 C CNN
F 4 "RC0603JR-071KL" V 1500 5150 60  0001 C CNN "Part Number"
	1    1500 5150
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 576DEB45
P 2250 4850
F 0 "R6" V 2330 4850 50  0000 C CNN
F 1 "1K" V 2250 4850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2180 4850 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 2250 4850 50  0001 C CNN
F 4 "RC0603JR-071KL" V 2250 4850 60  0001 C CNN "Part Number"
	1    2250 4850
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 576DEF74
P 2700 4100
F 0 "R9" V 2780 4100 50  0000 C CNN
F 1 "1K" V 2700 4100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2630 4100 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 2700 4100 50  0001 C CNN
F 4 "RC0603JR-071KL" V 2700 4100 60  0001 C CNN "Part Number"
	1    2700 4100
	1    0    0    -1  
$EndComp
$Comp
L R R10
U 1 1 576DF049
P 3000 5400
F 0 "R10" V 3080 5400 50  0000 C CNN
F 1 "1K" V 3000 5400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2930 5400 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 3000 5400 50  0001 C CNN
F 4 "RC0603JR-071KL" V 3000 5400 60  0001 C CNN "Part Number"
	1    3000 5400
	0    1    1    0   
$EndComp
$Comp
L R R14
U 1 1 576E06B8
P 7000 2250
F 0 "R14" V 7080 2250 50  0000 C CNN
F 1 "1K" V 7000 2250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 6930 2250 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 7000 2250 50  0001 C CNN
F 4 "RC0603JR-071KL" V 7000 2250 60  0001 C CNN "Part Number"
	1    7000 2250
	0    1    1    0   
$EndComp
$Comp
L R R7
U 1 1 576E475A
P 2450 1400
F 0 "R7" H 2550 1350 50  0000 C CNN
F 1 "10K" V 2450 1400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2380 1400 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 2450 1400 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 2450 1400 60  0001 C CNN "Part Number"
	1    2450 1400
	-1   0    0    1   
$EndComp
$Comp
L R R8
U 1 1 576E479D
P 2450 2150
F 0 "R8" H 2350 2100 50  0000 C CNN
F 1 "10K" V 2450 2150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2380 2150 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 2450 2150 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 2450 2150 60  0001 C CNN "Part Number"
	1    2450 2150
	-1   0    0    1   
$EndComp
$Comp
L R R12
U 1 1 576E4AC5
P 4400 2000
F 0 "R12" V 4480 2000 50  0000 C CNN
F 1 "10K" V 4400 2000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4330 2000 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 4400 2000 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 4400 2000 60  0001 C CNN "Part Number"
	1    4400 2000
	-1   0    0    1   
$EndComp
$Comp
L R R13
U 1 1 576E9D8A
P 4750 3550
F 0 "R13" V 4830 3550 50  0000 C CNN
F 1 "10K" V 4750 3550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4680 3550 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 4750 3550 50  0001 C CNN
F 4 "RC0603JR-0710KL" V 4750 3550 60  0001 C CNN "Part Number"
	1    4750 3550
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
P 2000 5200
F 0 "C2" H 2025 5300 50  0000 L CNN
F 1 "4.7uF" H 2000 5100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2038 5050 50  0001 C CNN
F 3 "http://media.digikey.com/pdf/Data%20Sheets/Samsung%20PDFs/CL_Series_MLCC_ds.pdf" H 2000 5200 50  0001 C CNN
F 4 "CL10B475KQ8NQNC" H 2000 5200 60  0001 C CNN "Part Number"
	1    2000 5200
	-1   0    0    1   
$EndComp
$Comp
L LED D1
U 1 1 57710784
P 1500 4550
F 0 "D1" H 1500 4650 50  0000 C CNN
F 1 "LED" H 1650 4500 50  0000 C CNN
F 2 "LEDs:LED_0603" H 1500 4550 50  0001 C CNN
F 3 "http://www.osram-os.com/Graphics/XPic9/00128792_0.pdf" H 1500 4550 50  0001 C CNN
F 4 "LS Q976-NR-1" H 1500 4550 60  0001 C CNN "Part Number"
	1    1500 4550
	0    -1   -1   0   
$EndComp
$Comp
L LED D2
U 1 1 577107D2
P 2700 3650
F 0 "D2" H 2700 3750 50  0000 C CNN
F 1 "LED" H 2850 3600 50  0000 C CNN
F 2 "LEDs:LED_0603" H 2700 3650 50  0001 C CNN
F 3 "http://www.osram-os.com/Graphics/XPic9/00128792_0.pdf" H 2700 3650 50  0001 C CNN
F 4 "LS Q976-NR-1" H 2700 3650 60  0001 C CNN "Part Number"
	1    2700 3650
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
Text GLabel 3500 1750 2    60   Input ~ 0
ADC
Text GLabel 4800 4150 0    60   Input ~ 0
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
L GND #PWR020
U 1 1 57704A85
P 4800 4550
F 0 "#PWR020" H 4800 4300 50  0001 C CNN
F 1 "GND" H 4800 4400 50  0000 C CNN
F 2 "" H 4800 4550 50  0000 C CNN
F 3 "" H 4800 4550 50  0000 C CNN
	1    4800 4550
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR021
U 1 1 577051B4
P 4800 3250
F 0 "#PWR021" H 4800 3100 50  0001 C CNN
F 1 "+3.3V" H 4800 3390 50  0000 C CNN
F 2 "" H 4800 3250 50  0000 C CNN
F 3 "" H 4800 3250 50  0000 C CNN
	1    4800 3250
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P5
U 1 1 57706983
P 10150 1800
F 0 "P5" H 10150 2050 50  0000 C CNN
F 1 "CONN_01X04" V 10250 1800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x04" H 10150 1800 50  0001 C CNN
F 3 "http://sullinscorp.com/catalogs/101_PAGE114-115_.100_FEMALE_HDR.pdf" H 10150 1800 50  0001 C CNN
F 4 "PPTC041LGBN-RC" H 10150 1800 60  0001 C CNN "Part Number"
	1    10150 1800
	1    0    0    -1  
$EndComp
Text Label 9750 750  0    60   ~ 0
Ultra-Sonic_Sensor
Text Label 650  3250 0    60   ~ 0
Switches_N_Stuff
Text Label 3650 3200 0    60   ~ 0
wifi-microcontroller
$Comp
L PT5529B/L2/H2-F U5
U 1 1 57717626
P 11750 700
F 0 "U5" H 11450 600 60  0000 C CNN
F 1 "PT5529B/L2/H2-F" H 12350 700 60  0000 C CNN
F 2 "libraries:PT5529B" H 11750 700 60  0001 C CNN
F 3 "http://www.everlight.com/file/ProductFile/PT5529B-L2-H2-F.pdf" H 11750 700 60  0001 C CNN
F 4 "PT5529B/L2/H2-F" H 11750 700 60  0001 C CNN "Part Number"
	1    11750 700 
	1    0    0    -1  
$EndComp
Text Label 10900 750  0    60   ~ 0
IR-Blaster
$Comp
L +5V #PWR022
U 1 1 5771C30D
P 12050 1000
F 0 "#PWR022" H 12050 850 50  0001 C CNN
F 1 "+5V" H 12050 1140 50  0000 C CNN
F 2 "" H 12050 1000 50  0000 C CNN
F 3 "" H 12050 1000 50  0000 C CNN
	1    12050 1000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 5771C669
P 11750 1150
F 0 "#PWR023" H 11750 900 50  0001 C CNN
F 1 "GND" H 11750 1000 50  0000 C CNN
F 2 "" H 11750 1150 50  0000 C CNN
F 3 "" H 11750 1150 50  0000 C CNN
	1    11750 1150
	1    0    0    -1  
$EndComp
Text GLabel 11050 1100 0    60   Input ~ 0
IO4
$Comp
L R R17
U 1 1 5771E483
P 11300 1100
F 0 "R17" V 11380 1100 50  0000 C CNN
F 1 "1K" V 11300 1100 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 11230 1100 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 11300 1100 50  0001 C CNN
F 4 "RC0603JR-071KL" V 11300 1100 60  0001 C CNN "Part Number"
	1    11300 1100
	0    1    1    0   
$EndComp
$Comp
L LED D8
U 1 1 57722A1E
P 12700 1700
F 0 "D8" H 12700 1800 50  0000 C CNN
F 1 "LED" H 12700 1600 50  0000 C CNN
F 2 "LEDs:LED-3MM" H 12700 1700 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/149/QED123-890063.pdf" H 12700 1700 50  0001 C CNN
F 4 "QED123" H 12700 1700 60  0001 C CNN "Part Number"
	1    12700 1700
	0    -1   -1   0   
$EndComp
$Comp
L R R25
U 1 1 577249CF
P 12700 1250
F 0 "R25" V 12780 1250 50  0000 C CNN
F 1 "36" V 12700 1250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 12630 1250 50  0001 C CNN
F 3 "http://www.yageo.com.tw/exep/pages/download/literatures/PYu-R_INT-thick_7.pdf" H 12700 1250 50  0001 C CNN
F 4 "RC0603JR-0736RL" V 12700 1250 60  0001 C CNN "Part Number"
	1    12700 1250
	1    0    0    -1  
$EndComp
$Comp
L R R24
U 1 1 5772613E
P 12000 2200
F 0 "R24" V 12080 2200 50  0000 C CNN
F 1 "1K" V 12000 2200 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 11930 2200 50  0001 C CNN
F 3 "http://www.digikey.com/en/resources/datasheets/yageo/rc-series-l-suffix-datasheet" H 12000 2200 50  0001 C CNN
F 4 "RC0603JR-071KL" V 12000 2200 60  0001 C CNN "Part Number"
	1    12000 2200
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR024
U 1 1 57726F3B
P 12700 950
F 0 "#PWR024" H 12700 800 50  0001 C CNN
F 1 "+5V" H 12700 1090 50  0000 C CNN
F 2 "" H 12700 950 50  0000 C CNN
F 3 "" H 12700 950 50  0000 C CNN
	1    12700 950 
	1    0    0    -1  
$EndComp
$Comp
L Q_NPN_BEC Q4
U 1 1 577276B5
P 12600 2200
F 0 "Q4" H 12900 2250 50  0000 R CNN
F 1 "Q_NPN_BEC" H 13200 2150 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 12800 2300 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/308/MMBT3904LT1-D-113169.pdf" H 12600 2200 50  0001 C CNN
F 4 "MMBT3904LT1G" H 12600 2200 60  0001 C CNN "Part Number"
	1    12600 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR025
U 1 1 57727DF2
P 12700 2600
F 0 "#PWR025" H 12700 2350 50  0001 C CNN
F 1 "GND" H 12700 2450 50  0000 C CNN
F 2 "" H 12700 2600 50  0000 C CNN
F 3 "" H 12700 2600 50  0000 C CNN
	1    12700 2600
	1    0    0    -1  
$EndComp
Text GLabel 11550 2200 0    60   Input ~ 0
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
L GND #PWR026
U 1 1 57744532
P 6400 5000
F 0 "#PWR026" H 6400 4750 50  0001 C CNN
F 1 "GND" H 6400 4850 50  0000 C CNN
F 2 "" H 6400 5000 50  0000 C CNN
F 3 "" H 6400 5000 50  0000 C CNN
	1    6400 5000
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR027
U 1 1 57744A7C
P 6900 5150
F 0 "#PWR027" H 6900 5000 50  0001 C CNN
F 1 "+5V" H 6900 5290 50  0000 C CNN
F 2 "" H 6900 5150 50  0000 C CNN
F 3 "" H 6900 5150 50  0000 C CNN
	1    6900 5150
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
EN
Text GLabel 4600 5850 0    60   Input ~ 0
EN
$Comp
L GND #PWR028
U 1 1 57746D84
P 4550 5050
F 0 "#PWR028" H 4550 4800 50  0001 C CNN
F 1 "GND" H 4550 4900 50  0000 C CNN
F 2 "" H 4550 5050 50  0000 C CNN
F 3 "" H 4550 5050 50  0000 C CNN
	1    4550 5050
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR029
U 1 1 57746FCF
P 4600 5950
F 0 "#PWR029" H 4600 5800 50  0001 C CNN
F 1 "+3.3V" H 4600 6090 50  0000 C CNN
F 2 "" H 4600 5950 50  0000 C CNN
F 3 "" H 4600 5950 50  0000 C CNN
	1    4600 5950
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR030
U 1 1 57758382
P 7600 1050
F 0 "#PWR030" H 7600 900 50  0001 C CNN
F 1 "+5V" H 7600 1190 50  0000 C CNN
F 2 "" H 7600 1050 50  0000 C CNN
F 3 "" H 7600 1050 50  0000 C CNN
	1    7600 1050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR031
U 1 1 57771C8C
P 6750 4200
F 0 "#PWR031" H 6750 3950 50  0001 C CNN
F 1 "GND" H 6750 4050 50  0000 C CNN
F 2 "" H 6750 4200 50  0000 C CNN
F 3 "" H 6750 4200 50  0000 C CNN
	1    6750 4200
	1    0    0    -1  
$EndComp
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
Wire Wire Line
	9000 1650 9050 1650
Wire Wire Line
	9000 1950 9000 1650
Wire Wire Line
	8750 1950 9000 1950
Connection ~ 7600 1150
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
	11850 2200 11550 2200
Wire Wire Line
	12700 2400 12700 2600
Wire Wire Line
	12150 2200 12400 2200
Wire Wire Line
	12700 2000 12700 1900
Wire Wire Line
	12700 1400 12700 1500
Wire Wire Line
	12700 950  12700 1100
Wire Wire Line
	11150 1100 11050 1100
Wire Wire Line
	12050 1100 12050 1000
Wire Wire Line
	11850 1100 12050 1100
Wire Wire Line
	11850 1000 11850 1100
Wire Wire Line
	11750 1000 11750 1150
Wire Wire Line
	11650 1100 11450 1100
Wire Wire Line
	11650 1000 11650 1100
Wire Notes Line
	10750 550  10750 2950
Wire Wire Line
	4800 4550 4800 4450
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
	4800 4450 4900 4450
Wire Wire Line
	6150 4050 6400 4050
Wire Wire Line
	6150 3950 6400 3950
Wire Wire Line
	6150 4250 6400 4250
Wire Wire Line
	6150 4450 6400 4450
Wire Wire Line
	4900 4150 4800 4150
Wire Wire Line
	6150 3850 6400 3850
Wire Wire Line
	3350 1750 3500 1750
Wire Notes Line
	500  550  13000 550 
Wire Notes Line
	9650 550  9650 2950
Wire Notes Line
	13000 550  13000 6250
Wire Notes Line
	500  2950 13000 2950
Wire Notes Line
	500  6250 500  550 
Wire Notes Line
	7900 6250 7900 2950
Wire Notes Line
	13000 6250 500  6250
Wire Notes Line
	3550 2950 3550 6250
Wire Wire Line
	2700 3450 2700 3350
Wire Wire Line
	3150 5400 3300 5400
Wire Wire Line
	2700 5050 2700 5550
Wire Wire Line
	2450 5400 2850 5400
Wire Wire Line
	2000 5050 2250 5050
Connection ~ 2000 5400
Wire Wire Line
	2000 5350 2000 5550
Wire Wire Line
	2050 5400 2000 5400
Connection ~ 2250 5050
Wire Wire Line
	2250 5000 2250 5100
Connection ~ 2700 4350
Wire Wire Line
	2700 4250 2700 4450
Wire Wire Line
	2550 4350 2950 4350
Wire Wire Line
	2250 4700 2250 4550
Connection ~ 2700 3900
Wire Wire Line
	2250 3900 2250 4150
Wire Wire Line
	2700 3900 2250 3900
Wire Wire Line
	2700 3850 2700 3950
Wire Wire Line
	1500 4350 1500 4100
Wire Wire Line
	1500 5300 1500 5550
Wire Wire Line
	1500 5000 1500 4750
Wire Wire Line
	1050 4750 1050 4400
Wire Wire Line
	1050 5050 1050 5300
Wire Wire Line
	650  5050 650  5250
Wire Wire Line
	650  4750 650  4500
Wire Wire Line
	6200 3650 6200 3600
Wire Wire Line
	6150 3650 6200 3650
Wire Wire Line
	4750 3700 4750 3750
Connection ~ 4800 3400
Wire Wire Line
	4800 3400 4800 3250
Wire Wire Line
	4750 3400 4900 3400
Wire Wire Line
	4900 3400 4900 3650
Wire Wire Line
	7600 2450 7600 2550
Wire Wire Line
	9050 2050 9050 1750
Wire Wire Line
	8550 2050 9050 2050
Wire Wire Line
	8550 1950 8550 2050
Wire Wire Line
	6850 2250 6800 2250
Wire Wire Line
	7300 2250 7150 2250
Connection ~ 7600 1950
Wire Wire Line
	7600 1950 8050 1950
Wire Wire Line
	7600 1750 7600 2050
Wire Wire Line
	7600 1050 7600 1450
Wire Wire Line
	8050 1150 7600 1150
Wire Notes Line
	6500 550  6500 2950
Wire Notes Line
	3850 2950 3850 550 
Wire Wire Line
	4000 950  4000 850 
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
	4000 1350 4000 1850
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
	1450 1150 1450 1000
Wire Wire Line
	2450 1250 2450 1050
Connection ~ 2450 2400
Wire Wire Line
	2250 2400 2450 2400
Wire Wire Line
	2250 2300 2250 2400
Wire Wire Line
	2450 2300 2450 2700
Wire Wire Line
	2250 1850 2250 2000
Connection ~ 2450 1850
Wire Wire Line
	2450 1550 2450 2000
Wire Wire Line
	2250 1850 2750 1850
Wire Wire Line
	2950 2050 2950 2300
Connection ~ 3400 1750
Wire Wire Line
	2950 1450 2950 1350
Wire Wire Line
	2350 1650 2750 1650
Connection ~ 2700 1650
Connection ~ 3400 950 
Wire Wire Line
	3400 950  3200 950 
Wire Wire Line
	3400 750  3400 1750
Wire Wire Line
	3200 750  3400 750 
Wire Wire Line
	2700 750  2900 750 
Wire Wire Line
	2700 750  2700 1650
Connection ~ 2700 950 
Wire Wire Line
	2700 950  2900 950 
Wire Wire Line
	1850 1650 2050 1650
Wire Wire Line
	950  1650 1550 1650
Connection ~ 1450 1650
Wire Wire Line
	1450 1650 1450 1450
Wire Wire Line
	1150 1950 1150 2150
Wire Wire Line
	950  1950 1150 1950
$Comp
L +3.3V #PWR032
U 1 1 577743ED
P 6750 5050
F 0 "#PWR032" H 6750 4900 50  0001 C CNN
F 1 "+3.3V" H 6750 5190 50  0000 C CNN
F 2 "" H 6750 5050 50  0000 C CNN
F 3 "" H 6750 5050 50  0000 C CNN
	1    6750 5050
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
L VDDA #PWR033
U 1 1 57C657E4
P 2800 9750
F 0 "#PWR033" H 2800 9600 50  0001 C CNN
F 1 "VDDA" V 2818 9878 50  0000 L CNN
F 2 "" H 2800 9750 50  0000 C CNN
F 3 "" H 2800 9750 50  0000 C CNN
	1    2800 9750
	0    -1   -1   0   
$EndComp
$Comp
L Diode_Bridge BR1
U 1 1 57C81314
P 3600 9250
F 0 "BR1" H 3250 9550 50  0000 L CNN
F 1 "DF08S" H 3000 9550 50  0001 L CNN
F 2 "Diodes_ThroughHole:Diode_Bridge_15.7x15.7" H 3600 9250 50  0001 C CNN
F 3 "http://www.digikey.com/scripts/DkSearch/dksus.dll?Detail&itemSeq=206259955&uq=636087571188496593" H 3600 9250 50  0001 C CNN
F 4 "DF08S" H 3600 9250 60  0001 C CNN "Part Number"
	1    3600 9250
	0    -1   -1   0   
$EndComp
$Comp
L VDD #PWR034
U 1 1 57C65667
P 2800 9250
F 0 "#PWR034" H 2800 9100 50  0001 C CNN
F 1 "VDD" V 2818 9377 50  0000 L CNN
F 2 "" H 2800 9250 50  0000 C CNN
F 3 "" H 2800 9250 50  0000 C CNN
	1    2800 9250
	0    -1   -1   0   
$EndComp
$Comp
L C C12
U 1 1 57C8658B
P 12400 7550
F 0 "C12" H 12500 7650 50  0000 L CNN
F 1 "330pF" H 12450 7450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 12515 7505 50  0001 L CNN
F 3 "http://www.digikey.com/product-detail/en/kemet/C0805C331KCRACTU/399-7151-1-ND/3439391" H 12515 7414 50  0001 L CNN
	1    12400 7550
	-1   0    0    1   
$EndComp
$Comp
L CP1 C9
U 1 1 57C87C6C
P 6250 8900
F 0 "C9" H 6365 8946 50  0000 L CNN
F 1 "100uF" H 6365 8855 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Radial_D10_L20_P7.5" H 6365 8855 50  0001 L CNN
F 3 "http://www.mouser.com/Search/ProductDetail.aspx?R=UPZ2G101MHDvirtualkey64700000virtualkey647-UPZ2G101MHD" H 6365 8764 50  0001 L CNN
F 4 "UPZ2G101MHD" H 6250 8900 60  0001 C CNN "Part Number"
	1    6250 8900
	1    0    0    -1  
$EndComp
$Comp
L C C11
U 1 1 57C8DD11
P 12400 7100
F 0 "C11" H 12500 7200 50  0000 L CNN
F 1 "4.7nF" H 12450 7000 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Rect_L13_W4_P10" H 12515 7055 50  0001 L CNN
F 3 "http://www.mouser.com/ProductDetail/EPCOS-TDK/B32641B0472K/?qs=sGAEpiMZZMv1cc3ydrPrFzRLALvOkyF1YTrqCCdx%2fGdcBMeHHlEFGg%3d%3d" H 12515 6964 50  0001 L CNN
F 4 "B32641B0472K" H 12400 7100 60  0001 C CNN "Part Number"
	1    12400 7100
	1    0    0    -1  
$EndComp
$Comp
L D D5
U 1 1 57C8E8E8
P 12400 6800
F 0 "D5" H 12400 7015 50  0000 C CNN
F 1 "D" H 12400 6924 50  0000 C CNN
F 2 "Diodes_SMD:SMB_Handsoldering" H 12400 7015 50  0001 C CNN
F 3 "http://www.mouser.com/ProductDetail/Fairchild-Semiconductor/S1M/?qs=sGAEpiMZZMtbRapU8LlZD6Aoap19JQAxIRiMhMkksfk%3d" H 12400 6924 50  0001 C CNN
F 4 "S1M" H 12400 6800 60  0001 C CNN "Part Number"
	1    12400 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 9250 3200 9250
$Comp
L R RT1
U 1 1 57CEDD2A
P 3150 9750
F 0 "RT1" V 3050 9750 50  0000 C CNN
F 1 "5" V 3150 9750 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 3080 9750 50  0001 C CNN
F 3 "http://www.digikey.com/scripts/DkSearch/dksus.dll?Detail&itemSeq=206246827&uq=636087498279562389" H 3150 9750 50  0001 C CNN
F 4 "SL05 5R001-A" V 3150 9750 60  0001 C CNN "Part Number"
	1    3150 9750
	0    1    1    0   
$EndComp
Wire Wire Line
	2800 9750 3000 9750
Wire Wire Line
	3300 9750 4000 9750
Wire Wire Line
	4000 9750 4000 9250
$Comp
L BK0603HS330-T L1
U 1 1 57CF1BCD
P 4700 10100
F 0 "L1" H 4800 10050 60  0000 C CNN
F 1 "BEAD" H 4850 9950 60  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 4700 10100 60  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/tdk-corporation/MMZ2012D121BT000/445-2188-1-ND/765218" H 4700 10100 60  0001 C CNN
F 4 "MMZ2012D121BT000" H 4700 10100 60  0001 C CNN "Part Number"
	1    4700 10100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 9650 3600 10150
Wire Wire Line
	3600 10150 4600 10150
$Comp
L CP1 C8
U 1 1 57CF4182
P 5300 9200
F 0 "C8" H 5415 9246 50  0000 L CNN
F 1 "12uF" H 5415 9155 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Radial_D10_L13_P5" H 5415 9155 50  0001 L CNN
F 3 "http://www.digikey.com/product-detail/en/rubycon/400AX12MEFC10X12.5/1189-3120-ND/6049840" H 5415 9064 50  0001 L CNN
	1    5300 9200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 10150 5300 9350
$Comp
L CP1 C7
U 1 1 57CF494B
P 4350 9200
F 0 "C7" H 4465 9246 50  0000 L CNN
F 1 "12uF" H 4465 9155 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Radial_D10_L13_P5" H 4465 9155 50  0001 L CNN
F 3 "http://www.digikey.com/product-detail/en/rubycon/400AX12MEFC10X12.5/1189-3120-ND/6049840" H 4465 9064 50  0001 L CNN
	1    4350 9200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 9350 4350 10150
Connection ~ 4350 10150
Wire Wire Line
	5300 10150 5050 10150
Wire Wire Line
	4350 9050 4350 8650
Wire Wire Line
	5300 8650 5300 9050
$Comp
L BK0603HS330-T L2
U 1 1 57CF6277
P 4750 8600
F 0 "L2" H 4850 8550 60  0000 C CNN
F 1 "470uH" H 4900 8450 60  0000 C CNN
F 2 "Capacitors_ThroughHole:C_Radial_D10_L13_P5" H 4750 8600 60  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/bourns-inc/RLB0608-471KL/RLB0608-471KL-ND/2352761" H 4750 8600 60  0001 C CNN
F 4 "BK1608HS330-T" H 4750 8600 60  0001 C CNN "Part Number"
	1    4750 8600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 8650 4650 8650
Wire Wire Line
	5100 8650 5300 8650
Wire Wire Line
	3600 8850 3600 8650
Connection ~ 4350 8650
$Comp
L R R16
U 1 1 57CFCBB5
P 5700 10550
F 0 "R16" V 5700 10550 50  0000 C CNN
F 1 "0.91" V 5800 10550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5630 10550 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/susumu/RL1220S-R91-F/RL12S.91FCT-ND/567353" H 5700 10550 50  0001 C CNN
F 4 "RL1220S-R91-F" V 5700 10550 60  0001 C CNN "Part Number"
	1    5700 10550
	0    1    1    0   
$EndComp
$Comp
L R R15
U 1 1 57CFCDF7
P 5050 10500
F 0 "R15" V 5050 10500 50  0000 C CNN
F 1 "0.82" V 5150 10500 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4980 10500 50  0001 C CNN
F 3 "http://www.digikey.com/product-detail/en/susumu/RL1220S-R82-F/RL12S.82FCT-ND/433031" H 5050 10500 50  0001 C CNN
F 4 "RL1220S-R82-F" V 5050 10500 60  0001 C CNN "Part Number"
	1    5050 10500
	0    1    1    0   
$EndComp
$EndSCHEMATC
