
// MFCApplication1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "afxsock.h"
#include <cstring>
#include "Player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About
bool ifStop = false;
CWinThread *pCCUThread = NULL;
int nChatPosition = 0;
CString strChatPosition = { "\0" };
CSocket clientSocket;
CSocket ccuSocket;
CString strIP;
CString strPort;
int nPort;
CString strRecText;
CString strSendText = { "\0" };
Player player = Player(_T("lxk"), _T("123"), _T("Admin"));
CEdit CEDIT2;
const char signalChar = '/';
DWORD RUN_ERROR;

unsigned int CheckChatUpdate(LPVOID lParam)
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	CMFCApplication1Dlg *clientDlg = (CMFCApplication1Dlg *)lParam;
	clientDlg->GetDlgItem(IDC_EDIT_IP)->GetWindowText(strIP);
	clientDlg->GetDlgItem(IDC_EDIT_PORT)->GetWindowText(strPort);
	nPort = _ttoi(strPort);
	while (!ifStop)
	{
		if (!ccuSocket.Create())
		{
			CString szMsg;
			szMsg = "CCU Create Failed: " + clientSocket.GetLastError();
			AfxMessageBox(szMsg);
			return FALSE;
		}
		if (ccuSocket.Connect(strIP, nPort))
		{
			char strRecValue[1024] = { "\0" };

			char temp[5] = { "\0" };
			_itoa_s(nChatPosition, temp, 5, 10);
			strChatPosition = temp; 
			strChatPosition.Insert(0, player.getNickname() + ": " + signalChar);
			strChatPosition += "\r\n";//create str chat position

			ccuSocket.Send(strChatPosition, strChatPosition.GetLength());
			strChatPosition = "\0";
			ccuSocket.Receive((void *)strRecValue, 1024);
			if (strRecValue[0] != '\0')
			{
				strRecText += strRecValue;
				//clientDlg->GetDlgItem(IDC_EDIT2)->SetWindowText("");
				//clientDlg->Invalidate(FALSE);
				clientDlg->GetDlgItem(IDC_EDIT2)->SetWindowText(strRecText);
				CEDIT2.LineScroll(CEDIT2.GetLineCount());
			}
			char tempPosition[5] = { "\0" };
			ccuSocket.Receive((void *)tempPosition, 1024);
			nChatPosition = atoi(tempPosition);
			ccuSocket.Close();
		}
		else
		{
			char szMsg[1024];
			sprintf_s(szMsg, "CCU Failed: %d", clientSocket.GetLastError());
			AfxMessageBox(szMsg);
			return FALSE;
		}
		Sleep(500);
	}
	return TRUE;
}


