#pragma once


// SetPassWordDlg �Ի���

class SetPassWordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetPassWordDlg)

public:
	SetPassWordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetPassWordDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetPassWord();
	// ���������
	CString m_password;
	afx_msg void OnBnClickedButton2();
};
