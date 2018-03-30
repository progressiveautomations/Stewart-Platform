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
LIBS:multimoto
LIBS:pa-14p
LIBS:w_connectors
LIBS:electronics_layout-cache
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
Text Notes 7000 7100 0    60   ~ 0
Arduino and header symbols obtained from\nhttp://smisioto.no-ip.org/elettronica/kicad/kicad-en.htm
Text Notes 7325 7500 0    60   ~ 0
ENPH 459 - Project 1810: Electronics Layout
Text Notes 8150 7650 0    60   ~ 0
Mar. 30, 2018
$Comp
L PA-14P M4
U 1 1 5ABEAC15
P 2825 4650
F 0 "M4" H 2725 4950 60  0000 C CNN
F 1 "PA-14P" H 2825 4350 60  0000 C CNN
F 2 "" H 3175 4950 60  0001 C CNN
F 3 "" H 3175 4950 60  0001 C CNN
	1    2825 4650
	1    0    0    -1  
$EndComp
$Comp
L PA-14P M5
U 1 1 5ABEACE8
P 2825 5650
F 0 "M5" H 2725 5950 60  0000 C CNN
F 1 "PA-14P" H 2825 5350 60  0000 C CNN
F 2 "" H 3175 5950 60  0001 C CNN
F 3 "" H 3175 5950 60  0001 C CNN
	1    2825 5650
	1    0    0    -1  
$EndComp
$Comp
L PA-14P M6
U 1 1 5ABEAD52
P 2825 6675
F 0 "M6" H 2725 6975 60  0000 C CNN
F 1 "PA-14P" H 2825 6375 60  0000 C CNN
F 2 "" H 3175 6975 60  0001 C CNN
F 3 "" H 3175 6975 60  0001 C CNN
	1    2825 6675
	1    0    0    -1  
$EndComp
$Comp
L PA-14P M1
U 1 1 5ABEB08C
P 2850 1625
F 0 "M1" H 2750 1925 60  0000 C CNN
F 1 "PA-14P" H 2850 1325 60  0000 C CNN
F 2 "" H 3200 1925 60  0001 C CNN
F 3 "" H 3200 1925 60  0001 C CNN
	1    2850 1625
	1    0    0    -1  
$EndComp
$Comp
L PA-14P M2
U 1 1 5ABEB092
P 2850 2625
F 0 "M2" H 2750 2925 60  0000 C CNN
F 1 "PA-14P" H 2850 2325 60  0000 C CNN
F 2 "" H 3200 2925 60  0001 C CNN
F 3 "" H 3200 2925 60  0001 C CNN
	1    2850 2625
	1    0    0    -1  
$EndComp
$Comp
L PA-14P M3
U 1 1 5ABEB098
P 2850 3650
F 0 "M3" H 2750 3950 60  0000 C CNN
F 1 "PA-14P" H 2850 3350 60  0000 C CNN
F 2 "" H 3200 3950 60  0001 C CNN
F 3 "" H 3200 3950 60  0001 C CNN
	1    2850 3650
	1    0    0    -1  
$EndComp
$Comp
L MultiMoto U1
U 1 1 5ABEB11E
P 5525 2375
F 0 "U1" H 6275 3525 60  0000 C CNN
F 1 "MultiMoto" H 5525 3525 60  0000 C CNN
F 2 "" H 4825 2775 60  0001 C CNN
F 3 "" H 4825 2775 60  0001 C CNN
	1    5525 2375
	1    0    0    -1  
$EndComp
$Comp
L Arduino_Mega_Header J1
U 1 1 5ABEBFEA
P 8650 3450
F 0 "J1" H 8650 4700 60  0000 C CNN
F 1 "Arduino_Mega_Header" H 8650 2200 60  0000 C CNN
F 2 "" H 8650 3450 60  0000 C CNN
F 3 "" H 8650 3450 60  0000 C CNN
	1    8650 3450
	1    0    0    -1  
$EndComp
Text Notes 7000 6775 0    60   ~ 0
NB: - using the Arduino Mega layout in place of the Arduino Due
Text Notes 7000 6650 0    60   ~ 0
High level overview of electronics layout (abstracted from the PCBs).
$Comp
L +12V #PWR01
U 1 1 5ABEC72A
P 5125 3475
F 0 "#PWR01" H 5125 3325 50  0001 C CNN
F 1 "+12V" H 5275 3475 50  0000 C CNN
F 2 "" H 5125 3475 50  0001 C CNN
F 3 "" H 5125 3475 50  0001 C CNN
	1    5125 3475
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR02
U 1 1 5ABEC7D2
P 5225 3475
F 0 "#PWR02" H 5225 3225 50  0001 C CNN
F 1 "GND" H 5350 3475 50  0000 C CNN
F 2 "" H 5225 3475 50  0001 C CNN
F 3 "" H 5225 3475 50  0001 C CNN
	1    5225 3475
	1    0    0    -1  
