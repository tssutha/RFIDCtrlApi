// RFIDCtrlDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "RFIDApi.h"
#include "SerialPort.h"


// CRFIDCtrlDlg dialog
class CRFIDCtrlDlg : public CDialog
{
// Construction
public:
	CRFIDCtrlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RFIDCTRL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CRFIDCtrl m_rfidCtrl;
	CSerialPort m_serialPort;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_testData;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedBtnRead();
	afx_msg void OnBnClickedBtnWrite();
	afx_msg void OnBnClickedBtnReadId();
	afx_msg void OnBnClickedBtnTagSearch();
	afx_msg void OnBnClickedBtnCtrlInfo();
	afx_msg void OnBnClickedBtnCtrlConfig();
	INT nCheckMaterialValidityForBosch();
	INT nCheckMaterialValidityForBoschV2();
	INT nWriteBonderIDToRfidTagAndValidate(int nMemOffset, CString szBonderID, BOOL bIsTest);
	INT nWriteBonderIDToRfidTagAndValidateV2(int nMemOffset, CString szBonderID, BOOL bIsTest);
	afx_msg void OnBnClickedBtnWrite2();
};
