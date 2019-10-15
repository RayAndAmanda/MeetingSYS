// SetPassWordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EmBeddedWindows.h"
#include "SetPassWordDlg.h"
#include "afxdialogex.h"


// SetPassWordDlg �Ի���

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


// SetPassWordDlg ��Ϣ�������


void SetPassWordDlg::OnSetPassWord()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR password[10];
	GetDlgItemText(IDC_EDIT_setps, password, 10);
	//GetDlgItemTextW(IDC_EDIT_setps, password,10);

	m_password = password;
	
	if (m_password.IsEmpty())
	{
		AfxMessageBox(_T("��������Ч����"));
	}
	else {
		CDialogEx::OnOK();
	}
	
}


void SetPassWordDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
