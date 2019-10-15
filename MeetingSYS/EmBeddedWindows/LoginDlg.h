#pragma once
#include "afxwin.h"


// LoginDlg 对话框

class LoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LoginDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	// 是否为主持人
	bool m_isHost;
	
	afx_msg void OnTestPassWD();
	CString m_meetingPW;
	CButton m_buttontestPW;
	bool m_PWisOK;
	CString m_TruePW;
	CStatic m_waithost;
	bool m_destroywindow;
	//CStatic m_picture;
	virtual BOOL OnInitDialog();
};
