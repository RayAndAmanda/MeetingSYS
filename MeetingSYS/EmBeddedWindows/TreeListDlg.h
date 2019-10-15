#pragma once
#include "afxwin.h"
#include "afxcmn.h"
//#include"EmBeddedWindowsDlg.h"

// TreeListDlg 对话框

class TreeListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TreeListDlg)

public:
	TreeListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TreeListDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TreeListDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	
	

	
public:
	// 显示用户列表
	CTreeCtrl m_showTree;
	int ChildNodeChanged(HTREEITEM hitemNode, bool nState);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	// 更新列表
public:
	void UpDateTreeList();
	void KillProcess(TCHAR* processName);
};
