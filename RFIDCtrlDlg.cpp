// RFIDCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RFIDCtrl.h"
#include "RFIDCtrlDlg.h"
#include "RFIDApi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

unsigned __int64 start5 = 0, stop5 = 0,    perf5 = 0;

unsigned __int64  overhead2 = CalcOverhead(start5,stop5);    //get the overhead
//clock_t cs1, cs2,cst1, cst2;
//@End
////////////////////////////////////////////////////////////////////////////////


// CAboutDlg dialog used for App About
void ucCharTestFunc(unsigned char *pCmd);
void TestMCWMeter();

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRFIDCtrlDlg dialog



CRFIDCtrlDlg::CRFIDCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRFIDCtrlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRFIDCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LBL_TEST, m_testData);
}

BEGIN_MESSAGE_MAP(CRFIDCtrlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_READ, OnBnClickedBtnRead)
	ON_BN_CLICKED(IDC_BTN_WRITE, OnBnClickedBtnWrite)
	ON_BN_CLICKED(IDC_BTN_READ_ID, OnBnClickedBtnReadId)
	ON_BN_CLICKED(IDC_BTN_TAG_SEARCH, OnBnClickedBtnTagSearch)
	ON_BN_CLICKED(IDC_BTN_CTRL_INFO, OnBnClickedBtnCtrlInfo)
	ON_BN_CLICKED(IDC_BTN_CTRL_COnfig, OnBnClickedBtnCtrlConfig)
	ON_BN_CLICKED(IDC_BTN_WRITE2, OnBnClickedBtnWrite2)
END_MESSAGE_MAP()


// CRFIDCtrlDlg message handlers

BOOL CRFIDCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_rfidCtrl.Create(&m_serialPort);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRFIDCtrlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRFIDCtrlDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRFIDCtrlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRFIDCtrlDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	m_rfidCtrl.InitComPort(3);
	m_rfidCtrl.SetTimeoutValue(10);

}

void CRFIDCtrlDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	/*INT nStatus = RFID_NOK; 
	CHAR cBonderID[] =  {'0','0', '\0'};
	CHAR ccBonderID[] = {'0','0', '\0'};

	INT  nMemOffset = 34;
	CString szBonderID = "88";

	for(int i=0; i<2; i++)
	{
		cBonderID[i] = szBonderID[i];
		ccBonderID[i] = szBonderID[i];
	}

	nStatus = m_rfidCtrl.WriteBonderIDToRfidTag(3, cBonderID,nMemOffset);
	if(nStatus == RFID_OK)
	{		
		if(!m_rfidCtrl.ValidateBonderIDAtRFIDTag(3,cBonderID,nMemOffset))
		{
			CStringArray szArrMsg;
			szArrMsg.Add("Bonder ID is not written properly to RFID Tag.");
			AfxMessageBox(szArrMsg.GetAt(0));
				
		}


	}
	*/

	CString szTemp;
	CRfidUtil::GetObj().LogDebugTrace("NEW Method");
	StartPerfCounter(start5);
	nCheckMaterialValidityForBoschV2();
	StopPerfCounter(stop5);
	perf5 = CalcPerf(start5,stop5, overhead2);

	perf5 = (perf5/3190)/1000;
	szTemp.Format("****Total Time =%d", perf5);
	CRfidUtil::GetObj().LogDebugTrace(szTemp);


	CRfidUtil::GetObj().LogDebugTrace("OLD Method");

	StartPerfCounter(start5);
	nCheckMaterialValidityForBosch();
	StopPerfCounter(stop5);
	perf5 = CalcPerf(start5,stop5, overhead2);
	perf5 = (perf5/3190)/1000;
	szTemp.Format("****Total Time =%d", perf5);
	CRfidUtil::GetObj().LogDebugTrace(szTemp);
	
	unsigned char pCmd[] = "@A";

	//ucCharTestFunc(pCmd);

	//TestMCWMeter();
	
	
}

