
// tp_dlgDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "T1dlg.h"
#include "TestThread.h"
#include "memory"

using namespace std;

// Ctp_dlgDlg dialog
class Ctp_dlgDlg : public CDialogEx
{
// Construction
public:
	Ctp_dlgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TP_DLG_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	void WaitsForClientsBye();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnKickIdle(WPARAM wParam, LPARAM lCount);
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnDeviceChange(UINT nEventType,DWORD_PTR dwData );

private:
	enum : DWORD { ID_TIMER_T1 = 300	};
	static const UINT_PTR C_nTimerID = 10000;
	static const UINT C_nTimerElapse =10;	//unit : 10 msec
	static const int C_nThread = 2;

public:
//	virtual INT_PTR DoModal();
	// information list
	CListBox m_listInfo;

	HANDLE m_hExitEvent;
	CTestThread *m_th[C_nThread];
	HANDLE m_hTh[C_nThread];

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonBye();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStop();
};
