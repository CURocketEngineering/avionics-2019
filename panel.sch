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
LIBS:switches
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
LIBS:transf
LIBS:ttl_ieee
LIBS:video
LIBS:wiznet
LIBS:Worldsemi
LIBS:Xicor
LIBS:zetex
LIBS:Zilog
LIBS:panel-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Panel"
Date "2017-03-07"
Rev "1"
Comp "CU Rocket Engineering"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Screw_Terminal_1x07 J1
U 1 1 58BF4208
P 1700 900
F 0 "J1" H 1700 1650 50  0000 C TNN
F 1 "PanelTerm" V 1550 900 50  0000 C TNN
F 2 "Connect:DB9FC" H 1700 175 50  0001 C CNN
F 3 "" H 1675 1400 50  0001 C CNN
	1    1700 900 
	0    1    1    0   
$EndComp
$Comp
L SW_Push SW2
U 1 1 58BF4463
P 8850 1600
F 0 "SW2" H 8900 1700 50  0000 L CNN
F 1 "ResetButton" H 8850 1540 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH-12mm" H 8850 1800 50  0001 C CNN
F 3 "" H 8850 1800 50  0000 C CNN
	1    8850 1600
	1    0    0    -1  
$EndComp
$Comp
L SW_Push SW1
U 1 1 58BF46A2
P 8050 1600
F 0 "SW1" H 8100 1700 50  0000 L CNN
F 1 "ReadyButton" H 8050 1540 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH-12mm" H 8050 1800 50  0001 C CNN
F 3 "" H 8050 1800 50  0000 C CNN
	1    8050 1600
	1    0    0    -1  
$EndComp
$Comp
L LED_RCBG D1
U 1 1 58BF46E2
P 3500 1600
F 0 "D1" H 3500 1970 50  0000 C CNN
F 1 "Debug1" H 3500 1250 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm-4" H 3500 1550 50  0001 C CNN
F 3 "" H 3500 1550 50  0000 C CNN
	1    3500 1600
	0    1    1    0   
$EndComp
$Comp
L LED_RCBG D2
U 1 1 58BF478B
P 4400 1600
F 0 "D2" H 4400 1970 50  0000 C CNN
F 1 "Debug2" H 4400 1250 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm-4" H 4400 1550 50  0001 C CNN
F 3 "" H 4400 1550 50  0000 C CNN
	1    4400 1600
	0    1    1    0   
$EndComp
$Comp
L LED_RCBG D3
U 1 1 58BF47CA
P 5300 1600
F 0 "D3" H 5300 1970 50  0000 C CNN
F 1 "Debug3" H 5300 1250 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm-4" H 5300 1550 50  0001 C CNN
F 3 "" H 5300 1550 50  0000 C CNN
	1    5300 1600
	0    1    1    0   
$EndComp
$Comp
L LED_RCBG D4
U 1 1 58BF4814
P 6200 1600
F 0 "D4" H 6200 1970 50  0000 C CNN
F 1 "Debug4" H 6200 1250 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm-4" H 6200 1550 50  0001 C CNN
F 3 "" H 6200 1550 50  0000 C CNN
	1    6200 1600
	0    1    1    0   
$EndComp
$Comp
L LED_RCBG D5
U 1 1 58BF485D
P 7100 1600
F 0 "D5" H 7100 1970 50  0000 C CNN
F 1 "Debug5" H 7100 1250 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm-4" H 7100 1550 50  0001 C CNN
F 3 "" H 7100 1550 50  0000 C CNN
	1    7100 1600
	0    1    1    0   
$EndComp
$Comp
L 74HC595 U1
U 1 1 58BF490E
P 3750 3300
F 0 "U1" H 3900 3900 50  0000 C CNN
F 1 "74HC595" H 3750 2700 50  0000 C CNN
F 2 "Housings_DIP:DIP-16_W7.62mm" H 3750 3300 50  0001 C CNN
F 3 "" H 3750 3300 50  0000 C CNN
	1    3750 3300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2100 1100 2100 1500
Wire Wire Line
	2100 1400 8650 1400
Connection ~ 3500 1400
Connection ~ 4400 1400
Connection ~ 5300 1400
Connection ~ 6200 1400
Wire Wire Line
	7850 1400 7850 1600
Connection ~ 7100 1400
Wire Wire Line
	8650 1400 8650 1600
