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
LIBS:custom
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 6
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
L STM32F030K6Tx U7
U 1 1 59FC957C
P 6300 3950
F 0 "U7" H 5300 5100 50  0000 L BNN
F 1 "STM32F030K6Tx" H 7300 5100 50  0000 R BNN
F 2 "open-nsynth-super:LQFP-32_7x7mm_Pitch0.8mm" H 8300 5025 50  0001 R TNN
F 3 "" H 6300 3950 50  0001 C CNN
	1    6300 3950
	1    0    0    -1  
$EndComp
$Sheet
S 3950 6150 700  700 
U 59FB78B6
F0 "touch" 60
F1 "touch.sch" 60
F2 "scl" I L 3950 6250 39 
F3 "y_sda" I L 3950 6450 39 
F4 "x_sda" I L 3950 6350 39 
$EndSheet
$Comp
L GND #PWR021
U 1 1 59FC957D
P 6200 5300
F 0 "#PWR021" H 6200 5300 30  0001 C CNN
F 1 "GND" H 6200 5230 30  0001 C CNN
F 2 "" H 6200 5300 60  0001 C CNN
F 3 "" H 6200 5300 60  0001 C CNN
	1    6200 5300
	1    0    0    -1  
$EndComp
Text GLabel 5500 1700 2    39   Input ~ 0
3.3V_D
Wire Wire Line
	6200 5050 6200 5300
$Comp
L R R32
U 1 1 59FC9594
P 4650 3600
F 0 "R32" V 4730 3600 50  0000 C CNN
F 1 "10k" V 4650 3600 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 4580 3600 50  0001 C CNN
F 3 "" H 4650 3600 50  0000 C CNN
	1    4650 3600
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR022
U 1 1 59FC9595
P 4500 3950
F 0 "#PWR022" H 4500 3700 50  0001 C CNN
F 1 "GND" H 4500 3800 50  0000 C CNN
F 2 "" H 4500 3950 50  0000 C CNN
F 3 "" H 4500 3950 50  0000 C CNN
	1    4500 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 1900 6500 1900
$Comp
L C C13
U 1 1 59FC9596
P 6650 1900
F 0 "C13" H 6675 2000 50  0000 L CNN
F 1 "1uF" H 6675 1800 20  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 6688 1750 50  0001 C CNN
F 3 "" H 6650 1900 50  0000 C CNN
	1    6650 1900
	0    1    1    0   
$EndComp
Connection ~ 6400 1900
$Comp
L GND #PWR023
U 1 1 59FC9597
P 7000 2100
F 0 "#PWR023" H 7000 1850 50  0001 C CNN
F 1 "GND" H 7000 1950 50  0000 C CNN
F 2 "" H 7000 2100 50  0000 C CNN
F 3 "" H 7000 2100 50  0000 C CNN
	1    7000 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 1900 7000 1900
$Comp
L C C6
U 1 1 59FC9598
P 5250 1900
F 0 "C6" H 5275 2000 50  0000 L CNN
F 1 "0.1uF" H 5275 1800 20  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 5288 1750 50  0001 C CNN
F 3 "" H 5250 1900 50  0000 C CNN
	1    5250 1900
	0    -1   -1   0   
$EndComp
$Comp
L C C7
U 1 1 59FC9599
P 5250 1700
F 0 "C7" H 5275 1800 50  0000 L CNN
F 1 "0.1uF" H 5275 1600 20  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 5288 1550 50  0001 C CNN
F 3 "" H 5250 1700 50  0000 C CNN
	1    5250 1700
	0    -1   -1   0   
$EndComp
$Comp
L C C25
U 1 1 59FC959A
P 5250 2100
F 0 "C25" H 5275 2200 50  0000 L CNN
F 1 "2.2uF" H 5275 2000 20  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 5288 1950 50  0001 C CNN
F 3 "" H 5250 2100 50  0000 C CNN
	1    5250 2100
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR024
U 1 1 59FC959B
P 4850 2100
F 0 "#PWR024" H 4850 1850 50  0001 C CNN
F 1 "GND" H 4850 1950 50  0000 C CNN
F 2 "" H 4850 2100 50  0000 C CNN
F 3 "" H 4850 2100 50  0000 C CNN
	1    4850 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 2100 4850 1900