void CRFIDCtrlDlg::OnBnClickedBtnRead()
{
	// TODO: Add your control notification handler code here
	
	if(m_rfidCtrl.ReadRFIDTag(0x0000,300) == RFID_OK)
	{

		CString szTagData = m_rfidCtrl.GetTagDataString(0x0028);

		CString szPart1 = szTagData.Left(40);
		CString szPart2 = szTagData.Right(200).Left(40);
		CString szPart3 = szTagData.Right(100).Left(40);

		const CHAR cID1[] = {'4', '4', '\0'};


		CString szBonderID = szTagData.Left(34 +2).Right(2);
		CHAR cID2[3];
		cID2[0] = szBonderID[0];
		cID2[1] = szBonderID[1];
		cID2[2] = '\0';


		CString sz1, sz2;
		sz1.Format("%s", cID1);
		sz2.Format("%s", cID2);

		int n  = sz1.Compare(sz2);

		n = strcmp(cID1, cID2);

		m_testData.SetWindowText(m_rfidCtrl.GetTagDataString(0x0028));
	}
	else
	{
		AfxMessageBox("Read Operation Failed");
	}
	
}

void CRFIDCtrlDlg::OnBnClickedBtnWrite()
{
	// TODO: Add your control notification handler code here
	CHAR cData[40];
	CString szTestData = "This is a TEST Data which I am going to Write to RFID Tag";
	for(int i=0; i < 40; i++)
	{
		cData[i] = szTestData[i];
	}

	CHAR cBonderId[] = {'H','H','\0'};
	INT nBlockOffset = 34;
	for(int i =0; i <3 ; i++)
	{
		m_rfidCtrl.WriteRFIDTag(nBlockOffset + 100 * i ,0x0002,0,cBonderId);
	}

}

void CRFIDCtrlDlg::OnBnClickedBtnReadId()
{
	// TODO: Add your control notification handler code here
	m_rfidCtrl.ReadRFIDTagId();
	m_testData.SetWindowText(m_rfidCtrl.GetTagId());

	
}

void CRFIDCtrlDlg::OnBnClickedBtnTagSearch()
{
	// TODO: Add your control notification handler code here
	//m_rfidCtrl.SearchRfidTag();
	if(m_rfidCtrl.SearchForRfidTag())
		AfxMessageBox("Tag Found");
	else
		AfxMessageBox("Tag Not Found");
}

void CRFIDCtrlDlg::OnBnClickedBtnCtrlInfo()
{
	// TODO: Add your control notification handler code here
	m_testData.SetWindowText(m_rfidCtrl.GetControllerInfo());
}

void CRFIDCtrlDlg::OnBnClickedBtnCtrlConfig()
{
	// TODO: Add your control notification handler code here
	m_rfidCtrl.GetControllerConfiguration(BIS_M_410_);
}

INT StringTestFunc();
void CRFIDCtrlDlg::OnBnClickedBtnWrite2()
{
	// TODO: Add your control notification handler code here
	//m_rfidCtrl.ResetRfidController();
	StringTestFunc();
}



BOOL gbRFIDTagFound, gbIsValidWPC;
CString gszWorkPieceType="";
CString gszAllowedPartTypes = "6666;GCQX;0000;AAAA;";
INT gnMemOffsetForBonderID = 38;
CString gszBonderID="MM";
INT gnNumOfWP = 4;


INT CRFIDCtrlDlg::nWriteBonderIDToRfidTagAndValidate(int nMemOffset, CString szBonderID, BOOL bIsTest)
{
	INT nStatus = RFID_NOK; 
	CHAR cBonderID[] =  {'0','0', '\0'};
	
	for(int i=0; i<2; i++)
	{
		cBonderID[i] = szBonderID[i];
	}

	nStatus = m_rfidCtrl.WriteBonderIDToRfidTag(gnNumOfWP, cBonderID,nMemOffset);	
	if(nStatus == RFID_OK)
	{		
		if(!m_rfidCtrl.ValidateBonderIDAtRFIDTag(gnNumOfWP,cBonderID,nMemOffset))
		{
			if(bIsTest)
			{
				CStringArray szArrMsg;
				szArrMsg.Add("Bonder ID is not written properly to RFID Tag.");
				//HM_DisplayErrorMessageBox("RFID CONTROLLER", szArrMsg, enUtilMsgOK, DLG_MSG_NULL, KEY_ENTER, KEY_A);
				AfxMessageBox(szArrMsg.GetAt(0));
			}
			nStatus = RFID_NOK;				
		}  
	}
	return nStatus;
}

