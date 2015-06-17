/*
 * main.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: danny
 */
/*
 * EBBGPIO.cpp  Created on: 29 Apr 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone"
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European
 * Union Public License (the "EUPL") version 1.1 as published by the
 * European Union. Any use of this Software, other than as authorized
 * under this License is strictly prohibited (to the extent such use
 * is covered by a right of the copyright holder of this Software).
 *
 * This Software is provided under the License on an "AS IS" basis and
 * without warranties of any kind concerning the Software, including
 * without limitation merchantability, fitness for a particular purpose,
 * absence of defects or errors, accuracy, and non-infringement of
 * intellectual property rights other than copyright. This disclaimer
 * of warranty is an essential part of the License and a condition for
 * the grant of any rights to this Software.
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#include <iostream>
#include "L3GD20H.h"
#include "./Libraries/UartDevice.h"
#include "./Libraries/SPIDevice.h"
#include "ADXL345.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#define DT 0.02         // [s/loop] loop period. 20ms
#define AA 0.97         // complementary filter constant


#define A_GAIN 0.0573      // [deg/LSB]
#define G_GAIN 0.070     // [deg/s/LSB]
#define RAD_TO_DEG 57.29578
#define M_PI 3.14159265358979323846

using namespace std;
using namespace exploringBB;

int main() {

	cout << "Starting EBB ADXL345 SPI Test" << endl;
   SPIDevice *busDevice = new SPIDevice(1,0); // Using second SPI bus (both loaded)
   busDevice->setSpeed(1000000);              // Have access to SPI Device object
   ADXL345 acc(busDevice);

     UartDevice uart4(54);
   	uart4.openSerial();
   	//uart4.writeFloat(3.114);


   L3GD20H sensor(1,0x6b);
   sensor.setRange(L3GD20H::DPS_2000);

   short accX = 0;
   short accY = 0;
   short accZ = 0;

   short gyroX = 0;
   short gyroY = 0;
   short gyroZ = 0;

   float rate_gyr_y = 0.0;   // [deg/s]
   float rate_gyr_x = 0.0;    // [deg/s]
   float rate_gyr_z = 0.0;     // [deg/s]

   float gyroXangle = 0.0;
   float gyroYangle = 0.0;
   float gyroZangle = 0.0;

   float AccYangle = 0.0;
   float AccXangle = 0.0;
   float CFangleX = 0.0;
   float CFangleY = 0.0;
   int timer = 0;

   const float ROLL_OFFSET = -0.8;
   const float PITCH_OFFSET = -1.4;

   unsigned char buffer[6];

   while(1){

	   acc.readSensorState();
	   accX = acc.getAccelerationX();
	   accY = acc.getAccelerationY();
	   accZ = acc.getAccelerationZ();

	   sensor.readSensorState();
	   gyroX = sensor.getGyroX();
	   gyroY = sensor.getGyroY();
	   gyroZ = sensor.getGyroZ();



	   rate_gyr_x = (float) gyroX * 0.07;
	   rate_gyr_y = (float) gyroY * 0.07;
	   rate_gyr_z = (float) gyroZ * 0.07;

	   gyroXangle = rate_gyr_x * 0.02;
	   gyroYangle = rate_gyr_y * 0.02;
	   gyroZangle = rate_gyr_z * 0.02;

	   AccXangle = (float) (atan2(accY, accZ)+M_PI) * RAD_TO_DEG;
	   AccYangle = (float) (atan2(accZ, accX)+M_PI) * RAD_TO_DEG;

	   AccXangle -= (float)180.0;
	   	if (AccYangle > 90)
	   	        AccYangle -= (float)270;
	   	else
	   		AccYangle += (float)90;


	   	//Complementary filter used to combine the accelerometer and gyro values.
	   	CFangleX=(AA*(CFangleX+rate_gyr_x*DT) +(1 - AA) * AccXangle);
	   	CFangleY=(AA*(CFangleY+rate_gyr_y*DT) +(1 - AA) * AccYangle);


	  //cout << "Roll: "<< CFangleX + ROLL_OFFSET<< " Pitch: "<< CFangleY +PITCH_OFFSET<< endl;

	   if(timer > 50){
		   uart4.writeChar('q');
		   uart4.writeChar('!');

		   uart4.writeString("Roll:    ",8);
		   uart4.writeFloat(CFangleX+ROLL_OFFSET,4);
		   uart4.writeChar('@');

		   uart4.writeString("Pitch:   ",8);
		   uart4.writeFloat(CFangleY + PITCH_OFFSET,4);
		   uart4.writeChar('!');
		   timer = 0;
	   }

	   timer ++;

	   sleep(0.019);
   }

	uart4.closeSerial();

	return 0;
}
