/**********************************************************************
File : RFIDApi.h
Date : 17 June 2014
Author : Sivabalan Suthakaran
Purpose: This module was developed to support RFID read/write controller using
		 MFC a program.
         Even though there are many different RFID controller with different 
		 communication protocols, this module is specifically focus on 
		 Balluff BIS00W4[CO405-USB-01] RFID read/write controller which 
		 uses ABx_FAST_COMMUNICATION_PROTOCOL
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
***********************************************************************/

#include <stdafx.h>
#include "RFIDApi.h"


#ifndef _WINERROR_
#pragma message("if you want to avoid this message, put winerror.h into precompiled"
                "header file, normally stdafx.h")
#include <winerror.h>
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//Debug util 
/////////////////////////////////////////////////////////////////////////////////
//@Sutha_01Feb2012
#define StartPerfCounter(__start)        \
    __asm pusha                \
    __asm xor eax, eax            \
    __asm cpuid                \
    __asm rdtsc                \
    __asm mov dword ptr [__start + 0], eax    \
    __asm mov dword ptr [__start + 4], edx


#define StopPerfCounter(__stop)            \
    __asm rdtsc                \
    __asm mov dword ptr [__stop + 0], eax    \
    __asm mov dword ptr [__stop + 4], edx    \
    __asm popa                            


static __forceinline unsigned __int64 CalcOverhead(
       unsigned __int64 __start, unsigned __int64 __stop)
{
    StartPerfCounter(__start);
    StopPerfCounter(__stop);
    return __stop - __start;
}

static __forceinline unsigned __int64 CalcPerf(unsigned __int64 __start, 
       unsigned __int64 __stop, __int64 __overhead)
{
    return __stop - __start - __overhead;
}

unsigned __int64 start4 = 0, stop4 = 0,    perf4 = 0;
unsigned __int64 start1 = 0, stop1 = 0,    perf1 = 0;


unsigned __int64  overhead1 = CalcOverhead(start4,stop4);    //get the overhead
clock_t cs1, cs2,cst1, cst2;
//@End
////////////////////////////////////////////////////////////////////////////////


CRFIDException::CRFIDException()
:CException()
,m_dwLastError(0)
{

}

CRFIDException::CRFIDException(DWORD dwLastError)
:CException()
,m_dwLastError(dwLastError)
{

}

CRFIDException::CRFIDException(CHAR cRfidErrorCode)
:CException()
,m_dwLastError(0)
,m_cLastRFIDErrorCode(cRfidErrorCode)
{
   GenerateRfidErrorMsg(cRfidErrorCode);
   //AfxMessageBox(m_szRfidErrorMsg);
}

CRFIDException::~CRFIDException()
{

}

//IMPLEMENT_DYNAMIC(CRFIDException, CException)

#ifdef _DEBUG
void CRFIDException::Dump(CDumpContext &dc) const
{
	CObject::Dump(dc);
	dc << _T("m_dwLastError = ") <<m_dwLastError << _T("\0");
}
#endif

BOOL CRFIDException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
{
	//validate the inputs
   ASSERT(lpszError != NULL && AfxIsValidString(lpszError, nMaxError));
   if(pnHelpContext != NULL)
	   pnHelpContext = 0;

   BOOL bSuccess = FALSE;

   LPTSTR lpBuffer;
   DWORD dwReturn = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						 NULL, m_dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
						 reinterpret_cast<LPTSTR>(&lpBuffer), 0, NULL);

   if(dwReturn == 0)
	   lpszError = _T("\0");
   else
   {
	   bSuccess = TRUE;
	   CString szErrMsg;
	   szErrMsg.Format("RFID Controller Operation Error \n%s", m_szRfidErrorMsg);
	   strncpy(lpszError,szErrMsg.GetBuffer(4096), nMaxError);
	   szErrMsg.ReleaseBuffer();
   }
   return TRUE;
}

CString CRFIDException::GetErrorMessage()
{
	CString szRetVal;
	LPTSTR lpszError = szRetVal.GetBuffer(4096);
	GetErrorMessage(lpszError, 4096);
	szRetVal.ReleaseBuffer();
	return szRetVal + m_szRfidErrorMsg;
}

VOID CRFIDException::GenerateRfidErrorMsg(CHAR cRfidErrorCode)
{
	unsigned char ucErrorCode = (unsigned char) cRfidErrorCode;
	switch(cRfidErrorCode)
	{
	case ABx_FAST_ERR_FILL_TAG_FAILED:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = FILL_TAG_FAILED",ucErrorCode);
		break;
	case ABx_FAST_ERR_READ_DATA_FAILED:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = READ_DATA_FAILED",ucErrorCode);
		break;
	case ABx_FAST_ERR_WRITE_DATA_FAILED:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = WRITE_DATA_FAILED",ucErrorCode);
		break;
	case ABx_FAST_ERR_READ_TAG_ID_FAILED:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = READ_TAG_ID_FAILED",ucErrorCode);
		break;
	case ABx_FAST_ERR_INVALID_SYNTAX:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = INVALID_SYNTAX",ucErrorCode);
		break;
	case ABx_FAST_ERR_INVALID_TAG_TYPE:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = INVALID_TAG_TYPE",ucErrorCode);
		break;
	case ABx_FAST_ERR_LOCK_TAG_BLOCK_FAILED:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = LOCK_TAG_BLOCK_FAILED",ucErrorCode);
		break;
	case ABx_FAST_ERR_INTERNAL_CONTOLLER_ERROR:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = INTERNAL_CONTOLLER_ERROR",ucErrorCode);
		break;
	case ABx_FAST_ERR_INVALID_CONTROLLER_TYPE:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = INVALID_CONTROLLER_TYPE",ucErrorCode);
		break;
	case ABx_FAST_ERR_INVALID_PROGRAMMING_ADDRESS:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = INVALID_PROGRAMMING_ADDRESS",ucErrorCode);
		break;
	case ABx_FAST_ERR_CRC_ERROR:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \n ErrorDesc = CRC_ERROR",ucErrorCode);
		break;
	case ABx_FAST_ERR_INVALID_SOFTWARE_VERSION:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = INVALID_SOFTWARE_VERSION",ucErrorCode);
		break;
	case ABx_FAST_ERR_INVALID_RESET:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = INVALID_RESET",ucErrorCode);
		break;
	case ABx_FAST_ERR_SET_CONFIGURATION_ERROR:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = SET_CONFIGURATION_ERROR",ucErrorCode);
		break;
	case ABx_FAST_ERR_GET_CONGIGURATION_ERROR:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = GET_CONGIGURATION_ERROR",ucErrorCode);
		break;

	//custom Error Code
	case RFID_OP_CUSTOM_ERR_PORT_IS_OPEN:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = PORT_IS_OPEN",ucErrorCode);
		break;
	case RFID_OP_CUSTOM_ERR_PORT_IS_NOT_CREATED:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = PORT_IS_NOT_CREATED",ucErrorCode);
		break;
	case RFID_OP_CUSTOM_ERR_CREATE_PORT_FAILED:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = CREATE_PORT_FAILED",ucErrorCode);
		break;
	case RFID_OP_CUSTOM_ERR_PORT_IS_ALREADY_CREATED:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = PORT_IS_ALREADY_CREATED",ucErrorCode);
		break;
	case RFID_OP_CUSTOM_ERR_PORT_OPEN_FAILED:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \nErrorDesc = PORT_OPEN_FAILED",ucErrorCode);
		break;

	default:
		m_szRfidErrorMsg.Format("ErrorCode = 0x%02x \n UNKNOWN ERROR",ucErrorCode); 

	}

}