Connection ~ 7850 1400
$Comp
L 74HC595 U2
U 1 1 58BF4B51
P 5450 3300
F 0 "U2" H 5600 3900 50  0000 C CNN
F 1 "74HC595" H 5450 2700 50  0000 C CNN
F 2 "Housings_DIP:DIP-16_W7.62mm" H 5450 3300 50  0001 C CNN
F 3 "" H 5450 3300 50  0000 C CNN
	1    5450 3300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3300 2600 3300 1800
Wire Wire Line
	3500 1800 3400 1800
Wire Wire Line
	3400 1800 3400 2600
Wire Wire Line
	3700 1800 3700 1850
Wire Wire Line
	3700 1850 3500 1850
Wire Wire Line
	3500 1850 3500 2600
Wire Wire Line
	3600 2600 3600 1900
Wire Wire Line
	3600 1900 4200 1900
Wire Wire Line
	4200 1900 4200 1800
Wire Wire Line
	3700 2600 3700 1950
Wire Wire Line
	3700 1950 4400 1950
Wire Wire Line
	4400 1950 4400 1800
Wire Wire Line
	3800 2600 3800 2000
Wire Wire Line
	3900 2600 3900 2050
Wire Wire Line
	4000 2600 4000 2100
Wire Wire Line
	3800 2000 4600 2000
Wire Wire Line
	4600 2000 4600 1800
Wire Wire Line
	3900 2050 5100 2050
Wire Wire Line
	5100 2050 5100 1800
Wire Wire Line
	4000 2100 5300 2100
Wire Wire Line
	5300 2100 5300 1800
Wire Wire Line
	5000 2600 5000 2150
Wire Wire Line
	5000 2150 5500 2150
Wire Wire Line
	5500 2150 5500 1800
Wire Wire Line
	5100 2600 5100 2200
Wire Wire Line
	5100 2200 6000 2200
Wire Wire Line
	6000 2200 6000 1800
Wire Wire Line
	5200 2600 5200 2250
Wire Wire Line
	5200 2250 6200 2250
Wire Wire Line
	6200 2250 6200 1800
Wire Wire Line
	5300 2600 5300 2300
Wire Wire Line
	5300 2300 6400 2300
Wire Wire Line
	6400 2300 6400 1800
Wire Wire Line
	5400 2600 5400 2350
Wire Wire Line
	5400 2350 6900 2350
Wire Wire Line
	6900 2350 6900 1800
Wire Wire Line
	5500 2600 5500 2400
Wire Wire Line
	5500 2400 7100 2400
Wire Wire Line
	7100 2400 7100 1800
Wire Wire Line
	5600 2600 5600 2450
Wire Wire Line
	5600 2450 7300 2450
Wire Wire Line
	7300 2450 7300 1800
Wire Wire Line
	4200 2600 4700 2600
Wire Wire Line
	4700 2600 4700 4000
Wire Wire Line
	4700 4000 5000 4000
Wire Wire Line
	1500 4150 1500 1100
Wire Wire Line
	3300 4000 1900 4000
Wire Wire Line
	1900 4000 1900 1100
Wire Wire Line
	1700 4100 1700 1100
Wire Wire Line
	1300 1100 1300 4300
Wire Wire Line
	1300 4300 8250 4300
Wire Wire Line
	8250 4300 8250 1600
Wire Wire Line
	9050 1600 9050 4400
Wire Wire Line
	9050 4400 1100 4400
Wire Wire Line
	1100 4400 1100 1100
Text GLabel 2300 1100 3    60   Input ~ 0
VCC
Text GLabel 2100 1500 3    60   Input ~ 0
GND
Wire Wire Line
	1700 4100 5200 4100
Wire Wire Line
	3500 4100 3500 4000
Wire Wire Line
	5200 4100 5200 4000
Connection ~ 3500 4100
Wire Wire Line
	1500 4150 5500 4150
Wire Wire Line
	3800 4150 3800 4000
Wire Wire Line
	5500 4150 5500 4000
Connection ~ 3800 4150
Wire Wire Line
	3600 4000 3600 4550
Wire Wire Line
	5300 4550 5300 4000
Text GLabel 3600 4550 3    60   Input ~ 0
VCC
Text GLabel 5300 4550 3    60   Input ~ 0
VCC
Text GLabel 3900 4550 3    60   Input ~ 0
GND
Text GLabel 5600 4550 3    60   Input ~ 0
GND
Wire Wire Line
	3900 4550 3900 4000
Wire Wire Line
	5600 4550 5600 4000
NoConn ~ 5700 2600
NoConn ~ 5900 2600
Connection ~ 2100 1400
$EndSCHEMATC
