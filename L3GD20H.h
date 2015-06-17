/*
 * L3GD20H.h  Created on: 17 May 2014
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

#ifndef L3GD20H_H_
#define L3GD20H_H_
#include"./Libraries/I2CDevice.h"

/// The L3GD20H has 0x40 registers (0x01 to 0x1C are reserved and should not be accessed)
#define BUFFER_SIZE 0x40

namespace exploringBB {

//From Table 17. of the L3GD20H Data sheet
//Device ID : Default 11010111 (0xd7)
#define DEVID          0x0F
/* CTR1 Register Default: 00000111 (0x07)
 * [DR1, DR0, BW1, BW0, PD, Zen, Yen, Xen]
 * DR1-DR0 - Output data rate selection
 * BW1-BW0 - Bandwidth selection
 * PD - Power mode (0 = Power down, 1 = Normal mode, Sleep mode = PD:Zen:Yen:Xen to 1000
 * Zen - Z axis enable
 * Yen - Y axis enable
 * Xen - X axis enable
 */
#define CTRL1		   0x20
/* CTRL2 Register Default: 0000000 (0x00)
 * [EXTRen, LVLen, HPM1, HPM0, HPCF3, HPCF2, HPCF1, HPFC0]
 * EXTren - Edge sensitive trigger enable
 * LVLen  - Level sensitive trigger enable
 * HPM1-HPM0 - High Pass filter mode
 * HPCF3-HPCF- High Pass filer Cut Off Frequency selection
 */
#define CTRL2          0x21
/* CTRL3 Register Default: 0000000
 * [INT1_IG, INT1_Boot, H_Lactive, PP_OD, INT2_DRDY, INT2_FTH, INT2_ORun, INT2_Empty]
 * INT1_IG - Interrupt enable on INT1 pin
 * INT1_Boot - Boot status available on INT1 pin
 * H_Lactive - Interrupt active configuration on INT
 * PP_OD - Push- Pull / Open Drain (0: Push-pull, 1: open drain)
 * INT2_DRDY - Data ready on DRDY/INT2
 * INT2_FTH - FIFO Threshold interrupt on DRDY/INT2 pin
 * INT2_ORun - FIFO Overrun interrupt on DRDY/INT2 pin
 * INT2_Empty - FIFO Empty interrupt on DRDY/INT2 pin
 */
#define CTRL3          0x22
/* CTRL4 Register Default: 00000000 (0x00)
 * [BDU, BLE, FS1, FS0, IMPen, ST2, ST1, SIM]
 * BDU - Block data update (0: continuos update, 1: output registers not updated until MSB and LSB reading)
 * BLE - Big/little endian data selection (0: data LSB @lower address, 1: data MSB @ lower address)
 * FS1-FS0 - FUll scale selection (00: 245 dps, 01 500 dps; 1x: 2000 dps]
 * IMPen - Level sensitive enable
 * ST2-ST1 - Self-test (00, normal mode, 01 = self-test 0(+), 10 = unused, 11 = self-test 1(-)
 * SIM - SPI Serial Interface Mode selection (0: 4-wire interface, 1: 3-wire interface)
 */
#define CTRL4          0x23
/* CTRL5 Register Default: 00000000 (0x00)
 * [BOOT, FIFO_EN, StopOnFTH, HPen, IG_Sel1, IG_Sel0, Out,Sel1, Out,Sel0]
 * BOOT - Reboot memory content
 * FIFO_EN - FIFO enable
 * StopOnFTH - Sensing chain FIFO stop values memorization at FIFO Threshold (0 = FIFO depth not limited 32/axis 1: FIF) depth is limited to FIFO threshold defined FIFO_CTRL
 * HPen - High pass filter enable
 * IG_Sel1-IG_Sel0 - INT generator selection configuration see figure 23
 * Out_Sel1-Out_-Sel0 - Out selection configuration see figure 23
 */
#define CTRL5          0x24
/* Reference Register Default: 00000000 (0x00)
 * Ref7-Ref0 - Digital high pass filter reference value
 */
#define REFERENCE      0x25
/* OUT_TEMP Register Default: output
 * Temperature data (-1LSB/deg with 8 bit resolution)
 * Expressed as two's complement
 */
