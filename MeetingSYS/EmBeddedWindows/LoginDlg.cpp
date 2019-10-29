// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EmBeddedWindows.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// LoginDlg 对话框

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


// LoginDlg 消息处理程序


void LoginDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_isHost = true;
	
	CDialog::OnOK();
}


void LoginDlg::OnTestPassWD()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR password[10];
	GetDlgItemText(IDC_LoginPW, password, 10);
	m_meetingPW = password;
	if (m_meetingPW.IsEmpty())
	{
		AfxMessageBox(_T("请输入密码！"));
	}
	else
	{
		if (m_meetingPW == m_TruePW)
		{
			CDialog::OnOK();
		}
		else {
			AfxMessageBox(_T("密码不正确，请重新输入！！！"));
		}
		
	}
}


BOOL LoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//CBitmap bitmap;  // CBitmap对象，用于加载位图   
	//HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄   

	//bitmap.LoadBitmap(IDB_BITMAP1);  // 将位图IDB_BITMAP1加载到bitmap   
	//hBmp = (HBITMAP)bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄   
	//m_picture.SetBitmap(hBmp);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1   
	//m_picture;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void LoginDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bStealLogin = true;
	CDialog::OnOK();
}
