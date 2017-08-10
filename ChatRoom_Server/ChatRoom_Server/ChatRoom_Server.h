
// ChatRoom_Server.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CChatRoom_ServerApp:
// See ChatRoom_Server.cpp for the implementation of this class
//

class CChatRoom_ServerApp : public CWinApp
{
public:
	CChatRoom_ServerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CChatRoom_ServerApp theApp;