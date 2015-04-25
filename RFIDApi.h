
/**********************************************************************
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

 17 June 2014 : One thirtpart SerialPort class is used for RS232 communication
                purpose. 
				Module : SerialPort.H
				Purpose: Interface for an MFC wrapper class for serial ports
                Copyright (c) 1999 - 2013 by PJ Naughter.
************************************************************************/

#pragma once

#ifndef _RFID_API_H
#define _RFID_API_H

#if !defined RFID_EXT_CLASS
	#ifdef _RFID_EXT_CLASS
		#define RFID_EXT_CLASS AFX_CLASS_EXPORT
	#else
		#define RFID_EXT_CLASS AFX_CLASS_IMPORT
	#endif
#endif



#include "SerialPort.h"
#include "afxmt.h"
//#include "RFIDCommands.h"
//#include "RFIDConstants.h"

// BIS-M-410-068-001-09-S72	 RFID CONTROLLER COMMANDS
// ABx FAST communication Protocol

const INT ABx_FAST_CMD_HEADER                               =0x02;
const INT ABx_FAST_CMD_TERMINATOR                           =0x03;


//////////////RFID Tag Commands/////////////////////////////////
const INT ABx_FAST_CMD_FILL_TAG								=0x04;
const INT ABx_FAST_CMD_READ_TAG								=0x05;
const INT ABx_FAST_CMD_WRITE_TAG							=0x06;
const INT ABx_FAST_CMD_READ_TAG_ID							=0x07;
const INT ABx_FAST_CMD_TAG_SEARCH							=0x08;
const INT ABx_FAST_CMD_START_CONTINUOUS_READ				=0x0D;
const INT ABx_FAST_CMD_READ_TAGID_AND_DATA					=0x0E;
const INT ABx_FAST_CMD_START_CONTINUOUS_READ_TAGID_AND_DATA	=0x0F;
const INT ABx_FAST_CMD_LOCK_MEMORY_BLOCK					=0x27;
const INT ABx_FAST_CMD_READ_EPC_CODE                        =0xC2;
const INT ABx_FAST_CMD_WRITE_EPC_CODE                       =0xC3;

/////////////Controller Commands////////////////////////////////
const INT ABx_FAST_CMD_RESET_CONTROLLER						=0x35;
const INT ABx_FAST_CMD_SET_CONTROLLER_CONFIGURATION			=0x36;
const INT ABx_FAST_CMD_GET_CONTROLLER_CONFIGURATION			=0x37;
const INT ABx_FAST_CMD_GET_CONTROLLER_INFO					=0x38;
const INT ABx_FAST_CMD_SET_CONTROLLER_TIME					=0x51;
const INT ABx_FAST_CMD_EXECUTE_MACRO						=0x72;

/////////////Additional UHF-Series Controller Commands//////////
const INT ABx_FAST_CMD_SET_UHF_CONTORLLER_CONFIGURATION     =0xC0;
const INT ABx_FAST_CMD_GET_UHF_CONTORLLER_CONFIGURATION     =0xC1;

//////////// I/O Commands///////////////////////////////////////
const INT ABx_FAST_CMD_SET_CLEAR_DIGITAL_OUTPUTS			=0x10;
const INT ABx_FAST_CMD_GET_DIGITAL_INPUT					=0x11;
const INT ABx_FAST_CMD_GET_DIGITAL_OUTPUTS					=0x12;

////////////Multi-Tag RFID Commands////////////////////////////
const INT ABx_FAST_CMD_MULTI_TAG_READ_ID_AND_DATA_ALL		=0x82;
const INT ABx_FAST_CMD_MULTI_TAG_BLOCK_READ_ALL				=0x85;
const INT ABx_FAST_CMD_MULTI_TAG_BLOCK_WRITE_ALL			=0x86;
const INT ABx_FAST_CMD_MULTI_TAG_GET_INVENTORY				=0x87;
const INT ABx_FAST_CMD_MULTI_TAG_SEARCH_ALL					=0x88;
const INT ABx_FAST_CMD_MULTI_TAG_BLOCK_READ_BY_ID			=0x95;
const INT ABx_FAST_CMD_MULTI_TAG_BLOCK_WRITE_BY_ID			=0x96;
const INT ABx_FAST_CMD_MULTI_TAG_READ_EPC_CODE              =0xC4;



//ABx_FAST Protocol Type Codes
const CHAR ABx_FAST_WITHOUT_CHECKSUM  = 0x00;
const CHAR ABx_FAST_WITH_CHECKSUM     = 0x01;

