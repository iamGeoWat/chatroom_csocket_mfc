//解决消息存储队列的问题 要不给szrecmsg新的内存空间 要不给strrecqueue新的内存空间（乱码问题）
// ChatRoom_ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatRoom_Server.h"
#include "ChatRoom_ServerDlg.h"
#include "afxdialogex.h"
#include "ChatRoom_ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int QueueLength = 100;
bool ifExit = false;
char *strRecQueue[QueueLength] = { NULL };
int RecQueCount = 0;
CString strLogText;
CEdit CLog;
const char signalChar = '/';

int FindSignal(char strRecMsg[])
{
	int signal = 0;
	while (strRecMsg[signal] != ' ')
	{
		signal++;
	}
	return signal;
}



unsigned int StartServer(LPVOID lParam)
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	ifExit = false;

	CChatRoom_ServerDlg *serverDlg = (CChatRoom_ServerDlg *)lParam;
	CString strPort;
	serverDlg->GetDlgItemText(IDC_EDIT_PORT, strPort);
	int nPort = _ttoi(strPort);

	CSocket serverSocket, inComingSocket;
	if (!serverSocket.Socket())
	{
		char szError[1024];
		//szError = "Create Failed: " + serverSocket.GetLastError();
		sprintf_s(szError, "Create Failed: %d", serverSocket.GetLastError());
		AfxMessageBox(szError);
		return FALSE;
	}

	BOOL bOptVal = TRUE;
	int bOptLength = sizeof(BOOL);
	serverSocket.SetSockOpt(SO_REUSEADDR, (void *)&bOptVal, bOptLength, SOL_SOCKET);

	if (!serverSocket.Bind(nPort))
	{
		char szError[1024];
		//szError = "Bind Failed: " + GetLastError();
		//char szError[1024] = { 0 };
		sprintf_s(szError, "Bind Faild: %d", serverSocket.GetLastError());
		AfxMessageBox(szError);
		return FALSE;
	}

	if (!serverSocket.Listen())
	{
		//char szError[1024] = { 0 };
		char szError[1024];
		//szError = "Listen Failed: " + GetLastError();
		sprintf_s(szError, "Listen Faild: %d", serverSocket.GetLastError());
		AfxMessageBox(szError);
		return FALSE;
	}

	serverDlg->GetDlgItemText(IDC_EDIT_LOG, strLogText);
	strLogText = strLogText + "Server is on!" + "\r\n";
	serverDlg->SetDlgItemText(IDC_EDIT_LOG, strLogText);

	while (!ifExit)
	{
		if (!serverSocket.Accept(inComingSocket))
		{
			continue;
		}
		else
		{
			char szRecMsg[1024] = { 0 };
			char szShowMsg[1024] = { 0 };
			inComingSocket.Receive(szRecMsg, 1024);
			int signal = FindSignal(szRecMsg) + 1;
			if (szRecMsg[signal] == signalChar)
			{
				CString strSendback;
				char checker[5];
				for (unsigned int i = signal + 1, j = 0; i < strlen(szRecMsg); i++, j++)//signed unsignde mismatch
				{
					checker[j] = szRecMsg[i];
				}
				int clientChatPosition = atoi(checker);
				if (clientChatPosition == RecQueCount)
				{
					serverSocket.Send("", 1);//warnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn
					
				}
				else if ((clientChatPosition < RecQueCount) && (clientChatPosition%QueueLength == 0))
				{
					for (int i = 0; i < RecQueCount%QueueLength; i++)
					{
						strSendback = strSendback + strRecQueue[i];
					}
				}
				else if ((clientChatPosition < RecQueCount) && (clientChatPosition%QueueLength < RecQueCount%QueueLength))
				{
					for (int i = clientChatPosition % QueueLength; i < RecQueCount % QueueLength; i++)
					{
						strSendback = strSendback + strRecQueue[i];
					}
				}
				else if ((clientChatPosition < RecQueCount) && (clientChatPosition%QueueLength > RecQueCount%QueueLength))
				{
					for (int i = clientChatPosition%QueueLength; i < QueueLength; i++)
					{
						strSendback = strSendback + strRecQueue[i];
					}
					for (int i = 0; i < RecQueCount%QueueLength; i++)
					{
						strSendback = strSendback + strRecQueue[i];
					}
				}
				if (!inComingSocket.Send(strSendback, 1024))
				{

					char szShowMsg[1024] = { 0 };
					sprintf_s(szShowMsg, "Received: %d", serverSocket.GetLastError());
					serverDlg->GetDlgItemText(IDC_EDIT_LOG, strLogText);
					strLogText = strLogText + szShowMsg;
					serverDlg->SetDlgItemText(IDC_EDIT_LOG, strLogText);
					CLog.LineScroll(CLog.GetLineCount());
				}//compare and sendback
				char strRecQueCount[5] = { "\0" };
				_itoa_s(RecQueCount, strRecQueCount, 5, 10);
				inComingSocket.Send(strRecQueCount, 1024);

				inComingSocket.Close();
				
			}
			else
			{
				//AddtoQue(szRecMsg, strRecQueue);

				strRecQueue[RecQueCount%QueueLength] = (char *)malloc(sizeof(szRecMsg)+1);
				for (int i = 0; i < strlen(szRecMsg); i++)
				{
					strRecQueue[RecQueCount%QueueLength][i] = szRecMsg[i];
				}
				strRecQueue[RecQueCount%QueueLength][strlen(szRecMsg)] = '\0';
				//*strRecQueue[RecQueCount%QueueLength] = *szRecMsg;
				RecQueCount++;//add to queue

				sprintf_s(szShowMsg, "Received: %s", szRecMsg);
				serverDlg->GetDlgItemText(IDC_EDIT_LOG, strLogText);
				strLogText = strLogText + szShowMsg;
				serverDlg->SetDlgItemText(IDC_EDIT_LOG, strLogText);
				CLog.LineScroll(CLog.GetLineCount());

				//inComingSocket.Send(szRecMsg, 50);
				inComingSocket.Close();
			}
		}
	}

	serverSocket.Close();
	inComingSocket.Close();
	serverDlg->GetDlgItemText(IDC_EDIT_LOG, strLogText);
	strLogText = strLogText + "Server closed." + "/r/n";
	serverDlg->SetDlgItemText(IDC_EDIT_LOG, strLogText);
	return 0;
}

