
#ifndef LIBRARIES_UARTDEVICE_H_
#define LIBRARIES_UARTDEVICE_H_


namespace exploringBB {

/**
 * @class BusDevice
 * @brief This class is the parent of I2C and SPI devices, so that devices that use both
 * SPI and I2C interfaces can use those interfaces interchangeably. Because it contains
 * abstract methods, the child classes MUST implement the methods that are listed in this
 * class.
 */
class UartDevice {

public:
	protected:
		//unsigned int bus;    /**< the bus number */
		//unsigned int device; /**< the device number on the bus  */
		unsigned char tty;
		int file;            /**< the file handle to the device */
	public:
		UartDevice(unsigned char tty);
		virtual int openSerial();
		virtual int writeChar(unsigned char x);
		virtual int writeFloat(double x, char length);
		virtual int writeString(const char* x, unsigned char length);
		virtual int closeSerial();
		virtual ~UartDevice();
	};

} /* namespace exploringBB */


#endif /* BUSDEVICE_H_ */
