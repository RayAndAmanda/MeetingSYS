#pragma once


// CwzdSplash

class CwzdSplash : public CWnd
{
	DECLARE_DYNAMIC(CwzdSplash)

public:
	CwzdSplash();
	virtual ~CwzdSplash();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CBitmap m_bitmap;

	void Create(UINT nBitmapID);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};


