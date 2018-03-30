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
L HEADER_10x2 MULTIMOTO_1
U 1 1 5A56A687
P 3875 2625
F 0 "MULTIMOTO_1" H 3875 3175 60  0000 C CNN
F 1 "HEADER_10x2" H 3875 2075 60  0000 C CNN
F 2 "Connectors_IDC:IDC-Header_2x10_Pitch2.54mm_Straight" H 3875 2625 60  0001 C CNN
F 3 "" H 3875 2625 60  0000 C CNN
	1    3875 2625
	1    0    0    -1  
$EndComp
$Comp
L HEADER_10x2 MULTIMOTO_2
U 1 1 5A56A766
P 3875 3925
F 0 "MULTIMOTO_2" H 3875 4475 60  0000 C CNN
F 1 "HEADER_10x2" H 3875 3375 60  0000 C CNN
F 2 "Connectors_IDC:IDC-Header_2x10_Pitch2.54mm_Straight" H 3875 3925 60  0001 C CNN
F 3 "" H 3875 3925 60  0000 C CNN
	1    3875 3925
	1    0    0    -1  
$EndComp
Text GLabel 3725 3475 0    39   Input ~ 0
RESET
Text GLabel 3725 3575 0    39   Input ~ 0
+3.3V
Text GLabel 3725 3675 0    39   Input ~ 0
+5V
Text GLabel 3725 3775 0    39   Input ~ 0
GND
Text GLabel 3725 3875 0    39   Input ~ 0
GND
Text GLabel 3725 3975 0    39   Input ~ 0
Vin
Text GLabel 3725 4075 0    39   Input ~ 0
~ENABLE~_2
Text GLabel 4025 2175 2    39   Input ~ 0
+3.3V
Text GLabel 4025 2275 2    39   Input ~ 0
+5V
Text GLabel 4025 2375 2    39   Input ~ 0
PWM_1
Text GLabel 4025 2475 2    39   Input ~ 0
PWM_2
Text GLabel 4025 2575 2    39   Input ~ 0
PWM_3
Text GLabel 4025 2675 2    39   Input ~ 0
PWM_4
Text GLabel 4025 2775 2    39   Input ~ 0
DIR_1
Text GLabel 4025 2875 2    39   Input ~ 0
DIR_2
Text GLabel 4025 2975 2    39   Input ~ 0
DIR_3
Text GLabel 4025 3075 2    39   Input ~ 0
DIR_4
Text GLabel 3725 2175 0    39   Input ~ 0
RESET
Text GLabel 3725 2275 0    39   Input ~ 0
+3.3V
Text GLabel 3725 2375 0    39   Input ~ 0
+5V
Text GLabel 3725 2475 0    39   Input ~ 0
GND
Text GLabel 3725 2575 0    39   Input ~ 0
GND
Text GLabel 3725 2675 0    39   Input ~ 0
Vin
Text GLabel 3725 2775 0    39   Input ~ 0
~ENABLE~_1
NoConn ~ 3725 2875
NoConn ~ 3725 2975
NoConn ~ 3725 3075
Text GLabel 4025 3475 2    39   Input ~ 0
+3.3V
Text GLabel 4025 3575 2    39   Input ~ 0
+5V
Text GLabel 4025 3675 2    39   Input ~ 0
PWM_5
Text GLabel 4025 3775 2    39   Input ~ 0
PWM_6
Text GLabel 4025 3875 2    39   Input ~ 0
PWM_7
Text GLabel 4025 3975 2    39   Input ~ 0
PWM_8
Text GLabel 4025 4075 2    39   Input ~ 0
DIR_5
Text GLabel 4025 4175 2    39   Input ~ 0
DIR_6
Text GLabel 4025 4275 2    39   Input ~ 0
DIR_7
Text GLabel 4025 4375 2    39   Input ~ 0
DIR_8
NoConn ~ 3725 4175
NoConn ~ 3725 4275
NoConn ~ 3725 4375
$Comp
L HEADER_6 UTIL_1
U 1 1 5A56AB97
P 4825 2425
F 0 "UTIL_1" H 4825 2775 60  0000 C CNN
F 1 "HEADER_6" H 4825 2075 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 4825 2425 60  0001 C CNN
F 3 "" H 4825 2425 60  0000 C CNN
	1    4825 2425
	1    0    0    -1  
