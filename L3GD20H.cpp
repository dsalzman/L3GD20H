/*
 * L3GD20H.cpp  Created on: 17 May 2014
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

#include "L3GD20H.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>

using namespace std;

namespace exploringBB {

double gyroXangle;
double gyroYangle;
double gyroZangle;


/**
 * Method to combine two 8-bit registers into a single short, which is 16-bits on the BBB. It shifts
 * the MSB 8-bits to the left and then ORs the result with the LSB.
 * @param msb an unsigned character that contains the most significant byte
 * @param lsb an unsigned character that contains the least significant byte
 */
short L3GD20H::combineRegisters(unsigned char msb, unsigned char lsb){
   //shift the MSB left by 8 bits and OR with LSB
   return ((short)msb<<8)|(short)lsb;
}

/**
 * Method to calculate the pitch and roll state values. This calculation takes account of the scaling
 * factors due to the resolution and gravity range to determine gravity weighted values that are used
 * to calculate the angular pitch and roll values in degrees.
 */
//void L3GD20H::calculatePitchAndRoll(){
//	float gravity_range;
////	switch(L3GD20H::range){
////		case L3GD20H::PLUSMINUS_16_G: gravity_range=32.0f; break;
////		case L3GD20H::PLUSMINUS_8_G: gravity_range=16.0f; break;
////		case L3GD20H::PLUSMINUS_4_G: gravity_range=8.0f; break;
////		default: gravity_range=4.0f; break;
////	}
//    float resolution = 1024.0f;
//    ///if (this->resolution==L3GD20H::HIGH) resolution = 8192.0f; //13-bit resolution
//    float factor = gravity_range/resolution;
//
//    float accXg = this->accelerationX * factor;
//    float accYg = this->accelerationY * factor;
//    float accZg = this->accelerationZ * factor;
//	float accXSquared = accXg * accXg ;
//	float accYSquared = accYg * accYg ;
//	float accZSquared = accZg * accZg ;
//	this->pitch = 180 * atan(accXg/sqrt(accYSquared + accZSquared))/M_PI;
//	this->roll = 180 * atan(accYg/sqrt(accXSquared + accZSquared))/M_PI;
//}

/**
 * Method used to update the DATA_FORMAT register and any other registers that might be added
 * in the future.
 * @return 0 if the register is updated successfully
 */
//int L3GD20H::updateRegisters(){
   //update the DATA_FORMAT register
 //  char data_format = 0x00;  //+/- 2g with normal resolution
   //Full_resolution is the 3rd LSB
 //  data_format = data_format|((this->resolution)<<3);
   //data_format = data_format|this->range; // 1st and 2nd LSB therefore no shift
 //  return this->writeRegister(DATA_FORMAT, data_format);
//}

/**
 * The constructor for the L3GD20H accelerometer object. It passes the bus number and the
 * device address (with is 0x6b by default) to the constructor of I2CDevice. All of the states
 * are initialized and the registers are updated.
 * @param I2CBus The bus number that the L3GD20H device is on - typically 0 or 1
 * @param I2CAddress The address of the L3GD20H device (default 0x6b, but can be altered)
 */
L3GD20H::L3GD20H(unsigned int I2CBus, unsigned int I2CAddress):
	I2CDevice(I2CBus, I2CAddress){   // this member initialisation list calls the parent constructor
	this->I2CAddress = I2CAddress;
	this->I2CBus = I2CBus;
	this->writeRegister(CTRL1, 0x0F);
	this->writeRegister(CTRL2, 0x00);
	this->writeRegister(CTRL3, 0x00);
	this->writeRegister(CTRL4, 0x00);
	this->writeRegister(CTRL5, 0x00);
}

/*
 * Get The Device ID
 * Default value is 0xd7
 */
int L3GD20H::getDeviceID(){
	return this->readRegister(DEVID);
}

/*
 * Tests Connection to L3GD20H by checking device id
 */
bool L3GD20H::testConnection(){
	return this->getDeviceID() == 0xd7;
}

// CTRL1 register, r/w

/** Set the output data rate.  Makes use of the setLowODREnabled function.
 * @param rate The new data output rate (can be 12, 25, 50, 100, 200, 400, or 800)
 * @see L3GD20H_RA_CTRL1
 * @see L3GD20H_ODR_BIT
 * @see L3GD20H_ODR_LENGTH
 * @see L3GD20H_RATE_100_12
 * @see L3GD20H_RATE_200_25
 * @see L3GD20H_RATE_400_50
 * @see L3GD20H_RATE_800_50
 * @see L3GD20H_RA_LOW_ODR
 * @see L3GD20H_LOW_ODR_BIT
 */
void L3GD20H::setOutputDataRate(unsigned int rate){
	unsigned int writeVal;
	bool lowODRwriteVal;

	if (rate ==12) {
			writeVal = L3GD20H_RATE_100_12;
			setLowODREnabled(true);
	} else if (rate == 25) {
			writeVal = L3GD20H_RATE_200_25;
			setLowODREnabled(true);
	} else if (rate == 50) {
			writeVal = L3GD20H_RATE_400_50;
			setLowODREnabled(true);
	} else if (rate == 100) {
			writeVal = L3GD20H_RATE_100_12;
			setLowODREnabled(false);
	} else if (rate == 200) {
			writeVal = L3GD20H_RATE_200_25;
			setLowODREnabled(false);
	} else if (rate == 400) {
			writeVal = L3GD20H_RATE_400_50;
			setLowODREnabled(false);
	} else {
			writeVal = L3GD20H_RATE_800_50;
			setLowODREnabled(false);
	}

	this->writeBits(CTRL1, L3GD20H_ODR_BIT,L3GD20H_ODR_LENGTH, writeVal);
}



/** Get the current output data rate
 * @return Current data output rate
 * @see L3GD20H_RA_CTRL1
 * @see L3GD20H_ODR_BIT
 * @see L3GD20H_ODR_LENGTH
 * @see L3GD20H_RATE_100_12
 * @see L3GD20H_RATE_200_25
 * @see L3GD20H_RATE_400_50
 * @see L3GD20H_RATE_800_50
 * @see L3GD20H_RA_LOW_ODR
 * @see L3GD20H_LOW_ODR_BIT
 */
unsigned short L3GD20H::getOutputDataRate() {
	temp= this->readBits(CTRL1, L3GD20H_ODR_BIT, L3GD20H_ODR_LENGTH);
	unsigned char rate = temp;

	if (rate == L3GD20H_RATE_100_12) {
		if (getLowODREnabled() == true) {
			return 12;
		} else {
			return 100;
		}
	} else if (rate == L3GD20H_RATE_200_25) {
		if (getLowODREnabled() == true) {
			return 25;
		} else {
			return 200;
		}
	} else if (rate == L3GD20H_RATE_400_50) {
		if (getLowODREnabled() == true) {
			return 50;
		} else {
			return 400;
		}
	} else if (rate == L3GD20H_RATE_800_50) {
		if (getLowODREnabled() == true) {
			return 50;
		} else {
			return 800;
		}
	}
	return 800;
}


/** Set the bandwidth cut-off mode
 * @param mode The new bandwidth cut-off mode
 * @see L3GD20H_RA_CTRL1
 * @see L3GD20H_BW_BIT
 * @see L3GD20H_BW_LENGTH
 * @see L3GD20H_BW_LOW
 * @see L3GD20H_BW_MED_LOW
 * @see L3GD20H_BW_MED_HIGH
 * @see L3GD20H_BW_HIGH
 */
void L3GD20H::setBandwidthCutOffMode(unsigned short mode) {
	this->writeBits(CTRL1, L3GD20H_BW_BIT, L3GD20H_BW_LENGTH, mode);
}


