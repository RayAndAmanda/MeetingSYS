
// EmBeddedWindowsDlg.h : ͷ�ļ�
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
#define WM_GetFocus				WM_USER + 1004 //�����Զ�����Ϣ
// CEmBeddedWindowsDlg �Ի���


class CEmBeddedWindowsDlg : public CDialogEx
{
// ����
public:
	CEmBeddedWindowsDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CEmBeddedWindowsDlg();
	void	KillProcess(TCHAR* processName);
	//������С������
	NOTIFYICONDATA m_NotifyIcon;
	void AddTray();
	//add by ray
	HANDLE m_hudpthread;
	HANDLE m_hudpReceivethread;
	//���UDP�㲥�߳�

	vector<string> _VserverIp; //������յ���serverIP;
	vector<string> _VlocalIP;//���汾��IP
	map<string, string> _Mhostinfo;
	//�����û����ƺ�ip
	
	void    CreateMultiVncProcess();
	RECT SecondScreenRect;
	LoginDlg m_login;
	SetPassWordDlg m_SetPassWorddlg;
	//PROP1Dlg *m_prop1dlg;
public:
	TreeListDlg *m_TreeListdlg;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMBEDDEDWINDOWS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	//����UDP�߳�
	friend DWORD WINAPI	UDPBroadcastThread(void* para);
	friend DWORD WINAPI UDPReceiveThread(void*para);
	// �ٽ���
	
	CRITICAL_SECTION m_UDPfunctionMnanage;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool m_bMultiScreen;
	int m_primaryScreenW;
	int m_primaryScreenH;
	bool m_showWind;
	
	
	RECT m_origRect;
	
	
	// ��̬�ı������bmp
	void StaticLoadimag(int num);
	// ��ʾ��������
	CEdit m_EditShow;
	afx_msg void OnBnClickedButton3();
	afx_msg void SetChatPs();
	// ��������
	CEdit m_EditSend;
	afx_msg void OnTobehost();
	afx_msg void OnUpdateTobehost(CCmdUI *pCmdUI);
	int m_swich;
	bool m_notshow;
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	int m_UpdSentSocket;
	
	// �Ƿ�����
	string m_state;
	string m_hostName;
	afx_msg void Notshow();
	afx_msg void notshowUI(CCmdUI *pCmdUI);
	afx_msg void ShowListDlg();
	// ��ʾ�ź��б�
	bool m_showListdlg;
	afx_msg void OnUpdateListmode(CCmdUI *pCmdUI);
	// �����Ƿ��Ѿ�����
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
