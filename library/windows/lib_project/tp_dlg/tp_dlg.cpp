
// tp_dlg.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "tp_dlg.h"
#include "tp_dlgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctp_dlgApp

BEGIN_MESSAGE_MAP(Ctp_dlgApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Ctp_dlgApp construction

Ctp_dlgApp::Ctp_dlgApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Ctp_dlgApp object

Ctp_dlgApp theApp;


// Ctp_dlgApp initialization

BOOL Ctp_dlgApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	Ctp_dlgDlg dlg;
	m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int Ctp_dlgApp::Run()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWinApp::Run();
}


BOOL Ctp_dlgApp::OnIdle(LONG lCount)
{
	BOOL bMore = CWinApp::OnIdle(lCount);

	if (lCount == 0){
		TRACE(_T( "App idle for short period of time\n"));
		bMore = TRUE;
	}
	else if (lCount == 10){
		TRACE(_T( "App idle for longer amount of time\n"));
		bMore = TRUE;
	}
	else if (lCount == 100){
		TRACE(_T("App idle for even longer amount of time\n"));
		bMore = TRUE;
	}
	else if (lCount == 1000){
		TRACE(_T("App idle for quite a long period of time\n"));
		// bMore is not set to TRUE, no longer need idle// IMPORTANT: bMore is not set to FALSE since CWinApp::OnIdle may// have more idle tasks to complete.
   }

   
	return bMore;
	// return TRUE as long as there are any more idle tasks
}