/** Get the current bandwidth cut-off mode
 * @return Current bandwidth cut off mode
 * @see L3GD20H_RA_CTRL1
 * @see L3GD20H_BW_BIT
 * @see L3GD20H_BW_LENGTH
 * @see L3GD20H_BW_LOW
 * @see L3GD20H_BW_MED_LOW
 * @see L3GD20H_BW_MED_HIGH
 * @see L3GD20H_BW_HIGH
 */
unsigned short L3GD20H::getBandwidthCutOffMode() {
	temp = this->readBits(CTRL1, L3GD20H_BW_BIT, L3GD20H_BW_LENGTH);
	return temp;
}


/** Set power on or off
 * @param enabled The new power setting (true for on, false for off)
 * @see L3GD20H_RA_CTRL1
 * @see L3GD20H_PD_BIT
 */
void L3GD20H::setPowerOn(bool on){
	this->writeRegister(CTRL1,this->readRegister(CTRL1)|0x80);
}



/** Get the current power state
 * @return Powered on state (true for on, false for off)
 * @see L3GD20H_RA_CTRL1
 * @see L3GD20H_PD_BIT
 */
bool L3GD20H::getPowerOn(){
	return this->readBit(CTRL1,3);
}

/** Enables or disables the ability to get Z data
 * @param enabled The new enabled state of the Z axis
 * @see L3GD20H_RA_CTRL1
 * @see L3GD20H_ZEN_BIT
 */
void L3GD20H::setZEnabled(bool enabled) {
	this->writeBit(CTRL1, L3GD20H_ZEN_BIT, enabled);
}

/** Get whether Z axis data is enabled
 * @return True if the Z axis is enabled, false otherwise
 * @see L3GD20H_RA_CTRL1
 * @see L3GD20H_ZEN_BIT
 */
bool L3GD20H::getZEnabled() {
	temp= this->readBit(CTRL1, L3GD20H_ZEN_BIT);
	return temp;
}

/** Enables or disables the ability to get Y data
 * @param enabled The new enabled state of the Y axis
 * @see L3GD20H_RA_CTRL1
 * @see L3GD20H_YEN_BIT
 */
void L3GD20H::setYEnabled(bool enabled) {
	this->writeBit(CTRL1, L3GD20H_YEN_BIT, enabled);
}

/** Get whether Y axis data is enabled
 * @return True if the Y axis is enabled, false otherwise
 * @see L3GD20H_RA_CTRL1
 * @see L3GD20H_YEN_BIT
 */
bool L3GD20H::getYEnabled() {
	temp= this->readBit(CTRL1, L3GD20H_YEN_BIT);
	return temp;
}

/** Enables or disables the ability to get X data
 * @param enabled The new enabled state of the X axis
 * @see L3GD20H_RA_CTRL1
 * @see L3GD20H_XEN_BIT
 */
void L3GD20H::setXEnabled(bool enabled) {
	this->writeBit(CTRL1, L3GD20H_XEN_BIT, enabled);
}

/** Get whether X axis data is enabled
 * @return True if the X axis is enabled, false otherwise
 * @see L3GD20H_RA_CTRL1
 * @see L3GD20H_XEN_BIT
 */
bool L3GD20H::getXEnabled() {
	temp= this->readBit(CTRL1, L3GD20H_XEN_BIT);
	return temp;
}


// CTRL2 register, r/w

/** Set the high pass mode
 * @param mode The new high pass mode
 * @see L3GD20H_RA_CTRL2
 * @see L3GD20H_HPM_BIT
 * @see L3GD20H_HPM_LENGTH
 * @see L3GD20H_HPM_HRF
 * @see L3GD20H_HPM_REFERENCE
 * @see L3GD20H_HPM_NORMAL
 * @see L3GD20H_HPM_AUTORESET
 */
void L3GD20H::setHighPassMode(unsigned char mode) {
	this->writeBits(CTRL2, L3GD20H_HPM_BIT, L3GD20H_HPM_LENGTH, mode);
}

/** Get the high pass mode
 * @return High pass mode
 * @see L3GD20H_RA_CTRL2
 * @see L3GD20H_HPM_BIT
 * @see L3GD20H_HPM_LENGTH
 * @see L3GD20H_HPM_HRF
 * @see L3GD20H_HPM_REFERENCE
 * @see L3GD20H_HPM_NORMAL
 * @see L3GD20H_HPM_AUTORESET
 */
unsigned char L3GD20H::getHighPassMode() {
	temp= this->readBits(CTRL2, L3GD20H_HPM_BIT, L3GD20H_HPM_LENGTH);
	return temp;
}

/** Set the high pass filter cut off frequency level (1 - 10)
 * @param level The new level for the hpcf, using one of the defined levels
 * @see L3GD20H_RA_CTRL2
 * @see L3GD20H_HPCF_BIT
 * @see L3GD20H_HPCF_LENGTH
 * @see L3GD20H_HPCF1
 * @see L3GD20H_HPCF2
 * @see L3GD20H_HPCF3
 * @see L3GD20H_HPCF4
 * @see L3GD20H_HPCF5
 * @see L3GD20H_HPCF6
 * @see L3GD20H_HPCF7
 * @see L3GD20H_HPCF8
 * @see L3GD20H_HPCF9
 * @see L3GD20H_HPCF10
 */
void L3GD20H::setHighPassFilterCutOffFrequencyLevel(unsigned char level) {
	this->writeBits(CTRL2, L3GD20H_HPCF_BIT, L3GD20H_HPCF_LENGTH, level);
}

/** Get the high pass filter cut off frequency level (1 - 10)
 * @return High pass filter cut off frequency level
 * @see L3GD20H_RA_CTRL2
 * @see L3GD20H_HPCF_BIT
 * @see L3GD20H_HPCF_LENGTH
 * @see L3GD20H_HPCF1
 * @see L3GD20H_HPCF2
 * @see L3GD20H_HPCF3
 * @see L3GD20H_HPCF4
 * @see L3GD20H_HPCF5
 * @see L3GD20H_HPCF6
 * @see L3GD20H_HPCF7
 * @see L3GD20H_HPCF8
 * @see L3GD20H_HPCF9
 * @see L3GD20H_HPCF10
 */
unsigned char L3GD20H::getHighPassFilterCutOffFrequencyLevel() {
	temp= this->readBits(CTRL2, L3GD20H_HPCF_BIT,L3GD20H_HPCF_LENGTH);
	return temp;
}


// CTRL3 register, r/w

/** Set the INT1 interrupt enabled state
 * @param enabled New enabled state for the INT1 interrupt
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_INT1_IG_BIT
 */
void L3GD20H::setINT1InterruptEnabled(bool enabled) {
	this->writeBit(CTRL3, L3GD20H_INT1_IG_BIT, enabled);
}

/** Get the INT1 interrupt enabled state
 * @return True if the INT1 interrupt is enabled, false otherwise
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_INT1_IG_BIT
 */
bool L3GD20H::getINT1InterruptEnabled() {
	temp= this->readBit(CTRL3, L3GD20H_INT1_IG_BIT);
	return temp;
}

/** Set the INT1 boot status enabled state
 * @param enabled New enabled state for the INT1 boot status
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_INT1_BOOT_BIT
 */
void L3GD20H::setINT1BootStatusEnabled(bool enabled) {
	this->writeBit(CTRL3, L3GD20H_INT1_BOOT_BIT, enabled);
}

/** Get the INT1 boot status enabled state
 * @return INT1 boot status status
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_INT1_BOOT_BIT
 */
bool L3GD20H::getINT1BootStatusEnabled() {
	temp= this->readBit(CTRL3, L3GD20H_INT1_BOOT_BIT);
	return temp;
}

