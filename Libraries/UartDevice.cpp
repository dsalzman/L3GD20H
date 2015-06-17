/*
 * BusDevice.cpp  Created on: 23 May 2014
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

#include "UartDevice.h"
#include <stdio.h>
#include <fcntl.h>
#include<unistd.h>
#include<termios.h>   // using the termios.h library



namespace exploringBB {

/**
 */
UartDevice::UartDevice(int tty) {
	this->file=-1;
	this->tty = tty;
}


int UartDevice::openSerial(){


		if ((this->file = open("/dev/ttyO4", O_RDWR | O_NOCTTY | O_NDELAY))<0){
				  perror("UART: Failed to open the file.\n");
				  return -1;
		}


	   struct termios options;               //The termios structure is vital
	   tcgetattr(this->file, &options);            //Sets the parameters associated with file

	   // Set up the communications options:
	   //   9600 baud, 8-bit, enable receiver, no modem control lines
	   options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
	   options.c_iflag = IGNPAR | ICRNL;    //ignore partity errors, CR -> newline
	   tcflush(this->file, TCIFLUSH);             //discard file information not transmitted
	   tcsetattr(this->file, TCSANOW, &options);  //changes occur immmediately

	   return 0;
}

int UartDevice::writeChar(unsigned char x){

	  // int count;

	   write(this->file, &x,1);      //send the Char

	   return 0;
}

int UartDevice::writeString(const char* x, unsigned char length){

	  // int count;
	   for(int i = 0; i < length; i++){
		   write(this->file, &x[i],1);      //send the Char
	   }


	   return 0;
}




int UartDevice::writeFloat(double x, char length){
	char buffer[18];
	sprintf(buffer,"%f",x);
    write(this->file, &buffer,length);
	return 0;
}

int UartDevice::closeSerial(){
	close(this->file);
	return 0;
}
/**
 * Destructor is unused
 */
UartDevice::~UartDevice() {}

} /* namespace exploringBB */
