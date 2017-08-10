
// MFCApplication1Dlg.h : header file
//

#pragma once
#include "afxwin.h"


// CMFCApplication1Dlg dialog
class CMFCApplication1Dlg : public CDialogEx
{
// Construction
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//virtual BOOL PreTranslateMessage(MSG* pMsg);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnStnClickedStatic1();
	//afx_msg void OnBnClickedOk();
	//afx_msg void OnBnClickedexit();
	//afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedSendBn();
	//afx_msg void OnEnUpdateEditSend();
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