Wire Wire Line
	4850 1900 5100 1900
Wire Wire Line
	5500 2100 5400 2100
Wire Wire Line
	5500 1700 5500 2300
Connection ~ 5500 1900
Wire Wire Line
	5100 1700 5000 1700
Wire Wire Line
	5000 1700 5000 2300
Connection ~ 5000 1900
Wire Wire Line
	5000 2100 5100 2100
$Comp
L C C3
U 1 1 59FC959C
P 6650 1700
F 0 "C3" H 6675 1800 50  0000 L CNN
F 1 "0.01uF" H 6675 1600 20  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 6688 1550 50  0001 C CNN
F 3 "" H 6650 1700 50  0000 C CNN
	1    6650 1700
	0    1    1    0   
$EndComp
Wire Wire Line
	6800 1700 7000 1700
Wire Wire Line
	7000 1700 7000 2100
$Comp
L C C8
U 1 1 59FC959D
P 4350 3650
F 0 "C8" H 4375 3750 50  0000 L CNN
F 1 "0.1uF" H 4375 3550 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 4388 3500 50  0001 C CNN
F 3 "" H 4350 3650 50  0000 C CNN
	1    4350 3650
	-1   0    0    1   
$EndComp
Connection ~ 6200 1900
Wire Wire Line
	5500 1700 5400 1700
Wire Wire Line
	4650 3350 5200 3350
Wire Wire Line
	4350 3850 4650 3850
Wire Wire Line
	4500 3850 4500 3950
Connection ~ 4500 3850
Wire Wire Line
	5200 3150 4350 3150
Wire Wire Line
	4350 3150 4350 3500
Wire Wire Line
	5000 4050 5200 4050
Wire Wire Line
	5000 4150 5200 4150
Wire Wire Line
	7400 3150 7600 3150
Wire Wire Line
	7400 3250 7600 3250
Wire Wire Line
	7400 3350 7600 3350
Wire Wire Line
	7400 3450 7600 3450
Wire Wire Line
	7400 3550 7600 3550
Wire Wire Line
	7400 3650 7600 3650
Wire Wire Line
	7400 3750 7600 3750
Wire Wire Line
	7400 3850 7600 3850
Wire Wire Line
	7400 3950 7600 3950
Wire Wire Line
	7400 4050 7600 4050
Wire Wire Line
	7400 4150 7600 4150
Wire Wire Line
	7400 4250 7600 4250
Wire Wire Line
	7400 4450 7600 4450
Wire Wire Line
	7400 4550 7600 4550
Wire Wire Line
	6300 5050 6300 5250
Wire Wire Line
	6300 5250 6200 5250
Connection ~ 6200 5250
Wire Wire Line
	6400 1700 6500 1700
Connection ~ 6300 1900
Wire Wire Line
	4350 3850 4350 3800
Wire Wire Line
	4650 3850 4650 3750
Wire Wire Line
	4650 3350 4650 3450
Connection ~ 7000 1900
NoConn ~ 5200 3750
NoConn ~ 5200 3850
NoConn ~ 7400 4350
Text HLabel 4350 3150 0    39   Input ~ 0
ICP_RST
Text HLabel 7600 4450 2    39   Input ~ 0
ICP_IO
Text HLabel 7600 4550 2    39   Input ~ 0
ICP_CLK
NoConn ~ 7400 4650
$Comp
L POT RV4
U 1 1 59FCF912
P 9900 4550
F 0 "RV4" V 9725 4550 50  0000 C CNN
F 1 "POT" V 9800 4550 50  0000 C CNN
F 2 "open-nsynth-super:Potentiometer_Alps_RK09K" H 9900 4550 50  0001 C CNN
F 3 "" H 9900 4550 50  0001 C CNN
	1    9900 4550
	0    1    1    0   