$EndComp
$Comp
L MultiMoto U2
U 1 1 5ABEC968
P 5525 5225
F 0 "U2" H 6275 6375 60  0000 C CNN
F 1 "MultiMoto" H 5525 6375 60  0000 C CNN
F 2 "" H 4825 5625 60  0001 C CNN
F 3 "" H 4825 5625 60  0001 C CNN
	1    5525 5225
	1    0    0    -1  
$EndComp
$Comp
L +12V #PWR03
U 1 1 5ABEC96E
P 5125 6325
F 0 "#PWR03" H 5125 6175 50  0001 C CNN
F 1 "+12V" H 5275 6325 50  0000 C CNN
F 2 "" H 5125 6325 50  0001 C CNN
F 3 "" H 5125 6325 50  0001 C CNN
	1    5125 6325
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR04
U 1 1 5ABEC974
P 5225 6325
F 0 "#PWR04" H 5225 6075 50  0001 C CNN
F 1 "GND" H 5350 6325 50  0000 C CNN
F 2 "" H 5225 6325 50  0001 C CNN
F 3 "" H 5225 6325 50  0001 C CNN
	1    5225 6325
	1    0    0    -1  
$EndComp
Text GLabel 3300 1725 2    39   Input ~ 0
+3.3V
Text GLabel 3300 1825 2    39   Input ~ 0
GND
Text GLabel 3300 2725 2    39   Input ~ 0
+3.3V
Text GLabel 3300 3750 2    39   Input ~ 0
+3.3V
Text GLabel 3275 4750 2    39   Input ~ 0
+3.3V
Text GLabel 3275 5750 2    39   Input ~ 0
+3.3V
Text GLabel 3275 6775 2    39   Input ~ 0
+3.3V
Text GLabel 3300 2825 2    39   Input ~ 0
GND
Text GLabel 3300 3850 2    39   Input ~ 0
GND
Text GLabel 3275 4850 2    39   Input ~ 0
GND
Text GLabel 3275 5850 2    39   Input ~ 0
GND
Text GLabel 3275 6875 2    39   Input ~ 0
GND
NoConn ~ 4525 1375
NoConn ~ 4525 1475
NoConn ~ 4525 2675
NoConn ~ 4525 2775
NoConn ~ 4525 2875
NoConn ~ 4525 2975
NoConn ~ 4525 3075
NoConn ~ 4525 3175
NoConn ~ 6525 3075
NoConn ~ 6525 3175
NoConn ~ 6525 1575
NoConn ~ 6525 1675
NoConn ~ 6525 1775
NoConn ~ 6525 1875
NoConn ~ 6525 1975
NoConn ~ 4525 4225
NoConn ~ 4525 4325
NoConn ~ 4525 5525
NoConn ~ 4525 5625
NoConn ~ 4525 5725
NoConn ~ 4525 5825
NoConn ~ 4525 5925
NoConn ~ 4525 6025
NoConn ~ 6525 5925
NoConn ~ 6525 6025
NoConn ~ 6525 4425
NoConn ~ 6525 4525
NoConn ~ 6525 4625
NoConn ~ 6525 4725
NoConn ~ 6525 4825
NoConn ~ 8150 2300
Text GLabel 4525 4825 0    39   Input ~ 0
RESET
Text GLabel 4525 4925 0    39   Input ~ 0
+3.3V
Text GLabel 4525 5025 0    39   Input ~ 0
+5V
Text GLabel 4525 5125 0    39   Input ~ 0
GND
Text GLabel 4525 5225 0    39   Input ~ 0
GND
Text GLabel 4525 5325 0    39   Input ~ 0
Vin
NoConn ~ 4525 2675
NoConn ~ 4525 2775
Text GLabel 4525 1975 0    39   Input ~ 0
RESET
Text GLabel 4525 2075 0    39   Input ~ 0
+3.3V
Text GLabel 4525 2175 0    39   Input ~ 0
+5V
Text GLabel 4525 2275 0    39   Input ~ 0
GND
Text GLabel 4525 2375 0    39   Input ~ 0
GND
Text GLabel 4525 2475 0    39   Input ~ 0
Vin
Text GLabel 7600 1825 0    39   Input ~ 0
RESET
Text GLabel 7600 1925 0    39   Input ~ 0
+3.3V
Text GLabel 7600 2025 0    39   Input ~ 0
+5V
Text GLabel 7600 2125 0    39   Input ~ 0
GND
Text GLabel 7600 2225 0    39   Input ~ 0
GND
Text GLabel 7600 2325 0    39   Input ~ 0
Vin
Text GLabel 3300 1625 2    39   Input ~ 0
POT_1
Text GLabel 3300 2625 2    39   Input ~ 0
POT_2
Text GLabel 3300 3650 2    39   Input ~ 0
POT_3
Text GLabel 3275 4650 2    39   Input ~ 0
POT_4
Text GLabel 3275 5650 2    39   Input ~ 0
POT_5
Text GLabel 3275 6675 2    39   Input ~ 0
POT_6
Text GLabel 7625 2650 0    39   Input ~ 0
POT_1
Text GLabel 7625 2750 0    39   Input ~ 0
POT_2
Text GLabel 7625 2850 0    39   Input ~ 0
POT_3
Text GLabel 7625 2950 0    39   Input ~ 0
POT_4
Text GLabel 7625 3050 0    39   Input ~ 0
POT_5
Text GLabel 7625 3150 0    39   Input ~ 0
POT_6
Wire Wire Line
	3050 1175 4075 1175
