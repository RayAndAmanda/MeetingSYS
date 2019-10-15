#pragma once


// SetPassWordDlg 对话框

class SetPassWordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetPassWordDlg)

public:
	SetPassWordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetPassWordDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetPassWord();
	// 保存的密码
	CString m_password;
	afx_msg void OnBnClickedButton2();
};