//Existing controller Types that uses ABx Fast Protocol
const CHAR BIS_M_62_                =0x02;
const CHAR BIS_M_410_               =0x03;
const CHAR BIS_M_411_               =0x04;
const CHAR BIS_U_62_                =0x0B;

#define CTRL_TYPE_BIS_M_62          "BIS_M_62_"
#define CTRL_TYPE_BIS_M_410         "BIS_M_410_"
#define CTRL_TYPE_BIS_M_411         "BIS_M_411_"
#define CTRL_TYPE_BIS_U_62          "BIS_U_62_"

//Existing RFID Tag Type which uses ABx_FAST_PROTOCOL
const CHAR RFID_TAG_BIS_M_1__03_            =0x01;
const CHAR RFID_TAG_BIS_M_1__07_            =0x03;
const CHAR RFID_TAG_BIS_M_1__10_            =0x05;
const CHAR RFID_TAG_BIS_M_1__02_            =0x06;


//Device Interface Type
const INT RFID_COM_RS422                    =0x00;
const INT RFID_COM_RS232_USB                =0x01;
const INT RFID_COM_RS485                    =0x02;
const INT RFID_COM_OTHER                    =0x03;


// BIS-M-410-068-001-09-S72	 RFID CONTROLLER STATION ERROR CODES
// ABx FAST communication Protocol
const INT  ABx_FAST_ERR_FILL_TAG_FAILED				 =0x04; //Fill Tag operation failed
const INT  ABx_FAST_ERR_READ_DATA_FAILED             =0x05; //Read Data Command Failed
const INT  ABx_FAST_ERR_WRITE_DATA_FAILED            =0x06; //Write Data Command Failed
const INT  ABx_FAST_ERR_READ_TAG_ID_FAILED           =0x07; //Read Tag ID or Tag Search Command Failed [Tag not found]
const INT  ABx_FAST_ERR_INVALID_SYNTAX               =0x21; //Command Contained a Syntax Error
const INT  ABx_FAST_ERR_INVALID_TAG_TYPE             =0x23; //Invalid/Unsupported Tag type or RF Command
const INT  ABx_FAST_ERR_LOCK_TAG_BLOCK_FAILED        =0x27; //Lock tag block operation failed
const INT  ABx_FAST_ERR_INTERNAL_CONTOLLER_ERROR     =0x30; //Generic internal controller error, buffer overflow
const INT  ABx_FAST_ERR_INVALID_CONTROLLER_TYPE      =0x31; //Invalid controller type, when setting configuration
const INT  ABx_FAST_ERR_INVALID_PROGRAMMING_ADDRESS  =0x32; //Invalid Tag programming Address specified in the command
const INT  ABx_FAST_ERR_CRC_ERROR                    =0x33; //Cyclic Redundancy Check Error
const INT  ABx_FAST_ERR_INVALID_SOFTWARE_VERSION     =0x34; //Invalid Software version
const INT  ABx_FAST_ERR_INVALID_RESET                =0x35; //Invalid Hardware Reset
const INT  ABx_FAST_ERR_SET_CONFIGURATION_ERROR      =0x36; //Controller Configuration not written
const INT  ABx_FAST_ERR_GET_CONGIGURATION_ERROR	     =0x37; //Controller Configuration not read

const INT  ABx_FAST_CMD_ID_SIZE                      =0x01;
const INT  ABx_FAST_START_ADDRESS_SIZE               =0x02;
const INT  ABx_FAST_READ_WRITE_BLOCK_SIZE			 =0x02;
const INT  ABx_FAST_TIMEOUT_SIZE                     =0x02; 

/////////////////Command Data Packet Size////////////////////////////