Wire Wire Line
	4075 1175 4075 1775
Wire Wire Line
	4075 1775 4525 1775
Wire Wire Line
	2950 1175 2950 1075
Wire Wire Line
	2950 1075 4175 1075
Wire Wire Line
	4175 1075 4175 1675
Wire Wire Line
	4175 1675 4525 1675
Wire Wire Line
	3050 3200 3900 3200
Wire Wire Line
	3900 3200 3900 3925
Wire Wire Line
	3900 3925 6075 3925
Wire Wire Line
	6075 3925 6075 3475
Wire Wire Line
	2950 3200 2950 3100
Wire Wire Line
	2950 3100 4000 3100
Wire Wire Line
	4000 3100 4000 3825
Wire Wire Line
	4000 3825 5975 3825
Wire Wire Line
	5975 3825 5975 3475
Wire Wire Line
	5775 3475 5775 3725
Wire Wire Line
	5775 3725 4100 3725
Wire Wire Line
	4100 3725 4100 2175
Wire Wire Line
	4100 2175 3050 2175
Wire Wire Line
	2950 2175 2950 2075
Wire Wire Line
	2950 2075 4200 2075
Wire Wire Line
	4200 2075 4200 3625
Wire Wire Line
	4200 3625 5675 3625
Wire Wire Line
	5675 3625 5675 3475
Wire Wire Line
	4525 4625 3900 4625
Wire Wire Line
	3900 4625 3900 4200
Wire Wire Line
	3900 4200 3025 4200
Wire Wire Line
	2925 4200 2925 4100
Wire Wire Line
	2925 4100 4000 4100
Wire Wire Line
	4000 4100 4000 4525
Wire Wire Line
	4000 4525 4525 4525
Wire Wire Line
	2925 6225 2925 6125
Wire Wire Line
	2925 6125 4000 6125
Wire Wire Line
	4000 6125 4000 6675
Wire Wire Line
	4000 6675 5975 6675
Wire Wire Line
	3025 5200 4100 5200
Wire Wire Line
	4100 5200 4100 6575
Wire Wire Line
	4100 6575 5775 6575
Wire Wire Line
	4200 6475 4200 5100
Wire Wire Line
	4200 5100 2925 5100
Wire Wire Line
	2925 5100 2925 5200
Wire Wire Line
	8150 2600 7675 2600
Wire Wire Line
	7675 2600 7675 2325
Wire Wire Line
	7675 2325 7600 2325
Wire Wire Line
	7600 2225 7725 2225
Wire Wire Line
	7725 2225 7725 2550
Wire Wire Line
	7725 2550 8100 2550
Wire Wire Line
	7600 2125 7775 2125
Wire Wire Line
	7775 2125 7775 2500
Wire Wire Line
	7775 2500 8150 2500
Wire Wire Line
	7600 2025 7825 2025
Wire Wire Line
	7825 2025 7825 2450
Wire Wire Line
	7825 2450 8100 2450
Wire Wire Line
	7600 1925 7875 1925
Wire Wire Line
	7875 1925 7875 2400
Wire Wire Line
	7875 2400 8150 2400
Wire Wire Line
	8100 2350 7925 2350
