#pragma once
#include "afxwin.h"
#include "afxcmn.h"
//#include"EmBeddedWindowsDlg.h"

// TreeListDlg �Ի���

class TreeListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TreeListDlg)

public:
	TreeListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TreeListDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TreeListDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	
	

	
public:
	// ��ʾ�û��б�
	CTreeCtrl m_showTree;
	int ChildNodeChanged(HTREEITEM hitemNode, bool nState);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	// �����б�
public:
	void UpDateTreeList();
	void KillProcess(TCHAR* processName);
};
