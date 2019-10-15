// CwzdSplash.cpp : ʵ���ļ�
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



// CwzdSplash ��Ϣ�������


// CWzdSplash ��Ϣ�������
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CWnd::OnTimer(nIDEvent);
	DestroyWindow(); //���ٳ�ʼ���洰��
}