/** Interrupts the active INT1 configuration
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_H_LACTIVE_BIT
 */
void L3GD20H::interruptActiveINT1Config() {
	this->writeBit(CTRL3, L3GD20H_H_LACTIVE_BIT, 1);
}

/** Set output mode to push-pull or open-drain
 * @param mode New output mode
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_PP_OD_BIT
 * @see L3GD20H_PUSH_PULL
 * @see L3GD20H_OPEN_DRAIN
 */
void L3GD20H::setOutputMode(bool mode) {
	this->writeBit(CTRL3, L3GD20H_PP_OD_BIT,mode);
}

/** Get whether mode is push-pull or open drain
 * @return Output mode (TRUE for open-drain, FALSE for push-pull)
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_PP_OD_BIT
 * @see L3GD20H_PUSH_PULL
 * @see L3GD20H_OPEN_DRAIN
 */
bool L3GD20H::getOutputMode() {
	temp= this->readBit(CTRL3, L3GD20H_PP_OD_BIT);
	return temp;
}

/** Set data ready interrupt enabled state on INT2 pin
 * @param enabled New INT2 data ready interrupt enabled state
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_INT2_DRDY_BIT
 */
void L3GD20H::setINT2DataReadyEnabled(bool enabled) {
	this->writeBit(CTRL3, L3GD20H_INT2_DRDY_BIT, enabled);
}

/** Get whether the data ready interrupt is enabled on the INT2 pin
 * @return True if the INT2 data ready interrupt is enabled, false otherwise
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_INT2_DRDY_BIT
 */
bool L3GD20H::getINT2DataReadyEnabled() {
	temp= this->readBit(CTRL3, L3GD20H_INT2_DRDY_BIT);
	return temp;
}

/** Set whether the INT2 FIFO threshold (watermark) interrupt is enabled
 * The sensor contains a 32-slot FIFO buffer for storing data so that it may be
 * read later. If enabled, the sensor will generate an interrupt on the
 * INT2/DRDY pin when the threshold has been reached. The threshold can be
 * configured through the setFIFOWatermark function.
 * @param enabled New enabled state of the INT2 FIFO threshold (watermark)
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_I2_WTM_BIT
 */
void L3GD20H::setINT2FIFOWatermarkInterruptEnabled(bool enabled) {
	this->writeBit(CTRL3, L3GD20H_INT2_FTH_BIT, enabled);
}

/** Get the INT2 FIFO threshold (watermark) interrupt enabled state
 * @return true if the FIFO watermark is enabled, false otherwise
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_INT2_FTH_BIT
 */
bool L3GD20H::getINT2FIFOWatermarkInterruptEnabled() {
	temp= this->readBit(CTRL3, L3GD20H_INT2_FTH_BIT);
	return temp;
}

/** Set whether an interrupt is triggered on INT2 when the FIFO is overrun
 * @param enabled New FIFO overrun interrupt enabled state
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_INT2_ORUN_BIT
 */
void L3GD20H::setINT2FIFOOverrunInterruptEnabled(bool enabled) {
	this->writeBit(CTRL3, L3GD20H_INT2_ORUN_BIT, enabled);
}

/** Get whether an interrupt is triggered on INT2 when the FIFO is overrun
 * @return True if the INT2 FIFO overrun interrupt is enabled, false otherwise
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_INT2_ORUN_BIT
 */
bool L3GD20H::getINT2FIFOOverrunInterruptEnabled() {
	temp= this->readBit(CTRL3, L3GD20H_INT2_ORUN_BIT);
	return temp;
}

/** Set whether an interrupt is triggered on INT2 when the FIFO buffer is empty
 * @param enabled New INT2 FIFO empty interrupt state
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_INT2_EMPTY_BIT
 */
void L3GD20H::setINT2FIFOEmptyInterruptEnabled(bool enabled) {
	this->writeBit(CTRL3, L3GD20H_INT2_EMPTY_BIT, enabled);
}

/** Get whether the INT2 FIFO empty interrupt is enabled
 * @returns True if the INT2 FIFO empty interrupt is enabled, false otherwise
 * @see L3GD20H_RA_CTRL3
 * @see L3GD20H_INT2_EMPTY_BIT
 */
bool L3GD20H::getINT2FIFOEmptyInterruptEnabled() {
	temp= this->readBit(CTRL3, L3GD20H_INT2_EMPTY_BIT);
	return temp;
}


// CTRL4 register, r/w

/** Set the Block Data Update (BDU) enabled state
 * @param enabled New BDU enabled state
 * @see L3GD20H_RA_CTRL4
 * @see L3GD20H_BDU_BIT
 */
void L3GD20H::setBlockDataUpdateEnabled(bool enabled) {
	this->writeBit(CTRL4, L3GD20H_BDU_BIT, enabled);
}

/** Get the BDU enabled state
 * @return True if Block Data Update is enabled, false otherwise
 * @see L3GD20H_RA_CTRL4
 * @see L3GD20H_BDU_BIT
 */
bool L3GD20H::getBlockDataUpdateEnabled() {
	temp= this->readBit(CTRL4, L3GD20H_BDU_BIT);
	return temp;
}

/** Set the data endian modes
 * In Big Endian mode, the Most Significat Byte (MSB) is on the lower address,
 * and the Least Significant Byte (LSB) is on the higher address. Little Endian
 * mode reverses this order. Little Endian is the default mode.
 * @param endianness New endian mode
 * @see L3GD20H_RA_CTRL4
 * @see L3GD20H_BLE_BIT
 * @see L3GD20H_BIG_ENDIAN
 * @see L3GD20H_LITTLE_ENDIAN
 */
void L3GD20H::setEndianMode(bool endianness) {
	this->writeBit(CTRL4, L3GD20H_BLE_BIT,endianness);
	endianMode = getEndianMode();
}

/** Get the data endian mode
 * @return Current endian mode
 * @see L3GD20H_RA_CTRL4
 * @see L3GD20H_BLE_BIT
 * @see L3GD20H_BIG_ENDIAN
 * @see L3GD20H_LITTLE_ENDIAN
 */
bool L3GD20H::getEndianMode() {
	temp= this->readBit(CTRL4, L3GD20H_BLE_BIT);
	return temp;
}

/** Set the full scale of the data output (in dps)
 * @param scale The new scale of the data output (250 [actual 245], 500, 2000)
 * @see L3GD20H_RA_CTRL4
 * @see L3GD20H_FS_BIT
 * @see L3GD20H_FS_LENGTH
 * @see L3GD20H_FS_250
 * @see L3GD20H_FS_500
 * @see L3GD20H_FS_2000
 */
void L3GD20H::setFullScale(unsigned int scale) {
	unsigned char writeBits;

	if (scale == 250) {
		writeBits = L3GD20H_FS_250;
	} else if (scale == 500) {
		writeBits = L3GD20H_FS_500;
	} else {
		writeBits = L3GD20H_FS_2000;
	}

	this->writeBits(CTRL4, L3GD20H_FS_BIT, L3GD20H_FS_LENGTH, writeBits);
}

/** Get the current full scale of the output data (in dps)
 * @return Current scale of the output data
 * @see L3GD20H_RA_CTRL4
 * @see L3GD20H_FS_BIT
 * @see L3GD20H_FS_LENGTH
 * @see L3GD20H_FS_250
 * @see L3GD20H_FS_500
 * @see L3GD20H_FS_2000
 */
unsigned int L3GD20H::getFullScale() {
	temp= this->readBits(CTRL4,L3GD20H_FS_BIT, L3GD20H_FS_LENGTH);
	unsigned char readBits = temp;

	if (readBits == L3GD20H_FS_250) {
		return 250;
	} else if (readBits == L3GD20H_FS_500) {
		return 500;
	} else {
		return 2000;
	}
}

