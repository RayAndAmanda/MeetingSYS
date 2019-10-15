#pragma once


// PROP1Dlg 对话框

class PROP1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(PROP1Dlg)

public:
	PROP1Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PROP1Dlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROP1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
