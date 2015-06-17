# L3GD20H
Sample Project to Demonstrate L3GD20H Gyroscope and ADXL355 Accelerometer
This project connects the L3GD20H and the ADXL355 to the Beaglebone Black. The progam running on the BBB calculates
the pitch and roll of the breadboard using data from both sensors combined with a complimentary filter. This data is 
sent over UART to an Arduino Pro Mini and displayed on a LCD characted display.