$EndComp
$Comp
L HEADER_6 POT_OUT_1
U 1 1 5A56ACBA
P 4825 3875
F 0 "POT_OUT_1" H 4825 4225 60  0000 C CNN
F 1 "HEADER_6" H 4825 3525 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 4825 3875 60  0001 C CNN
F 3 "" H 4825 3875 60  0000 C CNN
	1    4825 3875
	1    0    0    -1  
$EndComp
Text GLabel 4725 2175 0    39   Input ~ 0
RESET
Text GLabel 4725 2275 0    39   Input ~ 0
+3.3V
Text GLabel 4725 2375 0    39   Input ~ 0
+5V
Text GLabel 4725 2475 0    39   Input ~ 0
GND
Text GLabel 4725 2575 0    39   Input ~ 0
GND
Text GLabel 4725 2675 0    39   Input ~ 0
Vin
Text GLabel 4725 4125 0    39   Input ~ 0
POT_1
Text GLabel 4725 4025 0    39   Input ~ 0
POT_2
Text GLabel 4725 3925 0    39   Input ~ 0
POT_3
Text GLabel 4725 3825 0    39   Input ~ 0
POT_4
Text GLabel 4725 3725 0    39   Input ~ 0
POT_5
Text GLabel 4725 3625 0    39   Input ~ 0
POT_6
$Comp
L HEADER_4 DIR_1-4
U 1 1 5A56C333
P 5500 4975
F 0 "DIR_1-4" H 5500 5225 60  0000 C CNN
F 1 "HEADER_4" H 5500 4725 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 5500 4975 60  0001 C CNN
F 3 "" H 5500 4975 60  0000 C CNN
	1    5500 4975
	0    -1   -1   0   
$EndComp
$Comp
L HEADER_4 DIR_5-8
U 1 1 5A56C543
P 6150 4975
F 0 "DIR_5-8" H 6150 5225 60  0000 C CNN
F 1 "HEADER_4" H 6150 4725 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 6150 4975 60  0001 C CNN
F 3 "" H 6150 4975 60  0000 C CNN
	1    6150 4975
	0    -1   -1   0   
$EndComp
Text GLabel 5350 5075 3    39   Input ~ 0
DIR_1
Text GLabel 5450 5075 3    39   Input ~ 0
DIR_2
Text GLabel 5550 5075 3    39   Input ~ 0
DIR_3
Text GLabel 5650 5075 3    39   Input ~ 0
DIR_4
Text GLabel 6000 5075 3    39   Input ~ 0
DIR_5
Text GLabel 6100 5075 3    39   Input ~ 0
DIR_6
Text GLabel 6200 5075 3    39   Input ~ 0
DIR_7
Text GLabel 6300 5075 3    39   Input ~ 0
DIR_8
$Comp
L HEADER_4 PWM_1-4
U 1 1 5A56CD31
P 6525 2325
F 0 "PWM_1-4" H 6525 2575 60  0000 C CNN
F 1 "HEADER_4" H 6525 2075 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 6525 2325 60  0001 C CNN
F 3 "" H 6525 2325 60  0000 C CNN
	1    6525 2325
	-1   0    0    -1  
$EndComp
$Comp
L HEADER_4 PWM_5-8
U 1 1 5A56CE10
P 6525 2975
F 0 "PWM_5-8" H 6525 3225 60  0000 C CNN
F 1 "HEADER_4" H 6525 2725 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 6525 2975 60  0001 C CNN
F 3 "" H 6525 2975 60  0000 C CNN
	1    6525 2975
	-1   0    0    -1  
$EndComp
Text GLabel 6625 2175 2    39   Input ~ 0
PWM_1
Text GLabel 6625 2275 2    39   Input ~ 0
PWM_2
Text GLabel 6625 2375 2    39   Input ~ 0
PWM_3
Text GLabel 6625 2475 2    39   Input ~ 0
PWM_4
Text GLabel 6625 2925 2    39   Input ~ 0
PWM_6
Text GLabel 6625 3025 2    39   Input ~ 0
PWM_7
Text GLabel 6625 3125 2    39   Input ~ 0
PWM_8
Text GLabel 6625 2825 2    39   Input ~ 0
PWM_5
$Comp
L HEADER_2 ~ENABLE~_1
U 1 1 5A56CFA6
P 6525 3675
F 0 "~ENABLE~_1" H 6525 3825 60  0000 C CNN
F 1 "HEADER_2" H 6525 3525 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 6525 3675 60  0001 C CNN
F 3 "" H 6525 3675 60  0000 C CNN
	1    6525 3675
	-1   0    0    -1  
