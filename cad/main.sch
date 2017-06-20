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
LIBS:74xgxx
LIBS:ac-dc
LIBS:actel
LIBS:allegro
LIBS:Altera
LIBS:analog_devices
LIBS:battery_management
LIBS:bbd
LIBS:brooktre
LIBS:cmos_ieee
LIBS:dc-dc
LIBS:diode
LIBS:elec-unifil
LIBS:ESD_Protection
LIBS:ftdi
LIBS:gennum
LIBS:graphic
LIBS:hc11
LIBS:ir
LIBS:Lattice
LIBS:logo
LIBS:maxim
LIBS:mechanical
LIBS:microchip_dspic33dsc
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic32mcu
LIBS:motor_drivers
LIBS:motors
LIBS:msp430
LIBS:nordicsemi
LIBS:nxp_armmcu
LIBS:onsemi
LIBS:Oscillators
LIBS:powerint
LIBS:Power_Management
LIBS:pspice
LIBS:references
LIBS:relays
LIBS:rfcom
LIBS:sensors
LIBS:silabs
LIBS:stm8
LIBS:stm32
LIBS:supertex
LIBS:switches
LIBS:transf
LIBS:ttl_ieee
LIBS:video
LIBS:wiznet
LIBS:Worldsemi
LIBS:Xicor
LIBS:zetex
LIBS:Zilog
LIBS:main-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Main"
Date "2017-06-20"
Rev "4"
Comp "CU Rocket Engineering"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ATMEGA328P-P IC1
U 1 1 58B76AB6
P 2400 2200
F 0 "IC1" H 1650 3450 50  0000 L BNN
F 1 "AVR" H 2800 800 50  0000 L BNN
F 2 "Housings_DIP:DIP-28_W7.62mm_Socket" H 2400 2200 50  0001 C CIN
F 3 "" H 2400 2200 50  0000 C CNN
	1    2400 2200
	1    0    0    -1  
$EndComp
$Comp
L Crystal Y1
U 1 1 58B76B11
P 4050 1100
F 0 "Y1" H 4050 1250 50  0000 C CNN
F 1 "8 MHz Crystal" H 4500 1150 50  0000 C CNN
F 2 "Crystals:Crystal_HC49-U_Vertical" H 4050 1100 50  0001 C CNN
F 3 "" H 4050 1100 50  0000 C CNN
	1    4050 1100
	1    0    0    1   
$EndComp
$Comp
L C C4
U 1 1 58B76BBA
P 3900 850
F 0 "C4" H 3700 850 50  0000 L CNN
F 1 "27 pF" H 3650 950 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 3938 700 50  0001 C CNN
F 3 "" H 3900 850 50  0000 C CNN
	1    3900 850 
	1    0    0    1   
$EndComp
$Comp
L C C3
U 1 1 58B76C1D
P 4200 850
F 0 "C3" H 4000 850 50  0000 L CNN
F 1 "27 pF" H 3900 750 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 4238 700 50  0001 C CNN
F 3 "" H 4200 850 50  0000 C CNN
	1    4200 850 
	-1   0    0    -1  
$EndComp
$Comp
L LD1117S33TR U1
U 1 1 58B76DD1
P 900 2350
F 0 "U1" H 900 2650 50  0000 C CNN
F 1 "3.3V Reg" H 900 2550 50  0000 C CNN
F 2 "TO_SOT_Packages_THT:TO-220_Horizontal" H 900 2450 50  0001 C CNN
F 3 "" H 900 2350 50  0000 C CNN
	1    900  2350
	0    -1   -1   0   
$EndComp
$Comp
L CP C2
U 1 1 58B76EDA
P 1000 2800
F 0 "C2" H 1025 2900 50  0000 L CNN
F 1 "10 µF" H 1025 2700 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.00mm" H 1038 2650 50  0001 C CNN
F 3 "" H 1000 2800 50  0000 C CNN
	1    1000 2800
	0    -1   -1   0   