////Debug util
CRfidUtil::CRfidUtil()
:m_bDebugFlag(TRUE)
{
	//Nothing
}

void CRfidUtil::LogDebugTrace(const CString& szMessage)
{
	if(m_bDebugFlag == TRUE)
	{
		CStdioFile file;
		CFileException exce;
		BOOL bIsFileOpen = file.Open("C:\\System\\Rfid.txt", CFile::modeCreate|CFile::modeWrite|
			CFile::modeNoTruncate|CFile::shareExclusive, &exce);
		if(bIsFileOpen)
		{
			file.SeekToEnd();
			CString szTemp;
			szTemp.Format("%s\n", szMessage);
			file.WriteString(szTemp);
			file.Close();
		}
	}
}


/////////////CRFIDCtrl Class Related////////////////////////////////////

CRFIDCtrl::CRFIDCtrl()
:m_pSerialPort(NULL)
,m_bPortCreated(FALSE)
,m_bPortInit(FALSE)
,m_bDeviceCreated(FALSE)
,m_nDeviceId(0x00)
,m_nPortId(1)
,m_bTagFound(FALSE)
,m_nRfidOpTimeout(3000)
,m_bDebugFlag(FALSE)
{

}


CRFIDCtrl::CRFIDCtrl(CSerialPort *pSerialPort)
:m_pSerialPort(pSerialPort)
,m_bTagFound(FALSE)
,m_bDebugFlag(FALSE)
{

}

CRFIDCtrl::~CRFIDCtrl()
{

}

//IMPLEMENT_DYNAMIC(CRFIDCtrl, CObject)

#ifdef _DEBUG
void CRFIDCtrl::Dump(CDumpContext &dc) const
{
	CObject::Dump(dc);
	dc << _T("RFIDController");
}
#endif

/*
	Create a SerialPort Object for communication 
	with RFID Controller, Serial Port Properties to
	be defined by user and Set the port using this 
	function.
	INPUT : CSerialPort *pSerialPort 
	OUTPUT: 
*/
INT CRFIDCtrl::Create(CSerialPort *pSerialPort)
{
	CSingleLock lock(&m_csCtrlRfidOp);
	lock.Lock();

	if(pSerialPort == NULL)
	{
		return RFID_NOK;
	}

	try
	{

		if(m_bPortCreated == TRUE)
		{
			TRACE("COM Port is already created");
			//THROW(new CRFIDException((CHAR)RFID_OP_CUSTOM_ERR_PORT_IS_ALREADY_CREATED));
			return RFID_PORT_CREATED;
		}

		m_pSerialPort = pSerialPort;
		m_bPortCreated = TRUE;

		m_nDeviceId = 100;
		lock.Unlock();
	}
	catch(...)
	{
		lock.Unlock();
		TRACE("Error during COM PORT Creation.");
		//THROW(new CRFIDException((CHAR)RFID_OP_CUSTOM_ERR_CREATE_PORT_FAILED));
		return RFID_NOK;

	}
	return RFID_OK;
}

/*
	Once the SerialPort object is assigned to RFIDControler 
	object, calling process can initialize the SerailPort and
	Open for Communication.User must provide the correct COM 
	PORT number.

	INPUT : COM PORT Number
	OUTPUT: COM Port is initialized and listening
*/
INT CRFIDCtrl::InitComPort(int nPortNum)
{
	CSingleLock lock(&m_csCtrlRfidOp);
	lock.Lock();

	try
	{
		if(m_pSerialPort->IsOpen())
		{
			TRACE("COM Port is already opened");
			//THROW(new CRFIDException((CHAR)RFID_OP_CUSTOM_ERR_PORT_IS_OPEN));
			return RFID_PORT_OPENED;
		}

		m_pSerialPort->Open(nPortNum, CBR_115200);
		m_nPortId = nPortNum;
		
		COMMCONFIG stCommConfig;
		m_pSerialPort->GetConfig(stCommConfig);
		COMMTIMEOUTS stComTimeOut;
		m_pSerialPort->GetTimeouts(stComTimeOut);
		stComTimeOut.ReadTotalTimeoutMultiplier = 3;
		stComTimeOut.ReadTotalTimeoutConstant =5;
		m_pSerialPort->SetTimeouts(stComTimeOut);
		stCommConfig.dcb.BaudRate = CBR_115200;
		m_pSerialPort->SetConfig(stCommConfig); 	

		DWORD dwBaudRate = stCommConfig.dcb.BaudRate;

		m_bPortInit = TRUE;
		lock.Unlock();
	}
	catch(...)
	{
		lock.Unlock();
		TRACE("COM Port is already opened");
		//THROW(new CRFIDException((CHAR)RFID_OP_CUSTOM_ERR_PORT_OPEN_FAILED));
		return RFID_NOK;
	}

	return RFID_OK;

}

