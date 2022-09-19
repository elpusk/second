
// tp_wmilogDlg.h : ��� ����
//

#pragma once

#include "ns_WmiLog.h"
#include "ns_tchar.h"

// Ctp_wmilogDlg ��ȭ ����
class Ctp_wmilogDlg : public CDialogEx
{
// �����Դϴ�.
public:
	Ctp_wmilogDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TP_WMILOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

private:
	static const _tstring & GetLogName()
	{
		static const _tstring name( _T("ElpuskLog") );
		return name;
	}

	static const _tstring & GetEventSourceName()
	{
		static const _tstring name( _T("ng_SSS") );
		return name;
	}

	static const _tstring & GetEventMsgDllName()
	{
		static const _tstring name( _T("C:\\Program Files\\Elpusk\\ng_DevMg\\ng_EvtMsg.dll") );
		return name;
	}

	Cns_WmiLog *m_pLog;

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
