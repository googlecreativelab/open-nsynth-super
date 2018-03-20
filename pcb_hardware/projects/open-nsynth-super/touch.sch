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
Sheet 5 6
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
L AT42QT2120 U2
U 1 1 59FEA078
P 3600 6000
F 0 "U2" H 3600 6530 50  0000 C CNN
F 1 "AT42QT2120" H 3600 6600 50  0000 C CNN
F 2 "open-nsynth-super:TSSOP-20_4.4x6.5mm_Pitch0.65mm" H 3600 5950 60  0001 C CNN
F 3 "" H 3600 5950 60  0000 C CNN
	1    3600 6000
	-1   0    0    -1  
$EndComp
NoConn ~ 3100 5900
NoConn ~ 3100 6100
$Comp
L C C4
U 1 1 59FEA10B
P 3050 6650
F 0 "C4" H 3075 6750 50  0000 L CNN
F 1 "0.1uF" H 3075 6550 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 3088 6500 50  0001 C CNN
F 3 "" H 3050 6650 50  0001 C CNN
	1    3050 6650
	1    0    0    -1  
$EndComp
$Comp
L AT42QT2120 U3
U 1 1 59FEA114
P 3600 3950
F 0 "U3" H 3600 4480 50  0000 C CNN
F 1 "AT42QT2120" H 3600 4550 50  0000 C CNN
F 2 "open-nsynth-super:TSSOP-20_4.4x6.5mm_Pitch0.65mm" H 3600 3900 60  0001 C CNN
F 3 "" H 3600 3900 60  0000 C CNN
	1    3600 3950
	-1   0    0    -1  
$EndComp
NoConn ~ 3100 3850
NoConn ~ 3100 4050
$Comp
L C C5
U 1 1 59FEA1A7
P 3050 4600
F 0 "C5" H 3075 4700 50  0000 L CNN
F 1 "0.1uF" H 3075 4500 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206_HandSoldering" H 3088 4450 50  0001 C CNN
F 3 "" H 3050 4600 50  0001 C CNN
	1    3050 4600
	1    0    0    -1  
$EndComp
Text GLabel 2950 6500 0    39   Input ~ 0
3.3V_D
Text GLabel 2950 6200 0    39   Input ~ 0
3.3V_D
Text GLabel 2950 4450 0    39   Input ~ 0
3.3V_D
Text GLabel 2950 4150 0    39   Input ~ 0
3.3V_D
$Comp
L GND #PWR030
U 1 1 59FEA1B8
P 3050 4750
F 0 "#PWR030" H 3050 4500 50  0001 C CNN
F 1 "GND" H 3050 4600 50  0000 C CNN
F 2 "" H 3050 4750 50  0000 C CNN
F 3 "" H 3050 4750 50  0000 C CNN
	1    3050 4750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR031
U 1 1 59FEA1BE
P 3050 6800
F 0 "#PWR031" H 3050 6550 50  0001 C CNN
F 1 "GND" H 3050 6650 50  0000 C CNN
F 2 "" H 3050 6800 50  0000 C CNN
F 3 "" H 3050 6800 50  0000 C CNN
	1    3050 6800
	1    0    0    -1  
$EndComp
Text HLabel 2900 3950 0    39   Input ~ 0
scl
Text HLabel 2950 4250 0    39   Input ~ 0
y_sda
Text HLabel 2900 6000 0    39   Input ~ 0
scl
Text HLabel 2950 6300 0    39   Input ~ 0
x_sda
$Comp
L touch TC1
U 1 1 59FEF8A7
P 5550 4350
F 0 "TC1" H 5650 4250 40  0000 C CNN
F 1 "touch" H 5650 4450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5550 4350 60  0001 C CNN
F 3 "" H 5550 4350 60  0001 C CNN
	1    5550 4350
	1    0    0    -1  
$EndComp
$Comp
L touch TC2
U 1 1 59FEFAB5
P 5850 4350
F 0 "TC2" H 5950 4250 40  0000 C CNN
F 1 "touch" H 5950 4450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5850 4350 60  0001 C CNN
F 3 "" H 5850 4350 60  0001 C CNN
	1    5850 4350
	1    0    0    -1  
$EndComp
$Comp
L touch TC3
U 1 1 59FEFB10
P 6150 4350
F 0 "TC3" H 6250 4250 40  0000 C CNN
F 1 "touch" H 6250 4450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6150 4350 60  0001 C CNN
F 3 "" H 6150 4350 60  0001 C CNN
	1    6150 4350
	1    0    0    -1  
$EndComp
$Comp
L touch TC4
U 1 1 59FEFB73
P 6450 4350
F 0 "TC4" H 6550 4250 40  0000 C CNN
F 1 "touch" H 6550 4450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6450 4350 60  0001 C CNN
F 3 "" H 6450 4350 60  0001 C CNN
	1    6450 4350
	1    0    0    -1  
$EndComp
$Comp
L touch TC5
U 1 1 59FEFBEE
P 6750 4350
F 0 "TC5" H 6850 4250 40  0000 C CNN
F 1 "touch" H 6850 4450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6750 4350 60  0001 C CNN
F 3 "" H 6750 4350 60  0001 C CNN
	1    6750 4350
	1    0    0    -1  
$EndComp
$Comp
L touch TC6
U 1 1 59FEFC4F
P 7050 4350
F 0 "TC6" H 7150 4250 40  0000 C CNN
F 1 "touch" H 7150 4450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7050 4350 60  0001 C CNN
F 3 "" H 7050 4350 60  0001 C CNN
	1    7050 4350
	1    0    0    -1  
$EndComp
$Comp
L touch TC7
U 1 1 59FEFD00
P 7350 4350
F 0 "TC7" H 7450 4250 40  0000 C CNN
F 1 "touch" H 7450 4450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7350 4350 60  0001 C CNN
F 3 "" H 7350 4350 60  0001 C CNN
	1    7350 4350
	1    0    0    -1  
$EndComp
$Comp
L touch TC8
U 1 1 59FEFD69
P 7650 4350
F 0 "TC8" H 7750 4250 40  0000 C CNN
F 1 "touch" H 7750 4450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7650 4350 60  0001 C CNN
F 3 "" H 7650 4350 60  0001 C CNN
	1    7650 4350
	1    0    0    -1  
$EndComp
$Comp
L touch TC9
U 1 1 59FEFE5E
P 7950 4350
F 0 "TC9" H 8050 4250 40  0000 C CNN
F 1 "touch" H 8050 4450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7950 4350 60  0001 C CNN
F 3 "" H 7950 4350 60  0001 C CNN
	1    7950 4350
	1    0    0    -1  
$EndComp
$Comp
L touch TC10
U 1 1 59FEFEF2
P 8250 4350
F 0 "TC10" H 8350 4250 40  0000 C CNN
F 1 "touch" H 8350 4450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8250 4350 60  0001 C CNN
F 3 "" H 8250 4350 60  0001 C CNN
	1    8250 4350
	1    0    0    -1  
$EndComp
$Comp
L touch TC11
U 1 1 59FEFF89
P 8550 4350
F 0 "TC11" H 8650 4250 40  0000 C CNN
F 1 "touch" H 8650 4450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8550 4350 60  0001 C CNN
F 3 "" H 8550 4350 60  0001 C CNN
	1    8550 4350
	1    0    0    -1  
