#pragma once


// moniter �Ի���

class moniter : public CDialogEx
{
	DECLARE_DYNAMIC(moniter)

public:
	moniter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~moniter();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMBEDDEDWINDOWS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