$EndComp
$Comp
L CP C1
U 1 1 58B76F81
P 1000 1900
F 0 "C1" H 1025 2000 50  0000 L CNN
F 1 "0.1 µF" H 1025 1800 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D3.8mm_W2.6mm_P2.50mm" H 1038 1750 50  0001 C CNN
F 3 "" H 1000 1900 50  0000 C CNN
	1    1000 1900
	0    -1   -1   0   
$EndComp
Text GLabel 850  3000 3    60   Input ~ 0
VBAT
$Comp
L IRLB8721PBF Q3
U 1 1 58BC9496
P 7850 1400
F 0 "Q3" V 8100 1500 50  0000 L CNN
F 1 "DrogueCtrl" V 8100 1000 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Horizontal" H 8050 1325 50  0001 L CIN
F 3 "" H 7850 1400 50  0000 L CNN
	1    7850 1400
	0    1    -1   0   
$EndComp
$Comp
L IRLB8721PBF Q2
U 1 1 58BC95BC
P 5950 2700
F 0 "Q2" V 6200 2900 50  0000 L CNN
F 1 "ArmCtrl" V 6200 2550 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Horizontal" H 6150 2625 50  0001 L CIN
F 3 "" H 5950 2700 50  0000 L CNN
	1    5950 2700
	0    1    -1   0   
$EndComp
$Comp
L IRLB8721PBF Q1
U 1 1 58BC989B
P 5950 1400
F 0 "Q1" V 6200 1500 50  0000 L CNN
F 1 "MainCtrl" V 6200 1100 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Horizontal" H 6150 1325 50  0001 L CIN
F 3 "" H 5950 1400 50  0000 L CNN
	1    5950 1400
	0    1    -1   0   
$EndComp
$Comp
L Screw_Terminal_1x02 J4
U 1 1 58BCAD2D
P 5150 700
F 0 "J4" H 5150 950 50  0000 C TNN
F 1 "MainTerm" V 5000 700 50  0000 C TNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_MKDS1.5-2pol" H 5150 475 50  0001 C CNN
F 3 "" H 5125 700 50  0001 C CNN
	1    5150 700 
	0    1    1    0   
$EndComp
$Comp
L Screw_Terminal_1x02 J5
U 1 1 58BCAEDA
P 7050 700
F 0 "J5" H 7050 950 50  0000 C TNN
F 1 "DrogueTerm" V 6900 700 50  0000 C TNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_MKDS1.5-2pol" H 7050 475 50  0001 C CNN
F 3 "" H 7025 700 50  0001 C CNN
	1    7050 700 
	0    1    1    0   
$EndComp
$Comp
L Screw_Terminal_1x02 J1
U 1 1 58BC987E
P 1250 7150
F 0 "J1" H 1250 7400 50  0000 C TNN
F 1 "CtrlTerm" V 1100 7150 50  0000 C TNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_MKDS1.5-2pol" H 1250 6925 50  0001 C CNN
F 3 "" H 1225 7150 50  0001 C CNN
	1    1250 7150
	0    -1   -1   0   
$EndComp
$Comp
L Screw_Terminal_1x02 J2
U 1 1 58BC9B79
P 2150 7150
F 0 "J2" H 2150 7400 50  0000 C TNN
F 1 "PayloadTerm" V 2000 7150 50  0000 C TNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_MKDS1.5-2pol" H 2150 6925 50  0001 C CNN
F 3 "" H 2125 7150 50  0001 C CNN
	1    2150 7150
	0    -1   -1   0   
$EndComp
$Comp
L Screw_Terminal_1x09 J3
U 1 1 58BC9C47
P 3550 7150
F 0 "J3" H 3550 8100 50  0000 C TNN
F 1 "PanelTerm" V 3400 7150 50  0000 C TNN
F 2 "Connect:DB9FC" H 3550 6725 50  0001 C CNN
F 3 "" H 3525 7350 50  0001 C CNN
	1    3550 7150
	0    -1   -1   0   
