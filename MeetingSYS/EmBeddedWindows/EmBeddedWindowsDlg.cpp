
// EmBeddedWindowsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EmBeddedWindows.h"
#include "EmBeddedWindowsDlg.h"
#include "afxdialogex.h"
#include "tlhelp32.h" 
#include "Resource.h"
#include <vector> 
#include<json\json.h>
#include<iostream>
#include <sstream>
#include"CwzdSplash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#ifdef _DEBUG
#pragma   comment   (lib,   "json_vc71_libmtd.lib")  

#else
#pragma   comment   (lib,   "json_vc71_libmt.lib")  
#endif
using namespace std;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
// ö�ٴ��ڻ�ȡcmd���ھ��




#define WM_StartDisPMESSAGE        WM_USER + 1003 //�����Զ�����Ϣ
//

struct EnumParam
{
	HWND   hMainWnd;
	DWORD   dwProcessID;
};
BOOL   CALLBACK  EnumWinProc(HWND   hwnd, LPARAM   lParam)
{
	DWORD   dwID;

	EnumParam*   pep = (EnumParam*)lParam;
	GetWindowThreadProcessId(hwnd, &dwID);
	if (dwID == pep->dwProcessID) {
		pep->hMainWnd = hwnd;
		return   0;
	}
	return   TRUE;
}
HWND CreateVncProcess(TCHAR* cmd)
{
	HWND hProcess = NULL;
	// ����������Ϣ
	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	EnumParam   ep = { 0 };
	// ������Ϣ
	//TCHAR szCmd[] = _T(" -host=192.168.2.132 -port=5678 -x=960 -y=540 -width=960 -height=540");
	PROCESS_INFORMATION pi;
	if (CreateProcess(_T("./GQYViewer.exe"), cmd, NULL, NULL, false, 0, NULL, NULL, &si, &pi))
	{

		ep.dwProcessID = pi.dwProcessId;
		Sleep(1000);
		EnumWindows((WNDENUMPROC)EnumWinProc,
			(long)&ep);
	}
	hProcess = ep.hMainWnd;
	return hProcess;
}
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEmBeddedWindowsDlg �Ի���


CEmBeddedWindowsDlg::CEmBeddedWindowsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EMBEDDEDWINDOWS_DIALOG, pParent)
	, m_hudpthread(NULL)
	, m_hudpReceivethread(NULL)
	, m_bMultiScreen(false)
	, m_primaryScreenW(0)
	, m_primaryScreenH(0)
	, m_showWind(true)
	,m_swich(0)
	,m_notshow(false)
	, m_UpdSentSocket(0)
	, m_showListdlg(false)
	, m_isOk2Brocast(false)
	, m_ishost(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	InitializeCriticalSection(&m_UDPfunctionMnanage);
	m_state = "OnLine";
}

CEmBeddedWindowsDlg::~CEmBeddedWindowsDlg()
{

}

void CEmBeddedWindowsDlg::KillProcess(TCHAR * processName)
{
	HANDLE shot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(shot, &pe);

	do
	{
		if (wcsstr(pe.szExeFile, processName))
		{
			DWORD dwVersion = GetVersion();
			if (dwVersion < 0x80000000)
			{
				HANDLE hToken;
				TOKEN_PRIVILEGES tkp;
				OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
				LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
				tkp.PrivilegeCount = 1;
				tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
				AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
				//MessageBox("NT OS");
			}
			else
			{
				//AfxMessageBox("9x OS");
			}

			HANDLE process = ::OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
			if (process)
			{
				::TerminateProcess(process, 0);
				//MessageBox("���̽����ɹ�");
			}
			else
			{
				AfxMessageBox(_T("���̽���ʧ��"));
			}

		}
	} while (Process32Next(shot, &pe));
}


void CEmBeddedWindowsDlg::AddTray()
{
	m_NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	//AfxGetApp()->LoadIcon(IDR_ICON1);      //����ͼ�꣬IDR_ICON1��ͼ����Դ��ID
	m_NotifyIcon.hIcon = m_hIcon;
	m_NotifyIcon.hWnd = m_hWnd;
	lstrcpy(m_NotifyIcon.szTip, _T("��ϯ�������"));
	m_NotifyIcon.uCallbackMessage = TRAYMESSAGE;
	m_NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &m_NotifyIcon);                  //��ʾ����ͼ��
}

