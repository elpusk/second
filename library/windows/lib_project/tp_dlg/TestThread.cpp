// TestThread.cpp : 구현 파일입니다.
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
	
	// TODO: 여기에서 각 스레드에 대한 초기화를 수행합니다.
	return TRUE;
}

int CTestThread::ExitInstance()
{
	// TODO: 여기에서 각 스레드에 대한 정리를 수행합니다.
	if( m_pdlg ){
		delete m_pdlg;
	}

	int nResult = CWinThread::ExitInstance();
	
	return nResult;
}

BEGIN_MESSAGE_MAP(CTestThread, CWinThread)
END_MESSAGE_MAP()


// CTestThread 메시지 처리기입니다.


BOOL CTestThread::OnIdle(LONG lCount)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWinThread::OnIdle(lCount);
}


int CTestThread::Run()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//m_pdlg->DisplayCounter();
	return CWinThread::Run();
}

