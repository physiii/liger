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
Sheet 6 9
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
L CONN_01X04 P5
U 1 1 5909282F
P 5700 4150
F 0 "P5" H 5700 4400 50  0000 C CNN
F 1 "CONN_01X04" V 5800 4150 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 5700 4150 50  0001 C CNN
F 3 "http://sullinscorp.com/catalogs/101_PAGE114-115_.100_FEMALE_HDR.pdf" H 5700 4150 50  0001 C CNN
F 4 "PPTC041LGBN-RC" H 5700 4150 60  0001 C CNN "Part Number"
	1    5700 4150
	0    1    1    0   
$EndComp
Text Label 5250 3000 0    60   ~ 0
Ultra-Sonic_Sensor
Text GLabel 5750 3750 1    60   Input ~ 0
IO12
Text GLabel 5650 3750 1    60   Input ~ 0
IO13
$Comp
L GND #PWR019
U 1 1 59092834
P 5500 3900
F 0 "#PWR019" H 5500 3650 50  0001 C CNN
F 1 "GND" H 5500 3750 50  0000 C CNN
F 2 "" H 5500 3900 50  0000 C CNN
F 3 "" H 5500 3900 50  0000 C CNN
	1    5500 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	5750 3750 5750 3950
Wire Wire Line
	5650 3750 5650 3950
Wire Wire Line
	5850 3900 5850 3950
Wire Wire Line
	5900 3900 5850 3900
Wire Wire Line
	5550 3900 5550 3950
Wire Wire Line
	5500 3900 5550 3900
Text GLabel 5900 3900 2    60   Input ~ 0
5V
$EndSCHEMATC