//TODO
//Implement
//L3GD20H::setLevelSensitiveLatchedEnabled() and
//L3GD20H::getLevelSensitiveLatchedEnabled()
//

/** Set the self test mode
 * @param mode New self test mode (Normal, 0, 1)
 * @see L3GD20H_RA_CTRL4
 * @see L3GD20H_ST_BIT
 * @see L3GD20H_ST_LENGTH
 * @see L3GD20H_SELF_TEST_NORMAL
 * @see L3GD20H_SELF_TEST_0
 * @see L3GD20H_SELF_TEST_1
 */
void L3GD20H::setSelfTestMode(unsigned char mode) {
	this->writeBits(CTRL4, L3GD20H_ST_BIT, L3GD20H_ST_LENGTH, mode);
}

/** Get the current self test mode
 * @return Current self test mode
 * @see L3GD20H_RA_CTRL4
 * @see L3GD20H_ST_BIT
 * @see L3GD20H_ST_LENGTH
 * @see L3GD20H_SELF_TEST_NORMAL
 * @see L3GD20H_SELF_TEST_0
 * @see L3GD20H_SELF_TEST_1
 */
unsigned char L3GD20H::getSelfTestMode() {
	temp= readBits(CTRL4, L3GD20H_ST_BIT, L3GD20H_ST_LENGTH);
	return temp;
}

/** Set the SPI mode
 * @param mode New SPI mode
 * @see L3GD20H_RA_CTRL4
 * @see L3GD20H_SIM_BIT
 * @see L3GD20H_SPI_4_WIRE
 * @see L3GD20H_SPI_3_WIRE
 */
void L3GD20H::setSPIMode(bool mode) {
	this->writeBit(CTRL4, L3GD20H_SIM_BIT, mode);
}

/** Get the SPI mode
 * @return Current SPI mode
 * @see L3GD20H_RA_CTRL4
 * @see L3GD20H_SIM_BIT
 * @see L3GD20H_SPI_4_WIRE
 * @see L3GD20H_SPI_3_WIRE
 */
bool L3GD20H::getSPIMode() {
	temp= this->readBit(CTRL4, L3GD20H_SIM_BIT);
 	return temp;
}


// CTRL5 register, r/w

/** Reboots the FIFO memory content
 * @see L3GD20H_RA_CTRL5
 * @see L3GD20H_BOOT_BIT
 */
void L3GD20H::rebootMemoryContent() {
	this->writeBit(CTRL5, L3GD20H_BOOT_BIT, true);
}

/** Set whether the FIFO buffer is enabled
 * @param enabled New enabled state of the FIFO buffer
 * @see L3GD20H_RA_CTRL5
 * @see L3GD20H_FIFO_EN_BIT
 */
void L3GD20H::setFIFOEnabled(bool enabled) {
	this->writeBit(CTRL5, L3GD20H_FIFO_EN_BIT, enabled);
}

/** Get whether the FIFO buffer is enabled
 * @return True if the FIFO buffer is enabled, false otherwise
 * @see L3GD20H_RA_CTRL5
 * @see L3GD20H_FIFO_EN_BIT
 */
bool L3GD20H::getFIFOEnabled() {
	temp= this->readBit(CTRL5, L3GD20H_FIFO_EN_BIT);
	return temp;
}

/** Set whether the sensing chain FIFO stops writing new values once
 * the FIFO Threshold (watermark) is reached
 * @param enabled New state of the StopOnFTH bit
 * @see L3GD20H_RA_CTRL5
 * @see L3GD20H_FIFO_STOPONFTH_BIT
 */
void L3GD20H::setStopOnFIFOThresholdEnabled(bool enabled) {
	this->writeBit(CTRL5, L3GD20H_STOPONFTH_BIT, enabled);
}

/** Get whether the sensing chain FIFO stopping writing new values once
 * the FIFO Threshold (watermark) is enabled
 * @return True if the state of the StopOnFTH bit is high (enabled)
 * @see L3GD20H_RA_CTRL5
 * @see L3GD20H_FIFO_STOPONFTH_BIT
 */
bool L3GD20H::getStopOnFIFOThresholdEnabled() {
	temp= this->readBit(CTRL5, L3GD20H_STOPONFTH_BIT);
	return temp;
}


/** Set the high pass filter enabled state
 * @param enabled New high pass filter enabled state
 * @see L3GD20H_RA_CTRL5
 * @see L3GD20H_HPEN_BIT
 */
void L3GD20H::setHighPassFilterEnabled(bool enabled) {
	this->writeBit(CTRL5, L3GD20H_HPEN_BIT, enabled);
}

/** Get whether the high pass filter is enabled
 * @return True if the high pass filter is enabled, false otherwise
 * @see L3GD20H_RA_CTRL5
 * @see L3GD20H_HPEN_BIT
 */
bool L3GD20H::getHighPassFilterEnabled() {
	temp= this->readBit(CTRL5, L3GD20H_HPEN_BIT);
	return temp;
}

/** Sets the filter mode to one of the four provided.
 * This function also uses the setHighPassFilterEnabled function in order to set
 * the mode. That function does not have to be called in addition to this one.
 * In addition to setting the filter for the data in the FIFO buffer
 * (controlled by the bits written to OUT_SEL), this function also sets the
 * filter used for interrupt generation (the bits written to IG_SEL) to be the
 * same as the filter used for the FIFO buffer.  The filter used for interrupt
 * generation can be set separately with the setInterruptFilter function.
 * @param filter New method to be used when filtering data
 * @see L3GD20H_RA_CTRL5
 * @see L3GD20H_IG_SEL_BIT
 * @see L3GD20H_IG_SEL_LENGTH
 * @see L3GD20H_OUT_SEL_BIT
 * @see L3GD20H_OUT_SEL_LENGTH
 * @see L3GD20H_NON_HIGH_PASS
 * @see L3GD20H_HIGH_PASS
 * @see L3GD20H_LOW_PASS
 * @see L3GD20H_LOW_HIGH_PASS
 */
void L3GD20H::setDataFilter(unsigned char filter) {
	if (filter == L3GD20H_HIGH_PASS || filter == L3GD20H_LOW_HIGH_PASS) {
		setHighPassFilterEnabled(true);
	} else {
		setHighPassFilterEnabled(false);
	}

	this->writeBits(CTRL5, L3GD20H_OUT_SEL_BIT, L3GD20H_OUT_SEL_LENGTH, filter);
	this->writeBits(CTRL5, L3GD20H_IG_SEL_BIT, L3GD20H_IG_SEL_LENGTH, filter);
}

/** Gets the data filter currently in use
 * @return Defined value that represents the filter in use
 * @see L3GD20H_RA_CTRL5
 * @see L3GD20H_OUT_SEL_BIT
 * @see L3GD20H_OUT_SEL_LENGTH
 * @see L3GD20H_NON_HIGH_PASS
 * @see L3GD20H_HIGH_PASS
 * @see L3GD20H_LOW_PASS
 * @see L3GD20H_LOW_HIGH_PASS
 */
unsigned char L3GD20H::getDataFilter() {
	temp= this->readBits(CTRL5, L3GD20H_OUT_SEL_BIT,L3GD20H_OUT_SEL_LENGTH);
	unsigned char outBits = temp;

	if (outBits == L3GD20H_NON_HIGH_PASS || outBits == L3GD20H_HIGH_PASS) {
		return outBits;
	}

	if (getHighPassFilterEnabled()) {
		return L3GD20H_LOW_HIGH_PASS;
	} else {
		return L3GD20H_LOW_PASS;
	}
}

