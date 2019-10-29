// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EmBeddedWindows.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// LoginDlg �Ի���

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_isHost(false)
	, m_meetingPW(_T(""))
	, m_PWisOK(false)
	, m_TruePW(_T(""))
	, m_destroywindow(false)
	, m_bStealLogin(false)
{

}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_buttontestPW);
	DDX_Control(pDX, IDC_STATIC_waithost, m_waithost);
	//DDX_Control(pDX, IDC_STATIC_PictureCon, m_picture);
	
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &LoginDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &LoginDlg::OnTestPassWD)
	ON_BN_CLICKED(IDC_BUTTON3, &LoginDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// LoginDlg ��Ϣ�������


void LoginDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_isHost = true;
	
	CDialog::OnOK();
}


void LoginDlg::OnTestPassWD()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR password[10];
	GetDlgItemText(IDC_LoginPW, password, 10);
	m_meetingPW = password;
	if (m_meetingPW.IsEmpty())
	{
		AfxMessageBox(_T("���������룡"));
	}
	else
	{
		if (m_meetingPW == m_TruePW)
		{
			CDialog::OnOK();
		}
		else {
			AfxMessageBox(_T("���벻��ȷ�����������룡����"));
		}
		
	}
}


BOOL LoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//CBitmap bitmap;  // CBitmap�������ڼ���λͼ   
	//HBITMAP hBmp;    // ����CBitmap���ص�λͼ�ľ��   

	//bitmap.LoadBitmap(IDB_BITMAP1);  // ��λͼIDB_BITMAP1���ص�bitmap   
	//hBmp = (HBITMAP)bitmap.GetSafeHandle();  // ��ȡbitmap����λͼ�ľ��   
	//m_picture.SetBitmap(hBmp);    // ����ͼƬ�ؼ�m_jzmPicture��λͼͼƬΪIDB_BITMAP1   
	//m_picture;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void LoginDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bStealLogin = true;
	CDialog::OnOK();
}
