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
Sheet 6 6
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
L 6N136 IC1
U 1 1 59FE284C
P 6350 3550
F 0 "IC1" H 6140 3780 40  0000 C CNN
F 1 "6N139" H 6510 3310 40  0000 C CNN
F 2 "open-nsynth-super:DIP-8_SMD" H 6150 3320 30  0001 C CIN
F 3 "" H 6350 3550 60  0000 C CNN
	1    6350 3550
	1    0    0    -1  
$EndComp
$Comp
L DIODE D1
U 1 1 59FE2853
P 5750 3650
F 0 "D1" H 5750 3750 40  0000 C CNN
F 1 "DIODE" H 5750 3550 40  0000 C CNN
F 2 "open-nsynth-super:D_1206" H 5750 3650 60  0001 C CNN
F 3 "" H 5750 3650 60  0000 C CNN
	1    5750 3650
	0    -1   -1   0   
$EndComp
$Comp
L R R1
U 1 1 59FE285A
P 5500 3850
F 0 "R1" V 5580 3850 40  0000 C CNN
F 1 "220" V 5507 3851 40  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5430 3850 30  0001 C CNN
F 3 "" H 5500 3850 30  0000 C CNN
	1    5500 3850
	0    1    1    0   
$EndComp
$Comp
L R R7
U 1 1 59FE2861
P 7000 3650
F 0 "R7" V 7080 3650 40  0000 C CNN
F 1 "470" V 7007 3651 40  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 6930 3650 30  0001 C CNN
F 3 "" H 7000 3650 30  0000 C CNN
	1    7000 3650
	1    0    0    -1  
$EndComp
NoConn ~ 6650 3450
Text GLabel 7100 3350 2    39   Input ~ 0
3.3V_D
$Comp
L GND #PWR032
U 1 1 59FE2875
P 6650 4000
F 0 "#PWR032" H 6650 3750 50  0001 C CNN
F 1 "GND" H 6650 3850 50  0000 C CNN
F 2 "" H 6650 4000 50  0001 C CNN
F 3 "" H 6650 4000 50  0001 C CNN
	1    6650 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 3450 6050 3450
Wire Wire Line
	6000 3850 6000 3650
Wire Wire Line
	5650 3850 6000 3850
Wire Wire Line
	6750 3900 7100 3900
Connection ~ 7000 3900
Wire Wire Line
	6750 3900 6750 3650
Wire Wire Line
	6750 3650 6650 3650
Connection ~ 5750 3450
Wire Wire Line
	6000 3650 6050 3650
Wire Wire Line
	6650 4000 6650 3750
Wire Wire Line
	6650 3350 7100 3350
Wire Wire Line
	7000 3500 7000 3350
Connection ~ 7000 3350
Wire Wire Line
	7000 3900 7000 3800
Connection ~ 5750 3850
Wire Wire Line
	5350 3850 5250 3850
Text HLabel 7100 3900 2    39   Output ~ 0
MIDI_IN
Text HLabel 5250 3850 0    39   Input ~ 0
MIDI_5
Text HLabel 5250 3450 0    39   Input ~ 0
MIDI_4
$EndSCHEMATC