void CEmBeddedWindowsDlg::CreateMultiVncProcess()
{

	int MaxScreen = 0;


	
	CRect rc;
	
	HWND hwnd = NULL;
	HWND h2 = NULL;
	//KillProcess(_T("GQYViewer"));
	::EnterCriticalSection(&g_cs);
	
	
	if (g_VhostInfo.size() < 4)
	{
		//�Ļ��濪��
		MaxScreen = 4;
		
	}
	else
	{//�Ż���
		MaxScreen = 9;


	}
	int lastnum = g_VhostInfo.size();
	auto lastValue = g_VhostInfo.back();
	CString Tempcmd;
	if (lastnum <= 9)
	{
		GetDlgItem(IDC_STATIC1 + lastnum + 3)->GetWindowRect(&rc);
		

		//ScreenToClient(rc);
		string Ftem = lastValue._IP;
		CString Fbuf(Ftem.c_str());

		string Stem = "GQY��ϯ����-";
		Stem += lastValue._Name;
		CString Sbuf(Stem.c_str());

		
		
		Tempcmd.Format(_T(" -host=%s -port=%d -hostname=%s -x=%d -y=%d -width=%d -height=%d"), Fbuf, \
			5678, Sbuf, rc.left, \
			rc.top , rc.Width(), rc.Height());

		TCHAR *pBuf = Tempcmd.GetBuffer(_MAX_PATH);
		HWND temphwnd = CreateVncProcess(pBuf);

		CWnd * pWnd = NULL;

		//Sleep(1000);
		pWnd = FromHandle(temphwnd);
		::SetParent(temphwnd, this->m_hWnd);
		ScreenToClient(rc);
		pWnd->MoveWindow(&rc, true);
		pWnd->ShowWindow(TRUE);
		//�ƺ�λ��֮���ٷ��Ϳ���������ʾ��Ϣ
		pWnd->PostMessage(WM_StartDisPMESSAGE, NULL, NULL);
		
	}

	//auto iter = g_VhostInfo.begin();

	//for (int i = 0; i < MaxScreen; i++)
	//{
	//	CString Tempcmd;
	//	if (iter != g_VhostInfo.end())
	//	{
	//		if (MaxScreen == 4)
	//		{
	//			GetDlgItem(IDC_STATIC1 + i)->GetWindowRect(&rc);
	//		}
	//		else
	//		{
	//			GetDlgItem(IDC_STATIC1 + i+4)->GetWindowRect(&rc);
	//		}
	//				
	//		ScreenToClient(rc);
	//		string Ftem = iter->_IP;
	//		CString Fbuf(Ftem.c_str());

	//		string Stem="GQY��ϯ����-";
	//		Stem+=iter->_Name;
	//		CString Sbuf(Stem.c_str());
	//		//Sbuf += _T("��ϯ����ϵͳ");
	//		if (!m_bMultiScreen)
	//		{
	//			startX = rc.left;
	//			startY = rc.top;
	//		}
	//		else {
	//			startX = rc.left + this->SecondScreenRect.left;
	//			startY = rc.top + this->SecondScreenRect.top;
	//		}
	//		;
	//		
	//		//-host=192.168.2.132 -port=5678 -hostname=de-werfdf -x=0 -y=0 -width=500 -height=400
	//		/*Tempcmd.Format(_T(" -host=%s -port=%d -hostname=%s -x=%d -y=%d -width=%d -height=%d"), Fbuf, \
	//			5678, Sbuf, startX, \
	//			startY, rc.right-rc.left, rc.bottom-rc.top);*/
	//		Tempcmd.Format(_T(" -host=%s -port=%d -hostname=%s -x=%d -y=%d -width=%d -height=%d"), Fbuf, \
	//			5678, Sbuf, startX, \
	//			startY, rc.Width(), rc.Height());
	//		iter++;
	//		TCHAR *pBuf = Tempcmd.GetBuffer(_MAX_PATH);
	//		HWND hwnd = CreateVncProcess(pBuf);
	//		if (hwnd == NULL)break;//˵���������ɹ�
	//		CWnd * pWnd = NULL;
	//		//�ҵ���Ӧ��CWnd	
	//		Sleep(1000);

	//		HWND h3 = ::GetParent(hwnd);

	//		if (h3 != NULL)
	//		{
	//			hwnd = h3;
	//		}
	//		Sleep(1000);
	//		pWnd = FromHandle(hwnd);
	//		::SetParent(hwnd, this->m_hWnd);
	//		//CRect rc_view(rc.left,rc.top,rc.right/2,rc.bottom/2);
	//		//pWnd->MoveWindow(&rc_view, true);
	//		pWnd->MoveWindow(&rc, true);
	//		pWnd->ShowWindow(TRUE);
	//		//::ShowWindow(m_hWnd, SW_FORCEMINIMIZE);
	//	}

	//}
	::LeaveCriticalSection(&g_cs);	
}
	

	
	

	
	


void CEmBeddedWindowsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditShow);
	DDX_Control(pDX, IDC_EDIT2, m_EditSend);
}

BEGIN_MESSAGE_MAP(CEmBeddedWindowsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_BUTTON1, &CEmBeddedWindowsDlg::OnTestPassWD)
	ON_BN_CLICKED(IDOK, &CEmBeddedWindowsDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CEmBeddedWindowsDlg::OnBnClickedButton2)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_MESSAGE(TRAYMESSAGE, OnTrayMessage)
	ON_COMMAND(ID_Exit, OnExit)
	
	
	
	ON_BN_CLICKED(IDC_BUTTON3, &CEmBeddedWindowsDlg::OnBnClickedButton3)
	ON_COMMAND(ID_TobeHost, &CEmBeddedWindowsDlg::OnTobehost)
	ON_COMMAND(ID_setchatps,&CEmBeddedWindowsDlg::SetChatPs)
	ON_UPDATE_COMMAND_UI(ID_TobeHost, &CEmBeddedWindowsDlg::OnUpdateTobehost)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_NotShow, &CEmBeddedWindowsDlg::Notshow)
	ON_UPDATE_COMMAND_UI(ID_NotShow, &CEmBeddedWindowsDlg::notshowUI)
	ON_COMMAND(ID_ListMode, &CEmBeddedWindowsDlg::ShowListDlg)
	ON_UPDATE_COMMAND_UI(ID_ListMode, &CEmBeddedWindowsDlg::OnUpdateListmode)
	ON_WM_CTLCOLOR() 
	ON_MESSAGE(WM_GetFocus, &CEmBeddedWindowsDlg::OnGetfocus)
END_MESSAGE_MAP()


BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
	HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	CEmBeddedWindowsDlg *pdlg = (CEmBeddedWindowsDlg*)dwData;

	MONITORINFO monitorinfo;
	monitorinfo.cbSize = sizeof(MONITORINFO);

	//�����ʾ����Ϣ������Ϣ���浽monitorinfo��
	GetMonitorInfo(hMonitor, &monitorinfo);

	//����⵽����
	if (monitorinfo.dwFlags == MONITORINFOF_PRIMARY)
	{
		return TRUE;
	}
	//����ڶ�������Ϣ
	pdlg->SecondScreenRect = monitorinfo.rcMonitor;
	return false;
}

// CEmBeddedWindowsDlg ��Ϣ�������

BOOL CEmBeddedWindowsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	CwzdSplash wndSplash;
	wndSplash.Create(IDB_BITMAP1);
	wndSplash.CenterWindow();
	wndSplash.UpdateWindow();          //send WM_PAINT
	Sleep(2000);
	wndSplash.DestroyWindow();
	m_TreeListdlg = new TreeListDlg();
	m_TreeListdlg->Create(IDD_TreeListDlg);
	AddTray();
	//���ӵ�¼����
	InitializeCriticalSection(&g_cs);//��ʼ���ٽ���
	InitializeCriticalSection(&m_UDPfunctionMnanage);
	
	m_hudpReceivethread = CreateThread(NULL, 0, UDPReceiveThread, this, 0, NULL);


	H3:
	m_login.m_isHost = false;
	m_login.DoModal();
	m_login.m_destroywindow = true;
	if (m_login.m_isHost)
	{
		m_SetPassWorddlg.DoModal();
		if (m_SetPassWorddlg.m_password.IsEmpty())
		{
			goto H3;
		}
	}
	m_ishost = m_login.m_isHost;
	m_isOk2Brocast = true;//����֮��Ϳ��Թ㲥�Լ�����Ϣ��,���ҽ���ini��Ϣ
	m_hudpthread = CreateThread(NULL, 0, UDPBroadcastThread, this, 0, NULL);
	
	
	
	

	//end
	m_SetPassWorddlg.m_password;
	m_primaryScreenW= GetSystemMetrics(SM_CXSCREEN);
	m_primaryScreenH= GetSystemMetrics(SM_CYSCREEN);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//m_brush.CreateSolidBrush(RGB(173, 173, 173));//�ı䱳����ɫ
	//�ж��ǲ��Ƕ���ʾ��
	

	INT iNumber = GetSystemMetrics(SM_CMONITORS);
	if (iNumber > 1)
	{
		m_bMultiScreen = TRUE;
		EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)this);
		ShowWindow(SW_MAXIMIZE);
	}
	else {
		//������ʱ
		//::SetWindowPos(m_hWnd, HWND_NOTOPMOST, m_primaryScreenW, 0, SecondScreenRect.right - SecondScreenRect.left, SecondScreenRect.bottom - SecondScreenRect.top, SWP_SHOWWINDOW);

		::SetWindowPos(m_hWnd, HWND_TOPMOST, 960, 540, 960, 540, SWP_SHOWWINDOW);
	}
	//����еڶ��������ƶ����ڶ�������
	if ((SecondScreenRect.right - SecondScreenRect.left) > 0)
	{
		//::SetWindowPos(m_hWnd, HWND_NOTOPMOST, SecondScreenRect.left, SecondScreenRect.top, SecondScreenRect.right- SecondScreenRect.left, SecondScreenRect.bottom- SecondScreenRect.top, SWP_SHOWWINDOW);
		MoveWindow(&SecondScreenRect);
	}
		
	
	
	
	

	//SetTimer(0, 1000, NULL);
	//SetTimer(1, 100, NULL);
	//LONG dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	//::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | WS_MINIMIZEBOX);
	//�����������ĳ���ͼ��
	//::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE)
		//&~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW);
	//����vncserver ����
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!appIsRunning(_T("GQYserver")))
	{
		// Start the child process. 
		if (!CreateProcess(_T("./GQYserver.exe"),   // No module name (use command line)
			NULL,
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi)           // Pointer to PROCESS_INFORMATION structure
			)
		{
			printf("CreateProcess failed (%d).\n", GetLastError());
			//return;
		}
	}
	//��������
	//m_prop1dlg = new PROP1Dlg();
	//m_prop1dlg->Create(IDD_PROP1);
	//m_prop1dlg->ShowWindow(SW_SHOWNORMAL);
	
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEmBeddedWindowsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_MINIMIZE)
	{
		
		
		//ShowWindow(SW_HIDE);
		::ShowWindow(m_hWnd, SW_FORCEMINIMIZE);
	}
	else if (nID == SC_CLOSE)
	{
		OnExit();
	}
	else if (nID == SC_MAXIMIZE)
	{
		ShowWindow(SW_MAXIMIZE);
	}
	else if (nID == SC_RESTORE)
	{
		CDialogEx::OnSysCommand(nID, lParam);
		if ((SecondScreenRect.right - SecondScreenRect.left) > 0)
		{
			SecondScreenRect.top = SecondScreenRect.top + 1;
			MoveWindow(&SecondScreenRect);
			
		}
			
	}
	
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEmBeddedWindowsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CEmBeddedWindowsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CEmBeddedWindowsDlg::OnTrayMessage(WPARAM wParam, LPARAM lParam)
{
	UINT uID;
	UINT uMsg;
	uID = (UINT)wParam;
	uMsg = (UINT)lParam;
	//if(uID==IDR_MAINFRAME)
	{
		switch (uMsg)
		{
		case WM_RBUTTONUP:
		{
			CMenu menu;
			menu.LoadMenu(IDR_MENU1);                       //���ز˵���Դ
			CMenu* pContextMenu = menu.GetSubMenu(0);       //ȡ��ָ��menu�˵�����Ϊ0���Ӳ˵���ָ��
			//����������������һ���˵�
			if (m_login.m_isHost)
			{
				pContextMenu->AppendMenuW(MF_STRING, ID_setchatps, _T("���ñ��λ�������"));
			}
			//end
			POINT point;
			GetCursorPos(&point);                            //ȡ���������
															 //��ʾ�˵�
			SetForegroundWindow();
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON |
				TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
			::PostMessage(NULL, WM_NULL, 0, 0);
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
			if (!m_showWind)
			{
				m_showWind = true;
				//RECT lpRect;
				//GetWindowRect(&lpRect);
				
				////
				::ShowWindow(m_hWnd, SW_RESTORE);
				::SetWindowPos(m_hWnd, HWND_NOTOPMOST, m_primaryScreenW, 0, SecondScreenRect.right - SecondScreenRect.left, SecondScreenRect.bottom - SecondScreenRect.top, SWP_SHOWWINDOW);

				return TRUE;
			}
			else
			{
				m_showWind = false;
				//::ShowWindow(m_hWnd, SW_HIDE);
				::ShowWindow(m_hWnd, SW_FORCEMINIMIZE);
			}
			
		}
		default:
			return FALSE;

		}
	}
	return 0L;

}

