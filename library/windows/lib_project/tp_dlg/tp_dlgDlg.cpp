
// tp_dlgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tp_dlg.h"
#include "tp_dlgDlg.h"
#include "afxdialogex.h"
#include <afxpriv.h> // for WM_KICKIDLE declaration

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
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
public:
	afx_msg void OnClose();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// Ctp_dlgDlg dialog




Ctp_dlgDlg::Ctp_dlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ctp_dlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for( int i=0; i<C_nThread; i++ )
		m_th[i] =NULL;
}

void Ctp_dlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_listInfo);
}

BEGIN_MESSAGE_MAP(Ctp_dlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_BN_CLICKED(IDC_BUTTON_BYE, &Ctp_dlgDlg::OnBnClickedButtonBye)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_START, &Ctp_dlgDlg::OnBnClickedButtonStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &Ctp_dlgDlg::OnBnClickedButtonStop)
	ON_WM_DEVICECHANGE()
END_MESSAGE_MAP()


// Ctp_dlgDlg message handlers

BOOL Ctp_dlgDlg::OnInitDialog()
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

	// TODO: Add extra initialization here
	m_listInfo.AddString( _T("the first of system") );
	m_listInfo.SetCurSel( m_listInfo.GetCount() -1 );

	//SetTimer( ID_TIMER_T1, 3000, NULL );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Ctp_dlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Ctp_dlgDlg::OnPaint()
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
HCURSOR Ctp_dlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL Ctp_dlgDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	/*
	static int nCounter = 0;
	TRACE( _T("%d\n"), nCounter++ );
	*/

	return CDialogEx::PreTranslateMessage(pMsg);
}

void Ctp_dlgDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogEx::OnOK();
}


void Ctp_dlgDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogEx::OnCancel();
}


LRESULT Ctp_dlgDlg::OnKickIdle(WPARAM wParam, LPARAM lCount)
{
	// LPARAM is the idle count

	// Do something here
	static int nCounter = 0;
	CString stemp;
	stemp.Format( _T("%d"), nCounter++ );
	m_listInfo.AddString( stemp );
	m_listInfo.SetCurSel( m_listInfo.GetCount() -1 );
	//return (lCount <= 2);

	::Sleep( 50 );
	return 1;
}

LRESULT Ctp_dlgDlg::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	static int nCounter = 0;
	CString stemp;
	stemp.Format( _T("%d"), nCounter++ );
	m_listInfo.AddString( stemp );
	m_listInfo.SetCurSel( m_listInfo.GetCount() -1 );

	return 0;
}


void Ctp_dlgDlg::OnBnClickedButtonBye()
{
	EndDialog( 100 );
}


void CAboutDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnClose();
}


void Ctp_dlgDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	WaitsForClientsBye();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	for( int i=0; i<C_nThread; i++ )
		::WaitForSingleObject( m_hTh[i],INFINITE );

}


void Ctp_dlgDlg::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for( int i = 0; i<C_nThread; i++ ){
		m_th[i] = (CTestThread*)AfxBeginThread(RUNTIME_CLASS(CTestThread), THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED,0);
		m_hTh[i] = m_th[i]->m_hThread;
		m_th[i]->ResumeThread();
	}
}


void Ctp_dlgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	static int i =0;
	CString stemp;

	stemp.Format( _T("counter : %d"), i++ );

	if( ID_TIMER_T1 == nIDEvent ){
		m_listInfo.AddString( stemp );
		m_listInfo.SetCurSel( m_listInfo.GetCount() -1 );
	}

	CDialogEx::OnTimer(nIDEvent);
}


void Ctp_dlgDlg::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	WaitsForClientsBye();
}

void Ctp_dlgDlg::WaitsForClientsBye()
{
	DWORD dwResult;
	HANDLE hTh;
	CTestThread *pTh;

	for( int i = 0; i<C_nThread; i++ ){
		hTh = m_hTh[i];
		pTh = m_th[i];

		dwResult =::WaitForSingleObject( hTh,0 );
		switch( dwResult ){
			case WAIT_TIMEOUT:
				if( pTh ){
					::PostMessage( pTh->GetMainWnd()->GetSafeHwnd(),WM_CLOSE,0,0 );
				}
				TRACE( _T("WAIT_TIMEOUT.\n") );
				break;
			case WAIT_ABANDONED:
				TRACE( _T("WAIT_ABANDONED.\n") );
				break;
			case WAIT_OBJECT_0:
				TRACE( _T("WAIT_OBJECT_0.\n") );
				break;
			case WAIT_FAILED:
				TRACE( _T("WAIT_FAILED.\n") );
				break;
			default:
				TRACE( _T("WAIT_?.\n") );
				break;
		}//end switch
	}//end for

}



BOOL Ctp_dlgDlg::OnDeviceChange( UINT nEventType,DWORD_PTR dwData )
{
	TRACE( _T("OnDeviceChange()") );
	return TRUE;
}