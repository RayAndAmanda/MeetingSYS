// CwzdSplash.cpp : 实现文件
//

#include "stdafx.h"
#include "EmBeddedWindows.h"
#include "CwzdSplash.h"


// CwzdSplash

IMPLEMENT_DYNAMIC(CwzdSplash, CWnd)

CwzdSplash::CwzdSplash()
{

}

CwzdSplash::~CwzdSplash()
{
}


BEGIN_MESSAGE_MAP(CwzdSplash, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()

END_MESSAGE_MAP()



// CwzdSplash 消息处理程序


// CWzdSplash 消息处理程序
void CwzdSplash::Create(UINT nBitmapID)
{
	m_bitmap.LoadBitmap(nBitmapID);
	BITMAP bitmap;
	m_bitmap.GetBitmap(&bitmap);
	
	CreateEx(0,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, bitmap.bmWidth, bitmap.bmHeight, NULL, NULL);
}
void CwzdSplash::OnPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CPaintDC dc(this); // device context forpainting
	BITMAP bitmap;
	m_bitmap.GetBitmap(&bitmap);
	CDC dcComp;
	dcComp.CreateCompatibleDC(&dc);
	dcComp.SelectObject(&m_bitmap);
	// draw bitmap
	dc.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, &dcComp, 0, 0, SRCCOPY);
}
void CwzdSplash::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CWnd::OnTimer(nIDEvent);
	DestroyWindow(); //销毁初始画面窗口
}