void CEmBeddedWindowsDlg::OnExit()
{
	if (IDOK == MessageBox(_T("�Ƿ�ȷ���˳���"), _T("��ʾ"), MB_OKCANCEL))
	{
		delete m_TreeListdlg;
		KillProcess(_T("GQYserver"));
		
		KillProcess(_T("GQYViewer"));
		exit(0);
	}
}

//void CEmBeddedWindowsDlg::OnTestPassWD()
//{
//}

void CEmBeddedWindowsDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//::SendMessage(g_hWnd, WM_CLOSE, 0, 0);
	
	
	//CDialogEx::OnOK();
}
//void CEmBeddedWindowsDlg::KillProcess(TCHAR* processName)
//{
//	HANDLE shot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//	PROCESSENTRY32 pe;
//	pe.dwSize = sizeof(PROCESSENTRY32);
//	Process32First(shot, &pe);
//
//	do
//	{
//		
//
//		if (wcsstr(pe.szExeFile, processName))
//		{
//
//			{
//				GUITHREADINFO gui;
//				
//				FillMemory(&gui, sizeof(GUITHREADINFO), 0);
//				gui.cbSize = sizeof(GUITHREADINFO);
//
//				GetGUIThreadInfo(pe.th32ProcessID, &gui);
//				DWORD errorcode = GetLastError();
//				HWND mHwnd = gui.hwndActive;
//				
//
//
//			}
//			DWORD dwVersion = GetVersion();
//			if (dwVersion < 0x80000000)
//			{
//				HANDLE hToken;
//				TOKEN_PRIVILEGES tkp;
//				OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
//				LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
//				tkp.PrivilegeCount = 1;
//				tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
//				AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
//				//MessageBox("NT OS");
//			}
//			else
//			{
//				//AfxMessageBox("9x OS");
//			}
//
//			HANDLE process = ::OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
//			if (process)
//			{
//				::TerminateProcess(process, 0);
//				//MessageBox("���̽����ɹ�");
//			}
//			else
//			{
//				AfxMessageBox(_T("���̽���ʧ��"));
//			}
//
//		}
//	} while (Process32Next(shot, &pe));
//}

void CEmBeddedWindowsDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillProcess(_T("GQYserver"));
	
	KillProcess(_T("GQYViewer"));
	//delete(m_prop1dlg);
	delete m_TreeListdlg;
	CDialogEx::OnClose();
}


void CEmBeddedWindowsDlg::OnBnClickedButton2()
{

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CEmBeddedWindowsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType == 1 )
	{
		return;
	}

	// TODO: �ڴ˴������Ϣ����������
	int dlgitem[] = { IDC_STATIC1,IDC_STATIC2,IDC_STATIC3,IDC_STATIC4,\
		IDC_STATIC5, IDC_STATIC6, IDC_STATIC7, IDC_STATIC8, IDC_STATIC9, IDC_STATIC10, IDC_STATIC11, \
		IDC_STATIC12, IDC_STATIC13,IDC_EDIT1,IDC_STATIC14, IDC_EDIT2 , IDC_BUTTON3 };
	for (int i = 0; i <size(dlgitem); i++)//��Ϊ�Ƕ���ؼ���������������ѭ��
	{
		CWnd *pWnd = GetDlgItem(dlgitem[i]);
		//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
		if (pWnd && nType != 1 && m_wndRect.Width() && m_wndRect.Height())
		{
			CRect rect;   //��ȡ�ؼ��仯ǰ�Ĵ�С 
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
			rect.left = rect.left*cx / m_wndRect.Width();//�����ؼ���С
			rect.right = rect.right*cx / m_wndRect.Width();
			rect.top = rect.top*cy / m_wndRect.Height();
			rect.bottom = rect.bottom*cy / m_wndRect.Height();
			pWnd->MoveWindow(rect);//���ÿؼ���С 
		}
	}
	//���»�ô��ڳߴ�
	GetClientRect(&m_wndRect);
}


void CEmBeddedWindowsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	switch (nIDEvent)
	{
	case 0:
		KillTimer(0);
		//::ShowWindow(m_hWnd, SW_HIDE);
		break;
	case 1:
		KillTimer(1);       //���������������ʵ��
		
		break;
		//GetWindowRect(&m_origRect);
		//::SetWindowPos(m_hWnd, HWND_NOTOPMOST, m_primaryScreenW - (m_origRect.right - m_origRect.left), m_primaryScreenH - (m_origRect.bottom - m_origRect.top) - 50, m_origRect.right - m_origRect.left, m_origRect.bottom - m_origRect.top, SWP_SHOWWINDOW);
		//::ShowWindow(m_hWnd, SW_FORCEMINIMIZE);
		//::ShowWindow(m_hWnd,SW_HIDE);
		//m_showWind = false;
		//this->OnBnClickedButton1();
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

DWORD WINAPI UDPBroadcastThread(void * para)
{
	CEmBeddedWindowsDlg* pEmBeddedWindows = (CEmBeddedWindowsDlg*)para;
	//�����������
	char szHost[256] = { 0 };
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	::gethostname(szHost, 256);
	
	//������������µ�������Ϣ
	hostent *pHost = ::gethostbyname(szHost);	//����ָ���������İ����������ֺ͵�ַ��Ϣ��hostent�ṹ��ָ��
												/*struct hostent
												{
												char *h_name;	//��ʽ������
												char ** h_aliases;	//��������
												short h_addrtype;	//����IP��ַ���ͣ�IPV4-AF_INET
												short h_length;	//����IƱ��ַ�ֽڳ��ȣ�IPV4��4�ֽڣ���32λ
												char ** h_addr_list;	//������IP��ַ�б�
												};*/

												//�����õ�������Ϣ
	pEmBeddedWindows->m_hostName = szHost;
	in_addr addr;
	CString strTmp, strBuf;

	for (int i = 0;; i++)
	{
		char *p = pHost->h_addr_list[i];
		if (p == NULL)
			break;
		memcpy(&addr.S_un.S_addr, p, pHost->h_length);
		char *slzp = ::inet_ntoa(addr);	//��sin_addr�����IP����ֵ��ת�����ַ�����ʽ(127.0.0.1)
		strBuf = slzp;
		string te(slzp);
		strTmp.Append((strBuf + "|"));
		pEmBeddedWindows->_VlocalIP.push_back(te);
	}

	//AfxMessageBox(_T("I am Coming!"));
	//int m_UpdSentSocket = -1;
	if ((pEmBeddedWindows->m_UpdSentSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		AfxMessageBox(_T("socket error"));
		return false;
	}

	const int opt = 1;
	//���ø��׽���Ϊ�㲥���ͣ�
	int nb = 0;
	nb = setsockopt(pEmBeddedWindows->m_UpdSentSocket, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
	if (nb == -1)
	{
		AfxMessageBox(_T("set socket error..."));
		return false;
	}
	//ָ��Server IP  ��  ���͸�Client�Ķ˿�
	sockaddr_in addrto;
	addrto.sin_family = AF_INET;
	addrto.sin_addr.s_addr = inet_addr("255.255.255.255");;
	addrto.sin_port = htons(50000);
	int nlen = sizeof(addrto);
	char smsg[256] = { 0 };
	//::wsprintfA(smsg, "%ls", (LPCTSTR)strTmp);
	//::sprintf_s(smsg, 256, "%s", szHost);
	/*{
		"cmd":"INT",
		"Name":"hu"
	}*/
	string hostname ;
	hostname += szHost;
	hostname += "����";
	while (1)
	{
		
		Sleep(3000);
		//�ӹ㲥��ַ������Ϣ
		::EnterCriticalSection(&pEmBeddedWindows->m_UDPfunctionMnanage);
		if (pEmBeddedWindows->m_isOk2Brocast)
		{
			
			Json::Value root;
			root["cmd"] = "Init";
			root["Name"] = hostname;
			root["State"] = pEmBeddedWindows->m_state;
			ostringstream oss;
			oss << root;
			cout << oss.str() << endl;
			string send = oss.str();
			int ret = sendto(pEmBeddedWindows->m_UpdSentSocket, send.c_str(), send.length(), 0, (sockaddr*)&addrto, nlen);
			if (ret<0)
			{
				AfxMessageBox(_T("send error...."));
			}
			else
			{
				printf("ok ");
			}
			
		}
		if (pEmBeddedWindows->m_ishost) {
			
			Json::Value root;
			string temppw= (CT2A)pEmBeddedWindows->m_SetPassWorddlg.m_password.GetString();
			root["cmd"] = "ToBeHost";
			root["Name"] = hostname;
			root["State"] = pEmBeddedWindows->m_state;
			root["PW"] = temppw;
			ostringstream oss;
			oss << root;
			cout << oss.str() << endl;
			string send = oss.str();
			int ret = sendto(pEmBeddedWindows->m_UpdSentSocket, send.c_str(), send.length(), 0, (sockaddr*)&addrto, nlen);
			if (ret<0)
			{
				AfxMessageBox(_T("send error...."));
			}
			else
			{
				printf("ok ");
			}
			
		}
		::LeaveCriticalSection(&pEmBeddedWindows->m_UDPfunctionMnanage);
	}

	return 0;
}

DWORD WINAPI UDPReceiveThread(void * para)
{
	Sleep(100);
	CEmBeddedWindowsDlg* pEmBeddedWindows = (CEmBeddedWindowsDlg*)para;
	// ����socket
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	int err=0;
	SOCKET connect_socket;
	connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == connect_socket)
	{
		err = WSAGetLastError();
		printf("error! error code is %d/n", err);
		return false;
	}

	// �������׽���
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(50000);
	sin.sin_addr.s_addr = 0;

	// �����������ϵĹ㲥��ַ��������
	SOCKADDR_IN sin_from;
	sin_from.sin_family = AF_INET;
	sin_from.sin_port = htons(50000);
	sin_from.sin_addr.s_addr = INADDR_BROADCAST;

	//���ø��׽���Ϊ�㲥���ͣ�
	bool bOpt = true;
	setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));

	// ���׽���
	err = bind(connect_socket, (SOCKADDR*)&sin, sizeof(SOCKADDR));
	if (SOCKET_ERROR == err)
	{
		err = WSAGetLastError();
		AfxMessageBox(_T("bind error..."));
		return false;
	}

	int nAddrLen = sizeof(SOCKADDR);
	char buff[1024] = "";
	char ReserveIp[256] = "";
	int nLoop = 0;
	while (1)
	{
		// ��������
		int nSendSize = recvfrom(connect_socket, buff, 1024, 0, (SOCKADDR*)&sin_from, &nAddrLen);
		if (SOCKET_ERROR == nSendSize)
		{
			err = WSAGetLastError();
			AfxMessageBox(_T("error! error code is %d/n", err));
			return false;
		}

		buff[nSendSize] = '\0';
		sprintf(ReserveIp, inet_ntoa(sin_from.sin_addr));
		/*TRACE("Recv: %s\n", buff);
		TRACE("ReserveIp: is %s\n", ReserveIp);*/
		Json::Reader reader;                                    //����json��Json::Reader
		Json::Value value;

		std::string strTrees = buff;
		if (reader.parse(strTrees, value))
		{
			if (value.isMember("cmd"))
			{
				string cmd = value["cmd"].asString();
				if (cmd == "Init")
				{
					if (pEmBeddedWindows->m_isOk2Brocast)
					{
						HOSTINFO hostinfo;

						hostinfo._Name = value["Name"].asString();
						hostinfo._state = value["State"].asString();
						hostinfo._IP = ReserveIp;


						auto iter = pEmBeddedWindows->_VlocalIP.begin();
						auto ir = g_VhostInfo.begin();
						while (iter != pEmBeddedWindows->_VlocalIP.end())
						{
							//ȥ������ip
							if (*iter == hostinfo._IP)
							{
								//�Ѿ����ڣ��������
								goto E1;
							}
							iter++;
						}
						while (ir != g_VhostInfo.end())
						{//�ж��Ƿ��Ѿ�����
							if (*ir == hostinfo)
							{
								//�ҵ���,˵���Ѿ�������ֱ������
								goto E1;
							}
							if (ir->_Name == hostinfo._Name)
							{
								//˵��״̬��һ����ֱ�Ӹ���״̬
								//�ҵ���,����״̬
								::EnterCriticalSection(&g_cs);
								ir->_state = hostinfo._state;
								::LeaveCriticalSection(&g_cs);
								pEmBeddedWindows->m_TreeListdlg->UpDateTreeList();
								goto E2;
							}
							ir++;
						}
						::EnterCriticalSection(&g_cs);
						g_VhostInfo.push_back(hostinfo);

						::LeaveCriticalSection(&g_cs);
						//pEmBeddedWindows->m_TreeListdlg->ShowWindow(SW_SHOWNORMAL);
						pEmBeddedWindows->m_TreeListdlg->UpDateTreeList();
					E2:
						if (hostinfo._state == "OnLine" && pEmBeddedWindows->m_showListdlg == false)
							pEmBeddedWindows->CreateMultiVncProcess();
					}
			

				}
				else if (cmd == "Chat")
				{
					string Message = value["message"].asString();
					string PCName = value["PCName"].asString();
					TRACE("Recv: State is %s\n", Message.c_str());
					
					CString strSend, strShow;
					CString str_Time; //��ȡϵͳʱ�� ����
					CTime tm;
					tm = CTime::GetCurrentTime();
					//str_Time = tm.Format("%Y-%m-%d %H:%M:%S: ");
					str_Time = tm.Format("%H:%M:%S  ");
					//str_Time += PCName.c_str();
					//pEmBeddedWindows->m_EditSend.GetWindowText(strSend);
					//CStringA stra(strSend.GetBuffer(0));
					//temp = strSend.GetBuffer(0);
					pEmBeddedWindows->m_EditShow.GetWindowText(strShow);

					//pEmBeddedWindows->m_EditSend.SetWindowText(_T(""));
					strSend += str_Time;
					strSend += PCName.c_str();
					strSend += _T(" : ");
					strSend += Message.c_str();
					strSend += "\r\n";
					strSend += strShow;
					//str_Time += strSend;
					pEmBeddedWindows->m_EditShow.SetWindowText(strSend);

				}
				else if (cmd == "ToBeHost")
				{
					CString tempname=_T("�����ˣ�");
					//TRACE("Recv to be host!!!");
					pEmBeddedWindows->m_login.m_TruePW = value["PW"].asCString();
					tempname+=value["Name"].asCString();
					tempname += _T("�����ߣ�");
					if (!pEmBeddedWindows->m_login.m_destroywindow)
					{
						pEmBeddedWindows->m_login.m_waithost.SetWindowTextW(tempname);
						pEmBeddedWindows->m_login.m_buttontestPW.EnableWindow(TRUE);
					}
					
				}
				else if(cmd=="TestPW"){
				//���������Ƿ���ȷ
					if (!pEmBeddedWindows->m_SetPassWorddlg.m_password.IsEmpty())
					{
						CString userPW(value["PW"].asCString());
						sockaddr_in addrto;
						addrto.sin_family = AF_INET;
						addrto.sin_addr.s_addr = inet_addr(inet_ntoa(sin_from.sin_addr));;
						addrto.sin_port = htons(50000);
						int nlen = sizeof(addrto);
						Json::Value root;
						root["cmd"] = "TestResult";
						string send;
						if (userPW == pEmBeddedWindows->m_SetPassWorddlg.m_password)
						{
							
							
							root["result"] = true;
							ostringstream oss;
							oss << root;
							cout << oss.str() << endl;
							send = oss.str();
							
						}
						else {
							root["result"] = false;
							ostringstream oss;
							oss << root;
							cout << oss.str() << endl;
							send = oss.str();
						}
						::EnterCriticalSection(&pEmBeddedWindows->m_UDPfunctionMnanage);
						int ret = sendto(pEmBeddedWindows->m_UpdSentSocket, send.c_str(), send.length(), 0, (sockaddr*)&addrto, nlen);
						if (ret<0)
						{
							AfxMessageBox(_T("send error...."));
						}
						else
						{
							printf("ok ");
						}
						::LeaveCriticalSection(&pEmBeddedWindows->m_UDPfunctionMnanage);
					}
				}
				else if (cmd == "TestResult")
				{
					
					pEmBeddedWindows->m_login.m_PWisOK== value["result"].asBool();
				}
				
			}
		}

		E1:;
	}

}