$EndComp
Text GLabel 5250 900  3    60   Input ~ 0
VBAT
Text GLabel 7150 900  3    60   Input ~ 0
VBAT
Text GLabel 2750 6950 1    60   Input ~ 0
VCC
Text GLabel 850  950  1    60   Input ~ 0
VCC
$Comp
L Screw_Terminal_1x02 J7
U 1 1 58BD4B0C
P 10800 700
F 0 "J7" H 10800 950 50  0000 C TNN
F 1 "BatteryTerm" V 10650 700 50  0000 C TNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_MKDS1.5-2pol" H 10800 475 50  0001 C CNN
F 3 "" H 10775 700 50  0001 C CNN
	1    10800 700 
	0    1    1    0   
$EndComp
Text GLabel 10900 1100 3    60   Input ~ 0
VBAT
Text GLabel 10700 1100 3    60   Input ~ 0
GND
Text GLabel 1200 2350 2    60   Input ~ 0
GND
Text GLabel 2950 6950 1    60   Input ~ 0
GND
Text GLabel 4500 700  2    60   Input ~ 0
GND
Text GLabel 6200 2600 2    60   Input ~ 0
GND
NoConn ~ 1500 1700
$Comp
L DIL6 P1
U 1 1 58BF7CD7
P 10400 2750
F 0 "P1" H 10400 3000 50  0000 C CNN
F 1 "ProgramHeader" H 10400 2500 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x03_Pitch2.54mm" H 10400 2750 50  0001 C CNN
F 3 "" H 10400 2750 50  0000 C CNN
	1    10400 2750
	1    0    0    -1  
$EndComp
Text GLabel 10050 2750 0    60   Input ~ 0
VCC
Text GLabel 10750 2650 2    60   Input ~ 0
GND
$Comp
L CONN_01X06 P2
U 1 1 58C02A3F
P 10700 3550
F 0 "P2" H 10700 3900 50  0000 C CNN
F 1 "Accelerometer" V 10800 3550 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 10700 3550 50  0001 C CNN
F 3 "" H 10700 3550 50  0000 C CNN
	1    10700 3550
	1    0    0    -1  
$EndComp
Text GLabel 10500 3300 0    60   Input ~ 0
VCC
Text GLabel 10500 3800 0    60   Input ~ 0
GND
$Comp
L CONN_01X07 P3
U 1 1 58C05810
P 10700 4500
F 0 "P3" H 10700 4900 50  0000 C CNN
F 1 "Barometer" V 10800 4500 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x07_Pitch2.54mm" H 10700 4500 50  0001 C CNN
F 3 "" H 10700 4500 50  0000 C CNN
	1    10700 4500
	1    0    0    -1  
$EndComp
Text GLabel 10500 4800 0    60   Input ~ 0
VCC
Text GLabel 10250 4700 0    60   Input ~ 0
GND
Text GLabel 10500 4200 0    60   Input ~ 0
VCC
Text GLabel 10250 4300 0    60   Input ~ 0
GND
$Comp
L Screw_Terminal_1x02 J6
U 1 1 58DD1C52
P 8950 700
F 0 "J6" H 8950 950 50  0000 C TNN
F 1 "IgniterTerm" V 8800 700 50  0000 C TNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_MKDS1.5-2pol" H 8950 475 50  0001 C CNN
F 3 "" H 8925 700 50  0001 C CNN
	1    8950 700 
	0    1    1    0   
$EndComp
$Comp
L IRLB8721PBF Q4
U 1 1 58DD1CDC
P 9650 1400
F 0 "Q4" V 9900 1500 50  0000 L CNN
F 1 "IgniterCtrl" V 9900 1000 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-220_Horizontal" H 9850 1325 50  0001 L CIN
F 3 "" H 9650 1400 50  0000 L CNN
	1    9650 1400
	0    1    -1   0   
$EndComp
Text GLabel 9050 900  3    60   Input ~ 0
VBAT
Connection ~ 8050 2000
Wire Wire Line
	9850 2000 9850 1300
