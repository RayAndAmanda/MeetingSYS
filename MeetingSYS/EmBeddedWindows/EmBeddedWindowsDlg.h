
// EmBeddedWindowsDlg.h : 头文件
//

#pragma once
#include "atltypes.h"
#include<vector>
#include <string>
#include<map>
#include "Winsock2.h"
#include "afxwin.h"
#include "TreeListDlg.h"
#include"LoginDlg.h"
#include"SetPassWordDlg.h"
//#include"PROP1Dlg.h"
using namespace std;
#define TRAYMESSAGE WM_USER + 0x1
#define WM_GetFocus				WM_USER + 1004 //增加自定义消息
// CEmBeddedWindowsDlg 对话框


class CEmBeddedWindowsDlg : public CDialogEx
{
// 构造
public:
	CEmBeddedWindowsDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CEmBeddedWindowsDlg();
	void	KillProcess(TCHAR* processName);
	//增加最小化托盘
	NOTIFYICONDATA m_NotifyIcon;
	void AddTray();
	//add by ray
	HANDLE m_hudpthread;
	HANDLE m_hudpReceivethread;
	//添加UDP广播线程

	vector<string> _VserverIp; //保存接收到的serverIP;
	vector<string> _VlocalIP;//保存本地IP
	map<string, string> _Mhostinfo;
	//保存用户名称和ip
	
	void    CreateMultiVncProcess();
	RECT SecondScreenRect;
	LoginDlg m_login;
	SetPassWordDlg m_SetPassWorddlg;
	//PROP1Dlg *m_prop1dlg;
public:
	TreeListDlg *m_TreeListdlg;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMBEDDEDWINDOWS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnTrayMessage(WPARAM  wParam, LPARAM  lParam);
	afx_msg void OnExit();
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTestPassWD();
	afx_msg void OnBnClickedOk();
	//void KillProcess(TCHAR* processName);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	CRect m_wndRect;

	//增加UDP线程
	friend DWORD WINAPI	UDPBroadcastThread(void* para);
	friend DWORD WINAPI UDPReceiveThread(void*para);
	// 临界区
	
	CRITICAL_SECTION m_UDPfunctionMnanage;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool m_bMultiScreen;
	int m_primaryScreenW;
	int m_primaryScreenH;
	bool m_showWind;
	
	
	RECT m_origRect;
	
	
	// 静态文本框添加bmp
	void StaticLoadimag(int num);
	// 显示聊天内容
	CEdit m_EditShow;
	afx_msg void OnBnClickedButton3();
	afx_msg void SetChatPs();
	// 发送内容
	CEdit m_EditSend;
	afx_msg void OnTobehost();
	afx_msg void OnUpdateTobehost(CCmdUI *pCmdUI);
	int m_swich;
	bool m_notshow;
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	int m_UpdSentSocket;
	
	// 是否在线
	string m_state;
	string m_hostName;
	afx_msg void Notshow();
	afx_msg void notshowUI(CCmdUI *pCmdUI);
	afx_msg void ShowListDlg();
	// 显示信号列表
	bool m_showListdlg;
	afx_msg void OnUpdateListmode(CCmdUI *pCmdUI);
	// 程序是否已经启动
	bool appIsRunning(TCHAR* processName);
protected:
	CBrush m_brush;
	//HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	bool m_isOk2Brocast;
	bool m_ishost;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	afx_msg LRESULT OnGetfocus(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
