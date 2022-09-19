
// tp_wmilogDlg.h : 헤더 파일
//

#pragma once

#include "ns_WmiLog.h"
#include "ns_tchar.h"

// Ctp_wmilogDlg 대화 상자
class Ctp_wmilogDlg : public CDialogEx
{
// 생성입니다.
public:
	Ctp_wmilogDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TP_WMILOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

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

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
