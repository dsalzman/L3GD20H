
#ifndef LIBRARIES_UARTDEVICE_H_
#define LIBRARIES_UARTDEVICE_H_


namespace exploringBB {

/**
 * This is the header file for the serial UART device Object
 */
class UartDevice {

public:
	protected:
	 	int file;            /**< the file handle to the device */
	 	int tty;
	public:
		UartDevice(int tty);
		virtual int openSerial();
		virtual int writeChar(unsigned char x);
		virtual int writeFloat(double x, char length);
		virtual int writeString(const char* x, unsigned char length);
		virtual int closeSerial();
		virtual ~UartDevice();
	};

} /* namespace exploringBB */


#endif /* BUSDEVICE_H_ */