/*
	Close the exisiting COM PORT 
*/
VOID CRFIDCtrl::CloseComPort()
{
	CSingleLock lock(&m_csCtrlRfidOp);
	lock.Lock();
	if(m_pSerialPort)
	{
		m_pSerialPort->Close();
	}
	lock.Unlock();
}

/*
  SetCommand is general purpose function can be used to execute many ABx_FAST 
  communication protocol command on BIS00W4, BIS00W1 and BIS00W2 controller.
  INPUT : nCmd	     : Command ID (ABx_FAST)
		  nAddress   : Starting Address in RFID Tag to execute the Operation[Read/Write]
		  nBlockSize : Memory Block size to be manipulated [Read/Write]
		  bCheckSum  : Decide whether checksum to be included or not Command Stack
		  pData      : Pointer to Data structure to be written to  RFID Tag or to be Read	
  Read the ABx_FAST User guide to format of COMMAND_STACK
*/
INT CRFIDCtrl::SetCommand(INT nCmd, INT nAddress, INT nBlockSize, BOOL bCheckSum, CHAR *pData)
{
	CString szParameter;
	CSingleLock lock(&m_csCtrlRfidOp);
	INT nBufCount	=0;
	UINT unCheckSum =0;
	UINT unCmdSize =0 ;
	UINT unDataSize = 0;
	BOOL bConsiderAddressBlock = FALSE;
	INT nStatus = RFID_OK;

	if(m_bPortInit == FALSE)
	{
		TRACE("Serial Port is not initialized.");
		//THROW(new CRFIDException((CHAR)RFID_OP_CUSTOM_ERR_PORT_IS_NOT_CREATED));
		return RFID_NOK;
	}

	lock.Lock();
	m_pSerialPort->Purge(PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR);
	Sleep(20);

	//Command Header
	m_ucWrBuffer[nBufCount++] = ABx_FAST_CMD_HEADER;
	m_ucWrBuffer[nBufCount++] = ABx_FAST_CMD_HEADER;

	//Command Size
	//Convert two byte data into two single bytes for Command Stack
	switch(nCmd)
	{
	case ABx_FAST_CMD_FILL_TAG: 
		m_ucWrBuffer[nBufCount++] = ABx_FAST_FILL_TAG_COMMAND_SIZE >> 8;
		m_ucWrBuffer[nBufCount++] = ABx_FAST_FILL_TAG_COMMAND_SIZE & 0xFF;
		unDataSize = ABx_FAST_FILL_TAG_RESP_SIZE;
		bConsiderAddressBlock = TRUE;
		break;	
	case ABx_FAST_CMD_READ_TAG:
		m_ucWrBuffer[nBufCount++] = ABx_FAST_READ_TAG_COMMAND_SIZE >> 8;
		m_ucWrBuffer[nBufCount++] = ABx_FAST_READ_TAG_COMMAND_SIZE & 0xFF;
		unDataSize = ABx_FAST_READ_TAG_RESP_SIZE + nBlockSize;
		bConsiderAddressBlock = TRUE;
		m_szTagData.Empty();
		break;
	case ABx_FAST_CMD_WRITE_TAG:
		m_ucWrBuffer[nBufCount++] = (ABx_FAST_WRITE_TAG_COMMAND_SIZE + nBlockSize) >> 8;
		m_ucWrBuffer[nBufCount++] = (ABx_FAST_WRITE_TAG_COMMAND_SIZE + nBlockSize) & 0xFF;
		unDataSize = ABx_FAST_WRITE_TAG_RESP_SIZE;
		bConsiderAddressBlock = TRUE;
		break;	
	case ABx_FAST_CMD_READ_TAG_ID:
		m_ucWrBuffer[nBufCount++] = ABx_FAST_READ_TAG_ID_COMMAND_SIZE >>8;
		m_ucWrBuffer[nBufCount++] = ABx_FAST_READ_TAG_ID_COMMAND_SIZE & 0xFF;
		unDataSize = ABx_FAST_READ_TAGID_RESP_SIZE;
		ResetTagId();
		break; 
	case ABx_FAST_CMD_TAG_SEARCH:
		m_ucWrBuffer[nBufCount++] = ABx_FAST_TAG_SEARCH_COMMAND_SIZE >> 8;
		m_ucWrBuffer[nBufCount++] = ABx_FAST_TAG_SEARCH_COMMAND_SIZE & 0xFF;
		unDataSize = ABx_FAST_TAG_SEARCH_RESP_SIZE;
		break;
	case ABx_FAST_CMD_READ_TAGID_AND_DATA:
		m_ucWrBuffer[nBufCount++] = ABx_FAST_READ_TAGID_AND_DATA_CMD_SIZE >> 8;
		m_ucWrBuffer[nBufCount++] = ABx_FAST_READ_TAGID_AND_DATA_CMD_SIZE & 0xFF;
		unDataSize = ABx_FAST_READ_TAGID_AND_DATA_RESP_SIZE + nBlockSize;
		bConsiderAddressBlock = TRUE;
		m_szTagData.Empty();
		break;

	default:
		unDataSize = nBufCount + nBlockSize;
	}

	//CommandID
	m_ucWrBuffer[nBufCount++] = nCmd;

	//Not every command needs a Start Address and Memory Block Size
	//to be manipulated
	if(bConsiderAddressBlock == TRUE)
	{
		//Start Address
		m_ucWrBuffer[nBufCount++] = nAddress >> 8;
		m_ucWrBuffer[nBufCount++] = nAddress & 0xFF;

		//Read/Write Block Size
		m_ucWrBuffer[nBufCount++] = nBlockSize >> 8;
		m_ucWrBuffer[nBufCount++] = nBlockSize & 0xFF;
	}


	//Timeout Value
	//m_ucWrBuffer[nBufCount++] = ABx_FAST_TIMEOUT >> 8;
	//m_ucWrBuffer[nBufCount++] = ABx_FAST_TIMEOUT & 0xFF;

	m_ucWrBuffer[nBufCount++] = m_nRfidOpTimeout >> 8;
	m_ucWrBuffer[nBufCount++] = m_nRfidOpTimeout & 0xFF;
	

	//Data Byte Value, Optional
	switch(nCmd)
	{
	case ABx_FAST_CMD_FILL_TAG:
		m_ucWrBuffer[nBufCount++] = pData[0];
		break;
	case ABx_FAST_CMD_WRITE_TAG:
		//for(int i=0; i< nBlockSize; i++)
		//{
		//	m_ucWrBuffer[nBufCount++] = pData[i];
		//} 
		strncpy(&m_ucWrBuffer[nBufCount],pData, nBlockSize); 
		nBufCount += nBlockSize; 
		break;
	}


	//CheckSum ,Optional
	/*Calculate CheckSum and Apply*/
	if(bCheckSum == TRUE)
	{
		m_ucWrBuffer[nBufCount++] = GetCheckSum(nBufCount);
	}

	//Command Terminator
	m_ucWrBuffer[nBufCount++] = ABx_FAST_CMD_TERMINATOR;


	//Execute the Command
	m_pSerialPort->Write(m_ucWrBuffer,nBufCount);
	//Wait for sufficient time to complete the operation
	//Sleep(10/*(nbufcount + nblocksize)*15*/);

	lock.Unlock();
	//Check the response	
	unDataSize = ABx_FAST_RESPONSE_PACKET_HEADER_TERMINATOR + unDataSize;
	nStatus = CheckResponse(nCmd, unDataSize);
	
	return nStatus;
}


