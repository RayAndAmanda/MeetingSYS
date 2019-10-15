// SetPassWordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EmBeddedWindows.h"
#include "SetPassWordDlg.h"
#include "afxdialogex.h"


// SetPassWordDlg 对话框

IMPLEMENT_DYNAMIC(SetPassWordDlg, CDialogEx)

SetPassWordDlg::SetPassWordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, m_password(_T(""))
{

}

SetPassWordDlg::~SetPassWordDlg()
{
}

void SetPassWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SetPassWordDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &SetPassWordDlg::OnSetPassWord)
	ON_BN_CLICKED(IDC_BUTTON2, &SetPassWordDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// SetPassWordDlg 消息处理程序


void SetPassWordDlg::OnSetPassWord()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR password[10];
	GetDlgItemText(IDC_EDIT_setps, password, 10);
	//GetDlgItemTextW(IDC_EDIT_setps, password,10);

	m_password = password;
	
	if (m_password.IsEmpty())
	{
		AfxMessageBox(_T("请输入有效密码"));
	}
	else {
		CDialogEx::OnOK();
	}
	
}


void SetPassWordDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