$EndComp
$Comp
L touch TC12
U 1 1 59FF05BA
P 5550 4050
F 0 "TC12" H 5650 3950 40  0000 C CNN
F 1 "touch" H 5650 4150 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5550 4050 60  0001 C CNN
F 3 "" H 5550 4050 60  0001 C CNN
	1    5550 4050
	1    0    0    -1  
$EndComp
$Comp
L touch TC13
U 1 1 59FF05C0
P 5850 4050
F 0 "TC13" H 5950 3950 40  0000 C CNN
F 1 "touch" H 5950 4150 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5850 4050 60  0001 C CNN
F 3 "" H 5850 4050 60  0001 C CNN
	1    5850 4050
	1    0    0    -1  
$EndComp
$Comp
L touch TC14
U 1 1 59FF05C6
P 6150 4050
F 0 "TC14" H 6250 3950 40  0000 C CNN
F 1 "touch" H 6250 4150 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6150 4050 60  0001 C CNN
F 3 "" H 6150 4050 60  0001 C CNN
	1    6150 4050
	1    0    0    -1  
$EndComp
$Comp
L touch TC15
U 1 1 59FF05CC
P 6450 4050
F 0 "TC15" H 6550 3950 40  0000 C CNN
F 1 "touch" H 6550 4150 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6450 4050 60  0001 C CNN
F 3 "" H 6450 4050 60  0001 C CNN
	1    6450 4050
	1    0    0    -1  
$EndComp
$Comp
L touch TC16
U 1 1 59FF05D2
P 6750 4050
F 0 "TC16" H 6850 3950 40  0000 C CNN
F 1 "touch" H 6850 4150 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6750 4050 60  0001 C CNN
F 3 "" H 6750 4050 60  0001 C CNN
	1    6750 4050
	1    0    0    -1  
$EndComp
$Comp
L touch TC17
U 1 1 59FF05D8
P 7050 4050
F 0 "TC17" H 7150 3950 40  0000 C CNN
F 1 "touch" H 7150 4150 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7050 4050 60  0001 C CNN
F 3 "" H 7050 4050 60  0001 C CNN
	1    7050 4050
	1    0    0    -1  
$EndComp
$Comp
L touch TC18
U 1 1 59FF05DE
P 7350 4050
F 0 "TC18" H 7450 3950 40  0000 C CNN
F 1 "touch" H 7450 4150 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7350 4050 60  0001 C CNN
F 3 "" H 7350 4050 60  0001 C CNN
	1    7350 4050
	1    0    0    -1  
$EndComp
$Comp
L touch TC19
U 1 1 59FF05E4
P 7650 4050
F 0 "TC19" H 7750 3950 40  0000 C CNN
F 1 "touch" H 7750 4150 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7650 4050 60  0001 C CNN
F 3 "" H 7650 4050 60  0001 C CNN
	1    7650 4050
	1    0    0    -1  
$EndComp
$Comp
L touch TC20
U 1 1 59FF05EA
P 7950 4050
F 0 "TC20" H 8050 3950 40  0000 C CNN
F 1 "touch" H 8050 4150 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7950 4050 60  0001 C CNN
F 3 "" H 7950 4050 60  0001 C CNN
	1    7950 4050
	1    0    0    -1  
$EndComp
$Comp
L touch TC21
U 1 1 59FF05F0
P 8250 4050
F 0 "TC21" H 8350 3950 40  0000 C CNN
F 1 "touch" H 8350 4150 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8250 4050 60  0001 C CNN
F 3 "" H 8250 4050 60  0001 C CNN
	1    8250 4050
	1    0    0    -1  
$EndComp
$Comp
L touch TC22
U 1 1 59FF05F6
P 8550 4050
F 0 "TC22" H 8650 3950 40  0000 C CNN
F 1 "touch" H 8650 4150 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8550 4050 60  0001 C CNN
F 3 "" H 8550 4050 60  0001 C CNN
	1    8550 4050
	1    0    0    -1  
$EndComp
$Comp
L touch TC23
U 1 1 59FF083C
P 5550 3750
F 0 "TC23" H 5650 3650 40  0000 C CNN
F 1 "touch" H 5650 3850 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5550 3750 60  0001 C CNN
F 3 "" H 5550 3750 60  0001 C CNN
	1    5550 3750
	1    0    0    -1  
$EndComp
$Comp
L touch TC24
U 1 1 59FF0842
P 5850 3750
F 0 "TC24" H 5950 3650 40  0000 C CNN
F 1 "touch" H 5950 3850 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5850 3750 60  0001 C CNN
F 3 "" H 5850 3750 60  0001 C CNN
	1    5850 3750
	1    0    0    -1  
$EndComp
$Comp
L touch TC25
U 1 1 59FF0848
P 6150 3750
F 0 "TC25" H 6250 3650 40  0000 C CNN
F 1 "touch" H 6250 3850 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6150 3750 60  0001 C CNN
F 3 "" H 6150 3750 60  0001 C CNN
	1    6150 3750
	1    0    0    -1  
$EndComp
$Comp
L touch TC26
U 1 1 59FF084E
P 6450 3750
F 0 "TC26" H 6550 3650 40  0000 C CNN
F 1 "touch" H 6550 3850 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6450 3750 60  0001 C CNN
F 3 "" H 6450 3750 60  0001 C CNN
	1    6450 3750
	1    0    0    -1  
$EndComp
$Comp
L touch TC27
U 1 1 59FF0854
P 6750 3750
F 0 "TC27" H 6850 3650 40  0000 C CNN
F 1 "touch" H 6850 3850 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6750 3750 60  0001 C CNN
F 3 "" H 6750 3750 60  0001 C CNN
	1    6750 3750
	1    0    0    -1  
$EndComp
$Comp
L touch TC28
U 1 1 59FF085A
P 7050 3750
F 0 "TC28" H 7150 3650 40  0000 C CNN
F 1 "touch" H 7150 3850 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7050 3750 60  0001 C CNN
F 3 "" H 7050 3750 60  0001 C CNN
	1    7050 3750
	1    0    0    -1  
$EndComp
$Comp
L touch TC29
U 1 1 59FF0860
P 7350 3750
F 0 "TC29" H 7450 3650 40  0000 C CNN
F 1 "touch" H 7450 3850 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7350 3750 60  0001 C CNN
F 3 "" H 7350 3750 60  0001 C CNN
	1    7350 3750
	1    0    0    -1  
$EndComp
$Comp
L touch TC30
U 1 1 59FF0866
P 7650 3750
F 0 "TC30" H 7750 3650 40  0000 C CNN
F 1 "touch" H 7750 3850 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7650 3750 60  0001 C CNN
F 3 "" H 7650 3750 60  0001 C CNN
	1    7650 3750
	1    0    0    -1  
$EndComp
$Comp
L touch TC31
U 1 1 59FF086C
P 7950 3750
F 0 "TC31" H 8050 3650 40  0000 C CNN
F 1 "touch" H 8050 3850 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7950 3750 60  0001 C CNN
F 3 "" H 7950 3750 60  0001 C CNN
	1    7950 3750
	1    0    0    -1  
$EndComp
$Comp
L touch TC32
U 1 1 59FF0872
P 8250 3750
F 0 "TC32" H 8350 3650 40  0000 C CNN
F 1 "touch" H 8350 3850 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8250 3750 60  0001 C CNN
F 3 "" H 8250 3750 60  0001 C CNN
	1    8250 3750
	1    0    0    -1  