// REFERENCE register, r/w

/** Set the reference value for the high pass filter
 * @param reference New 8-bit digital high pass filter reference value
 * @see L3GD20H_RA_REFERENCE
 */
void L3GD20H::setHighPassFilterReference(unsigned char reference) {
	this->writeRegister(REFERENCE, reference);
}

/** Get the 8-bit reference value for the high pass filter
 * @return 8-bit reference value for the high pass filter
 * @see L3GD20H_RA_REFERENCE
 */
unsigned char L3GD20H::getHighPassFilterReference() {
	temp= this->readRegister(REFERENCE);
	return temp;
}


// OUT_TEMP register, read-only

/** Gets the current temperature reading from the sensor
 * @return Current temperature
 * @see L3GD20H_RA_OUT_TEMP
 */
unsigned char L3GD20H::getTemperature() {
	temp= this->readRegister(OUT_TEMP);
	return temp;
}


// STATUS register, read-only

/** Get whether new data overwrote the last set of data before it was read
 * @return True if the last set of data was overwritten before being read, false
 * otherwise
 * @see L3GD20H_RA_STATUS
 * @see L3GD20H_ZYXOR_BIT
 */
bool L3GD20H::getXYZOverrun() {
	temp= this->readBit(STATUS, L3GD20H_ZYXOR_BIT);
	return temp;
}

/** Get whether new Z data overwrote the last set of data before it was read
 * @return True if the last set of Z data was overwritten before being read,
 * false otherwise
 * @see L3GD20H_RA_STATUS
 * @see L3GD20H_ZOR_BIT
 */
bool L3GD20H::getZOverrun() {
	temp= this->readBit(STATUS, L3GD20H_ZOR_BIT);
	return temp;
}

/** Get whether new Y data overwrote the last set of data before it was read
 * @return True if the last set of Y data was overwritten before being read,
 * false otherwise
 * @see L3GD20H_RA_STATUS
 * @see L3GD20H_YOR_BIT
 */
bool L3GD20H::getYOverrun() {
	temp= this->readBit(STATUS, L3GD20H_YOR_BIT);
	return temp;
}

/** Get whether new X data overwrote the last set of data before it was read
 * @return True if the last set of X data was overwritten before being read,
 * false otherwise
 * @see L3GD20H_RA_STATUS
 * @see L3GD20H_XOR_BIT
 */
bool L3GD20H::getXOverrun() {
	temp= this->readBit(STATUS, L3GD20H_XOR_BIT);
	return temp;
}

/** Get whether there is new data avaialable
 * @return True if there is new data available, false otherwise
 * @see L3GD20H_RA_STATUS
 * @see L3GD20H_ZYXDA_BIT
 */
bool L3GD20H::getXYZDataAvailable() {
	temp= this->readBit(STATUS, L3GD20H_ZYXDA_BIT);
	return temp;
}

/** Get whether there is new Z data avaialable
 * @return True if there is new Z data available, false otherwise
 * @see L3GD20H_RA_STATUS
 * @see L3GD20H_ZDA_BIT
 */
bool L3GD20H::getZDataAvailable() {
	temp= this->readBit(STATUS, L3GD20H_ZDA_BIT);
	return temp;
}

/** Get whether there is new Y data avaialable
 * @return True if there is new Y data available, false otherwise
 * @see L3GD20H_RA_STATUS
 * @see L3GD20H_YDA_BIT
 */
bool L3GD20H::getYDataAvailable() {
	temp= this->readBit(STATUS, L3GD20H_YDA_BIT);
	return temp;
}

/** Get whether there is new X data avaialable
 * @return True if there is new X data available, false otherwise
 * @see L3GD20H_RA_STATUS
 * @see L3GD20H_XDA_BIT
 */
bool L3GD20H::getXDataAvailable() {
	temp= this->readBit(STATUS, L3GD20H_XDA_BIT);
	return temp;
}

// OUT_* registers, read-only

/** Get the angular velocity for all 3 axes
 * Due to the fact that this device supports two difference Endian modes, both
 * must be accounted for when reading data. In Little Endian mode, the first
 * byte (lowest address) is the least significant and in Big Endian mode the
 * first byte is the most significant.
 * @param x 16-bit integer container for the X-axis angular velocity
 * @param y 16-bit integer container for the Y-axis angular velocity
 * @param z 16-bit integer container for the Z-axis angular velocity
 */
//void L3GD20H::getAngularVelocity(unsigned int* x, unsigned int* y, unsigned int* z) {
//	buffer = this->readBytes(DATAX0 | 0x80, 6);
//	if (endianMode == L3GD20H_LITTLE_ENDIAN) {
//		*x = (((unsigned int)buffer[1]) << 8) | temp;
//		*y = (((unsigned int)buffer[3]) << 8) | buffer[2];
//		*z = (((unsigned int)buffer[5]) << 8) | buffer[4];
//	} else {
//		*x = (((unsigned int))temp) << 8) | buffer[1];
//		*y = (((unsigned int)buffer[2]) << 8) | buffer[3];
//		*z = (((unsigned int))buffer[4]) << 8) | buffer[5];
//	}
//}

/** Get the angular velocity about the X-axis
 * @return Angular velocity about the X-axis
 * @see L3GD20H_RA_OUT_X_L
 * @see L3GD20H_RA_OUT_X_H
 */
int L3GD20H::getAngularVelocityX() {
	temp = this->readRegister(DATAX0);
	buffer[1] = this->readRegister(DATAX1);
	if (endianMode == L3GD20H_LITTLE_ENDIAN) {
		return (((int)buffer[1]) << 8) | temp;
	} else {
		return (((int)temp) << 8) | buffer[1];
	}
}

/** Get the angular velocity about the Y-axis
 * @return Angular velocity about the Y-axis
 * @see L3GD20H_RA_OUT_Y_L
 * @see L3GD20H_RA_OUT_Y_H
 */
int L3GD20H::getAngularVelocityY() {
	temp = this->readRegister(DATAY0);
	buffer[1] = this->readRegister(DATAY1);

	if (endianMode == L3GD20H_LITTLE_ENDIAN) {
		return (((int)buffer[1]) << 8) | temp;
	} else {
		return (((int)temp) << 8) | buffer[1];
	}
}

/** Get the angular velocity about the Z-axis
 * @return Angular velocity about the Z-axis
 * @see L3GD20H_RA_OUT_Z_L
 * @see L3GD20H_RA_OUT_Z_H
 */
int L3GD20H::getAngularVelocityZ() {
	temp = this->readRegister(DATAZ0);
	buffer[1] = this->readRegister(DATAZ1);

	if (endianMode == L3GD20H_LITTLE_ENDIAN) {
		return (((int)buffer[1]) << 8) | temp;
	} else {
		return (((int)temp) << 8) | buffer[1];
	}
}


// FIFO_CTRL register, r/w

/** Set the FIFO mode to one of the defined modes
 * @param mode New FIFO mode
 * @see L3GD20H_RA_FIFO_CTRL
 * @see L3GD20H_FIFO_MODE_BIT
 * @see L3GD20H_FIFO_MODE_LENGTH
 * @see L3GD20H_FM_BYPASS
 * @see L3GD20H_FM_FIFO
 * @see L3GD20H_FM_STREAM
 * @see L3GD20H_FM_STREAM_FIFO
 * @see L3GD20H_FM_BYPASS_STREAM
 * @see L3GD20H_FM_DYNAMIC_STREAM
 * @see L3GD20H_FM_BYPASS_FIFO
 */
void L3GD20H::setFIFOMode(unsigned char mode) {
	this->writeBits(FIFO_CTRL, L3GD20H_FIFO_MODE_BIT, L3GD20H_FIFO_MODE_LENGTH, mode);
}