/*
	Once the command is executed, RFID controller will write response to
	serial port, it is responsibility of calling process to check the data
	at the incoming port.

	Read the ABx_FAST user guide for RESPONSE DATA STRUCTURE.
	Once Read, Check Correctness of the Data using ErrorCode and Command ID.	
*/
INT CRFIDCtrl::CheckResponse( INT nWriteCmd, UINT unDataSize)
{ 	
	CSingleLock lock(&m_csCtrlRfidOp);
	lock.Lock();
	int nIndex =0;
	DWORD dwBytesRead = 0;
	unsigned char ucIsError   = 0x30;
	unsigned char ucErrorCode = 0x30;
	try
	{
		StartPerfCounter(start4);
		ClearReadWriteBuffer();
		do
		{
			dwBytesRead = m_pSerialPort->Read(&m_ucRdBuffer,unDataSize);
			ucIsError = m_ucRdBuffer[4];
			
			//CString szTemp;
			//szTemp.Format("BytesRead =%d, cError = %d",dwBytesRead,ucIsError); 
			//CRfidUtil::GetObj().LogDebugTrace(szTemp);
			
		}while( dwBytesRead < unDataSize && (ucIsError != 0xFFFFFFFF) );
		StopPerfCounter(stop4);

		perf4 = CalcPerf(start4,stop4, overhead1);

		perf4 = (perf4/3190)/1000;

		CString szTemp;
		szTemp.Format("BytesRead =%d, cError = %d Time =%d",dwBytesRead,ucIsError, perf4);
		CRfidUtil::GetObj().LogDebugTrace(szTemp);

		//Received Error Data packet, 
		if(ucIsError == 0xFFFFFFFF) //0xFF
		{
			int x = 100;
			//handle error here
			ucErrorCode = m_ucRdBuffer[5]; 
			TRACE("Command Execution, Error Reported");
			return RFID_NOK;
		}

		//Check Command ID is reponse Data, 
		INT nReadCmd = (INT)m_ucRdBuffer[4];
		if(nWriteCmd != nReadCmd)
		{
			TRACE("Command Execution, Command ID Mismatch");
			return RFID_NOK;
		}
		m_bTagFound = TRUE;

		//Get the Response Size
		//INT nBlockSize = ((m_ucRdBuffer[2] << 8) | m_ucRdBuffer[3])
		int nBlockSize = (m_ucRdBuffer[2] * 256) + m_ucRdBuffer[3];
		nBlockSize = nBlockSize -1;
		int nStartIndex  = 5;
		//CString szTemp;
		szTemp.Format("%s",m_ucRdBuffer);
		switch(nWriteCmd)
		{
		case ABx_FAST_CMD_READ_TAG:
			{

				for(int i = nStartIndex; i<nStartIndex + nBlockSize; i++)
				{
					m_szTagData.AppendFormat("%c",m_ucRdBuffer[i]); 
				}

				//int nLength =szTemp.GetLength(); 
				//m_szTagData.Format("%s",szTemp.Right(szTemp.GetLength()-nStartIndex).Left(nBlockSize));

				
			}
			break;
		case ABx_FAST_CMD_READ_TAG_ID:	
			{
				//m_szTagID.Format("%s", szTemp.Right(szTemp.GetLength()-nStartIndex).Left(nBlockSize));
				for(int i=nStartIndex; i< nStartIndex + nBlockSize; i++)
				{
					m_ucTagID[nIndex++] = m_ucRdBuffer[i];
				}				
			}
			break;
		case ABx_FAST_CMD_READ_TAGID_AND_DATA:		
			
			for(int i=nStartIndex; i< (nStartIndex+8); i++)
			{
				m_ucTagID[nIndex++] = m_ucRdBuffer[i];
			} 			
			nBlockSize = (INT)m_ucRdBuffer[3] - 0x0001 - 0x0008;
			nStartIndex = nStartIndex + 8;			
			
			m_szTagData.Format("%s", szTemp.Right(szTemp.GetLength()-nStartIndex ).Left(nBlockSize));
			break;	
		}
	}
	catch(...)
	{
		TRACE("Command Execution, Error Reported during Fetching Data");
		lock.Unlock();
		return RFID_NOK;
	}


	return RFID_OK;
}

/*
	Calculate CheckSum for ABx Fast RFID communication 
	protocol.If the RFID controller is configured with CheckSum Enabled, 
	Command Stack must contains CheckSum value of Data being passed to 
	controller. Following alogorithm is defind by ABx_FAST protocol to
	calculate the checksum.

	CheckSum = 0xFF - (CommandSize + CommandID + StartAddress + BlockSize + TimeOut + DataBytes)
*/
UINT CRFIDCtrl::GetCheckSum(INT nBufIndex)
{
	UINT unCheckSum = 0x00;
	try
	{
		for(int i=1; i < nBufIndex; i++)
		{
		   unCheckSum += m_ucWrBuffer[i];
		}

		unCheckSum = 0xFF - unCheckSum;

	}
	catch(...)
	{
		TRACE("Error occured during CHECKSUM calculation");
	}

	return unCheckSum;

}

