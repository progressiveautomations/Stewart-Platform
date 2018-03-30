EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
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
LIBS:w_connectors
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
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
L HEADER_10x2 J1
U 1 1 5A13C6BC
P 5000 3700
F 0 "J1" H 5000 4250 60  0000 C CNN
F 1 "HEADER_10x2" H 5000 3150 60  0000 C CNN
F 2 "Connectors_IDC:IDC-Header_2x10_Pitch2.54mm_Straight" H 5000 3700 60  0001 C CNN
F 3 "" H 5000 3700 60  0000 C CNN
	1    5000 3700
	1    0    0    -1  
$EndComp
Text GLabel 4850 3250 0    39   Input ~ 0
RESET
Text GLabel 4850 3350 0    39   Input ~ 0
+3.3V
Text GLabel 4850 3450 0    39   Input ~ 0
+5V
Text GLabel 4850 3550 0    39   Input ~ 0
GND
Text GLabel 4850 3750 0    39   Input ~ 0
Vin
Text GLabel 4850 3650 0    39   Input ~ 0
GND
Text GLabel 4850 4150 0    39   Input ~ 0
SCLK
Text GLabel 4850 4050 0    39   Input ~ 0
MISO
Text GLabel 4850 3950 0    39   Input ~ 0
MOSI
Text GLabel 4850 3850 0    39   Input ~ 0
~ENABLE~
Text GLabel 5150 3850 2    39   Input ~ 0
DIR_1
Text GLabel 5150 3950 2    39   Input ~ 0
DIR_2
Text GLabel 5150 4050 2    39   Input ~ 0
DIR_3
Text GLabel 5150 4150 2    39   Input ~ 0
DIR_4
Text GLabel 5950 3250 0    39   Input ~ 0
RESET
Text GLabel 5950 3350 0    39   Input ~ 0
+3.3V
Text GLabel 5950 3450 0    39   Input ~ 0
+5V
Text GLabel 5950 3550 0    39   Input ~ 0
GND
Text GLabel 5950 3650 0    39   Input ~ 0
GND
Text GLabel 5950 3750 0    39   Input ~ 0
Vin
Text GLabel 6750 3150 2    39   Input ~ 0
SCLK
Text GLabel 6750 3250 2    39   Input ~ 0
MISO
Text GLabel 6750 3350 2    39   Input ~ 0
MOSI
Text GLabel 6750 3650 2    39   Input ~ 0
~ENABLE~
Text GLabel 6750 4350 2    39   Input ~ 0
DIR_1
Text GLabel 6750 4250 2    39   Input ~ 0
DIR_2
Text GLabel 6750 4150 2    39   Input ~ 0
DIR_3
Text GLabel 6750 3850 2    39   Input ~ 0
DIR_4
Text GLabel 6750 4050 2    39   Input ~ 0
PWM_3
Text GLabel 6750 3950 2    39   Input ~ 0
PWM_4
Text GLabel 6750 3550 2    39   Input ~ 0
PWM_1
Text GLabel 6750 3450 2    39   Input ~ 0
PWM_2
$Comp
L HEADER_6 J2
U 1 1 5A1509F0
P 6050 3500
F 0 "J2" H 6050 3850 60  0000 C CNN
F 1 "HEADER_6" H 6100 3950 60  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x06_Pitch2.54mm" H 6050 3500 60  0001 C CNN
F 3 "" H 6050 3500 60  0000 C CNN
	1    6050 3500
	1    0    0    -1  
$EndComp
$Comp
L HEADER_6 J3
U 1 1 5A150A54
P 6050 4300
F 0 "J3" H 6050 4650 60  0000 C CNN
F 1 "HEADER_6" H 6050 3950 60  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x06_Pitch2.54mm" H 6050 4300 60  0001 C CNN
F 3 "" H 6050 4300 60  0000 C CNN
	1    6050 4300
	1    0    0    -1  
$EndComp
$Comp
L HEADER_8 J4
U 1 1 5A150A8C
P 6650 3300
F 0 "J4" H 6650 3750 60  0000 C CNN
F 1 "HEADER_8" H 6650 3850 60  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x08_Pitch2.54mm" H 6650 3300 60  0001 C CNN
F 3 "" H 6650 3300 60  0000 C CNN
	1    6650 3300
	-1   0    0    -1  
$EndComp
$Comp
L HEADER_8 J5
U 1 1 5A150ACE
P 6650 4200
F 0 "J5" H 6650 4650 60  0000 C CNN
F 1 "HEADER_8" H 6650 3750 60  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x08_Pitch2.54mm" H 6650 4200 60  0001 C CNN
F 3 "" H 6650 4200 60  0000 C CNN
	1    6650 4200
	-1   0    0    -1  
$EndComp
NoConn ~ 6750 4550
NoConn ~ 6750 4450
NoConn ~ 5950 4050
NoConn ~ 5950 4150
NoConn ~ 5950 4250
NoConn ~ 5950 4350
NoConn ~ 5950 4450
NoConn ~ 5950 4550
NoConn ~ 6750 2950
Text GLabel 5150 3250 2    39   Input ~ 0
+3.3V
Text GLabel 5150 3350 2    39   Input ~ 0
+5V
Text GLabel 5150 3450 2    39   Input ~ 0
PWM_1
Text GLabel 5150 3550 2    39   Input ~ 0
PWM_2
Text GLabel 5150 3650 2    39   Input ~ 0
PWM_3
Text GLabel 5150 3750 2    39   Input ~ 0
PWM_4
NoConn ~ 6750 3050
Text Notes 7000 6750 0    60   ~ 0
Header symbols obtained from:\nhttp://smisioto.no-ip.org/elettronica/kicad/kicad-en.htm
Text Notes 7350 7500 0    60   ~ 0
Multimoto Connector Board
Text Notes 10550 7650 0    60   ~ 0
1
Text Notes 8125 7650 0    60   ~ 0
Nov. 25, 2017
$EndSCHEMATC