$EndComp
$Comp
L touch TC33
U 1 1 59FF0878
P 8550 3750
F 0 "TC33" H 8650 3650 40  0000 C CNN
F 1 "touch" H 8650 3850 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8550 3750 60  0001 C CNN
F 3 "" H 8550 3750 60  0001 C CNN
	1    8550 3750
	1    0    0    -1  
$EndComp
$Comp
L touch TC34
U 1 1 59FF0A2D
P 5550 3450
F 0 "TC34" H 5650 3350 40  0000 C CNN
F 1 "touch" H 5650 3550 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5550 3450 60  0001 C CNN
F 3 "" H 5550 3450 60  0001 C CNN
	1    5550 3450
	1    0    0    -1  
$EndComp
$Comp
L touch TC35
U 1 1 59FF0A33
P 5850 3450
F 0 "TC35" H 5950 3350 40  0000 C CNN
F 1 "touch" H 5950 3550 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5850 3450 60  0001 C CNN
F 3 "" H 5850 3450 60  0001 C CNN
	1    5850 3450
	1    0    0    -1  
$EndComp
$Comp
L touch TC36
U 1 1 59FF0A39
P 6150 3450
F 0 "TC36" H 6250 3350 40  0000 C CNN
F 1 "touch" H 6250 3550 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6150 3450 60  0001 C CNN
F 3 "" H 6150 3450 60  0001 C CNN
	1    6150 3450
	1    0    0    -1  
$EndComp
$Comp
L touch TC37
U 1 1 59FF0A3F
P 6450 3450
F 0 "TC37" H 6550 3350 40  0000 C CNN
F 1 "touch" H 6550 3550 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6450 3450 60  0001 C CNN
F 3 "" H 6450 3450 60  0001 C CNN
	1    6450 3450
	1    0    0    -1  
$EndComp
$Comp
L touch TC38
U 1 1 59FF0A45
P 6750 3450
F 0 "TC38" H 6850 3350 40  0000 C CNN
F 1 "touch" H 6850 3550 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6750 3450 60  0001 C CNN
F 3 "" H 6750 3450 60  0001 C CNN
	1    6750 3450
	1    0    0    -1  
$EndComp
$Comp
L touch TC39
U 1 1 59FF0A4B
P 7050 3450
F 0 "TC39" H 7150 3350 40  0000 C CNN
F 1 "touch" H 7150 3550 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7050 3450 60  0001 C CNN
F 3 "" H 7050 3450 60  0001 C CNN
	1    7050 3450
	1    0    0    -1  
$EndComp
$Comp
L touch TC40
U 1 1 59FF0A51
P 7350 3450
F 0 "TC40" H 7450 3350 40  0000 C CNN
F 1 "touch" H 7450 3550 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7350 3450 60  0001 C CNN
F 3 "" H 7350 3450 60  0001 C CNN
	1    7350 3450
	1    0    0    -1  
$EndComp
$Comp
L touch TC41
U 1 1 59FF0A57
P 7650 3450
F 0 "TC41" H 7750 3350 40  0000 C CNN
F 1 "touch" H 7750 3550 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7650 3450 60  0001 C CNN
F 3 "" H 7650 3450 60  0001 C CNN
	1    7650 3450
	1    0    0    -1  
$EndComp
$Comp
L touch TC42
U 1 1 59FF0A5D
P 7950 3450
F 0 "TC42" H 8050 3350 40  0000 C CNN
F 1 "touch" H 8050 3550 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7950 3450 60  0001 C CNN
F 3 "" H 7950 3450 60  0001 C CNN
	1    7950 3450
	1    0    0    -1  
$EndComp
$Comp
L touch TC43
U 1 1 59FF0A63
P 8250 3450
F 0 "TC43" H 8350 3350 40  0000 C CNN
F 1 "touch" H 8350 3550 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8250 3450 60  0001 C CNN
F 3 "" H 8250 3450 60  0001 C CNN
	1    8250 3450
	1    0    0    -1  
$EndComp
$Comp
L touch TC44
U 1 1 59FF0A69
P 8550 3450
F 0 "TC44" H 8650 3350 40  0000 C CNN
F 1 "touch" H 8650 3550 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8550 3450 60  0001 C CNN
F 3 "" H 8550 3450 60  0001 C CNN
	1    8550 3450
	1    0    0    -1  
$EndComp
$Comp
L touch TC45
U 1 1 59FF0C97
P 5550 3150
F 0 "TC45" H 5650 3050 40  0000 C CNN
F 1 "touch" H 5650 3250 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5550 3150 60  0001 C CNN
F 3 "" H 5550 3150 60  0001 C CNN
	1    5550 3150
	1    0    0    -1  
$EndComp
$Comp
L touch TC46
U 1 1 59FF0C9D
P 5850 3150
F 0 "TC46" H 5950 3050 40  0000 C CNN
F 1 "touch" H 5950 3250 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5850 3150 60  0001 C CNN
F 3 "" H 5850 3150 60  0001 C CNN
	1    5850 3150
	1    0    0    -1  
$EndComp
$Comp
L touch TC47
U 1 1 59FF0CA3
P 6150 3150
F 0 "TC47" H 6250 3050 40  0000 C CNN
F 1 "touch" H 6250 3250 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6150 3150 60  0001 C CNN
F 3 "" H 6150 3150 60  0001 C CNN
	1    6150 3150
	1    0    0    -1  
$EndComp
$Comp
L touch TC48
U 1 1 59FF0CA9
P 6450 3150
F 0 "TC48" H 6550 3050 40  0000 C CNN
F 1 "touch" H 6550 3250 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6450 3150 60  0001 C CNN
F 3 "" H 6450 3150 60  0001 C CNN
	1    6450 3150
	1    0    0    -1  
$EndComp
$Comp
L touch TC49
U 1 1 59FF0CAF
P 6750 3150
F 0 "TC49" H 6850 3050 40  0000 C CNN
F 1 "touch" H 6850 3250 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6750 3150 60  0001 C CNN
F 3 "" H 6750 3150 60  0001 C CNN
	1    6750 3150
	1    0    0    -1  
$EndComp
$Comp
L touch TC50
U 1 1 59FF0CB5
P 7050 3150
F 0 "TC50" H 7150 3050 40  0000 C CNN
F 1 "touch" H 7150 3250 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7050 3150 60  0001 C CNN
F 3 "" H 7050 3150 60  0001 C CNN
	1    7050 3150
	1    0    0    -1  
$EndComp
$Comp
L touch TC51
U 1 1 59FF0CBB
P 7350 3150
F 0 "TC51" H 7450 3050 40  0000 C CNN
F 1 "touch" H 7450 3250 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7350 3150 60  0001 C CNN
F 3 "" H 7350 3150 60  0001 C CNN
	1    7350 3150
	1    0    0    -1  
$EndComp
$Comp
L touch TC52
U 1 1 59FF0CC1
P 7650 3150
F 0 "TC52" H 7750 3050 40  0000 C CNN
F 1 "touch" H 7750 3250 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7650 3150 60  0001 C CNN
F 3 "" H 7650 3150 60  0001 C CNN
	1    7650 3150
	1    0    0    -1  
$EndComp
$Comp
L touch TC53
U 1 1 59FF0CC7
P 7950 3150
F 0 "TC53" H 8050 3050 40  0000 C CNN
F 1 "touch" H 8050 3250 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7950 3150 60  0001 C CNN
F 3 "" H 7950 3150 60  0001 C CNN
	1    7950 3150
	1    0    0    -1  
