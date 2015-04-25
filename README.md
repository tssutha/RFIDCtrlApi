File : RFIDApi.h
Date : 17 June 2014
Author : Sivabalan Suthakaran
Purpose: This module was developed to operate RFID Read/Write controller,
         Even though there are many different RFID controller with different 
		 communication protocols, we choose Balluff BIS00W4[CO405-USB-01] RFID
		 read/write controller which uses ABx_FAST_COMMUNICATION_PROTOCOL

		 This module was developed for ABx_FAST communication protocol defined 
		 by DataLogic/Balluff.

		 This module can be extended to support different RFID conroller with 
		 different protocol, but need to make some generalization before adding 
		 a new controller type. 

		 This module is based on RS232 communication, USB based products must 
		 have their own USB-RS232 (UART) drivers to convert USB port communication
		 to RS232 port.

		 For more details about the ABx_FAST communication protocol, please refer
		 the user guides from Balluff website.
		 
		 Features:
		1. Establish Communication using RS232 port
		2. Search For RFID Tag
		3. Read RFID Read/Write Controller Information
		4. Read RFID Tag with given offset
		5. Write to RFID Tag with given offset
		6. Convert Binary Data to String representation
		7. Get/Set Controller Configuration
		8. Reset/Restart controller configuration
  
And it also includes some sample test cases by based on real data 
from some RFID Tags. 

 17 June 2014 : One thirtpart SerialPort class is used for RS232 communication
                purpose. 
				Module : SerialPort.H
				Purpose: Interface for an MFC wrapper class for serial ports
                Copyright (c) 1999 - 2013 by PJ Naughter.