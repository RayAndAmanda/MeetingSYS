// moniter.cpp : 实现文件
//

#include "stdafx.h"
#include "EmBeddedWindows.h"
#include "moniter.h"
#include "afxdialogex.h"


// moniter 对话框

IMPLEMENT_DYNAMIC(moniter, CDialogEx)

moniter::moniter(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EMBEDDEDWINDOWS_DIALOG, pParent)
{

}

moniter::~moniter()
{
}

void moniter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(moniter, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// moniter 消息处理程序


HBRUSH moniter::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	//return hbr;
}


void moniter::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	if (!IsWindow(GetSafeHwnd()))
	{
		return;
	}
	
	CRect rect;
	GetClientRect(&rect);
	HWND hwnd = NULL;
	


	hwnd = ::FindWindow(NULL, _T("folder_MFCApplication1"));
	
	::SetParent(hwnd, GetSafeHwnd());
	

	
		::PostMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, NULL);
		::MoveWindow(hwnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
	
}