$EndComp
$Comp
L touch TC54
U 1 1 59FF0CCD
P 8250 3150
F 0 "TC54" H 8350 3050 40  0000 C CNN
F 1 "touch" H 8350 3250 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8250 3150 60  0001 C CNN
F 3 "" H 8250 3150 60  0001 C CNN
	1    8250 3150
	1    0    0    -1  
$EndComp
$Comp
L touch TC55
U 1 1 59FF0CD3
P 8550 3150
F 0 "TC55" H 8650 3050 40  0000 C CNN
F 1 "touch" H 8650 3250 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8550 3150 60  0001 C CNN
F 3 "" H 8550 3150 60  0001 C CNN
	1    8550 3150
	1    0    0    -1  
$EndComp
$Comp
L touch TC56
U 1 1 59FF0F38
P 5550 2850
F 0 "TC56" H 5650 2750 40  0000 C CNN
F 1 "touch" H 5650 2950 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5550 2850 60  0001 C CNN
F 3 "" H 5550 2850 60  0001 C CNN
	1    5550 2850
	1    0    0    -1  
$EndComp
$Comp
L touch TC57
U 1 1 59FF0F3E
P 5850 2850
F 0 "TC57" H 5950 2750 40  0000 C CNN
F 1 "touch" H 5950 2950 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5850 2850 60  0001 C CNN
F 3 "" H 5850 2850 60  0001 C CNN
	1    5850 2850
	1    0    0    -1  
$EndComp
$Comp
L touch TC58
U 1 1 59FF0F44
P 6150 2850
F 0 "TC58" H 6250 2750 40  0000 C CNN
F 1 "touch" H 6250 2950 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6150 2850 60  0001 C CNN
F 3 "" H 6150 2850 60  0001 C CNN
	1    6150 2850
	1    0    0    -1  
$EndComp
$Comp
L touch TC59
U 1 1 59FF0F4A
P 6450 2850
F 0 "TC59" H 6550 2750 40  0000 C CNN
F 1 "touch" H 6550 2950 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6450 2850 60  0001 C CNN
F 3 "" H 6450 2850 60  0001 C CNN
	1    6450 2850
	1    0    0    -1  
$EndComp
$Comp
L touch TC60
U 1 1 59FF0F50
P 6750 2850
F 0 "TC60" H 6850 2750 40  0000 C CNN
F 1 "touch" H 6850 2950 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6750 2850 60  0001 C CNN
F 3 "" H 6750 2850 60  0001 C CNN
	1    6750 2850
	1    0    0    -1  
$EndComp
$Comp
L touch TC61
U 1 1 59FF0F56
P 7050 2850
F 0 "TC61" H 7150 2750 40  0000 C CNN
F 1 "touch" H 7150 2950 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7050 2850 60  0001 C CNN
F 3 "" H 7050 2850 60  0001 C CNN
	1    7050 2850
	1    0    0    -1  
$EndComp
$Comp
L touch TC62
U 1 1 59FF0F5C
P 7350 2850
F 0 "TC62" H 7450 2750 40  0000 C CNN
F 1 "touch" H 7450 2950 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7350 2850 60  0001 C CNN
F 3 "" H 7350 2850 60  0001 C CNN
	1    7350 2850
	1    0    0    -1  
$EndComp
$Comp
L touch TC63
U 1 1 59FF0F62
P 7650 2850
F 0 "TC63" H 7750 2750 40  0000 C CNN
F 1 "touch" H 7750 2950 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7650 2850 60  0001 C CNN
F 3 "" H 7650 2850 60  0001 C CNN
	1    7650 2850
	1    0    0    -1  
$EndComp
$Comp
L touch TC64
U 1 1 59FF0F68
P 7950 2850
F 0 "TC64" H 8050 2750 40  0000 C CNN
F 1 "touch" H 8050 2950 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7950 2850 60  0001 C CNN
F 3 "" H 7950 2850 60  0001 C CNN
	1    7950 2850
	1    0    0    -1  
$EndComp
$Comp
L touch TC65
U 1 1 59FF0F6E
P 8250 2850
F 0 "TC65" H 8350 2750 40  0000 C CNN
F 1 "touch" H 8350 2950 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8250 2850 60  0001 C CNN
F 3 "" H 8250 2850 60  0001 C CNN
	1    8250 2850
	1    0    0    -1  
$EndComp
$Comp
L touch TC66
U 1 1 59FF0F74
P 8550 2850
F 0 "TC66" H 8650 2750 40  0000 C CNN
F 1 "touch" H 8650 2950 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8550 2850 60  0001 C CNN
F 3 "" H 8550 2850 60  0001 C CNN
	1    8550 2850
	1    0    0    -1  
$EndComp
$Comp
L touch TC67
U 1 1 59FF1160
P 5550 2550
F 0 "TC67" H 5650 2450 40  0000 C CNN
F 1 "touch" H 5650 2650 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5550 2550 60  0001 C CNN
F 3 "" H 5550 2550 60  0001 C CNN
	1    5550 2550
	1    0    0    -1  
$EndComp
$Comp
L touch TC68
U 1 1 59FF1166
P 5850 2550
F 0 "TC68" H 5950 2450 40  0000 C CNN
F 1 "touch" H 5950 2650 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5850 2550 60  0001 C CNN
F 3 "" H 5850 2550 60  0001 C CNN
	1    5850 2550
	1    0    0    -1  
$EndComp
$Comp
L touch TC69
U 1 1 59FF116C
P 6150 2550
F 0 "TC69" H 6250 2450 40  0000 C CNN
F 1 "touch" H 6250 2650 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6150 2550 60  0001 C CNN
F 3 "" H 6150 2550 60  0001 C CNN
	1    6150 2550
	1    0    0    -1  
$EndComp
$Comp
L touch TC70
U 1 1 59FF1172
P 6450 2550
F 0 "TC70" H 6550 2450 40  0000 C CNN
F 1 "touch" H 6550 2650 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6450 2550 60  0001 C CNN
F 3 "" H 6450 2550 60  0001 C CNN
	1    6450 2550
	1    0    0    -1  
$EndComp
$Comp
L touch TC71
U 1 1 59FF1178
P 6750 2550
F 0 "TC71" H 6850 2450 40  0000 C CNN
F 1 "touch" H 6850 2650 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6750 2550 60  0001 C CNN
F 3 "" H 6750 2550 60  0001 C CNN
	1    6750 2550
	1    0    0    -1  
$EndComp
$Comp
L touch TC72
U 1 1 59FF117E
P 7050 2550
F 0 "TC72" H 7150 2450 40  0000 C CNN
F 1 "touch" H 7150 2650 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7050 2550 60  0001 C CNN
F 3 "" H 7050 2550 60  0001 C CNN
	1    7050 2550
	1    0    0    -1  
$EndComp
$Comp
L touch TC73
U 1 1 59FF1184
P 7350 2550
F 0 "TC73" H 7450 2450 40  0000 C CNN
F 1 "touch" H 7450 2650 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7350 2550 60  0001 C CNN
F 3 "" H 7350 2550 60  0001 C CNN
	1    7350 2550
	1    0    0    -1  
$EndComp
$Comp
L touch TC74
U 1 1 59FF118A
P 7650 2550
F 0 "TC74" H 7750 2450 40  0000 C CNN
F 1 "touch" H 7750 2650 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7650 2550 60  0001 C CNN
F 3 "" H 7650 2550 60  0001 C CNN
	1    7650 2550
	1    0    0    -1  