$EndComp
Text GLabel 6625 3625 2    39   Input ~ 0
~ENABLE~_1
Text GLabel 6625 3725 2    39   Input ~ 0
~ENABLE~_2
$Comp
L HEADER_3 ACTUATOR_6
U 1 1 5A56E812
P 7550 2275
F 0 "ACTUATOR_6" H 7550 2475 60  0000 C CNN
F 1 "HEADER_3" H 7550 2075 60  0000 C CNN
F 2 "Connectors_Terminal_Blocks:TerminalBlock_bornier-3_P5.08mm" H 7550 2275 60  0001 C CNN
F 3 "" H 7550 2275 60  0000 C CNN
	1    7550 2275
	-1   0    0    -1  
$EndComp
Text GLabel 7650 2175 2    39   Input ~ 0
+3.3V
Text GLabel 7650 2275 2    39   Input ~ 0
GND
$Comp
L C C6
U 1 1 5A56ECA7
P 8150 2375
F 0 "C6" H 8175 2475 50  0000 L CNN
F 1 "10nF" H 8175 2275 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 8188 2225 50  0001 C CNN
F 3 "" H 8150 2375 50  0001 C CNN
	1    8150 2375
	0    -1   -1   0   
$EndComp
Text GLabel 8300 2375 2    39   Input ~ 0
GND
Text GLabel 7950 2325 1    39   Input ~ 0
POT_6
Wire Wire Line
	8000 2375 7650 2375
Wire Wire Line
	7950 2325 7950 2375
Connection ~ 7950 2375
$Comp
L HEADER_3 ACTUATOR_5
U 1 1 5A56F416
P 7550 2825
F 0 "ACTUATOR_5" H 7550 3025 60  0000 C CNN
F 1 "HEADER_3" H 7550 2625 60  0000 C CNN
F 2 "Connectors_Terminal_Blocks:TerminalBlock_bornier-3_P5.08mm" H 7550 2825 60  0001 C CNN
F 3 "" H 7550 2825 60  0000 C CNN
	1    7550 2825
	-1   0    0    -1  
$EndComp
Text GLabel 7650 2725 2    39   Input ~ 0
+3.3V
Text GLabel 7650 2825 2    39   Input ~ 0
GND
$Comp
L C C5
U 1 1 5A56F41E
P 8150 2925
F 0 "C5" H 8175 3025 50  0000 L CNN
F 1 "10nF" H 8175 2825 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 8188 2775 50  0001 C CNN
F 3 "" H 8150 2925 50  0001 C CNN
	1    8150 2925
	0    -1   -1   0   
$EndComp
Text GLabel 8300 2925 2    39   Input ~ 0
GND
Text GLabel 7950 2875 1    39   Input ~ 0
POT_5
Wire Wire Line
	8000 2925 7650 2925
Wire Wire Line
	7950 2875 7950 2925
Connection ~ 7950 2925
$Comp
L HEADER_3 ACTUATOR_4
U 1 1 5A56F537
P 7550 3375
F 0 "ACTUATOR_4" H 7550 3575 60  0000 C CNN
F 1 "HEADER_3" H 7550 3175 60  0000 C CNN
F 2 "Connectors_Terminal_Blocks:TerminalBlock_bornier-3_P5.08mm" H 7550 3375 60  0001 C CNN
F 3 "" H 7550 3375 60  0000 C CNN
	1    7550 3375
	-1   0    0    -1  
$EndComp
Text GLabel 7650 3275 2    39   Input ~ 0
+3.3V
Text GLabel 7650 3375 2    39   Input ~ 0
GND
$Comp
L C C4
U 1 1 5A56F53F
P 8150 3475
F 0 "C4" H 8175 3575 50  0000 L CNN
F 1 "10nF" H 8175 3375 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 8188 3325 50  0001 C CNN
F 3 "" H 8150 3475 50  0001 C CNN
	1    8150 3475
	0    -1   -1   0   
$EndComp
Text GLabel 8300 3475 2    39   Input ~ 0
GND
Text GLabel 7950 3425 1    39   Input ~ 0
POT_4
Wire Wire Line
	8000 3475 7650 3475
Wire Wire Line
	7950 3425 7950 3475