#define OUT_TEMP       0x26
/* STATUS Register Default: 00000000 (0x00)
 * [ZYXOR, ZOR, YOR, XOR, ZYXDA, ZDA, YDA, XDA]
 * ZYXOR - X, Y, Z -axis data overrun
 * ZOR - Z axis data overrun
 * YOR - Y axis data overrun
 * XOR - X axis data overrun
 * ZYXDA - X, Y, Z -axis new data available
 * ZDA - Z axis new data available
 * YDA - Y axis new data available
 * XDA - X axis new data available
 */
#define STATUS         0x27
/* OUT_X_L Register
 * X-axis angular rate data
 * Two's compliment
 */
#define DATAX0         0x28
/* OUT_X_H Register
 * X-axis angular rate data
 * Two's compliment
 */
#define DATAX1         0x29
/* OUT_Y_L Register
 * Y-axis angular rate data
 * Two's compliment
 */
#define DATAY0         0x2A
/* OUT_Y_H Register
 * Y-axis angular rate data
 * Two's compliment
 */
#define DATAY1         0x2B
/* OUT_Z_L Register
 * Z-axis angular rate data
 * Two's compliment
 */
#define DATAZ0         0x2C
/* OUT_Z_H Register
 * Z-axis angular rate data
 * Two's compliment
 */
#define DATAZ1         0x2D
/* FIF0_CTRL Register Default: 00000000 (0x00)
 * FM2-FM0 - FIFO mode selection. See table 41
 * FTH4-FTH0 - FIFO threshold setting
 */
#define FIFO_CTRL      0x2E
/* FIFO_SRC Register Default: 0000000 (0x00)
 * [FTH, OVRN, EMPTY, FSS4, FSS3, FSS2, FSS1, FSS0]
 * FTH - FIFO threshold status (0: FIFO fillinf is lower than FTH level, 1: FIFO filling is equal or higher than FTH level)
 * OVRN - Overrun bit status (0:FIFO is not completely filled, 1:FIFO is filled)
 * EMPTY - FIFO Empty bit. (0:FIFO not empty, 1: FIFO empty)
 * FSS4-FSS0 - FIFO stored data level of the unread samples
 */
#define FIFO_SRC       0x2F
/* IG_CFG Register Default: 00000000 (0x00)
 * [AND/OR, LIR, ZHIE, ZLIE, YHIE, YLIE, XHIE, XLIE]
 * AND/OR - AND/OR combination of interrupt events
 * LIR - Latch Interrupt Request (cleared by reading IG_SRC reg)
 * ZHIE - Enable interrupt generation on Z high event
 * ZLIE - Enable interrupt generation on Z low event
 * YHIE - Enable interrupt generation on Y high event
 * YLIE - Enable interrupt generation on Y low event
 * XHIE - Enable interrupt generation on X high event
 * XLIE - Enable interrupt generation on X low event
 */
#define IG_CFG         0x30
/* IG_SRC Register Default: 00000000 (0x00)
 * [0, IA, ZH, ZL, YH, YL, XH, XL]
 * IA - Interrupt active (0: no interrupt has been generated, 1: one or more interrupts have been generated)
 * ZH - Z high
 * ZL - Z low
 * YH - Y high
 * YL - Y low
 * XH - X high
 * XL - X low
 */
#define IG_SRC         0x31
/* IG_THS_XH Register Default: 00000000 (0x00)
 * [DCRM, THSX14, THSX13, THSX12, THSX11, THSX10, THSX9, THSX8]
 * DCRM - Interrupt generation counter mode selection ( 0 = Reset, 1 = Decrement)
 * THSX14 - THSX8 - Interrupt threshold on X axis
 */
#define IG_THS_XH      0x32
/* IG_THS_XL Register Default: 00000000 (0x00)
 * [THSX7, THSX6, THSX5, THSX4, THSX3, THSX2, THSX1, THSX0]
 * THSX7-THSX0 - Interrupt threshold on X axis
 */
#define IG_THS_XL      0x33
/* IG_THS_YH Register Default: 00000000 (0x00)
 * [0, THSY14, THSY13, THSY12, THSY11, THSY10, THSY9, THSY8]
 * THSX14-THSX8 - Interrupt threshold on Y axis
 */
#define IG_THS_YH      0x34
/* IG_THS_YL Register Default: 00000000 (0x00)
 * [THSY7, THSY6, THSY5, THSY4, THSY3, THSY2, THSY1, THSY0]
 * THSY0-THSY0 - Interrupt threshold on Y axis
 */