// CMD_SIZE (1) + START_ADD(2) + BLOCK(2) + TIMEOUT(2) + DATA_BYTE(1)
const INT  ABx_FAST_FILL_TAG_COMMAND_SIZE            =0x0008; 
//CMD_SIZE(1) + START_ADD(2) + BLOCK(2) + TIMEOUT(2) 
const INT ABx_FAST_READ_TAG_COMMAND_SIZE             =0x0007; 
//CMD_SIZE(1) + START_ADD(2) + BLOCK(2) + TIMEOUT(2) + DATA_BYTE(N)
const INT ABx_FAST_WRITE_TAG_COMMAND_SIZE            =0x0007 + 0x0000;   // 1+2+2+2+N
const INT ABx_FAST_READ_TAG_ID_COMMAND_SIZE          =0x0003;
const INT ABx_FAST_TAG_SEARCH_COMMAND_SIZE           =0x0003;
const INT ABx_FAST_READ_TAGID_AND_DATA_CMD_SIZE      =0x0007;
const INT ABx_FAST_LOCK_MEMORY_BLOCK_CMD_SIZE        =0x0005;
const INT ABx_FAST_RESET_CONTROLLER_CMD_SIZE		 =0x0001;
const INT ABx_FAST_GET_CONTROLLER_CONFIG_CMD_SIZE    =0x0002;
const INT ABx_FAST_SET_CONTROLLER_CONFIG_CMD_SIZE    =0x0014;
const INT ABx_FAST_GET_CONTROLLER_INFO_CMD_SIZE      =0x0001;
const INT ABx_FAST_SET_CONTROLLER_TIME_CMD_SIZE      =0x0008;
const INT ABx_FAST_EXECUTE_MACRO_CMD_SIZE            =0x0002;


/////////////////Response Data Packet Size//////////////////////////
const INT ABx_FAST_FILL_TAG_RESP_SIZE                =0x0001;
const INT ABx_FAST_READ_TAG_RESP_SIZE                =0x0001 + 0x0000; //+N, Read Block size
const INT ABx_FAST_WRITE_TAG_RESP_SIZE               =0x0001;
const INT ABx_FAST_READ_TAGID_RESP_SIZE              =0x0009;  //Tag ID 8-Byte in Size
const INT ABx_FAST_TAG_SEARCH_RESP_SIZE              =0x0001;
const INT ABx_FAST_READ_TAGID_AND_DATA_RESP_SIZE     =0x0009 + 0x0000; //+N, Read Block Size
const INT ABx_FAST_LOCK_MEMORY_BLOCK_RESP_SIZE       =0x0001;
const INT ABx_FAST_RESET_CONTROLLER_RESP_SIZE        =0x0001;
const INT ABx_FAST_GET_CONTROLLER_CONFIG_RESP_SIZE   =0x0014;
const INT ABx_FAST_SET_CONTROLLER_CONFIG_RESP_SIZE   =0x0001;
const INT ABx_FAST_GET_CONTROLLER_INFO_RESP_SIZE     =0x001B;
const INT ABx_FAST_SET_CONTROLLER_TIME_RESP_SIZE     =0x0001;
const INT ABx_FAST_EXECUTE_MACRO_RESP_SIZE           =0x0001;


const INT ABx_FAST_TIMEOUT                           =0x07D0;//0x00FA;//
const INT ABx_FAST_RESPONSE_PACKET_HEADER_TERMINATOR =0x0005;

const INT RFID_NOK   = -1;
const INT RFID_OK    = 0;
const INT RFID_PORT_CREATED = 2;
const INT RFID_PORT_OPENED  = 3;
const INT RFID_TAG_NOT_FOUND = 4;
const INT RFID_PART_NUM_MISMATCH_WITH_PGM = 5;
const INT RFID_MULTIPLE_PART_TYPES = 6;
const INT RFID_ID_WRITE_OP_FAILED  = 7;

//Customize Error Codes For Proper Exception Handling 
const INT  RFID_OP_CUSTOM_ERR_CREATE_PORT_FAILED     =0x60; //Create Serial Port Failed
const INT  RFID_OP_CUSTOM_ERR_PORT_IS_OPEN           =0x61; //Port is already opened
const INT  RFID_OP_CUSTOM_ERR_PORT_IS_NOT_CREATED    =0x62; //Port is not created yet
const INT  RFID_OP_CUSTOM_ERR_PORT_IS_ALREADY_CREATED=0x63; //Port is already created
const INT  RFID_OP_CUSTOM_ERR_PORT_OPEN_FAILED       =0x64; //Open Port operation failed
const INT  RFID_OP_CUSTOM_ERR_ERROR_DURING_CMD_EXECU =0x65; //Error During Command Execution