$EndComp
$Comp
L touch TC75
U 1 1 59FF1190
P 7950 2550
F 0 "TC75" H 8050 2450 40  0000 C CNN
F 1 "touch" H 8050 2650 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7950 2550 60  0001 C CNN
F 3 "" H 7950 2550 60  0001 C CNN
	1    7950 2550
	1    0    0    -1  
$EndComp
$Comp
L touch TC76
U 1 1 59FF1196
P 8250 2550
F 0 "TC76" H 8350 2450 40  0000 C CNN
F 1 "touch" H 8350 2650 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8250 2550 60  0001 C CNN
F 3 "" H 8250 2550 60  0001 C CNN
	1    8250 2550
	1    0    0    -1  
$EndComp
$Comp
L touch TC77
U 1 1 59FF119C
P 8550 2550
F 0 "TC77" H 8650 2450 40  0000 C CNN
F 1 "touch" H 8650 2650 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8550 2550 60  0001 C CNN
F 3 "" H 8550 2550 60  0001 C CNN
	1    8550 2550
	1    0    0    -1  
$EndComp
$Comp
L touch TC78
U 1 1 59FF137D
P 5550 2250
F 0 "TC78" H 5650 2150 40  0000 C CNN
F 1 "touch" H 5650 2350 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5550 2250 60  0001 C CNN
F 3 "" H 5550 2250 60  0001 C CNN
	1    5550 2250
	1    0    0    -1  
$EndComp
$Comp
L touch TC79
U 1 1 59FF1383
P 5850 2250
F 0 "TC79" H 5950 2150 40  0000 C CNN
F 1 "touch" H 5950 2350 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5850 2250 60  0001 C CNN
F 3 "" H 5850 2250 60  0001 C CNN
	1    5850 2250
	1    0    0    -1  
$EndComp
$Comp
L touch TC80
U 1 1 59FF1389
P 6150 2250
F 0 "TC80" H 6250 2150 40  0000 C CNN
F 1 "touch" H 6250 2350 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6150 2250 60  0001 C CNN
F 3 "" H 6150 2250 60  0001 C CNN
	1    6150 2250
	1    0    0    -1  
$EndComp
$Comp
L touch TC81
U 1 1 59FF138F
P 6450 2250
F 0 "TC81" H 6550 2150 40  0000 C CNN
F 1 "touch" H 6550 2350 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6450 2250 60  0001 C CNN
F 3 "" H 6450 2250 60  0001 C CNN
	1    6450 2250
	1    0    0    -1  
$EndComp
$Comp
L touch TC82
U 1 1 59FF1395
P 6750 2250
F 0 "TC82" H 6850 2150 40  0000 C CNN
F 1 "touch" H 6850 2350 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6750 2250 60  0001 C CNN
F 3 "" H 6750 2250 60  0001 C CNN
	1    6750 2250
	1    0    0    -1  
$EndComp
$Comp
L touch TC83
U 1 1 59FF139B
P 7050 2250
F 0 "TC83" H 7150 2150 40  0000 C CNN
F 1 "touch" H 7150 2350 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7050 2250 60  0001 C CNN
F 3 "" H 7050 2250 60  0001 C CNN
	1    7050 2250
	1    0    0    -1  
$EndComp
$Comp
L touch TC84
U 1 1 59FF13A1
P 7350 2250
F 0 "TC84" H 7450 2150 40  0000 C CNN
F 1 "touch" H 7450 2350 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7350 2250 60  0001 C CNN
F 3 "" H 7350 2250 60  0001 C CNN
	1    7350 2250
	1    0    0    -1  
$EndComp
$Comp
L touch TC85
U 1 1 59FF13A7
P 7650 2250
F 0 "TC85" H 7750 2150 40  0000 C CNN
F 1 "touch" H 7750 2350 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7650 2250 60  0001 C CNN
F 3 "" H 7650 2250 60  0001 C CNN
	1    7650 2250
	1    0    0    -1  
$EndComp
$Comp
L touch TC86
U 1 1 59FF13AD
P 7950 2250
F 0 "TC86" H 8050 2150 40  0000 C CNN
F 1 "touch" H 8050 2350 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7950 2250 60  0001 C CNN
F 3 "" H 7950 2250 60  0001 C CNN
	1    7950 2250
	1    0    0    -1  
$EndComp
$Comp
L touch TC87
U 1 1 59FF13B3
P 8250 2250
F 0 "TC87" H 8350 2150 40  0000 C CNN
F 1 "touch" H 8350 2350 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8250 2250 60  0001 C CNN
F 3 "" H 8250 2250 60  0001 C CNN
	1    8250 2250
	1    0    0    -1  
$EndComp
$Comp
L touch TC88
U 1 1 59FF13B9
P 8550 2250
F 0 "TC88" H 8650 2150 40  0000 C CNN
F 1 "touch" H 8650 2350 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8550 2250 60  0001 C CNN
F 3 "" H 8550 2250 60  0001 C CNN
	1    8550 2250
	1    0    0    -1  
$EndComp
$Comp
L touch TC89
U 1 1 59FF16D3
P 5550 1950
F 0 "TC89" H 5650 1850 40  0000 C CNN
F 1 "touch" H 5650 2050 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5550 1950 60  0001 C CNN
F 3 "" H 5550 1950 60  0001 C CNN
	1    5550 1950
	1    0    0    -1  
$EndComp
$Comp
L touch TC90
U 1 1 59FF16D9
P 5850 1950
F 0 "TC90" H 5950 1850 40  0000 C CNN
F 1 "touch" H 5950 2050 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5850 1950 60  0001 C CNN
F 3 "" H 5850 1950 60  0001 C CNN
	1    5850 1950
	1    0    0    -1  
$EndComp
$Comp
L touch TC91
U 1 1 59FF16DF
P 6150 1950
F 0 "TC91" H 6250 1850 40  0000 C CNN
F 1 "touch" H 6250 2050 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6150 1950 60  0001 C CNN
F 3 "" H 6150 1950 60  0001 C CNN
	1    6150 1950
	1    0    0    -1  
$EndComp
$Comp
L touch TC92
U 1 1 59FF16E5
P 6450 1950
F 0 "TC92" H 6550 1850 40  0000 C CNN
F 1 "touch" H 6550 2050 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6450 1950 60  0001 C CNN
F 3 "" H 6450 1950 60  0001 C CNN
	1    6450 1950
	1    0    0    -1  
$EndComp
$Comp
L touch TC93
U 1 1 59FF16EB
P 6750 1950
F 0 "TC93" H 6850 1850 40  0000 C CNN
F 1 "touch" H 6850 2050 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6750 1950 60  0001 C CNN
F 3 "" H 6750 1950 60  0001 C CNN
	1    6750 1950
	1    0    0    -1  
$EndComp
$Comp
L touch TC94
U 1 1 59FF16F1
P 7050 1950
F 0 "TC94" H 7150 1850 40  0000 C CNN
F 1 "touch" H 7150 2050 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7050 1950 60  0001 C CNN
F 3 "" H 7050 1950 60  0001 C CNN
	1    7050 1950
	1    0    0    -1  