Wire Wire Line
	7925 2350 7925 1825
Wire Wire Line
	7925 1825 7600 1825
Wire Wire Line
	7675 3150 7675 3300
Wire Wire Line
	7675 3150 7625 3150
Wire Wire Line
	7725 3050 7725 3250
Wire Wire Line
	7725 3050 7625 3050
Wire Wire Line
	7625 2950 7775 2950
Wire Wire Line
	7775 2950 7775 3200
Wire Wire Line
	7825 2850 7825 3150
Wire Wire Line
	7825 2850 7625 2850
Wire Wire Line
	7875 2750 7875 3050
Wire Wire Line
	7875 2750 7625 2750
Wire Wire Line
	7675 3300 8150 3300
Wire Wire Line
	7725 3250 8100 3250
Wire Wire Line
	7775 3200 8150 3200
Wire Wire Line
	7825 3150 8100 3150
Wire Wire Line
	7875 3050 8150 3050
Wire Wire Line
	8100 3000 7925 3000
Wire Wire Line
	7925 3000 7925 2650
Wire Wire Line
	7925 2650 7625 2650
NoConn ~ 6525 2475
NoConn ~ 6525 2575
NoConn ~ 6525 5325
NoConn ~ 6525 5425
Wire Wire Line
	9200 4050 9350 4050
Wire Wire Line
	9150 4100 9350 4100
Wire Wire Line
	9200 4150 9350 4150
Wire Wire Line
	9200 4250 9350 4250
Wire Wire Line
	9150 4200 9350 4200
Wire Wire Line
	9150 4000 9350 4000
Entry Wire Line
	9350 4000 9450 4100
Entry Wire Line
	9350 4050 9450 4150
Entry Wire Line
	9350 4100 9450 4200
Entry Wire Line
	9350 4150 9450 4250
Entry Wire Line
	9350 4200 9450 4300
Entry Wire Line
	9350 4250 9450 4350
Text Label 9325 4000 2    20   ~ 0
DIR_7
Text Label 9325 4050 2    20   ~ 0
DIR_6
Text Label 9325 4100 2    20   ~ 0
DIR_5
Text Label 9325 4150 2    20   ~ 0
DIR_3
Text Label 9325 4200 2    20   ~ 0
DIR_2
Text Label 9325 4250 2    20   ~ 0
DIR_1
Wire Bus Line
	9450 4100 9450 5925
Wire Bus Line
	9450 5925 6750 5925
Wire Bus Line
	6750 5925 6750 2875
Entry Wire Line
	6650 2775 6750 2875
Entry Wire Line
	6650 2875 6750 2975
Entry Wire Line
	6650 2975 6750 3075
Entry Wire Line
	6650 5625 6750 5725
Entry Wire Line
	6650 5725 6750 5825
Entry Wire Line
	6650 5825 6750 5925
Wire Wire Line
	6650 2775 6525 2775
Wire Wire Line
	6525 2875 6650 2875
Wire Wire Line
	6525 2975 6650 2975
Wire Wire Line
	6650 5625 6525 5625
Wire Wire Line
	6525 5725 6650 5725
Wire Wire Line
	6650 5825 6525 5825
Text Label 6650 2775 2    28   ~ 0
DIR_3
Text Label 6650 2875 2    28   ~ 0
DIR_2
Text Label 6650 2975 2    28   ~ 0
DIR_1
Text Label 6650 5625 2    28   ~ 0
DIR_7
Text Label 6650 5725 2    28   ~ 0
DIR_6
Text Label 6650 5825 2    28   ~ 0
DIR_5
Wire Wire Line
	3900 6225 3025 6225
Wire Wire Line
	3900 6775 3900 6225
Wire Wire Line
	5775 6575 5775 6325
Wire Wire Line
	5675 6325 5675 6475
Wire Wire Line
	5675 6475 4200 6475
Wire Wire Line
	5975 6675 5975 6325
Wire Wire Line
	6075 6325 6075 6775
Wire Wire Line
	6075 6775 3900 6775
Wire Wire Line
	9675 3850 9675 5325
Wire Wire Line
	9675 3850 9200 3850
Wire Wire Line
	7200 3800 8150 3800
Wire Wire Line
	9200 2500 9325 2500
Wire Wire Line
	9150 2550 9325 2550
Wire Wire Line
	9200 2600 9325 2600
Wire Wire Line
	9150 2650 9325 2650
Wire Wire Line
	9200 2700 9325 2700
Wire Wire Line
	9150 2750 9325 2750
Entry Wire Line
	9325 2500 9425 2600
