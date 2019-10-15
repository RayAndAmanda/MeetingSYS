// PROP1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EmBeddedWindows.h"
#include "PROP1Dlg.h"
#include "afxdialogex.h"


// PROP1Dlg 对话框

IMPLEMENT_DYNAMIC(PROP1Dlg, CDialogEx)

PROP1Dlg::PROP1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROP1, pParent)
{

}

PROP1Dlg::~PROP1Dlg()
{
}

void PROP1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PROP1Dlg, CDialogEx)
END_MESSAGE_MAP()


// PROP1Dlg 消息处理程序