#define IG_THS_YL      0x35
/* IG_THS_ZH Register Default: 00000000 (0x00)
 * [0, THSZ14, THSZ13, THSZ12, THSZ11, THSZ10, THSZ9, THSZ8]
 * THSZ14-THSZ8 - Interrupt threshold on Z axis
 */
#define IG_THS_ZH      0x36
/* IG_THS_ZL Register Default: 00000000 (0x00)
 * [THSZ7, THSZ6, THSZ5, THSZ4, THSZ3, THSZ2, THSZ1, THSZ0]
 * THSZ7-THSZ0 - Interrupt threshold on Z axis
 */
#define IG_THS_ZL      0x37
/* IG_duration Register Default: 00000000 (0x00)
 * [WAIT, D6, D5, D4, D3, D2, D1, D0]
 * WAIT - WAIT enable
 * D6-D0 - Duration value ( sets the min duration of the Interrupt event to be recognized)
 */
#define IG_DURATION    0x38
/* LOW_ODR Register Default: 00000000 (0x00)
 * [--, --, DRDY_HL, 0, I2C_dis, SW_RES, 0, Low_ODR]
 * DRDY_HL - DRDY/INT2 pin (0: DRDY active high, 1: DRDY active low)
 * I2C_dis - 0: both the I2C and SPI interfaces enabled, 1: SPI only
 * SW-RES - Software reset (0: normal mode, 1: Reset Device (bit is cleared by HW after next boot)
 * Low_ODR - Low Speed ODR (0: Low Speed ODR Disabled, 1: Low Speed ODR Enabled)
 * Bits 1 and 4 need to be set to 0 for device to work
 */
#define LOW_ODR	                  0x39

#define L3GD20H_ODR_BIT           7
#define L3GD20H_ODR_LENGTH        2
#define L3GD20H_BW_BIT            5
#define L3GD20H_BW_LENGTH         2
#define L3GD20H_PD_BIT            3
#define L3GD20H_ZEN_BIT           2
#define L3GD20H_YEN_BIT           1
#define L3GD20H_XEN_BIT           0

#define L3GD20H_RATE_100_12       0b00 //selection of high vs low rate is via Low_ODR
#define L3GD20H_RATE_200_25       0b01 //selection of high vs low rate is via Low_ODR
#define L3GD20H_RATE_400_50       0b10 //selection of high vs low rate is via Low_ODR
#define L3GD20H_RATE_800_50       0b11 //selection of high vs low rate is via Low_ODR

#define L3GD20H_BW_LOW            0b00
#define L3GD20H_BW_MED_LOW        0b01
#define L3GD20H_BW_MED_HIGH       0b10
#define L3GD20H_BW_HIGH           0b11

#define L3GD20H_HPM_BIT           5
#define L3GD20H_HPM_LENGTH        2
#define L3GD20H_HPCF_BIT          3
#define L3GD20H_HPCF_LENGTH       4

#define L3GD20H_HPM_HRF           0b00 //this resets on reading REFERENCE
#define L3GD20H_HPM_REFERENCE     0b01
#define L3GD20H_HPM_NORMAL        0b10
#define L3GD20H_HPM_AUTORESET     0b11

#define L3GD20H_HPCF1             0b0000
#define L3GD20H_HPCF2             0b0001
#define L3GD20H_HPCF3             0b0010
#define L3GD20H_HPCF4             0b0011
#define L3GD20H_HPCF5             0b0100
#define L3GD20H_HPCF6             0b0101
#define L3GD20H_HPCF7             0b0110
#define L3GD20H_HPCF8             0b0111
#define L3GD20H_HPCF9             0b1000
#define L3GD20H_HPCF10            0b1001

#define L3GD20H_INT1_IG_BIT       7
#define L3GD20H_INT1_BOOT_BIT     6
#define L3GD20H_H_LACTIVE_BIT     5
#define L3GD20H_PP_OD_BIT         4
#define L3GD20H_INT2_DRDY_BIT     3
#define L3GD20H_INT2_FTH_BIT      2
#define L3GD20H_INT2_ORUN_BIT     1
#define L3GD20H_INT2_EMPTY_BIT    0

#define L3GD20H_PUSH_PULL         0
#define L3GD20H_OPEN_DRAIN        1