/*
	Clear the Read Data Buffer, call this function 
	before reading the response structure after 
	executing any SetCommand Function
*/  
void CRFIDCtrl::ClearReadWriteBuffer()
{
	memset(m_ucRdBuffer,0,BIS_RFID_MAX_BUF_SIZE);

}

/*
	General purpose function to check correctness of the 
	response data received from RFID controller.
*/
INT CRFIDCtrl::CheckForErrorResponse(unsigned char *pData)
{
	if(pData == NULL)
		return RFID_NOK;

	//Check for ErrorCode
	unsigned char cIsError = pData[4];
	unsigned char cErrorCode = 0x00;
	//Received Error Data packet, 
	if(cIsError == 0xFF) //0xFF
	{
		//handle error here
		cErrorCode = pData[5]; 
		TRACE("Get Controller Info, Error Reported");
		//THROW(new CRFIDException((CHAR)cErrorCode));
		return RFID_NOK;
	}

	return RFID_OK;
}


/*********************************************************************************
 purpose : Simplified Functions to end users.just call the 
           required functions with relavent input values and 
		   check the return status, if the return status is 
		   OK (RFID_OK), then proceed with received data.

**********************************************************************************/
INT CRFIDCtrl::SearchRfidTag()
{
	return SetCommand(ABx_FAST_CMD_TAG_SEARCH);    
}

BOOL CRFIDCtrl::SearchForRfidTag()
{
	INT nStatus = SearchRfidTag();
	if(nStatus == RFID_OK)
		return GetSearchTagStatus();
	else
		return FALSE;

}

INT CRFIDCtrl::ReadRFIDTagId()
{
	return SetCommand(ABx_FAST_CMD_READ_TAG_ID);
}



BOOL CRFIDCtrl::CheckAllPartTypesAreAllowed(int nNumOfWP, CString& szAllowedPartTypes)
{
	INT nIndex =0;

	if(m_szArrPartType.GetCount() < nNumOfWP)
		return FALSE;

	while(nNumOfWP-- > 0)
	{
		if(szAllowedPartTypes.Find(m_szArrPartType.GetAt(nIndex++)) == -1)
			return FALSE;
		
	}
	return TRUE;

}

BOOL CRFIDCtrl::IsAllPartInTheBoatSameType(int nNumOfWP, CStringArray& szArrParts)
{
	if(nNumOfWP > szArrParts.GetCount())
		return FALSE;

	CString szPart1 = szArrParts.GetAt(0);
	BOOL bIsValid = TRUE;
	INT nIndex = 1;
	while(bIsValid && nIndex < nNumOfWP)
	{
		if(szPart1.Compare(szArrParts.GetAt(nIndex++)) != 0)
			bIsValid = FALSE;
	}

	return bIsValid;
}

BOOL CRFIDCtrl::IsAllPartInTheBoatSameType(int nNumOfWP, CString szTagData)
{
	CString szPart1 = szTagData.Left(4);	
	BOOL bIsValid = TRUE;	
	while(bIsValid && nNumOfWP >1)
	{
		nNumOfWP--;
		if(szPart1.Compare(szTagData.Right(nNumOfWP * 100).Left(4)) != 0)
			bIsValid = FALSE;
	}
	return bIsValid;
}

BOOL CRFIDCtrl::CheckMaterialValidity(int nNumOfWP)
{

	INT nStartAddress = 0x00;
	INT nBlockCount = nNumOfWP * 100;

	INT nStatus = ReadRFIDTag(nStartAddress,nBlockCount);

	if(nStatus == RFID_OK)
	{
		CString szTagData = GetTagDataString(0x30);
		if(IsAllPartInTheBoatSameType(nNumOfWP,szTagData))
		{
			m_szArrPartType.RemoveAll();
			m_szArrPartBarcodes.RemoveAll();

			for(int i = nNumOfWP; i > 0 ; i--)
			{
				m_szArrPartType.Add(szTagData.Right(nNumOfWP*100).Left(4));
				m_szArrPartBarcodes.Add(szTagData.Right(nNumOfWP*100).Left(11));                		
			}

			return TRUE;
		}
		else
		{
			return FALSE;
		}

		
	}
	return FALSE;
}

BOOL CRFIDCtrl::CheckMaterialValidityV2(int nNumOfWP)
{
	INT nBlockMemSizeForPart = 100;
	INT nStartAddress = 0;
	INT nBlockCount = 4;
	INT nStatus = RFID_NOK;

	m_szArrPartType.RemoveAll();
	m_szArrPartBarcodes.RemoveAll();
	for(int i=0; i < nNumOfWP; i++)
	{
		nStartAddress = nBlockMemSizeForPart * i;
		nStatus = ReadRFIDTag(nStartAddress,nBlockCount);
		if(nStatus == RFID_OK)
		{
			CString szTagData = GetTagDataString(0x00);
			m_szArrPartType.Add(szTagData.Left(4));
		}
		else
		{
			return FALSE;
		}
	}
	return IsAllPartInTheBoatSameType(nNumOfWP,m_szArrPartType);	
}

INT CRFIDCtrl::WriteBonderIDToRfidTag(int nNumOfWP,  CHAR *pBonderID, INT nBlockOffset)
{
	INT nStatus = RFID_NOK;
	if(pBonderID == NULL)
		return RFID_NOK;

	//StartPerfCounter(start1);

	CString szTagData;
	szTagData.Format(GetTagDataString(3000));

	for(int i=0; i < nNumOfWP; i++)
	{
		szTagData.SetAt(nBlockOffset + (100*i), pBonderID[0]); 
		szTagData.SetAt(nBlockOffset+1 + (100 *i), pBonderID[1]);
	}

	memset(m_cData,0,BIS_RFID_MAX_BUF_SIZE);
	strncpy(m_cData, szTagData.GetBuffer(0),nNumOfWP*100);
	szTagData.ReleaseBuffer();

	nStatus = WriteRFIDTag(0x00, 100*nNumOfWP, 0, m_cData);

	//StopPerfCounter(stop1);
	//perf1 = CalcPerf(start1,stop1, overhead1);

	//perf1 = (perf1/3190)/1000;

	

   return nStatus;
}