$EndComp
$Comp
L POT RV3
U 1 1 59FCFCF0
P 9900 4150
F 0 "RV3" V 9725 4150 50  0000 C CNN
F 1 "POT" V 9800 4150 50  0000 C CNN
F 2 "open-nsynth-super:Potentiometer_Alps_RK09K" H 9900 4150 50  0001 C CNN
F 3 "" H 9900 4150 50  0001 C CNN
	1    9900 4150
	0    1    1    0   
$EndComp
$Comp
L POT RV2
U 1 1 59FCFD94
P 9900 3750
F 0 "RV2" V 9725 3750 50  0000 C CNN
F 1 "POT" V 9800 3750 50  0000 C CNN
F 2 "open-nsynth-super:Potentiometer_Alps_RK09K" H 9900 3750 50  0001 C CNN
F 3 "" H 9900 3750 50  0001 C CNN
	1    9900 3750
	0    1    1    0   
$EndComp
$Comp
L POT RV1
U 1 1 59FCFE37
P 9900 3350
F 0 "RV1" V 9725 3350 50  0000 C CNN
F 1 "POT" V 9800 3350 50  0000 C CNN
F 2 "open-nsynth-super:Potentiometer_Alps_RK09K" H 9900 3350 50  0001 C CNN
F 3 "" H 9900 3350 50  0001 C CNN
	1    9900 3350
	0    1    1    0   
$EndComp
$Comp
L POT RV0
U 1 1 59FCFEE5
P 9900 2950
F 0 "RV0" V 9725 2950 50  0000 C CNN
F 1 "POT" V 9800 2950 50  0000 C CNN
F 2 "open-nsynth-super:Potentiometer_Alps_RK09K" H 9900 2950 50  0001 C CNN
F 3 "" H 9900 2950 50  0001 C CNN
	1    9900 2950
	0    1    1    0   
$EndComp
$Comp
L POT RV5
U 1 1 59FCFFEF
P 9900 4950
F 0 "RV5" V 9725 4950 50  0000 C CNN
F 1 "POT" V 9800 4950 50  0000 C CNN
F 2 "open-nsynth-super:Potentiometer_Alps_RK09K" H 9900 4950 50  0001 C CNN
F 3 "" H 9900 4950 50  0001 C CNN
	1    9900 4950
	0    1    1    0   
$EndComp
Text GLabel 10050 2950 2    39   Input ~ 0
3.3V_D
Wire Wire Line
	10050 2950 10050 4950
Connection ~ 10050 3350
Wire Wire Line
	9750 2950 9750 4950
Connection ~ 9750 3350
$Comp
L GND #PWR025
U 1 1 59FD0DFC
P 9750 2950
F 0 "#PWR025" H 9750 2700 50  0001 C CNN
F 1 "GND" H 9750 2800 50  0000 C CNN
F 2 "" H 9750 2950 50  0000 C CNN
F 3 "" H 9750 2950 50  0000 C CNN
	1    9750 2950
	0    1    1    0   
$EndComp
Text Label 7600 3150 0    39   ~ 0
pot0
Text Label 7600 3250 0    39   ~ 0
pot1
Text Label 7600 3350 0    39   ~ 0
pot2
Text Label 7600 3450 0    39   ~ 0
pot3
Text Label 7600 3550 0    39   ~ 0
pot4
Text Label 7600 3650 0    39   ~ 0
pot5
Text Label 9900 3100 0    39   ~ 0
pot0
Text Label 9900 3500 0    39   ~ 0
pot1
Text Label 9900 3900 0    39   ~ 0
pot2
Text Label 9900 4300 0    39   ~ 0
pot3
Text Label 9900 4700 0    39   ~ 0
pot4
Text Label 9900 5100 0    39   ~ 0
pot5
Connection ~ 10050 3750
Connection ~ 10050 4150
Connection ~ 10050 4550
Connection ~ 9750 3750
Connection ~ 9750 4150
Connection ~ 9750 4550
$Comp
L Rotary_Encoder_Switch SW0
U 1 1 59FD2EA2
P 1950 2800
F 0 "SW0" H 1950 3060 50  0000 C CNN
F 1 "Rotary_Encoder" H 1950 2540 50  0000 C CNN
F 2 "open-nsynth-super:PEC11R-S" H 1850 2960 50  0001 C CNN
F 3 "" H 1950 3060 50  0001 C CNN
	1    1950 2800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR026