#define L3GD20H_BDU_BIT           7
#define L3GD20H_BLE_BIT           6
#define L3GD20H_FS_BIT            5
#define L3GD20H_FS_LENGTH         2
#define L3GD20H_IMPEN_BIT		  3 //new for this IC: Level sensitive latched enalble
#define L3GD20H_ST_BIT            2
#define L3GD20H_ST_LENGTH         2
#define L3GD20H_SIM_BIT           0

#define L3GD20H_BIG_ENDIAN        1
#define L3GD20H_LITTLE_ENDIAN     0

#define L3GD20H_FS_250            0b00
#define L3GD20H_FS_500            0b01
#define L3GD20H_FS_2000           0b10

#define L3GD20H_SELF_TEST_NORMAL  0b00
#define L3GD20H_SELF_TEST_0       0b01
#define L3GD20H_SELF_TEST_1       0b11

#define L3GD20H_SPI_4_WIRE        0
#define L3GD20H_SPI_3_WIRE        1

#define L3GD20H_BOOT_BIT          7
#define L3GD20H_FIFO_EN_BIT       6
#define L3GD20H_STOPONFTH_BIT	  5
#define L3GD20H_HPEN_BIT          4
#define L3GD20H_IG_SEL_BIT        3
#define L3GD20H_IG_SEL_LENGTH     2
#define L3GD20H_OUT_SEL_BIT       1
#define L3GD20H_OUT_SEL_LENGTH    2

#define L3GD20H_NON_HIGH_PASS     0b00
#define L3GD20H_HIGH_PASS         0b01
#define L3GD20H_LOW_PASS          0b10 //depends on HPEN
#define L3GD20H_LOW_HIGH_PASS     0b11 //depends on HPEN

#define L3GD20H_ZYXOR_BIT         7
#define L3GD20H_ZOR_BIT           6
#define L3GD20H_YOR_BIT           5
#define L3GD20H_XOR_BIT           4
#define L3GD20H_ZYXDA_BIT         3
#define L3GD20H_ZDA_BIT           2
#define L3GD20H_YDA_BIT           1
#define L3GD20H_XDA_BIT           0

#define L3GD20H_FIFO_MODE_BIT     7
#define L3GD20H_FIFO_MODE_LENGTH  3
#define L3GD20H_FIFO_TH_BIT       4
#define L3GD20H_FIFO_TH_LENGTH    5

#define L3GD20H_FM_BYPASS         0b000
#define L3GD20H_FM_FIFO           0b001
#define L3GD20H_FM_STREAM         0b010
#define L3GD20H_FM_STREAM_FIFO    0b011
#define L3GD20H_FM_BYPASS_STREAM  0b100
#define L3GD20H_FM_DYNAMIC_STREAM 0b110
#define L3GD20H_FM_BYPASS_FIFO    0b111


#define L3GD20H_FIFO_TH_STATUS_BIT   7
#define L3GD20H_OVRN_BIT     		 6
#define L3GD20H_EMPTY_BIT    		 5
#define L3GD20H_FIFO_FSS_BIT      	 4
#define L3GD20H_FIFO_FSS_LENGTH   	 5

#define L3GD20H_AND_OR_BIT   	  7
#define L3GD20H_LIR_BIT      	  6
#define L3GD20H_ZHIE_BIT          5
#define L3GD20H_ZLIE_BIT          4
#define L3GD20H_YHIE_BIT          3
#define L3GD20H_YLIE_BIT          2
#define L3GD20H_XHIE_BIT          1
#define L3GD20H_XLIE_BIT          0

#define L3GD20H_AND_OR_OR 		  0
#define L3GD20H_AND_OR_AND     	  1

#define L3GD20H_IA_BIT            6
#define L3GD20H_ZH_BIT            5
#define L3GD20H_ZL_BIT            4
#define L3GD20H_YH_BIT            3
#define L3GD20H_YL_BIT            2
#define L3GD20H_XH_BIT            1
#define L3GD20H_XL_BIT            0

#define L3GD20H_DCRM_BIT		  7

#define L3GD20H_DCRM_RESET		  0
#define L3GD20H_DCRM_DEC		  1

#define L3GD20H_WAIT_BIT          7
#define L3GD20H_DUR_BIT           6
#define L3GD20H_DUR_LENGTH        7

#define L3GD20H_LOW_ODR_BIT		  0
#define L3GD20H_SW_RESET_BIT	  2
#define L3GD20H_I2C_DIS_BIT		  3
#define L3GD20H_DRDY_HL_BIT		  5

