
// tp_wmilog.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Ctp_wmilogApp:
// �� Ŭ������ ������ ���ؼ��� tp_wmilog.cpp�� �����Ͻʽÿ�.
//

class Ctp_wmilogApp : public CWinApp
{
public:
	Ctp_wmilogApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern Ctp_wmilogApp theApp;