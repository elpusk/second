#pragma once


// CT1dlg ��ȭ �����Դϴ�.

class CT1dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CT1dlg)

public:
	CT1dlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CT1dlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_T1 };

	void DisplayCounter();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