// ��̬�ı������bmp
void CEmBeddedWindowsDlg::StaticLoadimag(int num)
{
	CRect rect;
	CStatic *pStatic;
	for (int i = 0; i < num; i++)
	{
		if (num == 4)
		{
			pStatic = (CStatic *)GetDlgItem(IDC_STATIC1 + i);
		}
		else
		{
			pStatic = (CStatic *)GetDlgItem(IDC_STATIC5 + i);
		}
		
		pStatic->GetWindowRect(&rect);


		HBITMAP hBmp = (HBITMAP)::LoadImage(0, _T(".\\1.bmp"), IMAGE_BITMAP, rect.Width(), rect.Height(), LR_LOADFROMFILE);


		pStatic->ModifyStyle(NULL, SS_BITMAP);

		pStatic->SetBitmap(hBmp);
	}
	
}


void CEmBeddedWindowsDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString strSend,strShow;
	//CString str; //��ȡϵͳʱ�� ����
	//CTime tm; 
	//string temp;
	//tm = CTime::GetCurrentTime();
	//str = tm.Format("%Y-%m-%d %H:%M:%S: ");

	m_EditSend.GetWindowText(strSend);
	CStringA stra(strSend.GetBuffer(0));
	
	//temp = strSend.GetBuffer(0);
	//m_EditShow.GetWindowText(strShow);

	//m_EditSend.SetWindowText(_T(""));
	//strSend += "\r\n";
	//strSend += strShow;
	//str += strSend;
	//m_EditShow.SetWindowText(str);

	UpdateData(TRUE);
	sockaddr_in addrto;
	addrto.sin_family = AF_INET;
	addrto.sin_addr.s_addr = inet_addr("255.255.255.255");;
	addrto.sin_port = htons(50000);
	int nlen = sizeof(addrto);
	

	Json::Value root;
	root["cmd"] = "Chat";
	root["message"] = stra.GetBuffer(0);
	root["PCName"] = m_hostName.c_str();
	ostringstream oss;
	oss << root;
	cout << oss.str() << endl;
	string send =  oss.str();
	::EnterCriticalSection(&m_UDPfunctionMnanage);
	int ret = sendto(m_UpdSentSocket, send.c_str(), send.length(), 0, (sockaddr*)&addrto, nlen);
	if (ret<0)
	{
		AfxMessageBox(_T("send error...."));
	}
	else
	{
		printf("ok ");
	}
	::LeaveCriticalSection(&m_UDPfunctionMnanage);
	m_EditSend.SetWindowText(_T(""));
}