/** Get the FIFO mode to one of the defined modes
 * @return Current FIFO mode
 * @see L3GD20H_RA_FIFO_CTRL
 * @see L3GD20H_FIFO_MODE_BIT
 * @see L3GD20H_FIFO_MODE_LENGTH
 * @see L3GD20H_FM_BYPASS
 * @see L3GD20H_FM_FIFO
 * @see L3GD20H_FM_STREAM
 * @see L3GD20H_FM_STREAM_FIFO
 * @see L3GD20H_FM_BYPASS_STREAM
 * @see L3GD20H_FM_DYNAMIC_STREAM
 * @see L3GD20H_FM_BYPASS_FIFO
 */
unsigned char L3GD20H::getFIFOMode() {
	temp= this->readBits(FIFO_CTRL, L3GD20H_FIFO_MODE_BIT, L3GD20H_FIFO_MODE_LENGTH);
	return temp;
}

/** Set the 5-bit FIFO (watermark) threshold
 * @param fth New 5-bit FIFO (watermark) threshold
 * @see L3GD20H_RA_FIFO_CTRL
 * @see L3GD20H_FIFO_TH_BIT
 * @see L3GD20H_FIFO_TH_LENGTH
 */
void L3GD20H::setFIFOThreshold(unsigned char fth) {
    this->writeBits(FIFO_CTRL, L3GD20H_FIFO_TH_BIT,L3GD20H_FIFO_TH_LENGTH, fth);
}

/** Get the FIFO watermark threshold
 * @return FIFO watermark threshold
 * @see L3GD20H_RA_FIFO_CTRL
 * @see L3GD20H_FIFO_TH_BIT
 * @see L3GD20H_FIFO_TH_LENGTH
 */
unsigned char L3GD20H::getFIFOThreshold() {
    temp= this->readBits(FIFO_CTRL, L3GD20H_FIFO_TH_BIT,L3GD20H_FIFO_TH_LENGTH);
    return temp;
}


/** Get whether the number of data sets in the FIFO buffer is less than the
 * watermark
 * @return True if the number of data sets in the FIFO buffer is more than or
 * equal to the watermark, false otherwise.
 * @see L3GD20H_RA_FIFO_SRC
 * @see L3GD20H_FIFO_TH_STATUS_BIT
 */
bool L3GD20H::getFIFOAtWatermark() {
   	temp= this->readBit(FIFO_SRC, L3GD20H_FIFO_TH_STATUS_BIT);
   	return temp;
}

/** Get whether the FIFO buffer is full
 * @return True if the FIFO buffer is full, false otherwise
 * @see L3GD20H_RA_FIFO_SRC
 * @see L3GD20H_FIFO_OVRN_BIT
 */
bool L3GD20H::getFIFOOverrun() {
    temp= this->readBit(FIFO_SRC, L3GD20H_OVRN_BIT);
    return temp;
}

/** Get whether the FIFO buffer is empty
 * @return True if the FIFO buffer is empty, false otherwise
 * @see L3GD20H_RA_FIFO_SRC
 * @see L3GD20H_FIFO_EMPTY_BIT
 */
bool L3GD20H::getFIFOEmpty() {
    temp= this->readBit(FIFO_SRC,L3GD20H_EMPTY_BIT);
    return temp;
}

/** Get the number of filled FIFO buffer slots
 * @return Number of filled slots in the FIFO buffer
 * @see L3GD20H_RA_FIFO_SRC
 * @see L3GD20H_FIFO_FSS_BIT
 * @see L3GD20H_FIFO_FSS_LENGTH
 */
unsigned char L3GD20H::getFIFOStoredDataLevel() {
    temp= this->readBits(FIFO_SRC,L3GD20H_FIFO_FSS_BIT, L3GD20H_FIFO_FSS_LENGTH);
    return temp;
}

// IG_CFG register, r/w

/** Set the combination mode for interrupt events
 * @param combination New combination mode for interrupt events.
 * L3GD20H_AND_OR_OR for OR and L3GD20H_AND_OR_AND for AND
 * @see L3GD20H_RA_IG_CFG
 * @see L3GD20H_AND_OR_BIT
 * @see L3GD20H_AND_OR_OR
 * @see L3GD20H_AND_OR_AND
 */
void L3GD20H::setInterruptCombination(bool combination) {
    this->writeBit(IG_CFG, L3GD20H_AND_OR_BIT, combination);
}

/** Get the combination mode for interrupt events
 * @return Combination mode for interrupt events. L3GD20H_AND_OR_OR for OR and
 * L3GD20H_AND_OR_AND for AND
 * @see L3GD20H_RA_IG_CFG
 * @see L3GD20H_AND_OR_BIT
 * @see L3GD20H_AND_OR_OR
 * @see L3GD20H_AND_OR_AND
 */
bool L3GD20H::getInterruptCombination() {
    temp= this->readBit(IG_CFG, L3GD20H_AND_OR_BIT);
    return temp;
}

/** Set whether an interrupt request is latched
 * This bit is cleared when the IG_SRC register is read
 * @param latched New status of the latched request
 * @see L3GD20H_RA_IG_CFG
 * @see L3GD20H_LIR_BIT
 */
void L3GD20H::setInterruptRequestLatched(bool latched) {
    this->writeBit(IG_CFG, L3GD20H_LIR_BIT, latched);
}

/** Get whether an interrupt request is latched
 * @return True if an interrupt request is latched, false otherwise
 * @see L3GD20H_RA_IG_CFG
 * @see L3GD20H_LIR_BIT
 */
bool L3GD20H::getInterruptRequestLatched() {
    temp= this->readBit(IG_CFG, L3GD20H_LIR_BIT);
    return temp;
};

/** Set whether the interrupt for Z high is enabled
 * @param enabled New enabled state for Z high interrupt.
 * @see L3GD20H_IG_CFG
 * @see L3GD20H_ZHIE_BIT
 */
void L3GD20H::setZHighInterruptEnabled(bool enabled) {
    this->writeBit(IG_CFG, L3GD20H_ZHIE_BIT, enabled);
}

/** Get whether the interrupt for Z high is enabled
 * @return True if the interrupt for Z high is enabled, false otherwise
 * @see L3GD20H_IG_CFG
 * @see L3GD20H_ZHIE_BIT
 */
bool L3GD20H::getZHighInterruptEnabled() {
    temp= this->readBit(IG_CFG, L3GD20H_ZHIE_BIT);
    return temp;
}

/** Set whether the interrupt for Z low is enabled
 * @param enabled New enabled state for Z low interrupt.
 * @see L3GD20H_IG_CFG
 * @see L3GD20H_ZLIE_BIT
 */
void L3GD20H::setZLowInterruptEnabled(bool enabled) {
    this->writeBit(IG_CFG, L3GD20H_ZLIE_BIT, enabled);
}

/** Get whether the interrupt for Z low is enabled
 * @return True if the interrupt for Z low is enabled, false otherwise
 * @see L3GD20H_IG_CFG
 * @see L3GD20H_ZLIE_BIT
 */
bool L3GD20H::getZLowInterruptEnabled() {
    temp= this->readBit(IG_CFG, L3GD20H_ZLIE_BIT);
    return temp;
}

/** Set whether the interrupt for Y high is enabled
 * @param enabled New enabled state for Y high interrupt.
 * @see L3GD20H_IG_CFG
 * @see L3GD20H_YHIE_BIT
 */
void L3GD20H::setYHighInterruptEnabled(bool enabled) {
    this->writeBit(IG_CFG, L3GD20H_YHIE_BIT, enabled);
}

