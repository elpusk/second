#pragma once


// CT1dlg 대화 상자입니다.

class CT1dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CT1dlg)

public:
	CT1dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CT1dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_T1 };

	void DisplayCounter();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	enum : DWORD { ID_TIMER_T1 = 200	};
	//CWnd** m_ppMainWnd;

	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg LRESULT OnKickIdle(WPARAM wParam, LPARAM lCount);
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);
};