void CEmBeddedWindowsDlg::SetChatPs()
{

	//m_SetPassWorddlg.DoModal();
	//���Է��͹㲥�ˡ�����
	
	
	
}


void CEmBeddedWindowsDlg::OnTobehost()
{
	// TODO: �ڴ���������������
	if (m_swich)
	{
		m_swich = false;
	}
	else
	{//��Ϊ�����ˣ���Ӵ�����
		m_swich = true;
		sockaddr_in addrto;
		addrto.sin_family = AF_INET;
		addrto.sin_addr.s_addr = inet_addr("255.255.255.255");;
		addrto.sin_port = htons(50000);
		int nlen = sizeof(addrto);
		Json::Value root;
		root["cmd"] = "ToBeHost";
		
		ostringstream oss;
		oss << root;
		cout << oss.str() << endl;
		string send = oss.str();
		::EnterCriticalSection(&m_UDPfunctionMnanage);
		int ret = sendto(m_UpdSentSocket, send.c_str(), send.length(), 0, (sockaddr*)&addrto, nlen);
		if (ret<0)
		{
			AfxMessageBox(_T("send error...."));
		}
		else
		{
			printf("ok ");
		}
		::LeaveCriticalSection(&m_UDPfunctionMnanage);
	}
}


void CEmBeddedWindowsDlg::OnUpdateTobehost(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if (m_swich)
	{
		pCmdUI->SetText(_T("ȡ��������"));
		//pCmdUI->SetCheck(1);

	}
	else {
		pCmdUI->SetText(_T("��Ϊ������"));
		//pCmdUI->SetCheck(0);
	}
}


void CEmBeddedWindowsDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialogEx::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: �ڴ˴������Ϣ����������
	CCmdUI state;

	state.m_pMenu = pPopupMenu;

	ASSERT(state.m_pOther == NULL);

	ASSERT(state.m_pParentMenu == NULL);



	// Determine if menu is popup in top-level menu and set m_pOther to

	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).

	HMENU hParentMenu;

	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)

		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.

	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)

	{

		CWnd* pParent = this;

		// Child windows don't have menus--need to go to the top!

		if (pParent != NULL &&

			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)

		{

			int nIndexMax = ::GetMenuItemCount(hParentMenu);

			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)

			{

				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)

				{

					// When popup is found, m_pParentMenu is containing menu.

					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);

					break;

				}

			}

		}

	}



	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();

	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;

		state.m_nIndex++)

	{

		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);

		if (state.m_nID == 0)

			continue; // Menu separator or invalid cmd - ignore it.



		ASSERT(state.m_pOther == NULL);

		ASSERT(state.m_pMenu != NULL);

		if (state.m_nID == (UINT)-1)

		{

			// Possibly a popup menu, route to first item of that popup.

			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);

			if (state.m_pSubMenu == NULL ||

				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||

				state.m_nID == (UINT)-1)

			{

				continue;       // First item of popup can't be routed to.

			}

			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.

		}

		else

		{

			// Normal menu item.

			// Auto enable/disable if frame window has m_bAutoMenuEnable

			// set and command is _not_ a system command.

			state.m_pSubMenu = NULL;

			state.DoUpdate(this, FALSE);

		}



		// Adjust for menu deletions and additions.

		UINT nCount = pPopupMenu->GetMenuItemCount();

		if (nCount < state.m_nIndexMax)

		{

			state.m_nIndex -= (state.m_nIndexMax - nCount);

			while (state.m_nIndex < nCount &&

				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)

			{

				state.m_nIndex++;

			}

		}

		state.m_nIndexMax = nCount;

	}
}


void CEmBeddedWindowsDlg::Notshow()
{
	// TODO: �ڴ���������������
	::EnterCriticalSection(&m_UDPfunctionMnanage);
	 m_state = "Offline";
	::LeaveCriticalSection(&m_UDPfunctionMnanage);
	
	if(!m_notshow)
	{//�رշ������
		KillProcess(_T("GQYserver"));
		m_notshow = true;
	}
	else 
	{
		m_notshow = false;
		//����vncserver ����
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));


		// Start the child process. 
		CreateProcess(_T("./GQYserver.exe"),   // No module name (use command line)
			NULL,
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi);           // Pointer to PROCESS_INFORMATION structure
			
	}
}


void CEmBeddedWindowsDlg::notshowUI(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	
	if (m_notshow)
	{
		
		pCmdUI->SetText(_T("�ɱ�����"));
		

	}
	else {
		
		pCmdUI->SetText(_T("��������"));
		
	}
}


void CEmBeddedWindowsDlg::ShowListDlg()
{
	// TODO: �ڴ���������������
	if (m_showListdlg == false)
	{
		m_showListdlg = true; 
		::ShowWindow(m_hWnd, SW_HIDE);
		m_TreeListdlg->ShowWindow(SW_SHOWNORMAL);
		KillProcess(_T("GQYViewer"));
		::EnterCriticalSection(&g_cs);
		g_VhostInfo.clear();
		::LeaveCriticalSection(&g_cs);
	}
	else {
		m_showListdlg = false;
		m_TreeListdlg->ShowWindow(SW_HIDE);
		::ShowWindow(m_hWnd, SW_SHOW);
		::EnterCriticalSection(&g_cs);
		g_VhostInfo.clear();
		::LeaveCriticalSection(&g_cs);
		
	}
	
	
	
	
	//m_prop1dlg->ShowWindow(SW_HIDE);
}


void CEmBeddedWindowsDlg::OnUpdateListmode(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if (m_showListdlg)
	{
		pCmdUI->SetText(_T("Ԥ��ģʽ"));
	}
	else {
		pCmdUI->SetText(_T("�б�ģʽ"));
	}
}


// �����Ƿ��Ѿ�����
bool CEmBeddedWindowsDlg::appIsRunning(TCHAR* processName)
{
	HANDLE shot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(shot, &pe);

	do
	{
		if (wcsstr(pe.szExeFile, processName))
		{
			DWORD dwVersion = GetVersion();
			if (dwVersion < 0x80000000)
			{
				HANDLE hToken;
				TOKEN_PRIVILEGES tkp;
				OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
				LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
				tkp.PrivilegeCount = 1;
				tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
				AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
				//MessageBox("NT OS");
			}
			else
			{
				//AfxMessageBox("9x OS");
			}

			HANDLE process = ::OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
			if (process)
			{
				return true;
			}
		

		}
	} while (Process32Next(shot, &pe));
	return false;
}
//
//HBRUSH CEmBeddedWindowsDlg::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
//{
//	//return m_brush;
//	return HBRUSH();
//}


BOOL CEmBeddedWindowsDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (GetFocus() == GetDlgItem(IDC_EDIT2))
		if (pMsg->message == WM_KEYDOWN)
		{
			if (pMsg->wParam== VK_RETURN)
			{
				OnBnClickedButton3();
			
			}
		}
		
	return CDialogEx::PreTranslateMessage(pMsg);
}


afx_msg LRESULT CEmBeddedWindowsDlg::OnGetfocus(WPARAM wParam, LPARAM lParam)
{

	//::SetFocus(m_hWnd);
	TRACE(_T("WM_GetFocus\n"));
	return 0;
}