/** Get whether the interrupt for Y high is enabled
 * @return True if the interrupt for Y high is enabled, false otherwise
 * @see L3GD20H_IG_CFG
 * @see L3GD20H_YHIE_BIT
 */
bool L3GD20H::getYHighInterruptEnabled() {
    temp= readBit(IG_CFG, L3GD20H_YHIE_BIT);
    return temp;
}

/** Set whether the interrupt for Y low is enabled
 * @param enabled New enabled state for Y low interrupt.
 * @see L3GD20H_IG_CFG
 * @see L3GD20H_YLIE_BIT
 */
void L3GD20H::setYLowInterruptEnabled(bool enabled) {
    this->writeBit(IG_CFG, L3GD20H_YLIE_BIT, enabled);
}

/** Get whether the interrupt for Y low is enabled
 * @return True if the interrupt for Y low is enabled, false otherwise
 * @see L3GD20H_IG_CFG
 * @see L3GD20H_YLIE_BIT
 */
bool L3GD20H::getYLowInterruptEnabled() {
    temp= this->readBit(IG_CFG, L3GD20H_YLIE_BIT);
    return temp;
}

/** Set whether the interrupt for X high is enabled
 * @param enabled New enabled state for X high interrupt.
 * @see L3GD20H_IG_CFG
 * @see L3GD20H_XHIE_BIT
 */
void L3GD20H::setXHighInterruptEnabled(bool enabled) {
    this->writeBit(IG_CFG, L3GD20H_XHIE_BIT, enabled);
}

/** Get whether the interrupt for X high is enabled
 * @return True if the interrupt for X high is enabled, false otherwise
 * @see L3GD20H_IG_CFG
 * @see L3GD20H_XHIE_BIT
 */
bool L3GD20H::getXHighInterruptEnabled() {
    temp= this->readBit(IG_CFG, L3GD20H_XHIE_BIT);
    return temp;
}

/** Set whether the interrupt for X low is enabled
 * @param enabled New enabled state for X low interrupt.
 * @see L3GD20H_IG_CFG
 * @see L3GD20H_XLIE_BIT
 */
void L3GD20H::setXLowInterruptEnabled(bool enabled) {
    this->writeBit(IG_CFG, L3GD20H_XLIE_BIT, enabled);
}

/** Get whether the interrupt for X low is enabled
 * @return True if the interrupt for X low is enabled, false otherwise
 * @see L3GD20H_IG_CFG
 * @see L3GD20H_XLIE_BIT
 */
bool L3GD20H::getXLowInterruptEnabled() {
    temp= this->readBit(IG_CFG, L3GD20H_XLIE_BIT);
    return temp;
}

// IG_SRC register, read-only

/** Get whether an interrupt has been generated
 * @return True if one or more interrupts has been generated, false otherwise
 * @see L3GD20H_RA_IG_SRC
 * @see L3GD20H_IA_BIT
 */
bool L3GD20H::getInterruptActive() {
    temp= this->readBit(IG_SRC, L3GD20H_IA_BIT);
    return temp;
}

/** Get whether a Z high event has occurred
 * @return True if a Z high event has occurred, false otherwise
 * @see L3GD20H_RA_IG_SRC
 * @see L3GD20H_ZH_BIT
 */
bool L3GD20H::getZHigh() {
    temp= this->readBit(IG_SRC, L3GD20H_ZH_BIT);
    return temp;
}

/** Get whether a Z low event has occurred
 * @return True if a Z low event has occurred, false otherwise
 * @see L3GD20H_RA_IG_SRC
 * @see L3GD20H_ZL_BIT
 */
bool L3GD20H::getZLow() {
    temp= this->readBit(IG_SRC, L3GD20H_ZL_BIT);
    return temp;
}

/** Get whether a Y high event has occurred
 * @return True if a Y high event has occurred, false otherwise
 * @see L3GD20H_RA_IG_SRC
 * @see L3GD20H_YH_BIT
 */
bool L3GD20H::getYHigh() {
    temp= this->readBit(IG_SRC, L3GD20H_YH_BIT);
    return temp;
}

/** Get whether a Y low event has occurred
 * @return True if a Y low event has occurred, false otherwise
 * @see L3GD20H_RA_IG_SRC
 * @see L3GD20H_YL_BIT
 */
bool L3GD20H::getYLow() {
   	temp= this->readBit(IG_SRC, L3GD20H_YL_BIT);
    return temp;
}

/** Get whether a X high event has occurred
 * @return True if a X high event has occurred, false otherwise
 * @see L3GD20H_RA_IG_SRC
 * @see L3GD20H_XH_BIT
 */
bool L3GD20H::getXHigh() {
   temp= this->readBit(IG_SRC, L3GD20H_XH_BIT);
    return temp;
}

/** Get whether a X low event has occurred
 * @return True if a X low event has occurred, false otherwise
 * @see L3GD20H_RA_IG_SRC
 * @see L3GD20H_XL_BIT
 */
bool L3GD20H::getXLow() {
    temp= this->readBit(IG_SRC, L3GD20H_XL_BIT);
    return temp;
}

// IG_THS_* registers, r/w

/** Set the interrupt generation counter mode selection.
 * @param enabled New enabled state for X low interrupt.
 * @see L3GD20H_IG_THS_XH
 * @see L3GD20H_DCRM_BIT
 * @see L3GD20H_DCRM_RESET
 * @see L3GD20H_DCRM_DEC
 */
void L3GD20H::setDecrementMode(bool mode) {
    this->writeBit(IG_THS_XH, L3GD20H_DCRM_BIT, mode);
}

/** Get the interrupt generation counter mode selection.
 * @return Mode Interrupt generation counter mode
 * @see L3GD20H_IG_THS_XH
 * @see L3GD20H_DCRM_BIT
 * @see L3GD20H_DCRM_RESET
 * @see L3GD20H_DCRM_DEC
 */
bool L3GD20H::getDecrementMode() {
    temp= this->readBit(IG_THS_XH, L3GD20H_DCRM_BIT);
    return temp;
}

/** Set the 7-bit threshold for a high interrupt on the X axis
 * @param threshold New 7-bit threshold for a high interrupt on the X axis
 * @see L3GD20H_IG_THS_XH
 */
void L3GD20H::setXHighThreshold(unsigned char threshold) {
    this->writeRegister(IG_THS_XH, threshold);
}

/** Retrieve the threshold for a high interrupt on the X axis
 * @return X high interrupt threshold
 * @see L3GD20H_IG_THS_XH
 */
unsigned char L3GD20H::getXHighThreshold() {
	temp= this->readRegister(IG_THS_XH);
	return temp;
}

/** Set the 8-bit threshold for a low interrupt on the X axis
 * @param threshold New 8-bit threshold for a low interrupt on the X axis
 * @see L3GD20H_IG_THS_XL
 */
void L3GD20H::setXLowThreshold(unsigned char threshold) {
    this->writeRegister(IG_THS_XL, threshold);
}

/** Retrieve the threshold for a low interrupt on the X axis
 * @return X low interrupt threshold
 * @see L3GD20H_IG_THS_XL
 */
unsigned char L3GD20H::getXLowThreshold() {
	temp= this->readRegister(IG_THS_XL);
	return temp;
}

/** Set the 7-bit threshold for a high interrupt on the Y axis
 * @param threshold New 7-bit threshold for a high interrupt on the Y axis
 * @see L3GD20H_IG_THS_YH
 */
void L3GD20H::setYHighThreshold(unsigned char threshold) {
    this->writeRegister(IG_THS_YH, threshold);
}

/** Retrieve the threshold for a high interrupt on the Y axis
 * @return Y high interrupt threshold
 * @see L3GD20H_IG_THS_YH
 */