INT CRFIDCtrl::WriteBonderIDToRfidTagV2(int nNumOfWP,  CHAR *pBonderID, INT nBlockOffset)
{
	INT nBlockMemSizeForPart = 100;
	INT nStartAddress = 0;
	INT nBlockCount = 2;
	INT nStatus = RFID_NOK;
	
    if(pBonderID == NULL)
		return RFID_NOK;

	//StartPerfCounter(start1);
	for(int i=0; i < nNumOfWP; i++)
	{
		nStartAddress = i * nBlockMemSizeForPart + nBlockOffset;
		nStatus = WriteRFIDTag(nStartAddress, nBlockCount, 0, pBonderID);
		if(nStatus == RFID_NOK)
		{
			return FALSE;
		}
	}

	//StopPerfCounter(stop1);
	//perf1 = CalcPerf(start1,stop1, overhead1);
	//perf1 = (perf1/3190)/1000;

	return nStatus;
}

INT CRFIDCtrl::ReadBonderIDFromRfidTag(int nNumOfWP,  CHAR *pBonderID, INT nBlockOffset)
{
	INT nStatus = RFID_NOK;
	INT nStartAddress = 0x00;
	INT nBlockCount = nNumOfWP * 100;

	nStatus = ReadRFIDTag(nStartAddress,nBlockCount);
	if(nStatus == RFID_OK)
	{
		CString szTagData = GetTagDataString(0x30);
		CString szBonderID = szTagData.Right(100).Left(nBlockOffset +2).Right(2);

		pBonderID[0] = szBonderID[0];
		pBonderID[1] = szBonderID[1];
	}
	else
	{
	  nStatus = RFID_NOK;
	}

	return nStatus;

}
INT CRFIDCtrl::ReadLastWrittenBonderIDFromRfidTag(int nNumOfWP,  CHAR *pBonderID, INT nBlockOffset)
{
	INT nStatus = RFID_NOK;
	INT nStartAddress = (100*(nNumOfWP -1))+nBlockOffset;
	INT nBlockCount = 10; //Cannot read 2 byte exactly, cause some issues with null terminator
	nStatus = ReadRFIDTag(nStartAddress,nBlockCount);
	if(nStatus == RFID_OK)
	{
		CString szTagData = GetTagDataString(0x00);
		CString szBonderID = szTagData.Left(2);
		if(szBonderID.GetLength()==2)
		{
			pBonderID[0] = szBonderID[0];
			pBonderID[1] = szBonderID[1];
		}
		else
		{
			nStatus = RFID_NOK;
		}
	}
	else
	{
	  nStatus = RFID_NOK;
	}
	return nStatus;
}

BOOL CRFIDCtrl::ValidateBonderIDAtRFIDTag(int nNumOfWP, const CHAR *pBonderID, INT nBlockOffset)
{
	CHAR cBonderID[3] = {'0','0','\0'};
	//INT nStatus = ReadBonderIDFromRfidTag(nNumOfWP,cBonderID, nBlockOffset);
	INT nStatus = ReadLastWrittenBonderIDFromRfidTag(nNumOfWP,cBonderID, nBlockOffset);
	if(nStatus == RFID_OK)
	{
		CString szInputBonderID, szOutputBonderID;
		szInputBonderID.Format("%s", pBonderID);
		szOutputBonderID.Format("%s", cBonderID);
		if(szInputBonderID.Compare(szOutputBonderID)== 0)
            return TRUE;
		else
			return FALSE;
	}
	else
	{
		return FALSE;
	}
}

CString CRFIDCtrl::GetPartTypeInfo(int nPartIndex)
{
	return m_szArrPartType.GetAt(nPartIndex);
}

CString CRFIDCtrl::GetPartBarcode(int nPartIndex)
{
	return m_szArrPartBarcodes.GetAt(nPartIndex);
}



INT CRFIDCtrl::ReadRFIDTag(INT nAddress, INT nCount, INT nChannel, CHAR *pData)
{
	return SetCommand(ABx_FAST_CMD_READ_TAG, nAddress, nCount);


}
INT CRFIDCtrl::WriteRFIDTag(INT nAddress, INT nCount, INT nChannel, CHAR *pData)
{
	return SetCommand(ABx_FAST_CMD_WRITE_TAG, nAddress, nCount, FALSE, pData);

}

//Set the Search RFID Tag event result
void CRFIDCtrl::SetSearchTagStatus(BOOL bTagFound)
{
	m_bTagFound = bTagFound;
}
//Get the Search RFID Tag Event result
BOOL CRFIDCtrl::GetSearchTagStatus()
{
	BOOL bTagFound = m_bTagFound;
	//once get the result, set the status to FALSE for
	//next search tag operation
	SetSearchTagStatus(FALSE);
	return bTagFound;
}

//Get the Tag Data as a CString variable
CString CRFIDCtrl::GetTagDataString(INT nCount)
{
    return m_szTagData;	
}

/*	Get the Tag ID, convert the Binary Data into
	HEX string 
*/
CString CRFIDCtrl::GetTagId()
{
	CString szTagIdHex;
	for(int i=0; i < 8; i++)
	{
		CString szTemp;
		szTemp.Format(_T("%02X"), (unsigned char)m_ucTagID[i]);
		szTagIdHex +=szTemp;
	}
	return szTagIdHex;
	
}

/*
	Reset The Tag ID at application level, not at RFID Tag.
	RFID Tag ID cannot be modified and fixed value during the 
	production.
*/
VOID CRFIDCtrl::ResetTagId()
{
    for(int i=0; i < 8; i++)
	{
		m_ucTagID[i] = ' ';
	}
}

VOID CRFIDCtrl::SetTimeoutValue(INT nTimeout)//Seconds
{
	m_nRfidOpTimeout = nTimeout * 1000;
}

/*********************************************************************************/ 