U 1 1 59FD3EA7
P 1450 2800
F 0 "#PWR026" H 1450 2550 50  0001 C CNN
F 1 "GND" H 1450 2650 50  0000 C CNN
F 2 "" H 1450 2800 50  0000 C CNN
F 3 "" H 1450 2800 50  0000 C CNN
	1    1450 2800
	0    1    1    0   
$EndComp
Text Label 1650 2700 2    39   ~ 0
rot0a
Wire Wire Line
	1650 2800 1450 2800
Text Label 1650 2900 2    39   ~ 0
rot0b
$Comp
L Rotary_Encoder_Switch SW1
U 1 1 59FD480F
P 1950 3500
F 0 "SW1" H 1950 3760 50  0000 C CNN
F 1 "Rotary_Encoder" H 1950 3240 50  0000 C CNN
F 2 "open-nsynth-super:PEC11R-S" H 1850 3660 50  0001 C CNN
F 3 "" H 1950 3760 50  0001 C CNN
	1    1950 3500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR027
U 1 1 59FD4815
P 1450 3500
F 0 "#PWR027" H 1450 3250 50  0001 C CNN
F 1 "GND" H 1450 3350 50  0000 C CNN
F 2 "" H 1450 3500 50  0000 C CNN
F 3 "" H 1450 3500 50  0000 C CNN
	1    1450 3500
	0    1    1    0   
$EndComp
Text Label 1650 3400 2    39   ~ 0
rot1a
Wire Wire Line
	1650 3500 1450 3500
Text Label 1650 3600 2    39   ~ 0
rot1b
$Comp
L Rotary_Encoder_Switch SW2
U 1 1 59FD4872
P 1950 4200
F 0 "SW2" H 1950 4460 50  0000 C CNN
F 1 "Rotary_Encoder" H 1950 3940 50  0000 C CNN
F 2 "open-nsynth-super:PEC11R-S" H 1850 4360 50  0001 C CNN
F 3 "" H 1950 4460 50  0001 C CNN
	1    1950 4200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR028
U 1 1 59FD4878
P 1450 4200
F 0 "#PWR028" H 1450 3950 50  0001 C CNN
F 1 "GND" H 1450 4050 50  0000 C CNN
F 2 "" H 1450 4200 50  0000 C CNN
F 3 "" H 1450 4200 50  0000 C CNN
	1    1450 4200
	0    1    1    0   
$EndComp
Text Label 1650 4100 2    39   ~ 0
rot2a
Wire Wire Line
	1650 4200 1450 4200
Text Label 1650 4300 2    39   ~ 0
rot2b
$Comp
L Rotary_Encoder_Switch SW3
U 1 1 59FD4983
P 1950 4900
F 0 "SW3" H 1950 5160 50  0000 C CNN
F 1 "Rotary_Encoder" H 1950 4640 50  0000 C CNN
F 2 "open-nsynth-super:PEC11R-S" H 1850 5060 50  0001 C CNN
F 3 "" H 1950 5160 50  0001 C CNN
	1    1950 4900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR029
U 1 1 59FD4989
P 1450 4900
F 0 "#PWR029" H 1450 4650 50  0001 C CNN
F 1 "GND" H 1450 4750 50  0000 C CNN
F 2 "" H 1450 4900 50  0000 C CNN
F 3 "" H 1450 4900 50  0000 C CNN
	1    1450 4900
	0    1    1    0   
$EndComp
Text Label 1650 4800 2    39   ~ 0
rot3a
Wire Wire Line
	1650 4900 1450 4900
