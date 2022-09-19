#pragma once

#include "T1dlg.h"

// CTestThread

class CTestThread : public CWinThread
{
	DECLARE_DYNCREATE(CTestThread)

public:
	CTestThread();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTestThread();

	CT1dlg *m_pdlg;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnIdle(LONG lCount);
	virtual int Run();
};


