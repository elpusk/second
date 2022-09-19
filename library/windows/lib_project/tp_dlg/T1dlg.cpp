// T1dlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "tp_dlg.h"
#include "T1dlg.h"
#include "afxdialogex.h"
#include <afxpriv.h> // for WM_KICKIDLE declaration

// CT1dlg ��ȭ �����Դϴ�.

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


// CT1dlg �޽��� ó�����Դϴ�.


void CT1dlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//CDialogEx::OnOK();
	DestroyWindow();
	TRACE( _T("DestroyWindow()\n") );
}


void CT1dlg::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//CDialogEx::OnCancel();
	DestroyWindow();
	TRACE( _T("DestroyWindow()\n") );
}


void CT1dlg::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CDialogEx::PostNcDestroy();
}


void CT1dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//SetTimer( ID_TIMER_T1, 5000, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CT1dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CT1dlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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