/*
	Special Function to Get the information of the controller with 
	ABx_FAST command protocol support.

	READ the ABx_FAST protocol user guide for how to decode the received 
	data into controller information.
*/
CString CRFIDCtrl::GetControllerInfo()
{
	//Read Buffer
	unsigned char ucRdCtrlInfo[48];
	//Write Buffer
	CHAR ucWrCtrlInfo[] = {
		ABx_FAST_CMD_HEADER,ABx_FAST_CMD_HEADER, /*Command Header     0x02, 0x02*/
		0x00,0x01,                               /*Command Size       0x0001    */
		ABx_FAST_CMD_GET_CONTROLLER_INFO,        /*Command ID         0x38      */
		ABx_FAST_CMD_TERMINATOR};				 /*Command Terminator 0x03      */

	if(m_bPortInit == FALSE)
	{
		TRACE("Serial Port is not initialized.");
		//THROW(new CRFIDException());
	}

	CSingleLock lock(&m_csCtrlRfidOp);
	//try
	//{
		lock.Lock();
		m_pSerialPort->Purge(PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR);

		//Execute the Command
		m_pSerialPort->Write(ucWrCtrlInfo,6);
		//Wait for sufficient time to complete the operation
		Sleep(6 *15);
		//Check the response	
		m_pSerialPort->Read(&ucRdCtrlInfo,32);

		//Check for Error Response
		if(CheckForErrorResponse(ucRdCtrlInfo) == RFID_NOK)
			return "Error Reported";

		//Convert Byte Array to Control Info structure
		int nIndex = 5;
		//Controller Type
		m_stRfidControllerInfo.cControllerType = ucRdCtrlInfo[nIndex++];

		//Controller Software Infomation
		//Sw Version = Major.Minor.Correction.Point
		m_stRfidControllerInfo.cSwVerMajor     = ucRdCtrlInfo[nIndex++];
		m_stRfidControllerInfo.cSWVerMinor     = ucRdCtrlInfo[nIndex++];
		m_stRfidControllerInfo.cSwVerCorrection= ucRdCtrlInfo[nIndex++];
		m_stRfidControllerInfo.cSwVerPoint     = ucRdCtrlInfo[nIndex++];

		//Reserved Byte
		m_stRfidControllerInfo.cReserved = ucRdCtrlInfo[nIndex++];

		//Software CRC
		m_stRfidControllerInfo.cCRC[0] = ucRdCtrlInfo[nIndex++];
		m_stRfidControllerInfo.cCRC[1] = ucRdCtrlInfo[nIndex++];

		//Loader Software version
		m_stRfidControllerInfo.cLoaderSwVer[0] = ucRdCtrlInfo[nIndex++];
		m_stRfidControllerInfo.cLoaderSwVer[1] = ucRdCtrlInfo[nIndex++];

		//Processor ID
		for(int i = 0; i < 5; i++)
			m_stRfidControllerInfo.cProcessorId[i] = ucRdCtrlInfo[nIndex++];

		//Processor RFU
		for(int i=0; i<3; i++)
			m_stRfidControllerInfo.cProcessorRfu[i] = ucRdCtrlInfo[nIndex++];

		//Processor Serial Number
		for(int i=0; i<4; i++)
			m_stRfidControllerInfo.cProcessorSerialNo[i] = ucRdCtrlInfo[nIndex++];

		//Processor Internal Info
		for(int i=0; i<2; i++)
			m_stRfidControllerInfo.cProcessorInternalInfo[i] = ucRdCtrlInfo[nIndex++];

		//Processor RsMaxP
		m_stRfidControllerInfo.cProcessorRsMaxP = ucRdCtrlInfo[nIndex++];

		//Processor Information CRC 
		m_stRfidControllerInfo.cProcessorInfoCRC = ucRdCtrlInfo[nIndex++];

		//Check Sum
		m_stRfidControllerInfo.cCheckSum = ucRdCtrlInfo[nIndex++];



		//Get RFID Controller Info , as String Data
		m_stRfidControllerInfo.szControllerType = GetControllerTypeName(m_stRfidControllerInfo.cControllerType);

		m_stRfidControllerInfo.szSwVer.Format(_T("%d.%d.%c.%d"),m_stRfidControllerInfo.cSwVerMajor,
			m_stRfidControllerInfo.cSWVerMinor,
			m_stRfidControllerInfo.cSwVerCorrection,
			m_stRfidControllerInfo.cSwVerPoint);
		m_stRfidControllerInfo.szSwVer.MakeUpper();


		m_stRfidControllerInfo.szSerialNo.Format(_T("%02x%02x%02x%02x"),
			(unsigned char)m_stRfidControllerInfo.cProcessorSerialNo[0],
			(unsigned char)m_stRfidControllerInfo.cProcessorSerialNo[1],
			(unsigned char)m_stRfidControllerInfo.cProcessorSerialNo[2],
			(unsigned char)m_stRfidControllerInfo.cProcessorSerialNo[3]);
		m_stRfidControllerInfo.szSerialNo.MakeUpper();


		m_stRfidControllerInfo.szFirmWareCrc.Format(_T("%02x%02x"),
			(unsigned char)m_stRfidControllerInfo.cCRC[0],
			(unsigned char)m_stRfidControllerInfo.cCRC[1]);
		m_stRfidControllerInfo.szFirmWareCrc.MakeUpper();														


		CString szTemp;
		szTemp.Format(_T("%02x%02x"),
			(unsigned char)m_stRfidControllerInfo.cLoaderSwVer[0],
			(unsigned char)m_stRfidControllerInfo.cLoaderSwVer[1]); 
		m_stRfidControllerInfo.szLoaderSwVer.Format(_T("%c.%c.%c.%c"),szTemp[0], szTemp[1], szTemp[2], szTemp[3]);
		m_stRfidControllerInfo.szLoaderSwVer.MakeUpper();
	//}
	//catch(CRFIDException *e)
	//{
		lock.Unlock();
		//THROW(new CRFIDException((CHAR)RFID_OP_CUSTOM_ERR_ERROR_DURING_CMD_EXECU));
	//	AfxMessageBox(e->GetErrorMessage());
	//}   


	CString szCtrlInfo;
	szCtrlInfo.Format(_T("Type: %s\n SwVer: %s\n FirmwareCRC: %s\n SerialNo: %s\n LoaderSwVer: %s\n")
					  ,m_stRfidControllerInfo.szControllerType
					  ,m_stRfidControllerInfo.szSwVer
					  ,m_stRfidControllerInfo.szFirmWareCrc
					  ,m_stRfidControllerInfo.szSerialNo
					  ,m_stRfidControllerInfo.szLoaderSwVer);
	lock.Unlock();
	return szCtrlInfo;


}