/*
	This structure holds the information about the 
	RFID Read/Write Head. Call the GetControllerInfo
	Api to get the controller information
*/
struct /*RFID_EXT_CLASS*/ RFID_CONTROLLER_INFO
{
	CHAR cControllerType;
	CHAR cSwVerMajor;
	CHAR cSWVerMinor;
	CHAR cSwVerCorrection;
	CHAR cSwVerPoint;
	CHAR cReserved;
	CHAR cCRC[2];
	CHAR cLoaderSwVer[2];
	CHAR cProcessorId[5];
	CHAR cProcessorRfu[3];
	CHAR cProcessorSerialNo[4];
	CHAR cProcessorInternalInfo[2];
	CHAR cProcessorRsMaxP;
	CHAR cProcessorInfoCRC;
	CHAR cCheckSum; 
	CString szControllerType;
	CString szSwVer; 
	CString szSerialNo;
	CString szFirmWareCrc;
	CString szLoaderSwVer;
};


/*
	The following structures used to store the information
	about RFID Read/Write controller configurations. 
	use the GetControllerConfiguration Api to get the current
	configuration and make necessasary changes to write back to
	controllet using SetControllerConguration API.

	Read the ABx_FAST user guide for more details about the 
	configuration data
*/
typedef struct COMM_MODE
{
	unsigned char cBaudRate : 3;  //0:9600 1:19200 2:38400 3:57600 4:115200
	unsigned char cXON      : 1;  //0: Disabled 1: Enabled
	unsigned char cReserved : 2;  // 
	unsigned char cInterface: 2;  //0:RS422 1:RS232/USB  2:RS485 3:OTHER
}COMM_MODE, *LPCOMM_MODE;

struct /*RFID_EXT_CLASS*/ RFID_CONTROLLER_CONFIG
{
	//Continuous Read at PowerUp Settings
	CHAR cContReadStartAddress[2];
	CHAR cContReadBlockSize[2];
	CHAR cDuplicateReadDelay;

	CHAR cNodeId;
	CHAR cReserved1;
	CHAR cProtocol; //ABxFast = 0x00, AbxFast With CheckSum = 0x01
	CHAR cTagType;
	CHAR cReserved2;
	CHAR cCommMode;
	CHAR cOptionByte1;
	CHAR cReserved3;
	CHAR cOptionByte2;
	CHAR cControllerType;

	CHAR cSwVerMajor;
	CHAR cSWVerMinor;
	CHAR cSwVerCorrection;
	CHAR cSwVerPoint;

	CHAR cCheckSum;

	COMM_MODE stCommMode;


};
//////////////constants//////////////////////////////////
const int BIS_RFID_MAX_BUF_SIZE = 1024;	//This is the maximum allowed size to read/Write from Tag at once
const int BIS_TAG_ID_SIZE       = 8;

/////////////////CRFIDException Class //////////////////
class /*RFID_EXT_CLASS*/ CRFIDException: public CException
{
public:
	CRFIDException();
	CRFIDException(DWORD dwLastError);
	CRFIDException(CHAR cRfidErrorCode);
	virtual ~CRFIDException();

#ifdef _DEBUG
	virtual void Dump(CDumpContext &dc) const;
#endif
	virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext= NULL);
	CString GetErrorMessage();

private:
	VOID GenerateRfidErrorMsg(CHAR cRfidErrorCode);

public:
	DWORD m_dwLastError;
	CHAR  m_cLastRFIDErrorCode;
	CString m_szRfidErrorMsg;

protected:
	//DECLARE_DYNAMIC(CRFIDException);
};

class CRfidUtil: public CObject
{
public:
		CRfidUtil();
public:
		void LogDebugTrace(const CString& szMessage);
		static CRfidUtil& GetObj()
		{
			static CRfidUtil m_stcRfidUtil;
			return m_stcRfidUtil;
		}
public: 
		BOOL m_bDebugFlag;
		
};


class /*RFID_EXT_CLASS*/ CRFIDCtrl : public CObject
{
public:
		CRFIDCtrl();
		CRFIDCtrl(CSerialPort *pSerialPort);
		virtual ~CRFIDCtrl();

protected:

	//Syncronization Related
	CCriticalSection m_csCtrlRfidOp;

	//SerialPort Related
	INT  m_nPortId;
	BOOL m_bPortInit;
	BOOL m_bPortCreated;
	CSerialPort *m_pSerialPort;

	//RFID Device Related
    INT  m_nDeviceId;
	BOOL m_bDeviceCreated;
	/*CHAR*/ unsigned char m_ucRdBuffer[BIS_RFID_MAX_BUF_SIZE];
	CHAR m_ucWrBuffer[BIS_RFID_MAX_BUF_SIZE];
	CHAR m_cData[BIS_RFID_MAX_BUF_SIZE];
	CHAR m_ucTagID[BIS_TAG_ID_SIZE];
	CString m_szTagData;
	CString m_szTagID;
	BOOL m_bTagFound;
	RFID_CONTROLLER_INFO m_stRfidControllerInfo;
	INT m_nRfidOpTimeout;


