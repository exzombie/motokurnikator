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
LIBS:special
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
LIBS:motokurnikator
LIBS:motokurnikator-cache
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
L ATTINY84-P IC1
U 1 1 55212D8C
P 4050 3250
F 0 "IC1" H 3200 4000 40  0000 C CNN
F 1 "ATTINY84-P" H 4750 2500 40  0000 C CNN
F 2 "DIP14" H 4050 3050 35  0000 C CIN
F 3 "" H 4050 3250 60  0000 C CNN
	1    4050 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR5
U 1 1 55212DD3
P 3000 3850
F 0 "#PWR5" H 3000 3850 30  0001 C CNN
F 1 "GND" H 3000 3780 30  0001 C CNN
F 2 "" H 3000 3850 60  0000 C CNN
F 3 "" H 3000 3850 60  0000 C CNN
	1    3000 3850
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR4
U 1 1 55212DE7
P 3000 2650
F 0 "#PWR4" H 3000 2750 30  0001 C CNN
F 1 "VCC" H 3000 2750 30  0000 C CNN
F 2 "" H 3000 2650 60  0000 C CNN
F 3 "" H 3000 2650 60  0000 C CNN
	1    3000 2650
	0    -1   -1   0   
$EndComp
$Comp
L CONN_4 P1
U 1 1 55212E80
P 5450 3200
F 0 "P1" V 5400 3200 50  0000 C CNN
F 1 "MOTOR" V 5500 3200 50  0000 C CNN
F 2 "" H 5450 3200 60  0000 C CNN
F 3 "" H 5450 3200 60  0000 C CNN
	1    5450 3200
	1    0    0    -1  
$EndComp
$Comp
L POT RV1
U 1 1 55212F35
P 5250 2300
F 0 "RV1" H 5250 2200 50  0000 C CNN
F 1 "POT 10k" H 5250 2300 50  0000 C CNN
F 2 "" H 5250 2300 60  0000 C CNN
F 3 "" H 5250 2300 60  0000 C CNN
	1    5250 2300
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR6
U 1 1 55212F76
P 5000 2300
F 0 "#PWR6" H 5000 2300 30  0001 C CNN
F 1 "GND" H 5000 2230 30  0001 C CNN
F 2 "" H 5000 2300 60  0000 C CNN
F 3 "" H 5000 2300 60  0000 C CNN
	1    5000 2300
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR8
U 1 1 55212F96
P 5500 2300
F 0 "#PWR8" H 5500 2400 30  0001 C CNN
F 1 "VCC" H 5500 2400 30  0000 C CNN
F 2 "" H 5500 2300 60  0000 C CNN
F 3 "" H 5500 2300 60  0000 C CNN
	1    5500 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2450 5250 2650
Wire Wire Line
	5250 2650 5100 2650
$Comp
L R R2
U 1 1 55212FCB
P 6250 2300
F 0 "R2" V 6330 2300 40  0000 C CNN
F 1 "R" V 6257 2301 40  0000 C CNN
F 2 "" V 6180 2300 30  0000 C CNN
F 3 "" H 6250 2300 30  0000 C CNN
	1    6250 2300
	0    -1   -1   0   
$EndComp
$Comp
L PHOTO_R R3
U 1 1 55213320
P 5750 2300
F 0 "R3" V 5830 2300 40  0000 C CNN
F 1 "PHOTO_R" V 5757 2301 40  0000 C CNN
F 2 "" V 5675 2290 30  0000 C CNN
F 3 "" H 5750 2300 30  0000 C CNN
	1    5750 2300
	0    1    1    0   
$EndComp
$Comp
L GND #PWR10
U 1 1 55213508
P 6500 2300
F 0 "#PWR10" H 6500 2300 30  0001 C CNN
F 1 "GND" H 6500 2230 30  0001 C CNN
F 2 "" H 6500 2300 60  0000 C CNN
F 3 "" H 6500 2300 60  0000 C CNN
	1    6500 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5100 2750 6000 2750
Wire Wire Line
	6000 2750 6000 2300
$Comp
L R R1
U 1 1 5521358D
P 5100 4100
F 0 "R1" V 5180 4100 40  0000 C CNN
F 1 "10k" V 5107 4101 40  0000 C CNN
F 2 "" V 5030 4100 30  0000 C CNN
F 3 "" H 5100 4100 30  0000 C CNN
	1    5100 4100
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR7
U 1 1 552135C4
P 5100 4350
F 0 "#PWR7" H 5100 4450 30  0001 C CNN
F 1 "VCC" H 5100 4450 30  0000 C CNN
F 2 "" H 5100 4350 60  0000 C CNN
F 3 "" H 5100 4350 60  0000 C CNN
	1    5100 4350
	-1   0    0    1   
$EndComp
$Comp
L SW_PUSH SW1
U 1 1 552135D8
P 6400 2850
F 0 "SW1" H 6550 2960 50  0000 C CNN
F 1 "END_SW" H 6400 2770 50  0000 C CNN
F 2 "" H 6400 2850 60  0000 C CNN
F 3 "" H 6400 2850 60  0000 C CNN
	1    6400 2850
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH SW2
U 1 1 5521361E
P 6400 3100
F 0 "SW2" H 6550 3210 50  0000 C CNN
F 1 "END_SW" H 6400 3020 50  0000 C CNN
F 2 "" H 6400 3100 60  0000 C CNN
F 3 "" H 6400 3100 60  0000 C CNN
	1    6400 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 2850 6100 2850
Wire Wire Line
	5100 2950 6100 2950
Wire Wire Line
	6100 2950 6100 3100