$EndComp
$Comp
L touch TC95
U 1 1 59FF16F7
P 7350 1950
F 0 "TC95" H 7450 1850 40  0000 C CNN
F 1 "touch" H 7450 2050 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7350 1950 60  0001 C CNN
F 3 "" H 7350 1950 60  0001 C CNN
	1    7350 1950
	1    0    0    -1  
$EndComp
$Comp
L touch TC96
U 1 1 59FF16FD
P 7650 1950
F 0 "TC96" H 7750 1850 40  0000 C CNN
F 1 "touch" H 7750 2050 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7650 1950 60  0001 C CNN
F 3 "" H 7650 1950 60  0001 C CNN
	1    7650 1950
	1    0    0    -1  
$EndComp
$Comp
L touch TC97
U 1 1 59FF1703
P 7950 1950
F 0 "TC97" H 8050 1850 40  0000 C CNN
F 1 "touch" H 8050 2050 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7950 1950 60  0001 C CNN
F 3 "" H 7950 1950 60  0001 C CNN
	1    7950 1950
	1    0    0    -1  
$EndComp
$Comp
L touch TC98
U 1 1 59FF1709
P 8250 1950
F 0 "TC98" H 8350 1850 40  0000 C CNN
F 1 "touch" H 8350 2050 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8250 1950 60  0001 C CNN
F 3 "" H 8250 1950 60  0001 C CNN
	1    8250 1950
	1    0    0    -1  
$EndComp
$Comp
L touch TC99
U 1 1 59FF170F
P 8550 1950
F 0 "TC99" H 8650 1850 40  0000 C CNN
F 1 "touch" H 8650 2050 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8550 1950 60  0001 C CNN
F 3 "" H 8550 1950 60  0001 C CNN
	1    8550 1950
	1    0    0    -1  
$EndComp
$Comp
L touch TC100
U 1 1 59FF195E
P 5550 1650
F 0 "TC100" H 5650 1550 40  0000 C CNN
F 1 "touch" H 5650 1750 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5550 1650 60  0001 C CNN
F 3 "" H 5550 1650 60  0001 C CNN
	1    5550 1650
	1    0    0    -1  
$EndComp
$Comp
L touch TC101
U 1 1 59FF1964
P 5850 1650
F 0 "TC101" H 5950 1550 40  0000 C CNN
F 1 "touch" H 5950 1750 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5850 1650 60  0001 C CNN
F 3 "" H 5850 1650 60  0001 C CNN
	1    5850 1650
	1    0    0    -1  
$EndComp
$Comp
L touch TC102
U 1 1 59FF196A
P 6150 1650
F 0 "TC102" H 6250 1550 40  0000 C CNN
F 1 "touch" H 6250 1750 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6150 1650 60  0001 C CNN
F 3 "" H 6150 1650 60  0001 C CNN
	1    6150 1650
	1    0    0    -1  
$EndComp
$Comp
L touch TC103
U 1 1 59FF1970
P 6450 1650
F 0 "TC103" H 6550 1550 40  0000 C CNN
F 1 "touch" H 6550 1750 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6450 1650 60  0001 C CNN
F 3 "" H 6450 1650 60  0001 C CNN
	1    6450 1650
	1    0    0    -1  
$EndComp
$Comp
L touch TC104
U 1 1 59FF1976
P 6750 1650
F 0 "TC104" H 6850 1550 40  0000 C CNN
F 1 "touch" H 6850 1750 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6750 1650 60  0001 C CNN
F 3 "" H 6750 1650 60  0001 C CNN
	1    6750 1650
	1    0    0    -1  
$EndComp
$Comp
L touch TC105
U 1 1 59FF197C
P 7050 1650
F 0 "TC105" H 7150 1550 40  0000 C CNN
F 1 "touch" H 7150 1750 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7050 1650 60  0001 C CNN
F 3 "" H 7050 1650 60  0001 C CNN
	1    7050 1650
	1    0    0    -1  
$EndComp
$Comp
L touch TC106
U 1 1 59FF1982
P 7350 1650
F 0 "TC106" H 7450 1550 40  0000 C CNN
F 1 "touch" H 7450 1750 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7350 1650 60  0001 C CNN
F 3 "" H 7350 1650 60  0001 C CNN
	1    7350 1650
	1    0    0    -1  
$EndComp
$Comp
L touch TC107
U 1 1 59FF1988
P 7650 1650
F 0 "TC107" H 7750 1550 40  0000 C CNN
F 1 "touch" H 7750 1750 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7650 1650 60  0001 C CNN
F 3 "" H 7650 1650 60  0001 C CNN
	1    7650 1650
	1    0    0    -1  
$EndComp
$Comp
L touch TC108
U 1 1 59FF198E
P 7950 1650
F 0 "TC108" H 8050 1550 40  0000 C CNN
F 1 "touch" H 8050 1750 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7950 1650 60  0001 C CNN
F 3 "" H 7950 1650 60  0001 C CNN
	1    7950 1650
	1    0    0    -1  
$EndComp
$Comp
L touch TC109
U 1 1 59FF1994
P 8250 1650
F 0 "TC109" H 8350 1550 40  0000 C CNN
F 1 "touch" H 8350 1750 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8250 1650 60  0001 C CNN
F 3 "" H 8250 1650 60  0001 C CNN
	1    8250 1650
	1    0    0    -1  
$EndComp
$Comp
L touch TC110
U 1 1 59FF199A
P 8550 1650
F 0 "TC110" H 8650 1550 40  0000 C CNN
F 1 "touch" H 8650 1750 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8550 1650 60  0001 C CNN
F 3 "" H 8550 1650 60  0001 C CNN
	1    8550 1650
	1    0    0    -1  
$EndComp
$Comp
L touch TC111
U 1 1 59FF1C0A
P 5550 1350
F 0 "TC111" H 5650 1250 40  0000 C CNN
F 1 "touch" H 5650 1450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5550 1350 60  0001 C CNN
F 3 "" H 5550 1350 60  0001 C CNN
	1    5550 1350
	1    0    0    -1  
$EndComp
$Comp
L touch TC112
U 1 1 59FF1C10
P 5850 1350
F 0 "TC112" H 5950 1250 40  0000 C CNN
F 1 "touch" H 5950 1450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 5850 1350 60  0001 C CNN
F 3 "" H 5850 1350 60  0001 C CNN
	1    5850 1350
	1    0    0    -1  
$EndComp
$Comp
L touch TC113
U 1 1 59FF1C16
P 6150 1350
F 0 "TC113" H 6250 1250 40  0000 C CNN
F 1 "touch" H 6250 1450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6150 1350 60  0001 C CNN
F 3 "" H 6150 1350 60  0001 C CNN
	1    6150 1350
	1    0    0    -1  
$EndComp
$Comp
L touch TC114
U 1 1 59FF1C1C
P 6450 1350
F 0 "TC114" H 6550 1250 40  0000 C CNN
F 1 "touch" H 6550 1450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6450 1350 60  0001 C CNN
F 3 "" H 6450 1350 60  0001 C CNN
	1    6450 1350
	1    0    0    -1  
$EndComp
$Comp
L touch TC115
U 1 1 59FF1C22
P 6750 1350
F 0 "TC115" H 6850 1250 40  0000 C CNN
F 1 "touch" H 6850 1450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 6750 1350 60  0001 C CNN
F 3 "" H 6750 1350 60  0001 C CNN
	1    6750 1350
	1    0    0    -1  
