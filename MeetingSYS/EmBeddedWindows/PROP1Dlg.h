#pragma once


// PROP1Dlg �Ի���

class PROP1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(PROP1Dlg)

public:
	PROP1Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PROP1Dlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROP1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
