#pragma once


// moniter 对话框

class moniter : public CDialogEx
{
	DECLARE_DYNAMIC(moniter)

public:
	moniter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~moniter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMBEDDEDWINDOWS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