$EndComp
$Comp
L touch TC116
U 1 1 59FF1C28
P 7050 1350
F 0 "TC116" H 7150 1250 40  0000 C CNN
F 1 "touch" H 7150 1450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7050 1350 60  0001 C CNN
F 3 "" H 7050 1350 60  0001 C CNN
	1    7050 1350
	1    0    0    -1  
$EndComp
$Comp
L touch TC117
U 1 1 59FF1C2E
P 7350 1350
F 0 "TC117" H 7450 1250 40  0000 C CNN
F 1 "touch" H 7450 1450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7350 1350 60  0001 C CNN
F 3 "" H 7350 1350 60  0001 C CNN
	1    7350 1350
	1    0    0    -1  
$EndComp
$Comp
L touch TC118
U 1 1 59FF1C34
P 7650 1350
F 0 "TC118" H 7750 1250 40  0000 C CNN
F 1 "touch" H 7750 1450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7650 1350 60  0001 C CNN
F 3 "" H 7650 1350 60  0001 C CNN
	1    7650 1350
	1    0    0    -1  
$EndComp
$Comp
L touch TC119
U 1 1 59FF1C3A
P 7950 1350
F 0 "TC119" H 8050 1250 40  0000 C CNN
F 1 "touch" H 8050 1450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 7950 1350 60  0001 C CNN
F 3 "" H 7950 1350 60  0001 C CNN
	1    7950 1350
	1    0    0    -1  
$EndComp
$Comp
L touch TC120
U 1 1 59FF1C40
P 8250 1350
F 0 "TC120" H 8350 1250 40  0000 C CNN
F 1 "touch" H 8350 1450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8250 1350 60  0001 C CNN
F 3 "" H 8250 1350 60  0001 C CNN
	1    8250 1350
	1    0    0    -1  
$EndComp
$Comp
L touch TC121
U 1 1 59FF1C46
P 8550 1350
F 0 "TC121" H 8650 1250 40  0000 C CNN
F 1 "touch" H 8650 1450 40  0000 C CNN
F 2 "open-nsynth-super:touch_cell" H 8550 1350 60  0001 C CNN
F 3 "" H 8550 1350 60  0001 C CNN
	1    8550 1350
	1    0    0    -1  
$EndComp
NoConn ~ 5550 1200
NoConn ~ 5850 1200
NoConn ~ 6150 1200
NoConn ~ 6450 1200
NoConn ~ 6750 1200
NoConn ~ 7050 1200
NoConn ~ 7350 1200
NoConn ~ 7650 1200
NoConn ~ 7950 1200
NoConn ~ 8250 1200
NoConn ~ 8550 1200
NoConn ~ 8700 1350
NoConn ~ 8700 1650
NoConn ~ 8700 1950
NoConn ~ 8700 2250
NoConn ~ 8700 2550
NoConn ~ 8700 2850
NoConn ~ 8700 3150
NoConn ~ 8700 3450
NoConn ~ 8700 3750
NoConn ~ 8700 4050
NoConn ~ 8700 4350
$Comp
L R R10
U 1 1 59FF7CCE
P 5550 4650
F 0 "R10" V 5630 4650 50  0000 C CNN
F 1 "10k" V 5550 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5480 4650 50  0001 C CNN
F 3 "" H 5550 4650 50  0001 C CNN
	1    5550 4650
	-1   0    0    1   
$EndComp
$Comp
L R R11
U 1 1 59FF826C
P 5850 4650
F 0 "R11" V 5930 4650 50  0000 C CNN
F 1 "10k" V 5850 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5780 4650 50  0001 C CNN
F 3 "" H 5850 4650 50  0001 C CNN
	1    5850 4650
	-1   0    0    1   
$EndComp
$Comp
L R R12
U 1 1 59FF83BA
P 6150 4650
F 0 "R12" V 6230 4650 50  0000 C CNN
F 1 "10k" V 6150 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 6080 4650 50  0001 C CNN
F 3 "" H 6150 4650 50  0001 C CNN
	1    6150 4650
	-1   0    0    1   
$EndComp
$Comp
L R R13
U 1 1 59FF86F4
P 6450 4650
F 0 "R13" V 6530 4650 50  0000 C CNN
F 1 "10k" V 6450 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 6380 4650 50  0001 C CNN
F 3 "" H 6450 4650 50  0001 C CNN
	1    6450 4650
	-1   0    0    1   
$EndComp
$Comp
L R R14
U 1 1 59FF883F
P 6750 4650
F 0 "R14" V 6830 4650 50  0000 C CNN
F 1 "10k" V 6750 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 6680 4650 50  0001 C CNN
F 3 "" H 6750 4650 50  0001 C CNN
	1    6750 4650
	-1   0    0    1   
$EndComp
$Comp
L R R15
U 1 1 59FF898C
P 7050 4650
F 0 "R15" V 7130 4650 50  0000 C CNN
F 1 "10k" V 7050 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 6980 4650 50  0001 C CNN
F 3 "" H 7050 4650 50  0001 C CNN
	1    7050 4650
	-1   0    0    1   
$EndComp
$Comp
L R R16
U 1 1 59FF8AD9
P 7350 4650
F 0 "R16" V 7430 4650 50  0000 C CNN
F 1 "10k" V 7350 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 7280 4650 50  0001 C CNN
F 3 "" H 7350 4650 50  0001 C CNN
	1    7350 4650
	-1   0    0    1   
$EndComp
$Comp
L R R17
U 1 1 59FF8C28
P 7650 4650
F 0 "R17" V 7730 4650 50  0000 C CNN
F 1 "10k" V 7650 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 7580 4650 50  0001 C CNN
F 3 "" H 7650 4650 50  0001 C CNN
	1    7650 4650
	-1   0    0    1   
$EndComp
$Comp
L R R18
U 1 1 59FF8D77
P 7950 4650
F 0 "R18" V 8030 4650 50  0000 C CNN
F 1 "10k" V 7950 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 7880 4650 50  0001 C CNN
F 3 "" H 7950 4650 50  0001 C CNN
	1    7950 4650
	-1   0    0    1   
$EndComp
$Comp
L R R19
U 1 1 59FF90BD
P 8250 4650
F 0 "R19" V 8330 4650 50  0000 C CNN
F 1 "10k" V 8250 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 8180 4650 50  0001 C CNN
F 3 "" H 8250 4650 50  0001 C CNN
	1    8250 4650
	-1   0    0    1   
$EndComp
$Comp
L R R20
U 1 1 59FF9216
P 8550 4650
F 0 "R20" V 8630 4650 50  0000 C CNN
F 1 "10k" V 8550 4650 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 8480 4650 50  0001 C CNN
F 3 "" H 8550 4650 50  0001 C CNN
	1    8550 4650
	-1   0    0    1   
$EndComp
$Comp
L R R21
U 1 1 59FF9695
P 5250 1350
F 0 "R21" V 5330 1350 50  0000 C CNN
F 1 "10k" V 5250 1350 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5180 1350 50  0001 C CNN
F 3 "" H 5250 1350 50  0001 C CNN
	1    5250 1350
	0    -1   -1   0   
$EndComp
$Comp
L R R22
U 1 1 59FF9B44
P 5250 1650
F 0 "R22" V 5330 1650 50  0000 C CNN
F 1 "10k" V 5250 1650 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5180 1650 50  0001 C CNN
F 3 "" H 5250 1650 50  0001 C CNN
	1    5250 1650
	0    -1   -1   0   