Entry Wire Line
	9325 2550 9425 2650
Entry Wire Line
	9325 2600 9425 2700
Entry Wire Line
	9325 2650 9425 2750
Entry Wire Line
	9325 2700 9425 2800
Entry Wire Line
	9325 2750 9425 2850
Text Label 9300 2500 2    20   ~ 0
PWM_1
Text Label 9300 2550 2    20   ~ 0
PWM_2
Text Label 9300 2600 2    20   ~ 0
PWM_3
Text Label 9300 2650 2    20   ~ 0
PWM_5
Text Label 9300 2700 2    20   ~ 0
PWM_6
Text Label 9300 2750 2    20   ~ 0
PWM_7
Text Notes 11050 6875 2    60   ~ 0
- connections numbered using the Multimoto interface, vs. by actuator in software
Wire Bus Line
	9425 1675 9425 2850
Wire Bus Line
	9425 1675 6975 1675
Wire Bus Line
	6975 1675 6975 5625
Wire Wire Line
	7200 2475 6650 2475
Wire Wire Line
	6650 2475 6650 2275
Wire Wire Line
	7200 2475 7200 3800
Wire Wire Line
	6650 2275 6525 2275
Entry Wire Line
	6875 2175 6975 2275
Entry Wire Line
	6875 2075 6975 2175
Wire Wire Line
	6875 2075 6525 2075
Wire Wire Line
	6525 2175 6875 2175
Wire Wire Line
	6525 2675 6875 2675
Entry Wire Line
	6875 2675 6975 2775
Text Label 6850 2075 2    28   ~ 0
PWM_2
Text Label 6850 2175 2    28   ~ 0
PWM_1
Text Label 6850 2675 2    28   ~ 0
PWM_3
Text Notes 10625 7650 2    60   ~ 0
2
Entry Wire Line
	6875 5525 6975 5625
Wire Wire Line
	6525 5125 6650 5125
Wire Wire Line
	6650 5125 6650 5325
Wire Wire Line
	6650 5325 9675 5325
Wire Wire Line
	6525 5525 6875 5525
Text Label 6725 5525 2    28   ~ 0
PWM_7
Text Label 6725 5025 2    28   ~ 0
PWM_5
Text Label 6725 4925 2    28   ~ 0
PWM_6
Entry Wire Line
	6875 5025 6975 5125
Entry Wire Line
	6875 4925 6975 5025
Wire Wire Line
	6875 4925 6525 4925
Wire Wire Line
	6525 5025 6875 5025
NoConn ~ 8100 2700
NoConn ~ 8150 2750
NoConn ~ 8100 2800
NoConn ~ 8150 2850
NoConn ~ 8100 2900
NoConn ~ 8150 2950
NoConn ~ 8100 3350
NoConn ~ 8150 3400
NoConn ~ 8100 3450
NoConn ~ 8150 3500
NoConn ~ 8150 3700
NoConn ~ 8100 3750
NoConn ~ 8100 3850
NoConn ~ 8150 3900
NoConn ~ 8100 3950
NoConn ~ 8150 4000
NoConn ~ 8100 4050
NoConn ~ 8150 4100
NoConn ~ 8100 4150
NoConn ~ 8150 4200
NoConn ~ 8100 4250
NoConn ~ 8150 4300
NoConn ~ 8100 4350
NoConn ~ 8150 4400
NoConn ~ 8100 4450
NoConn ~ 8150 4500
NoConn ~ 8100 4550
NoConn ~ 9150 4600
NoConn ~ 9200 4550
NoConn ~ 9150 4500
NoConn ~ 9200 4450
NoConn ~ 9150 4400
NoConn ~ 9200 4350
NoConn ~ 9150 4300
NoConn ~ 9200 3950
NoConn ~ 9150 3900
NoConn ~ 9150 3800
NoConn ~ 9200 3750
NoConn ~ 9150 3650
NoConn ~ 9200 3600
NoConn ~ 9150 3550
NoConn ~ 9200 3500
NoConn ~ 9200 3400
NoConn ~ 9150 3350
NoConn ~ 9200 3300
NoConn ~ 9150 3200
NoConn ~ 9200 3150
NoConn ~ 9150 3100
NoConn ~ 9200 3050
NoConn ~ 9150 3000
NoConn ~ 9200 2950
NoConn ~ 9150 2900
NoConn ~ 9200 2850
NoConn ~ 9150 2450
NoConn ~ 9200 2400
NoConn ~ 9150 2350
NoConn ~ 9200 2300
$EndSCHEMATC
