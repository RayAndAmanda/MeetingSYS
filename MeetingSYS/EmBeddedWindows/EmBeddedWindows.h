
// EmBeddedWindows.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include<vector>
using namespace std;
// CEmBeddedWindowsApp: 
// 有关此类的实现，请参阅 EmBeddedWindows.cpp
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

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CEmBeddedWindowsApp theApp;
extern vector<HOSTINFO> g_VhostInfo;
extern CRITICAL_SECTION g_cs;