# L3GD20H
##Sample Project to Demonstrate L3GD20H Gyroscope and ADXL355 Accelerometer

This project connects the L3GD20H and the ADXL355 to the Beaglebone Black. 

The progam running on the BBB calculates the pitch and roll of the breadboard using data from both sensors combined with a complimentary filter. This data is sent over UART to an Arduino Pro Mini and displayed on a LCD characted display.

##Connections:

###L3GD20H (I2C)   
Gyroscope Header |  Connect To
---------------- | -----------
* SCL | BBB P9_19 (I2C2_CLK)
* SDA | BBB P9_18 (I2C2_SDA)
* SA0 | NC
* CS  | NC
* DRDY | NC
* INT1 | NC
* GND | BBB P9_1 (DGND)
* 3VO | NC    
* Vin | BBB P9_3 (VDD_3.3V) 

###ADXL355 (SPI)
ADXL355 Header | Connect To
-------------- | ----------
* GND | BBB P9_2 (DGND)
* VCC | BBB P9_4 (VDD_3.3V)
* CS | BBB P9_17 (SPI0_CS0)
* INT1 | NC 
* INT2 | NC 
* SD0 | BBB P9_21 (SPI0_D0)
* SDA | BBB P9_18 (SPI0_D1)
* SCL | BBB P9_22 (SPI0_SCLK0)

###Ardunio Pro Mini
Arduino Header | Connect To
-------------- | ----------
* TX | BBB P9_11 (UART4_RX)
* RX | BBB P9_13 (UART4_TX)
* D4 | LCD RS Pin4
* D6 | LCD Enable Pin6
* D10 | LCD DB4 Pin 11
* D11 | LCD DB5 Pin 12
* D12 | LCD DB6 Pin 13
* D13 | LCD DB7 Pin 14
* GND | BBB P9_1 (DGND)
* 3.3V | BBB P9_3 (VDD_3.3V)

###LCD Character Display (Non Arduino Connections)
LCD Header | Connect To
---------- | ----------
* VSS | GND and 1 terminal of Potentiometer
* VDD | 5V
* V0  | 2 terminal of Potentiometer
* LED+ | 3.3V
* LED- | GND
