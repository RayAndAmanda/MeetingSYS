
// EmBeddedWindows.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include<vector>
using namespace std;
// CEmBeddedWindowsApp: 
// �йش����ʵ�֣������ EmBeddedWindows.cpp
//
struct HOSTINFO
{
	string _Name;
	string _IP;
	string _state;
	bool operator==(HOSTINFO& rhs) const {
		return (_Name == rhs._Name &&_IP == rhs._IP&&_state == rhs._state);
	}
};
class CEmBeddedWindowsApp : public CWinApp
{
public:
	CEmBeddedWindowsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEmBeddedWindowsApp theApp;
extern vector<HOSTINFO> g_VhostInfo;
extern CRITICAL_SECTION g_cs;