Connection ~ 7950 3475
$Comp
L HEADER_3 ACTUATOR_3
U 1 1 5A56F5A8
P 7550 3925
F 0 "ACTUATOR_3" H 7550 4125 60  0000 C CNN
F 1 "HEADER_3" H 7550 3725 60  0000 C CNN
F 2 "Connectors_Terminal_Blocks:TerminalBlock_bornier-3_P5.08mm" H 7550 3925 60  0001 C CNN
F 3 "" H 7550 3925 60  0000 C CNN
	1    7550 3925
	-1   0    0    -1  
$EndComp
Text GLabel 7650 3825 2    39   Input ~ 0
+3.3V
Text GLabel 7650 3925 2    39   Input ~ 0
GND
$Comp
L C C3
U 1 1 5A56F5B0
P 8150 4025
F 0 "C3" H 8175 4125 50  0000 L CNN
F 1 "10nF" H 8175 3925 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 8188 3875 50  0001 C CNN
F 3 "" H 8150 4025 50  0001 C CNN
	1    8150 4025
	0    -1   -1   0   
$EndComp
Text GLabel 8300 4025 2    39   Input ~ 0
GND
Text GLabel 7950 3975 1    39   Input ~ 0
POT_3
Wire Wire Line
	8000 4025 7650 4025
Wire Wire Line
	7950 3975 7950 4025
Connection ~ 7950 4025
$Comp
L HEADER_3 ACTUATOR_2
U 1 1 5A56F629
P 7550 4475
F 0 "ACTUATOR_2" H 7550 4675 60  0000 C CNN
F 1 "HEADER_3" H 7550 4275 60  0000 C CNN
F 2 "Connectors_Terminal_Blocks:TerminalBlock_bornier-3_P5.08mm" H 7550 4475 60  0001 C CNN
F 3 "" H 7550 4475 60  0000 C CNN
	1    7550 4475
	-1   0    0    -1  
$EndComp
Text GLabel 7650 4375 2    39   Input ~ 0
+3.3V
Text GLabel 7650 4475 2    39   Input ~ 0
GND
$Comp
L C C2
U 1 1 5A56F631
P 8150 4575
F 0 "C2" H 8175 4675 50  0000 L CNN
F 1 "10nF" H 8175 4475 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 8188 4425 50  0001 C CNN
F 3 "" H 8150 4575 50  0001 C CNN
	1    8150 4575
	0    -1   -1   0   
$EndComp
Text GLabel 8300 4575 2    39   Input ~ 0
GND
Text GLabel 7950 4525 1    39   Input ~ 0
POT_2
Wire Wire Line
	8000 4575 7650 4575
Wire Wire Line
	7950 4525 7950 4575
Connection ~ 7950 4575
$Comp
L HEADER_3 ACTUATOR_1
U 1 1 5A56F6DA
P 7550 5025
F 0 "ACTUATOR_1" H 7550 5225 60  0000 C CNN
F 1 "HEADER_3" H 7550 4825 60  0000 C CNN
F 2 "Connectors_Terminal_Blocks:TerminalBlock_bornier-3_P5.08mm" H 7550 5025 60  0001 C CNN
F 3 "" H 7550 5025 60  0000 C CNN
	1    7550 5025
	-1   0    0    -1  
$EndComp
Text GLabel 7650 4925 2    39   Input ~ 0
+3.3V
Text GLabel 7650 5025 2    39   Input ~ 0
GND
$Comp
L C C1
U 1 1 5A56F6E2
P 8150 5125
F 0 "C1" H 8175 5225 50  0000 L CNN
F 1 "10nF" H 8175 5025 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 8188 4975 50  0001 C CNN
F 3 "" H 8150 5125 50  0001 C CNN
	1    8150 5125
	0    -1   -1   0   
$EndComp
Text GLabel 8300 5125 2    39   Input ~ 0
GND
Text GLabel 7950 5075 1    39   Input ~ 0
POT_1
Wire Wire Line
	8000 5125 7650 5125
Wire Wire Line
	7950 5075 7950 5125
Connection ~ 7950 5125
Text Notes 7000 7100 0    60   ~ 0
Header symbols obtained from:\nhttp://smisioto.no-ip.org/elettronica/kicad/kicad-en.htm
Text Notes 7350 7500 0    60   ~ 0
Due Connector Board
Text Notes 10550 7650 0    60   ~ 0
1
Text Notes 8125 7650 0    60   ~ 0
Jan. 10, 2018
Text Notes 7000 6650 0    60   ~ 0
NB: - the SPI connections from the Multimoto connector boards are unused
Text Notes 10815 6750 2    60   ~ 0
- the PWM/DIR connections are labelled according to the Multimoto interface
$EndSCHEMATC
