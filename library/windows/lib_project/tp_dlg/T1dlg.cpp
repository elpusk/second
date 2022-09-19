// T1dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tp_dlg.h"
#include "T1dlg.h"
#include "afxdialogex.h"
#include <afxpriv.h> // for WM_KICKIDLE declaration

// CT1dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CT1dlg, CDialogEx)

CT1dlg::CT1dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CT1dlg::IDD, pParent)
{

}

CT1dlg::~CT1dlg()
{
}

void CT1dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CT1dlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
END_MESSAGE_MAP()


// CT1dlg 메시지 처리기입니다.


void CT1dlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//CDialogEx::OnOK();
	DestroyWindow();
	TRACE( _T("DestroyWindow()\n") );
}


void CT1dlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//CDialogEx::OnCancel();
	DestroyWindow();
	TRACE( _T("DestroyWindow()\n") );
}


void CT1dlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogEx::PostNcDestroy();
}


void CT1dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	static int i = 0;
	
	if( ID_TIMER_T1 == nIDEvent ){

		CString stemp;

		stemp.Format( _T("F : %d"), i++ );

		CWnd *pwnd = GetDlgItem( IDC_STATIC_T1 );
		pwnd->SetWindowText( stemp );
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CT1dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//SetTimer( ID_TIMER_T1, 5000, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CT1dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CT1dlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TRACE(_T("OnClose()\n"));

	CDialogEx::OnClose();
}

LRESULT CT1dlg::OnKickIdle(WPARAM wParam, LPARAM lCount)
{
	// LPARAM is the idle count

	// Do something here
	//DisplayCounter();

	//return (lCount <= 2);


	return 0;
}


LRESULT CT1dlg::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	DisplayCounter();

	return 0;
}


void CT1dlg::DisplayCounter()
{
	static int nCounter = 0;
	CString stemp;
	stemp.Format( _T("%d"), nCounter++ );
	CWnd *pwnd = GetDlgItem( IDC_STATIC_T1 );
	pwnd->SetWindowText( stemp );
}