$EndComp
$Comp
L R R23
U 1 1 59FF9CAE
P 5250 1950
F 0 "R23" V 5330 1950 50  0000 C CNN
F 1 "10k" V 5250 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5180 1950 50  0001 C CNN
F 3 "" H 5250 1950 50  0001 C CNN
	1    5250 1950
	0    -1   -1   0   
$EndComp
$Comp
L R R24
U 1 1 59FF9E29
P 5250 2250
F 0 "R24" V 5330 2250 50  0000 C CNN
F 1 "10k" V 5250 2250 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5180 2250 50  0001 C CNN
F 3 "" H 5250 2250 50  0001 C CNN
	1    5250 2250
	0    -1   -1   0   
$EndComp
$Comp
L R R25
U 1 1 59FF9F90
P 5250 2550
F 0 "R25" V 5330 2550 50  0000 C CNN
F 1 "10k" V 5250 2550 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5180 2550 50  0001 C CNN
F 3 "" H 5250 2550 50  0001 C CNN
	1    5250 2550
	0    -1   -1   0   
$EndComp
$Comp
L R R26
U 1 1 59FFA0F1
P 5250 2850
F 0 "R26" V 5330 2850 50  0000 C CNN
F 1 "10k" V 5250 2850 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5180 2850 50  0001 C CNN
F 3 "" H 5250 2850 50  0001 C CNN
	1    5250 2850
	0    -1   -1   0   
$EndComp
$Comp
L R R27
U 1 1 59FFA44C
P 5250 3150
F 0 "R27" V 5330 3150 50  0000 C CNN
F 1 "10k" V 5250 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5180 3150 50  0001 C CNN
F 3 "" H 5250 3150 50  0001 C CNN
	1    5250 3150
	0    -1   -1   0   
$EndComp
$Comp
L R R28
U 1 1 59FFA5B5
P 5250 3450
F 0 "R28" V 5330 3450 50  0000 C CNN
F 1 "10k" V 5250 3450 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5180 3450 50  0001 C CNN
F 3 "" H 5250 3450 50  0001 C CNN
	1    5250 3450
	0    -1   -1   0   
$EndComp
$Comp
L R R29
U 1 1 59FFA71C
P 5250 3750
F 0 "R29" V 5330 3750 50  0000 C CNN
F 1 "10k" V 5250 3750 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5180 3750 50  0001 C CNN
F 3 "" H 5250 3750 50  0001 C CNN
	1    5250 3750
	0    -1   -1   0   
$EndComp
$Comp
L R R30
U 1 1 59FFA88B
P 5250 4050
F 0 "R30" V 5330 4050 50  0000 C CNN
F 1 "10k" V 5250 4050 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5180 4050 50  0001 C CNN
F 3 "" H 5250 4050 50  0001 C CNN
	1    5250 4050
	0    -1   -1   0   
$EndComp
$Comp
L R R31
U 1 1 59FFA9F8
P 5250 4350
F 0 "R31" V 5330 4350 50  0000 C CNN
F 1 "10k" V 5250 4350 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 5180 4350 50  0001 C CNN
F 3 "" H 5250 4350 50  0001 C CNN
	1    5250 4350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2950 6500 3100 6500
Wire Wire Line
	2450 6800 4150 6800
Wire Wire Line
	4150 6800 4150 6500
Wire Wire Line
	4150 6500 4100 6500
Wire Wire Line
	3100 6400 2450 6400
Connection ~ 3050 6500
Connection ~ 3050 6800
Wire Wire Line
	2950 4450 3100 4450
Wire Wire Line
	2450 4750 4150 4750
Wire Wire Line
	4150 4750 4150 4450
Wire Wire Line
	4150 4450 4100 4450
Wire Wire Line
	2450 4350 3100 4350
Wire Wire Line
	2450 4350 2450 4750
Connection ~ 3050 4450
Connection ~ 3050 4750
Wire Wire Line
	2450 6400 2450 6800
Wire Wire Line
	3100 6200 2950 6200
Wire Wire Line
	2950 4150 3100 4150
Wire Wire Line
	2950 6300 3100 6300
Wire Wire Line
	3100 6000 2900 6000
Wire Wire Line
	3100 3950 2900 3950
Wire Wire Line
	2950 4250 3100 4250
NoConn ~ 3100 5800
NoConn ~ 3100 3750
Wire Wire Line
	4100 4350 5100 4350
Wire Wire Line
	4100 4250 5000 4250
Wire Wire Line
	5000 4250 5000 4050
Wire Wire Line
	5000 4050 5100 4050
Wire Wire Line
	4100 4150 4900 4150
Wire Wire Line
	4900 4150 4900 3750
Wire Wire Line
	4900 3750 5100 3750
Wire Wire Line
	4100 4050 4800 4050
Wire Wire Line
	4800 4050 4800 3450
Wire Wire Line
	4800 3450 5100 3450
Wire Wire Line
	4100 3950 4700 3950
Wire Wire Line
	4700 3950 4700 3150
Wire Wire Line
	4700 3150 5100 3150
Wire Wire Line
	4100 3850 4600 3850
Wire Wire Line
	4600 3850 4600 2850
Wire Wire Line
	4600 2850 5100 2850
Wire Wire Line
	4100 3750 4500 3750
Wire Wire Line
	4500 3750 4500 2550
Wire Wire Line
	4500 2550 5100 2550
Wire Wire Line
	4100 3650 4400 3650
Wire Wire Line
	4400 3650 4400 2250
Wire Wire Line
	4400 2250 5100 2250
Wire Wire Line
	4100 3550 4300 3550
Wire Wire Line
	4300 3550 4300 1950
Wire Wire Line
	4300 1950 5100 1950
Wire Wire Line
	3100 3550 3000 3550
Wire Wire Line
	3000 3550 3000 1650
Wire Wire Line
	3000 1650 5100 1650
Wire Wire Line
	3100 3650 2900 3650
Wire Wire Line
	2900 3650 2900 1350
Wire Wire Line
	2900 1350 5100 1350
Wire Wire Line
	5550 4800 5550 5200
Wire Wire Line
	5550 5200 2900 5200
Wire Wire Line
	2900 5200 2900 5700
Wire Wire Line
	2900 5700 3100 5700
Wire Wire Line
	5850 4800 5850 5300
Wire Wire Line
	5850 5300 3000 5300
Wire Wire Line
	3000 5300 3000 5600
Wire Wire Line
	3000 5600 3100 5600
Wire Wire Line
	6150 4800 6150 5600
Wire Wire Line
	6150 5600 4100 5600
Wire Wire Line
	6450 4800 6450 5700
Wire Wire Line
	6450 5700 4100 5700
Wire Wire Line
	6750 4800 6750 5800
Wire Wire Line
	6750 5800 4100 5800
Wire Wire Line
	7050 4800 7050 5900
Wire Wire Line
	7050 5900 4100 5900
Wire Wire Line
	7350 4800 7350 6000
Wire Wire Line
	7350 6000 4100 6000
Wire Wire Line
	7650 4800 7650 6100
Wire Wire Line
	7650 6100 4100 6100
Wire Wire Line
	7950 4800 7950 6200
Wire Wire Line
	7950 6200 4100 6200
Wire Wire Line
	8250 4800 8250 6300
Wire Wire Line
	8250 6300 4100 6300
Wire Wire Line
	8550 4800 8550 6400
Wire Wire Line
	8550 6400 4100 6400
$EndSCHEMATC