INT CRFIDCtrlDlg::nWriteBonderIDToRfidTagAndValidateV2(int nMemOffset, CString szBonderID, BOOL bIsTest)
{
	INT nStatus = RFID_NOK; 
	CHAR cBonderID[] =  {'0','0', '\0'};
	
	for(int i=0; i<2; i++)
	{
		cBonderID[i] = szBonderID[i];
	} 
	//nStatus = m_rfidCtrl.WriteBonderIDToRfidTag(gnNumOfWP, cBonderID,nMemOffset);
	nStatus = m_rfidCtrl.WriteBonderIDToRfidTagV2(gnNumOfWP, cBonderID,nMemOffset);
	if(nStatus == RFID_OK)
	{		
		if(!m_rfidCtrl.ValidateBonderIDAtRFIDTag(gnNumOfWP,cBonderID,nMemOffset))
		{
			if(bIsTest)
			{
				CStringArray szArrMsg;
				szArrMsg.Add("Bonder ID is not written properly to RFID Tag.");
				//HM_DisplayErrorMessageBox("RFID CONTROLLER", szArrMsg, enUtilMsgOK, DLG_MSG_NULL, KEY_ENTER, KEY_A);
				AfxMessageBox(szArrMsg.GetAt(0));
			}
			nStatus = RFID_NOK;				
		}  
	}
	return nStatus;
}






void ucCharTestFunc(unsigned char *pCmd)
{
	unsigned char pBuf[20];
	int i =0;

	while( *(pCmd + i) != NULL)
	{
		pBuf[i++] = *(pCmd + i);

	}

	pBuf[i++] = 0x0D;
	pBuf[i++] = 0x0A;
	pBuf[i++] = '\0';

	CString szTemp;
	szTemp.Format("%s", pBuf);
}



void TestMCWMeter()
{
	CSerialPort serialport;
	serialport.Open(3,9600, CSerialPort::Parity::OddParity,8, CSerialPort::StopBits::OneStopBit,CSerialPort::FlowControl::XonXoffFlowControl);

	COMMCONFIG stCommConfig;
	serialport.GetConfig(stCommConfig);
	COMMTIMEOUTS stComTimeOut;
	serialport.GetTimeouts(stComTimeOut);
	stComTimeOut.ReadTotalTimeoutMultiplier = 50;
	stComTimeOut.ReadTotalTimeoutConstant = 100;
	serialport.SetTimeouts(stComTimeOut);

	unsigned char cmd[] = {'@','A', 0x0D, 0x0A};

	serialport.Write(cmd, 2);
	Sleep(2000);

	unsigned char rcBuf[100];
	int nByteReceived = 0;
	int i =0;
	
	nByteReceived = serialport.Read(rcBuf,20);
	

	rcBuf[21] = '\0';
	CString szTemp(rcBuf);

	CStdioFile file;
	CFileException exce;

	file.Open("C:\\System\\Test.txt", CFile::modeCreate|CFile::modeWrite|CFile::typeText, &exce);
	
	file.WriteString(szTemp);

	file.Close();
}

BOOL IsAllPartInTheBoatSame(int nNumOfWP, CString szTagData);
INT StringTestFunc()
{
	CString szTemp = "AAAA0000000000BAAAA0000000000BAAAA0000000000BAAAA0000000000B";

	
	BOOL bValid = IsAllPartInTheBoatSame(4, szTemp); 
	
	CString sz = szTemp.Right(szTemp.GetLength()-5).Left(40);

	sz = szTemp.Right(10).Left(1);
	return 1;
}

BOOL IsAllPartInTheBoatSame(int nNumOfWP, CString szTagData)
{
	CString szPart1 = szTagData.Left(4);
	
	BOOL bIsValid = TRUE;
	
	while(bIsValid && nNumOfWP >1)
	{
		nNumOfWP--;
		if(szPart1.Compare(szTagData.Right(nNumOfWP * 15).Left(4)) != 0)
			bIsValid = FALSE;
	}

	return bIsValid;
}