$Comp
L GND #PWR11
U 1 1 552136A3
P 6700 2850
F 0 "#PWR11" H 6700 2850 30  0001 C CNN
F 1 "GND" H 6700 2780 30  0001 C CNN
F 2 "" H 6700 2850 60  0000 C CNN
F 3 "" H 6700 2850 60  0000 C CNN
	1    6700 2850
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR12
U 1 1 552136B7
P 6700 3100
F 0 "#PWR12" H 6700 3100 30  0001 C CNN
F 1 "GND" H 6700 3030 30  0001 C CNN
F 2 "" H 6700 3100 60  0000 C CNN
F 3 "" H 6700 3100 60  0000 C CNN
	1    6700 3100
	0    -1   -1   0   
$EndComp
$Comp
L SWITCH_INV_MSM SW3
U 1 1 55213748
P 6600 3550
F 0 "SW3" H 6401 3700 50  0000 C CNN
F 1 "MANUAL_OPEN" H 6400 3400 50  0000 C CNN
F 2 "" H 6600 3550 60  0000 C CNN
F 3 "" H 6600 3550 60  0000 C CNN
	1    6600 3550
	-1   0    0    1   
$EndComp
Wire Wire Line
	5100 3650 6100 3650
Wire Wire Line
	5100 3550 6100 3550
Wire Wire Line
	6100 3550 6100 3450
$Comp
L GND #PWR13
U 1 1 552137C9
P 7100 3550
F 0 "#PWR13" H 7100 3550 30  0001 C CNN
F 1 "GND" H 7100 3480 30  0001 C CNN
F 2 "" H 7100 3550 60  0000 C CNN
F 3 "" H 7100 3550 60  0000 C CNN
	1    7100 3550
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR2
U 1 1 55214022
P 2350 2400
F 0 "#PWR2" H 2350 2400 30  0001 C CNN
F 1 "GND" H 2350 2330 30  0001 C CNN
F 2 "" H 2350 2400 60  0000 C CNN
F 3 "" H 2350 2400 60  0000 C CNN
	1    2350 2400
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG1
U 1 1 5521409E
P 1950 1750
F 0 "#FLG1" H 1950 1845 30  0001 C CNN
F 1 "PWR_FLAG" H 1950 1930 30  0000 C CNN
F 2 "" H 1950 1750 60  0000 C CNN
F 3 "" H 1950 1750 60  0000 C CNN
	1    1950 1750
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG2
U 1 1 552140B2
P 2350 2350
F 0 "#FLG2" H 2350 2445 30  0001 C CNN
F 1 "PWR_FLAG" H 2350 2530 30  0000 C CNN
F 2 "" H 2350 2350 60  0000 C CNN
F 3 "" H 2350 2350 60  0000 C CNN
	1    2350 2350
	0    -1   -1   0   
$EndComp
$Comp
L LED D1
U 1 1 553CC120
P 5300 3750
F 0 "D1" H 5200 3700 50  0000 C CNN
F 1 "LED" H 5300 3650 50  0000 C CNN
F 2 "" H 5300 3750 60  0000 C CNN
F 3 "" H 5300 3750 60  0000 C CNN
	1    5300 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR9
U 1 1 553CC16E
P 5500 3750
F 0 "#PWR9" H 5500 3750 30  0001 C CNN
F 1 "GND" H 5500 3680 30  0001 C CNN
F 2 "" H 5500 3750 60  0000 C CNN
F 3 "" H 5500 3750 60  0000 C CNN
	1    5500 3750
	0    -1   -1   0   
$EndComp
$Comp
L +12V #PWR1
U 1 1 553CC360
P 1950 1750
F 0 "#PWR1" H 1950 1700 20  0001 C CNN
F 1 "+12V" H 1950 1850 30  0000 C CNN
F 2 "" H 1950 1750 60  0000 C CNN
F 3 "" H 1950 1750 60  0000 C CNN
	1    1950 1750
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR3
U 1 1 553CC37F
P 2750 1750
F 0 "#PWR3" H 2750 1850 30  0001 C CNN
F 1 "VCC" H 2750 1850 30  0000 C CNN
F 2 "" H 2750 1750 60  0000 C CNN
F 3 "" H 2750 1750 60  0000 C CNN
	1    2750 1750
	0    1    1    0   
$EndComp
$Comp
L LM7805CT U1
U 1 1 553CC3CA
P 2350 1800
F 0 "U1" H 2150 2000 40  0000 C CNN
F 1 "LM7805CT" H 2350 2000 40  0000 L CNN
F 2 "TO-220" H 2350 1900 30  0000 C CIN
F 3 "" H 2350 1800 60  0000 C CNN
	1    2350 1800
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 553CC485
P 1950 1950
F 0 "C1" H 1950 2050 40  0000 L CNN
F 1 "0.33u" H 1956 1865 40  0000 L CNN
F 2 "" H 1988 1800 30  0000 C CNN
F 3 "" H 1950 1950 60  0000 C CNN
	1    1950 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 2050 2350 2400
Connection ~ 2350 2350
Wire Wire Line
	1950 2150 2750 2150
Connection ~ 2350 2150
$Comp
L C C2
U 1 1 553CC580
P 2750 1950
F 0 "C2" H 2750 2050 40  0000 L CNN
F 1 "0.1u" H 2756 1865 40  0000 L CNN
F 2 "" H 2788 1800 30  0000 C CNN
F 3 "" H 2750 1950 60  0000 C CNN
	1    2750 1950
	1    0    0    -1  
$EndComp
Text Notes 7350 7500 0    60   ~ 0
Motokurnikator: chicken coop automatic door controller
Text Notes 8150 7650 0    60   ~ 0
11 April 2015
Text Notes 10600 7650 0    60   ~ 0
2
$EndSCHEMATC