/**
 * @class L3GD20H
 * @brief Specific class for the L3GD20H Accelerometer that is a child of the I2CDevice class
 * Protected inheritance means that the public I2CDevice methods are not publicly accessible
 * by an object of the L3GD20H class.
 */
class L3GD20H:protected I2CDevice{

public:

	enum RANGE {
		DPS_245		= 0x00,
		DPS_500		= 0x10,
		DPS_2000	= 0x20
	};


//	/// The resolution of the sensor. High is only available in +/- 16g range.
//	enum RESOLUTION {
//		NORMAL = 0,//!< NORMAL 10-bit resolution
//		HIGH = 1   //!< HIGH 13-bit resolution
//	};



private:
	unsigned int I2CBus, I2CAddress;
	//unsigned char *registers;
	L3GD20H::RANGE range;
	//L3GD20H::RESOLUTION resolution;
	short gyroX, gyroY, gyroZ; // raw 2's complement values
	//float pitch, roll;                                 // in degrees
	short combineRegisters(unsigned char msb, unsigned char lsb);


public:
	L3GD20H(unsigned int I2CBus, unsigned int I2CAddress=0x53);
	virtual int readSensorState();

	virtual void setRange(L3GD20H::RANGE range);
	virtual void swRestart();
	virtual void setPowerOn(bool on);
	virtual bool getPowerOn();
	virtual int getDeviceID();
	virtual bool testConnection();
	virtual L3GD20H::RANGE getRange() { return this->range; }
	//CTRL1 register
	virtual void setOutputDataRate(unsigned int rate);
	virtual unsigned short getOutputDataRate();
	virtual void setBandwidthCutOffMode(unsigned short mode);
	virtual unsigned short getBandwidthCutOffMode();
	virtual void setZEnabled(bool enabled);
	virtual bool getZEnabled();
	virtual void setYEnabled(bool enabled);
	virtual bool getYEnabled();
	virtual void setXEnabled(bool enabled);
	virtual bool getXEnabled();

	// CTRL2 register, r/w
	virtual void setHighPassMode(unsigned char mode);
	virtual unsigned char getHighPassMode();
	virtual void setHighPassFilterCutOffFrequencyLevel(unsigned char level);
	virtual unsigned char getHighPassFilterCutOffFrequencyLevel();

	// CTRL3 register, r/w
	virtual void setINT1InterruptEnabled(bool enabled);
	virtual bool getINT1InterruptEnabled();
	virtual void setINT1BootStatusEnabled(bool enabled);
	virtual bool getINT1BootStatusEnabled();
	virtual void interruptActiveINT1Config();
	virtual void setOutputMode(bool mode);
	virtual bool getOutputMode();
	virtual void setINT2DataReadyEnabled(bool enabled);
	virtual bool getINT2DataReadyEnabled();
	virtual void setINT2FIFOWatermarkInterruptEnabled(bool enabled);
	virtual bool getINT2FIFOWatermarkInterruptEnabled();
	virtual void setINT2FIFOOverrunInterruptEnabled(bool enabled);
	virtual bool getINT2FIFOOverrunInterruptEnabled();
	virtual void setINT2FIFOEmptyInterruptEnabled(bool enabled);
	virtual bool getINT2FIFOEmptyInterruptEnabled();

	// CTRL4 register, r/w
	virtual void setBlockDataUpdateEnabled(bool enabled);
	virtual bool getBlockDataUpdateEnabled();
	virtual void setEndianMode(bool endianness);
	virtual bool getEndianMode();
	virtual void setFullScale(unsigned int scale);
	virtual unsigned int getFullScale();
	virtual void setSelfTestMode(unsigned char mode);
	virtual unsigned char getSelfTestMode();
	virtual void setSPIMode(bool mode);
	virtual bool getSPIMode();

	// CTRL5 register, r/w
	virtual void rebootMemoryContent();
	virtual void setFIFOEnabled(bool enabled);
	virtual bool getFIFOEnabled();
	virtual void setStopOnFIFOThresholdEnabled(bool enabled);
	virtual bool getStopOnFIFOThresholdEnabled();
	virtual void setHighPassFilterEnabled(bool enabled);
	virtual bool getHighPassFilterEnabled();
	virtual void setDataFilter(unsigned char filter);
	virtual unsigned char getDataFilter();