Wire Wire Line
	8850 1300 8850 900 
Wire Wire Line
	9450 1300 8850 1300
Wire Wire Line
	9600 2250 9600 1600
Wire Wire Line
	3400 2250 9600 2250
Wire Wire Line
	3750 1100 3400 1100
Wire Wire Line
	3750 4400 3750 1100
Wire Wire Line
	10500 4400 3750 4400
Wire Wire Line
	3700 1200 3400 1200
Wire Wire Line
	3700 4500 3700 1200
Wire Wire Line
	10500 4500 3700 4500
Wire Wire Line
	10250 4300 10500 4300
Wire Wire Line
	10250 4700 10500 4700
Wire Wire Line
	5400 3500 10500 3500
Wire Wire Line
	5300 3600 10500 3600
Wire Wire Line
	5200 3700 10500 3700
Wire Wire Line
	4800 3050 11100 3050
Wire Wire Line
	11100 3050 11100 2750
Wire Wire Line
	4150 1550 4150 1400
Wire Wire Line
	11150 1550 4150 1550
Wire Wire Line
	11150 2850 11150 1550
Wire Wire Line
	10750 2850 11150 2850
Wire Wire Line
	4100 1500 3400 1500
Wire Wire Line
	4100 1900 4100 1500
Wire Wire Line
	10050 1900 4100 1900
Wire Wire Line
	10050 2650 10050 1900
Wire Wire Line
	8300 1600 3400 1600
Wire Wire Line
	8300 2850 8300 1600
Wire Wire Line
	10050 2850 8300 2850
Wire Wire Line
	11100 2750 10750 2750
Connection ~ 1150 3400
Wire Wire Line
	1300 1400 1300 1100
Wire Wire Line
	10700 900  10700 1100
Wire Wire Line
	10900 900  10900 1100
Wire Wire Line
	850  2750 850  3000
Wire Wire Line
	7800 1800 7800 1600
Wire Wire Line
	5900 1700 5900 1600
Wire Wire Line
	6200 2600 6150 2600
Connection ~ 850  2800
Connection ~ 850  1900
Connection ~ 1150 2800
Wire Wire Line
	1200 2350 1150 2350
Wire Wire Line
	1500 1400 1300 1400
Connection ~ 1300 1100
Connection ~ 850  1100
Connection ~ 1150 3300
Wire Wire Line
	1150 3300 1500 3300
Wire Wire Line
	1150 3400 1500 3400
Wire Wire Line
	4150 1400 3400 1400
Wire Wire Line
	1350 4500 1350 6950
Wire Wire Line
	3500 4500 1350 4500
Wire Wire Line
	1150 1900 1150 6950
Wire Wire Line
	2250 5000 2250 6950
Wire Wire Line
	2050 4850 2050 6950
Wire Wire Line
	5050 900  5050 1300
Wire Wire Line
	6950 1300 7650 1300
Wire Wire Line
	6950 900  6950 1300
Wire Wire Line
	5050 1300 5750 1300
Connection ~ 6150 2000
Wire Wire Line
	8050 2000 8050 1300
Wire Wire Line
	5750 2000 9850 2000
Wire Wire Line
	6150 1300 6150 2000
Wire Wire Line
	5750 2000 5750 2600
Wire Wire Line
	850  1100 1500 1100
Wire Wire Line
	850  950  850  1950
Connection ~ 1150 2350
Wire Wire Line
	3150 5200 3150 6950
Wire Wire Line
	3350 5300 3350 6950
Wire Wire Line
	3550 5400 3550 6950
$Comp
L R R3
U 1 1 59124B82
P 7950 2900
F 0 "R3" V 8030 2900 50  0000 C CNN
F 1 "10 kΩ" V 7800 2900 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7880 2900 50  0001 C CNN
F 3 "" H 7950 2900 50  0000 C CNN
	1    7950 2900
	1    0    0    -1  