	//DECLARE_DYNAMIC(CRFIDCtrl);

protected:
	INT SetCommand(INT nCmd, INT nAddress =0x0000, INT nBlockSize = 0x0000,
				   BOOL bCheckSum = FALSE, CHAR *pData=NULL);
	UINT GetCheckSum( INT nBufIndex);
	INT CheckResponse(INT nWriteCmd, UINT unDataSize);
	LONG GetData(INT nCount);
	CString GetStrData(INT nCount);
	void ClearReadWriteBuffer();
	CString GetControllerTypeName(CHAR cControllerType);
	VOID DecodeCommunicationMode(CHAR cCommMode, LPCOMM_MODE lpCommMode);
	INT CheckForErrorResponse(unsigned char *pData);
	INT GetBaudRate(unsigned char ucBaudRateCode);
	INT GetDeviceInterfaceType(unsigned char ucInterfaceType);
	BOOL GetXOnState(unsigned char ucXonXoffState);
	VOID ResetTagId();
    

public:
	INT Create(CSerialPort *pSerialPort);
	INT InitComPort(int nPortNum);
	VOID CloseComPort();
	VOID SetTimeoutValue(INT nTimeout); //Seconds
	INT ResetRfidController();
	INT SearchRfidTag();
	INT ReadRFIDTagId();
	INT ReadRFIDTag(INT nAddress, INT nCount, INT nChannel=0, CHAR *pData=NULL);
	INT WriteRFIDTag(INT nAddress, INT nCount, INT nChannel=0, CHAR *pData=NULL);
	CString GetTagDataString(INT nCount);
	CString GetTagId();
	BOOL GetSearchTagStatus();
	CString GetControllerInfo();
	VOID GetControllerConfiguration(CHAR cControllerType);
	VOID SetControllerConfiguration();

	void SetSearchTagStatus(BOOL bTagFound);

//Bosch Related
/*****************************************************************************************
 Data in RFID Tag is stored in following format for each part, every 100 bytes are reserved for
 each part in WPC(work piece carrier), theoritically single RFID Tag can contains 20 part information.

 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++...
 +						|						|					|						| 
 +		PART 1 DATA     |   PART 2 DATA     	|	 PART 3 DATA    |	PART 4 DATA      	|   PART.....
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++...
 0					   100					   200				   300						 


 PART DATA FORMAT 

 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.....++++
 +	PART TYPE  |	DATE TIME	       |  ...............   |	BONDER ID	   |  ...........			+
 +	(4 CHAR)   |	OTHER INFO	       |  ...............   |	WRITE AREA	   |  ...........			+
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++.....++++
 0	 		   4					  11					30	 [2 CHAR]      40			            100
 <-----BARCODE DATA (11 Bytes)--------->				    <-- Bonder ID ----->
******************************************************************************************/

public:
	BOOL SearchForRfidTag();
	BOOL CheckMaterialValidityForBosch(int nNumOfWP);
	BOOL CheckMaterialValidityForBoschV2(int nNumOfWP);
	CString GetPartTypeInfo(int nPartIndex);
	CString GetPartBarcode(int nPartIndex);
	INT WriteBonderIDToRfidTag(int nNumOfWP, CHAR *pBonderID, INT nBlockOffset);
	INT WriteBonderIDToRfidTagV2(int nNumOfWP,  CHAR *pBonderID, INT nBlockOffset);
	INT ReadBonderIDFromRfidTag(int nNumOfWP,  CHAR *pBonderID, INT nBlockOffset);
	INT ReadLastWrittenBonderIDFromRfidTag(int nNumOfWP,  CHAR *pBonderID, INT nBlockOffset);
	BOOL ValidateBonderIDAtRFIDTag(int nNumOfWP, const CHAR *pBonderID,INT nBlockOffset);
	BOOL IsAllPartInTheBoatSameType(int nNumOfWP, CString szTagData);
	BOOL IsAllPartInTheBoatSameType(int nNumOfWP, CStringArray& szArrParts);
	BOOL CheckAllPartTypesAreAllowed(int nNumOfWP, CString& szAllowedPartTypes);

public:
	CStringArray m_szArrPartType;
	CStringArray m_szArrPartBarcodes;
#ifdef _DEBUG
	virtual void Dump(CDumpContext &dc) const;
#endif

};

#endif