//unsigned int SendBack(LPVOID lParam)
//{
//	if (!AfxSocketInit())
//	{
//		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
//		return FALSE;
//	}
//	ifExit = false;
//
//	CChatRoom_ServerDlg *serverDlg = (CChatRoom_ServerDlg *)lParam;
//	CString strPort;
//	serverDlg->GetDlgItemText(IDC_EDIT_PORT, strPort);
//	int nPort = _ttoi(strPort);
//
//	CSocket serverSocket2, checkerSocket;
//	if (!serverSocket2.Socket())
//	{
//		char szError[1024];
//		//szError = "Create Failed: " + serverSocket.GetLastError();
//		sprintf_s(szError, "Checker Create Failed: %d", serverSocket2.GetLastError());
//		AfxMessageBox(szError);
//		return FALSE;
//	}
//
//	BOOL bOptVal = TRUE;
//	int bOptLength = sizeof(BOOL);
//	serverSocket2.SetSockOpt(SO_REUSEADDR, (void *)&bOptVal, bOptLength, SOL_SOCKET);
//
//	if (!serverSocket2.Bind(nPort))
//	{
//		char szError[1024];
//		sprintf_s(szError, "Checker Bind Faild: %d", serverSocket2.GetLastError());
//		AfxMessageBox(szError);
//		return FALSE;
//	}
//
//	if (!serverSocket2.Listen())
//	{
//		char szError[1024];
//		sprintf_s(szError, "Listen Faild: %d", serverSocket2.GetLastError());
//		AfxMessageBox(szError);
//		return FALSE;
//	}
//
//	serverDlg->GetDlgItemText(IDC_EDIT_LOG, strLogText);
//	strLogText = strLogText + "Checker Socket is on." + "\r\n";
//	serverDlg->SetDlgItemText(IDC_EDIT_LOG, strLogText);
//
//	while (!ifExit)
//	{
//		if (!serverSocket2.Accept(checkerSocket))
//		{
//			continue;
//		}
//		else
//		{
//			//change hereeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
//
//			CString strSendback;
//			char checker[5] = { 0 };
//			checkerSocket.Receive(checker, 5);
//			int clientChatPosition = atoi(checker);
//			if (clientChatPosition == RecQueCount)
//			{
//				serverSocket2.Send("", 1);
//			}
//			else if (clientChatPosition < RecQueCount)
//			{
//				for (int i = RecQueCount - clientChatPosition; i > 0; i--)
//				{
//					strSendback = strSendback + strRecQueue[RecQueCount - (i - 1)] + "\r\n";
//				}
//				if (!serverSocket2.Send(strSendback, 1024))
//				{
//
//					char szShowMsg[1024] = { 0 };
//					sprintf_s(szShowMsg, "Received: %s", serverSocket2.GetLastError());
//					serverDlg->GetDlgItemText(IDC_EDIT_LOG, strLogText);
//					strLogText = strLogText + szShowMsg + "\r\n";
//					serverDlg->SetDlgItemText(IDC_EDIT_LOG, strLogText);
//				}
//			}//compare and sendback
//			checkerSocket.Close();
//		}
//	}
//
//	serverSocket2.Close();
//	checkerSocket.Close();
//	return 0;
//}

void StopServer()
{
	ifExit = true;
}
// CChatRoom_ServerDlg dialog



CChatRoom_ServerDlg::CChatRoom_ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHATROOM_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatRoom_ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, CLog);
}

BEGIN_MESSAGE_MAP(CChatRoom_ServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_OPEN, &CChatRoom_ServerDlg::OnBnClickedOpen)
	ON_BN_CLICKED(ID_CLOSE, &CChatRoom_ServerDlg::OnBnClickedClose)
END_MESSAGE_MAP()


// CChatRoom_ServerDlg message handlers

BOOL CChatRoom_ServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatRoom_ServerDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatRoom_ServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CWinThread *pThreadRec = NULL;
//CWinThread *pThreadSnd = NULL;
void CChatRoom_ServerDlg::OnBnClickedOpen()
{
	ifExit = false;

	pThreadRec = new CWinThread(StartServer, (LPVOID)this);
	pThreadRec->CreateThread(CREATE_SUSPENDED);
	pThreadRec->ResumeThread();

	//pThreadSnd = new CWinThread(SendBack, (LPVOID)this);
	//pThreadSnd->CreateThread(CREATE_SUSPENDED);
	//pThreadSnd->ResumeThread();
	

	this->GetDlgItem(ID_OPEN)->EnableWindow(false);
	this->GetDlgItem(ID_CLOSE)->EnableWindow(true);
	// TODO: Add your control notification handler code here
}




void CChatRoom_ServerDlg::OnBnClickedClose()
{
	StopServer();
	pThreadRec->SuspendThread();
	delete pThreadRec;
	pThreadRec = NULL;

	//pThreadSnd->SuspendThread();
	//delete pThreadSnd;
	//pThreadSnd = NULL;
	this->GetDlgItem(ID_OPEN)->EnableWindow(true);
	this->GetDlgItem(ID_CLOSE)->EnableWindow(false);
	// TODO: Add your control notification handler code here
}