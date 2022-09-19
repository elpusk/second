
// tp_dlg.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Ctp_dlgApp:
// See tp_dlg.cpp for the implementation of this class
//

class Ctp_dlgApp : public CWinApp
{
public:
	Ctp_dlgApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

public:
	virtual int Run();
	virtual BOOL OnIdle(LONG lCount);
};

extern Ctp_dlgApp theApp;