unsigned char L3GD20H::getYHighThreshold() {
	temp= this->readRegister(IG_THS_YH);
	return temp;
}

/** Set the 8-bit threshold for a low interrupt on the Y axis
 * @param threshold New 8-bit threshold for a low interrupt on the Y axis
 * @see L3GD20H_IG_THS_YL
 */
void L3GD20H::setYLowThreshold(unsigned char threshold) {
    this->writeRegister(IG_THS_YL, threshold);
}

/** Retrieve the threshold for a low interrupt on the Y axis
 * @return Y low interrupt threshold
 * @see L3GD20H_IG_THS_YL
 */
unsigned char L3GD20H::getYLowThreshold() {
	temp= readRegister(IG_THS_YL);
	return temp;
}

/** Set the 7-bit threshold for a high interrupt on the Z axis
 * @param threshold New 7-bit threshold for a high interrupt on the Z axis
 * @see L3GD20H_IG_THS_ZH
 */
void L3GD20H::setZHighThreshold(unsigned char threshold) {
    this->writeRegister(IG_THS_ZH, threshold);
}

/** Retrieve the threshold for a high interrupt on the Z axis
 * @return Z high interrupt threshold
 * @see L3GD20H_IG_THS_ZH
 */
unsigned char L3GD20H::getZHighThreshold() {
	temp= this->readRegister(IG_THS_ZH);
	return temp;
}

/** Set the 8-bit threshold for a low interrupt on the Z axis
 * @param threshold New 8-bit threshold for a low interrupt on the Z axis
 * @see L3GD20H_RA_IG_THS_ZL
 */
void L3GD20H::setZLowThreshold(unsigned char threshold) {
    this->writeRegister(IG_THS_ZL, threshold);
}

/** Retrieve the threshold for a low interrupt on the Z axis
 * @return Z low interrupt threshold
 * @see L3GD20H_IG_THS_ZL
 */
unsigned char L3GD20H::getZLowThreshold() {
	temp= this->readRegister(IG_THS_ZL);
	return temp;
}

// IG_DURATION register, r/w

/* Set the minimum duration for an interrupt event to be recognized
 * This depends on the chosen output data rate
 * @param duration New 7-bit duration value necessary for an interrupt event to be
 * recognized
 * @see L3GD20H_RA_IG_DURATION
 * @see L3GD20H_DUR_BIT
 * @see L3GD20H_DUR_LENGTH
 */
void L3GD20H::setDuration(unsigned char duration) {
	this->writeBits(IG_DURATION, L3GD20H_DUR_BIT,L3GD20H_DUR_LENGTH, duration);
}

/** Get the minimum duration for an interrupt event to be recognized
 * @return Duration value necessary for an interrupt event to be recognized
 * @see L3GD20H_RA_IG_DURATION
 * @see L3GD20H_DUR_BIT
 * @see L3GD20H_DUR_LENGTH
 */
unsigned char L3GD20H::getDuration() {
	temp = this->readBits(IG_DURATION,L3GD20H_DUR_BIT, L3GD20H_DUR_LENGTH);
	return temp;
}

/** Set whether the interrupt wait feature is enabled
 * If false, the interrupt falls immediately if signal crosses the selected
 * threshold. Otherwise, if signal crosses the selected threshold, the interrupt
 * falls only after the duration has counted number of samples at the selected
 * data rate, written into the duration counter register.
 * @param enabled New enabled state of the interrupt wait
 * @see L3GD20H_RA_IG_DURATION
 * @see L3GD20H_WAIT_BIT
 */
void L3GD20H::setWaitEnabled(bool enabled) {
	this->writeBit(IG_DURATION, L3GD20H_WAIT_BIT, enabled);
}

/** Get whether the interrupt wait feature is enabled
 * @return True if the wait feature is enabled, false otherwise
 * @see L3GD20H_RA_IG_DURATION
 * @see L3GD20H_WAIT_BIT
 */
bool L3GD20H::getWaitEnabled() {
	temp= this->readBit(IG_DURATION, L3GD20H_WAIT_BIT);
	return temp;
}

// LOW_ODR register, r/w

/** Set whether the DRDY/INT2 pin is active low. If enabled is true then the
 * DRDY/INT2 pin will be active low.
 * @param enabled New enabled DRDY/INT2 active low configuration
 * @see L3GD20H_RA_LOW_ODR
 * @see L3GD20H_DRDY_HL_BIT
 */
void L3GD20H::setINT2DataReadyActiveLowEnabled(bool enabled){
	this->writeBit(LOW_ODR, L3GD20H_DRDY_HL_BIT,enabled);
}

/** Get whether the DRDY/INT2 pin is active low. If true then the
 * DRDY/INT2 pin IS active low.
 * @see L3GD20H_RA_LOW_ODR
 * @see L3GD20H_I2C_DIS_BIT
 */
bool L3GD20H::getINT2DataReadyActiveLowEnabled(){
	temp= this->readBit(LOW_ODR, L3GD20H_DRDY_HL_BIT);
	return temp;
}

/** Set whether only the SPI interface is enabled (i.e., I2C interface disabled)
 * @param enabled New SPI interface only enabled
 * @see L3GD20H_RA_LOW_ODR
 * @see L3GD20H_I2C_DIS_BIT
 */
void L3GD20H::setSPIOnlyEnabled(bool enabled){
	this->writeBit(LOW_ODR, L3GD20H_I2C_DIS_BIT, enabled);
}

/** Get whether only the SPI interface is enabled (i.e., I2C interface disabled)
 * @see L3GD20H_RA_LOW_ODR
 * @see L3GD20H_I2C_DIS_BIT
 */
bool L3GD20H::getSPIOnlyEnabled(){
	temp= this->readBit(LOW_ODR, L3GD20H_I2C_DIS_BIT);
	return temp;
}




void L3GD20H::setLowODREnabled(bool enabled){
	this->writeBit(LOW_ODR, L3GD20H_LOW_ODR_BIT, enabled);

}

bool L3GD20H::getLowODREnabled(){
	temp= this->readBit(LOW_ODR, L3GD20H_LOW_ODR_BIT);
		return temp;

}

/**
 * Read the sensor state. This method checks that the device is being correctly
 * read by using the device ID of the L3GD20H sensor. It will read in the accelerometer registers
 * and pass them to the combineRegisters() method to be processed.
 * @return 0 if the registers are successfully read and -1 if the device ID is incorrect.
 */
int L3GD20H::readSensorState(){
	this-> gyroX = this -> combineRegisters(this -> readRegister(DATAX1),this -> readRegister(DATAX0));
	this-> gyroY = this -> combineRegisters(this -> readRegister(DATAY1),this -> readRegister(DATAY0));
	this-> gyroZ = this -> combineRegisters(this -> readRegister(DATAZ1),this -> readRegister(DATAZ0));

	//cout << "X = " << x *0.07 << " Y =  " << y*0.07 << " Z = " << z*0.07 << endl;

//	gyroXangle+= x*0.07*.02;
//	gyroYangle+= y*0.07*.02;
//	gyroZangle+= z*0.07*.02;

	//cout << "X = " << gyroXangle << " Y =  " << gyroYangle << " Z = " << gyroZangle << endl;
	return 0;
}





void L3GD20H::setRange(L3GD20H::RANGE range) {
	this->range = range;
    this->writeRegister(CTRL4,(unsigned char)(this->readRegister(CTRL4)|range));
}

void L3GD20H::swRestart(){
	this->writeRegister(LOW_ODR,(unsigned char)(this->readRegister(LOW_ODR)|0x04));
}




L3GD20H::~L3GD20H() {}

} /* namespace exploringBB */