	// REFERENCE/DATACAPTURE register, r/w
	virtual void setHighPassFilterReference(unsigned char reference);
	virtual unsigned char getHighPassFilterReference();

	// OUT_TEMP register, read-only
	virtual unsigned char getTemperature();

	// STATUS register, read-only
	virtual bool getXYZOverrun();
	virtual bool getZOverrun();
	virtual bool getYOverrun();
	virtual bool getXOverrun();
	virtual bool getXYZDataAvailable();
	virtual bool getZDataAvailable();
	virtual bool getYDataAvailable();
	virtual bool getXDataAvailable();

	// OUT_* registers, read-only
	//virtual void getAngularVelocity(unsigned int* x, unsigned int* y, unsigned int* z);
	virtual int getAngularVelocityX();
	virtual int getAngularVelocityY();
	virtual int getAngularVelocityZ();

	// FIFO_CTRL register, r/w
	virtual void setFIFOMode(unsigned char mode);
	virtual unsigned char getFIFOMode();
	virtual void setFIFOThreshold(unsigned char wtm);
	virtual unsigned char getFIFOThreshold();

	// FIFO_SRC register, read-only
	virtual bool getFIFOAtWatermark();
	virtual bool getFIFOOverrun();
	virtual bool getFIFOEmpty();
	virtual unsigned char getFIFOStoredDataLevel();

	// IG_CFG register, r/w
	virtual void setInterruptCombination(bool combination);
	virtual bool getInterruptCombination();
	virtual void setInterruptRequestLatched(bool latched);
	virtual bool getInterruptRequestLatched();
	virtual void setZHighInterruptEnabled(bool enabled);
	virtual bool getZHighInterruptEnabled();
	virtual void setYHighInterruptEnabled(bool enabled);
	virtual bool getYHighInterruptEnabled();
	virtual void setXHighInterruptEnabled(bool enabled);
	virtual bool getXHighInterruptEnabled();
	virtual void setZLowInterruptEnabled(bool enabled);
	virtual bool getZLowInterruptEnabled();
	virtual void setYLowInterruptEnabled(bool enabled);
	virtual bool getYLowInterruptEnabled();
	virtual void setXLowInterruptEnabled(bool enabled);
	virtual bool getXLowInterruptEnabled();

	 // IG_SRC register, read-only
	virtual bool getInterruptActive();
	virtual bool getZHigh();
	virtual bool getZLow();
	virtual bool getYHigh();
	virtual bool getYLow();
	virtual bool getXHigh();
	virtual bool getXLow();

	 // IG_THS_* registers, r/w
	virtual void setDecrementMode(bool mode);
	virtual bool getDecrementMode();
	virtual void setXHighThreshold(unsigned char threshold);
	virtual unsigned char getXHighThreshold();
	virtual void setXLowThreshold(unsigned char threshold);
	virtual unsigned char getXLowThreshold();
	virtual void setYHighThreshold(unsigned char threshold);
	virtual unsigned char getYHighThreshold();
	virtual void setYLowThreshold(unsigned char threshold);
	virtual unsigned char getYLowThreshold();
	virtual void setZHighThreshold(unsigned char threshold);
	virtual unsigned char getZHighThreshold();
	virtual void setZLowThreshold(unsigned char threshold);
	virtual unsigned char getZLowThreshold();

	 // IG_DURATION register, r/w
	virtual void setDuration(unsigned char duration);
	virtual unsigned char getDuration();
	virtual void setWaitEnabled(bool enabled);
	virtual bool getWaitEnabled();

	 // LOW_ODR register, r/w
	virtual void setINT2DataReadyActiveLowEnabled(bool enabled);
	virtual bool getINT2DataReadyActiveLowEnabled();
	virtual void setSPIOnlyEnabled(bool enabled);
	virtual bool getSPIOnlyEnabled();
	virtual void setLowODREnabled(bool enabled);
	virtual bool getLowODREnabled();

	virtual short getGyroX() { return gyroX; }
	virtual short getGyroY() { return gyroY; }
	virtual short getGyroZ() { return gyroZ; }


	virtual ~L3GD20H();

private:
        unsigned char devAddr;
        unsigned char buffer[6];
        unsigned char temp;
        bool 	endianMode;
};


} /* namespace exploringBB */

#endif /* L3GD20H_H_ */