Text Label 1650 5000 2    39   ~ 0
rot3b
Text Label 5000 4050 2    39   ~ 0
rot1a
Text Label 5000 4150 2    39   ~ 0
rot1b
Text Label 7600 3750 0    39   ~ 0
rot0a
Text Label 7600 3850 0    39   ~ 0
rot0b
Text Label 7600 3950 0    39   ~ 0
rot2a
Text Label 7600 4050 0    39   ~ 0
rot2b
Text Label 7600 4150 0    39   ~ 0
rot3a
Text Label 7600 4250 0    39   ~ 0
rot3b
Wire Wire Line
	5200 4650 5000 4650
Wire Wire Line
	5200 4550 5000 4550
Text HLabel 5000 4550 0    39   Input ~ 0
SCL
Text HLabel 5000 4650 0    39   Input ~ 0
SDA
Wire Wire Line
	5200 4250 5000 4250
Wire Wire Line
	5200 4350 5000 4350
Wire Wire Line
	5200 4450 5000 4450
Text Label 5000 4250 2    39   ~ 0
ts0_sda
Text Label 5000 4350 2    39   ~ 0
ts1_sda
Text Label 5000 4450 2    39   ~ 0
ts0_1_scl
Wire Wire Line
	3950 6250 3750 6250
Wire Wire Line
	3950 6350 3750 6350
Wire Wire Line
	3950 6450 3750 6450
Text Label 3750 6250 2    39   ~ 0
ts0_1_scl
Text Label 3750 6350 2    39   ~ 0
ts0_sda
Text Label 3750 6450 2    39   ~ 0
ts1_sda
Wire Wire Line
	6400 1700 6400 2750
Wire Wire Line
	6300 1900 6300 2750
Wire Wire Line
	6200 1900 6200 2750
$Comp
L C C26
U 1 1 5A0C4B06
P 5250 2300
F 0 "C26" H 5275 2400 50  0000 L CNN
F 1 "2.2uF" H 5275 2200 20  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 5288 2150 50  0001 C CNN
F 3 "" H 5250 2300 50  0000 C CNN
	1    5250 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5500 2300 5400 2300
Connection ~ 5500 2100
Wire Wire Line
	5000 2300 5100 2300
Connection ~ 5000 2100
Wire Wire Line
	2250 2700 2400 2700
$Comp
L GND #PWR?
U 1 1 5B682297
P 2400 2700
F 0 "#PWR?" H 2400 2450 50  0001 C CNN
F 1 "GND" H 2400 2550 50  0000 C CNN
F 2 "" H 2400 2700 50  0000 C CNN
F 3 "" H 2400 2700 50  0000 C CNN
	1    2400 2700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2250 3400 2400 3400
$Comp
L GND #PWR?
U 1 1 5B6823BB
P 2400 3400
F 0 "#PWR?" H 2400 3150 50  0001 C CNN
F 1 "GND" H 2400 3250 50  0000 C CNN
F 2 "" H 2400 3400 50  0000 C CNN
F 3 "" H 2400 3400 50  0000 C CNN
	1    2400 3400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2250 4100 2400 4100
$Comp
L GND #PWR?
U 1 1 5B6824A4
P 2400 4100
F 0 "#PWR?" H 2400 3850 50  0001 C CNN
F 1 "GND" H 2400 3950 50  0000 C CNN
F 2 "" H 2400 4100 50  0000 C CNN
F 3 "" H 2400 4100 50  0000 C CNN
	1    2400 4100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2250 4800 2400 4800
$Comp
L GND #PWR?
U 1 1 5B682555
P 2400 4800
F 0 "#PWR?" H 2400 4550 50  0001 C CNN
F 1 "GND" H 2400 4650 50  0000 C CNN
F 2 "" H 2400 4800 50  0000 C CNN
F 3 "" H 2400 4800 50  0000 C CNN
	1    2400 4800
	0    -1   -1   0   
$EndComp
Text GLabel 2250 2900 2    39   Output ~ 0
rot0s
Text GLabel 2250 3600 2    39   Output ~ 0
rot1s
Text GLabel 2250 4300 2    39   Output ~ 0
rot2s
Text GLabel 2250 5000 2    39   Output ~ 0
rot3s
$EndSCHEMATC