void ConnectCCU(CMFCApplication1Dlg *const thisWnd)
{
	ifStop = false;
	pCCUThread = new CWinThread(CheckChatUpdate, (LPVOID)thisWnd);
	pCCUThread->CreateThread(CREATE_SUSPENDED);
	pCCUThread->ResumeThread();
	//if (!GetExitCodeThread(pCCUThread, &RUN_ERROR))
	//{
	//	//pCCUThread->SuspendThread();
	//	//delete pCCUThread;
	//	//pCCUThread = NULL;
	//	AfxMessageBox("CCUThread failed.");
	//}
	//else
	//{
	//	AfxMessageBox("CCUThread started.");
	//}
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg dialog



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, CEDIT2);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(btn_exit, &CMFCApplication1Dlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMFCApplication1Dlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(BTN_SEND, &CMFCApplication1Dlg::OnBnClickedSendBn)
	ON_WM_KEYDOWN()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMFCApplication1Dlg message handlers
BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	//ShowWindow(SW_MAXIMIZE);

	// TODO: Add extra initialization here
	::SetWindowPos(this->m_hWnd, HWND_BOTTOM, 0, 0, 800, 600, SWP_NOZORDER);

	CFont* font1 = new CFont;
	font1->CreateFont(
		125,            // nHeight 
		40,           // nWidth 
		0,           // nEscapement 
		0,           // nOrientation 
		FW_BOLD,     // nWeight 
		FALSE,        // bItalic 
		FALSE,       // bUnderline 
		0,           // cStrikeOut 
		ANSI_CHARSET,              // nCharSet 
		OUT_DEFAULT_PRECIS,        // nOutPrecision 
		CLIP_DEFAULT_PRECIS,       // nClipPrecision 
		DEFAULT_QUALITY,           // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("微软雅黑")
	);
	GetDlgItem(IDC_STATIC1)->SetFont(font1);
	GetDlgItem(IDC_STATIC2)->SetFont(font1);
	
	GetDlgItem(IDC_EDIT_PORT)->SetWindowText("61616");
	GetDlgItem(IDC_EDIT_IP)->SetWindowText("192.168.1.103");
	
	ConnectCCU(this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCApplication1Dlg::OnPaint()
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
		CPaintDC dc(this);
		CDC memodc;
		CBitmap hBmpbg;
		CRect ClientRect;
		GetClientRect(&ClientRect);
		CImage bgimage;
		bgimage.Load(_T("townbar.jpg"));
		hBmpbg.CreateCompatibleBitmap(&dc, ClientRect.Width(), ClientRect.Height());
		memodc.CreateCompatibleDC(&dc);
		CBitmap* OldBitMap = memodc.SelectObject(&hBmpbg); //设置位图
		bgimage.StretchBlt(memodc, 0, 0, ClientRect.Width(), ClientRect.Height(), 0, 0, bgimage.GetWidth(), bgimage
			.GetHeight(), SRCCOPY);
		dc.BitBlt(0, 0, ClientRect.Width(), ClientRect.Height(), &memodc, 0, 0, SRCCOPY);
		memodc.SelectObject(OldBitMap);
		memodc.DeleteDC();
		hBmpbg.DeleteObject();
		//bgimage.BitBlt(dc, 0, 0, bgimage.GetWidth(), bgimage.GetHeight(), 0, 0, SRCCOPY);
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication1Dlg::OnBnClickedExit()
{
	ifStop = true;
	//pCCUThread->SuspendThread();
	//delete pCCUThread;
	//pCCUThread = NULL;

	OnOK();
	// TODO: Add your control notification handler code here
}



void CMFCApplication1Dlg::OnBnClickedButtonConnect()
{
	ConnectCCU(this);
}


void CMFCApplication1Dlg::OnBnClickedSendBn()
{
	this->GetDlgItem(IDC_EDIT_IP)->GetWindowText(strIP);
	this->GetDlgItem(IDC_EDIT_PORT)->GetWindowText(strPort);
	nPort = _ttoi(strPort);
	if (!clientSocket.Create())
	{
		CString szMsg;
		szMsg = "Create Failed: " + clientSocket.GetLastError();
		AfxMessageBox(szMsg);
		//return FALSE;
	}
	if (clientSocket.Connect(strIP, nPort))
	{
		char szRecValue[1024] = { 0 };
		this->GetDlgItem(IDC_EDIT_SEND)->GetWindowText(strSendText);
		strSendText.Insert(0, player.getNickname() + ": ");//////////
		strSendText += "\r\n";
		clientSocket.Send(strSendText, strSendText.GetLength());
		clientSocket.Receive((void *)szRecValue, 1024);
		strRecText += szRecValue;
		this->GetDlgItem(IDC_EDIT2)->SetWindowText(strRecText);
		clientSocket.Close();
		strSendText = "";
		this->GetDlgItem(IDC_EDIT_SEND)->SetWindowText("");
		CEDIT2.LineScroll(CEDIT2.GetLineCount());
		//Invalidate(FALSE);
	}
	else
	{
		char szMsg[1024];
		sprintf_s(szMsg, "Create Failed: %d", clientSocket.GetLastError());
		AfxMessageBox(szMsg);
	}
	// TODO: Add your control notification handler code here
}



HBRUSH CMFCApplication1Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	// TODO:  Change any attributes of the DC here
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	//if (pWnd->GetDlgCtrlID() == IDC_EDIT2)
	//{
	//	pDC->SetTextColor(RGB(255, 255, 0));
	//	pDC->SetBkMode(TRANSPARENT);
	//	hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	//}//设置指定ID控件的透明
	//else if (pWnd->GetDlgCtrlID() == IDC_EDIT_SEND)
	//{
	//	pDC->SetTextColor(RGB(255, 255, 0));
	//	pDC->SetBkMode(TRANSPARENT);
	//	hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	//}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(251, 226, 81));
		hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STATIC2)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(251, 226, 81));
		hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
