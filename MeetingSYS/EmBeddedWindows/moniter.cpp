// moniter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EmBeddedWindows.h"
#include "moniter.h"
#include "afxdialogex.h"


// moniter �Ի���

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


// moniter ��Ϣ�������


HBRUSH moniter::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	//return hbr;
}


void moniter::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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
