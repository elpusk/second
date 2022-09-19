// TestThread.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "tp_dlg.h"
#include "TestThread.h"

// CTestThread

IMPLEMENT_DYNCREATE(CTestThread, CWinThread)

CTestThread::CTestThread() : m_pdlg(NULL)
{
}

CTestThread::~CTestThread()
{
}

BOOL CTestThread::InitInstance()
{

	m_pdlg = new CT1dlg;
	m_pMainWnd = m_pdlg;
	m_pdlg->Create( CT1dlg::IDD );
	m_pdlg->ShowWindow( SW_SHOW );
	
	// TODO: ���⿡�� �� �����忡 ���� �ʱ�ȭ�� �����մϴ�.
	return TRUE;
}

int CTestThread::ExitInstance()
{
	// TODO: ���⿡�� �� �����忡 ���� ������ �����մϴ�.
	if( m_pdlg ){
		delete m_pdlg;
	}

	int nResult = CWinThread::ExitInstance();
	
	return nResult;
}

BEGIN_MESSAGE_MAP(CTestThread, CWinThread)
END_MESSAGE_MAP()


// CTestThread �޽��� ó�����Դϴ�.


BOOL CTestThread::OnIdle(LONG lCount)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CWinThread::OnIdle(lCount);
}


int CTestThread::Run()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	//m_pdlg->DisplayCounter();
	return CWinThread::Run();
}