$EndComp
Connection ~ 7950 3050
Text GLabel 7950 2750 1    60   Input ~ 0
VCC
Wire Wire Line
	5900 1700 5600 1700
Wire Wire Line
	5600 1700 5600 3200
Wire Wire Line
	7800 1800 6650 1800
Wire Wire Line
	6650 1800 6650 3300
Connection ~ 4200 700 
Wire Wire Line
	4200 1000 4200 1800
Wire Wire Line
	3900 1000 3900 1700
Wire Wire Line
	3900 700  4500 700 
Wire Wire Line
	4200 1800 3400 1800
Connection ~ 4200 1100
Wire Wire Line
	3900 1700 3400 1700
Connection ~ 3900 1100
Wire Wire Line
	3400 1950 5400 1950
Wire Wire Line
	5400 1950 5400 3500
Wire Wire Line
	3400 2350 5050 2350
Wire Wire Line
	5050 2350 5050 5000
Wire Wire Line
	5050 5000 2250 5000
Wire Wire Line
	2050 4850 4900 4850
Wire Wire Line
	4900 4850 4900 2450
Wire Wire Line
	4900 2450 3400 2450
Wire Wire Line
	5300 3600 5300 2050
Wire Wire Line
	5300 2050 3400 2050
Wire Wire Line
	5200 3700 5200 2150
Wire Wire Line
	5200 2150 3400 2150
Wire Wire Line
	4700 2900 4700 5400
Wire Wire Line
	4700 5400 3550 5400
Wire Wire Line
	4600 3000 4600 5300
Wire Wire Line
	4600 5300 3350 5300
Wire Wire Line
	4500 5200 3150 5200
Wire Wire Line
	3750 6950 3750 5650
Text GLabel 10250 3400 0    60   Input ~ 0
GND
Wire Wire Line
	10250 3400 10500 3400
Wire Wire Line
	10500 4600 3650 4600
Wire Wire Line
	3650 4600 3650 1300
Wire Wire Line
	3650 1300 3400 1300
Wire Wire Line
	3500 4500 3500 3400
Wire Wire Line
	3500 3400 3400 3400
Wire Wire Line
	6650 3300 3400 3300
Wire Wire Line
	5600 3200 3400 3200
Wire Wire Line
	4500 3100 3400 3100
Wire Wire Line
	4500 5200 4500 3100
Wire Wire Line
	4600 3000 3400 3000
Wire Wire Line
	4700 2900 3400 2900
$Comp
L CONN_01X02 J8
U 1 1 593F4A48
P 9700 4000
F 0 "J8" H 9700 4150 50  0000 C CNN
F 1 "DebugHeader" V 9800 4000 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 9700 4000 50  0001 C CNN
F 3 "" H 9700 4000 50  0001 C CNN
	1    9700 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 2700 4350 2700
Wire Wire Line
	4350 2700 4350 3950
Wire Wire Line
	4350 3950 9500 3950
Wire Wire Line
	3400 2800 4250 2800
Wire Wire Line
	4250 2800 4250 4050
Wire Wire Line
	4250 4050 9500 4050
Wire Wire Line
	5900 2900 5900 5650
Wire Wire Line
	5900 5650 3750 5650
Wire Wire Line
	3400 2550 4800 2550
Wire Wire Line
	4800 2550 4800 3050
NoConn ~ 4350 6950
NoConn ~ 4150 6950
NoConn ~ 3950 6950
$Comp
L SW_Push SW1
U 1 1 59495DD8
P 7400 2850
F 0 "SW1" H 7350 3050 50  0000 L CNN
F 1 "Reset" H 7400 2750 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_Tactile_SPST_Angled_PTS645Vx39-2LFS" H 7400 3050 50  0001 C CNN
F 3 "" H 7400 3050 50  0001 C CNN
	1    7400 2850
	0    1    1    0   
$EndComp
Text GLabel 7400 2650 1    60   Input ~ 0
GND
Connection ~ 7400 3050
$EndSCHEMATC