/********************************************************************************
Special Functions to Get/Set the configuration of the controller which supports
ABx_FAST command protocol support.

READ the ABx_FAST protocol user guide for how to decode the received 
data into controller configuration.
*********************************************************************************/
VOID CRFIDCtrl::GetControllerConfiguration(CHAR cControllerType)
{
	//Read Buffer
	unsigned char ucRdCtrlInfo[32] = {0};
	//Write Buffer
	CHAR ucWrCtrlInfo[] = {
		ABx_FAST_CMD_HEADER,ABx_FAST_CMD_HEADER,  /*Command Header     0x02, 0x02*/
		0x00,0x02,                                /*Command Size       0x0001    */
        ABx_FAST_CMD_GET_CONTROLLER_CONFIGURATION,/*Command ID         0x38      */
		0x01,                                     /*Controller Type    0x03      */ //0x03- BIS_M_410
		ABx_FAST_CMD_TERMINATOR};				  /*Command Terminator 0x03      */
	
	CSingleLock lock(&m_csCtrlRfidOp);

	if(m_bPortInit == FALSE)
	{
		TRACE("Serial Port is not initialized.");
		THROW(new CRFIDException());
	}

	try{

		lock.Lock();
		//Execute the Command
		m_pSerialPort->Write(ucWrCtrlInfo,6);
		//Wait for sufficient time to complete the operation
		Sleep(6 *15);
		//Check the response	
		m_pSerialPort->Read(&ucRdCtrlInfo,26);

		//Check for Error Response
		if(CheckForErrorResponse(ucRdCtrlInfo) == RFID_NOK)
			return;

		//Received Correct Reponse, process the data
		int x= 0;
	}
	catch(CException *e)
	{
		lock.Unlock();

	}

	lock.Unlock();

}

/********************************************************************************
Special Functions to Get/Set the configuration of the controller which supports
ABx_FAST command protocol support.

READ the ABx_FAST protocol user guide for how to decode the received 
data into controller configuration.
*********************************************************************************/
VOID CRFIDCtrl::SetControllerConfiguration()
{

}


/********************************************************************
Decode the communcation mode, many communication information are 
packed into a single byte using indiviual bits,this function helps to
decode the communication modes by manipulating each bits
**********************************************************************/

//Decode Communication Mode
VOID CRFIDCtrl::DecodeCommunicationMode(CHAR cCommMode, LPCOMM_MODE lpCommMode)
{
	lpCommMode->cBaudRate =  (unsigned char)cCommMode & 0x07;
	lpCommMode->cXON      =  ((unsigned char)cCommMode & 0x08) >> 3;
	lpCommMode->cReserved =  ((unsigned char)cCommMode & 0x30) >> 4;
	lpCommMode->cInterface=  ((unsigned char)cCommMode & 0xC0) >> 6;
}


//Decode Baud Rate for SeriaPprt Communication
INT CRFIDCtrl::GetBaudRate(unsigned char ucBaudRateCode)
{
	INT nBaudRate = 9600;
	switch(ucBaudRateCode)
	{
	case 0: nBaudRate = 9600;  break;
	case 1: nBaudRate = 19200; break;
	case 2: nBaudRate = 38400; break;
	case 3: nBaudRate = 57600; break;
	case 4: nBaudRate = 115200; break;
	default:
		nBaudRate = 9600;
	}
	return nBaudRate;
}

//Get the Device Interface Type (RS232, USB,RS422 or RS485)
INT CRFIDCtrl::GetDeviceInterfaceType(unsigned char ucInterfaceType)
{
	INT nDeviceInterfaceType = RFID_COM_RS232_USB;

	switch(ucInterfaceType)
	{
	case 0: nDeviceInterfaceType = RFID_COM_RS422; break;
	case 1: nDeviceInterfaceType = RFID_COM_RS232_USB; break;
	case 2: nDeviceInterfaceType = RFID_COM_RS485; break;
	case 3: nDeviceInterfaceType = RFID_COM_OTHER; break;		
	}

	return nDeviceInterfaceType;

}
//Get the XON / XOFF State
BOOL CRFIDCtrl::GetXOnState(unsigned char ucXonXoffState)
{
	if(ucXonXoffState == 0)
		return FALSE; //Disabled
	else
		return TRUE; //Enabled 

}

/*
	Auxillary Function to Get The controller Name
	based on controller type received from Controller
	Informaton.

	More Details : ABx_FAST Communication protocol
*/
CString CRFIDCtrl::GetControllerTypeName(CHAR cControllerType)
{
	CString szCtrlName("Not Known");
	if(cControllerType == BIS_M_62_)
		szCtrlName = CTRL_TYPE_BIS_M_62;
	else if(cControllerType == BIS_M_410_)
		szCtrlName = CTRL_TYPE_BIS_M_410;
	else if(cControllerType == BIS_M_411_)
		szCtrlName = CTRL_TYPE_BIS_M_411;
	else if(cControllerType == BIS_U_62_)
		szCtrlName = CTRL_TYPE_BIS_U_62;
	else
		szCtrlName = CTRL_TYPE_BIS_M_410;

	return szCtrlName;
}

/*******************************************************************************/

INT CRFIDCtrl::ResetRfidController()
{
	//Read Buffer
	unsigned char ucRdCtrlInfo[48];
	//Write Buffer
	CHAR ucWrCtrlInfo[] = {
		ABx_FAST_CMD_HEADER,ABx_FAST_CMD_HEADER, /*Command Header     0x02, 0x02*/
		0x00,0x01,                               /*Command Size       0x0001    */
		ABx_FAST_CMD_RESET_CONTROLLER,           /*Command ID         0x38      */
		ABx_FAST_CMD_TERMINATOR};				 /*Command Terminator 0x03      */

	if(m_bPortInit == FALSE)
	{
		TRACE("Serial Port is not initialized.");
		//THROW(new CRFIDException());
	}

	CSingleLock lock(&m_csCtrlRfidOp);
	lock.Lock();

	m_pSerialPort->Purge(PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR);

	//Execute the Command
	m_pSerialPort->Write(ucWrCtrlInfo,6);
	//Wait for sufficient time to complete the operation
	Sleep(6 *15);
	//Check the response	
	m_pSerialPort->Read(&ucRdCtrlInfo,32);

	//Check for Error Response
	return CheckForErrorResponse(ucRdCtrlInfo);
}

