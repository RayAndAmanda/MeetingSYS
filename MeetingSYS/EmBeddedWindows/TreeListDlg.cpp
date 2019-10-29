// TreeListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EmBeddedWindows.h"
#include "TreeListDlg.h"
#include "afxdialogex.h"
#include "Resource.h"
#include<stack>
#include "tlhelp32.h" 
// TreeListDlg 对话框

IMPLEMENT_DYNAMIC(TreeListDlg, CDialogEx)

TreeListDlg::TreeListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TreeListDlg, pParent)
{
	
}

TreeListDlg::~TreeListDlg()
{
}

void TreeListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_showTree);
}







BEGIN_MESSAGE_MAP(TreeListDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &TreeListDlg::OnNMClickTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &TreeListDlg::OnNMDblclkTree1)
END_MESSAGE_MAP()


int TreeListDlg::ChildNodeChanged(HTREEITEM hitemNode, bool nState)
{
	std::stack <HTREEITEM> stackTreeItem;
	HTREEITEM hitemNext = m_showTree.GetChildItem(hitemNode);
	stackTreeItem.push(hitemNext);

	while (!stackTreeItem.empty())
	{
		hitemNext = stackTreeItem.top();
		stackTreeItem.pop();

		while (hitemNext != NULL)
		{
			if (nState == BST_CHECKED)
			{
				m_showTree.SetCheck(hitemNext);
			}
			else
			{
				m_showTree.SetCheck(hitemNext, BST_UNCHECKED);
			}

			HTREEITEM hitemChild = m_showTree.GetChildItem(hitemNext);
			if (hitemChild != NULL)
			{
				stackTreeItem.push(hitemChild);
			}

			hitemNext = m_showTree.GetNextItem(hitemNext, TVGN_NEXT);
		}
	}
	return 0;
}

void TreeListDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


BOOL TreeListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_showTree.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);
	HTREEITEM hTreeItemParent;
	hTreeItemParent = m_showTree.InsertItem(_T("远程桌面控制"));
	m_showTree.SetItemData(hTreeItemParent, 0);
	/*HTREEITEM hTreeItemNodeNew = m_showTree.InsertItem(_T("虚拟电脑1"), hTreeItemParent);
	m_showTree.SetItemData(hTreeItemNodeNew, (DWORD_PTR)hTreeItemNodeNew);
	HTREEITEM hTreeItemNodeNew2 = m_showTree.InsertItem(_T("虚拟电脑2"), hTreeItemParent);
	m_showTree.SetItemData(hTreeItemNodeNew2, (DWORD_PTR)hTreeItemNodeNew2);*/
	

	//InitTreeControl(hTreeItemParent, 0);
	/*hTreeItemParent = m_showTree.InsertItem(_T("不可控电脑信号"));
	m_showTree.SetItemData(hTreeItemParent, 1);

	HTREEITEM hTreeItemNodeNew5 = m_showTree.InsertItem(_T("虚拟电脑5"), hTreeItemParent);
	m_showTree.SetItemData(hTreeItemNodeNew5, (DWORD_PTR)hTreeItemNodeNew5);

	HTREEITEM hTreeItemNodeNew6 = m_showTree.InsertItem(_T("虚拟电脑6"), hTreeItemParent);
	m_showTree.SetItemData(hTreeItemNodeNew6, (DWORD_PTR)hTreeItemNodeNew6);

	HTREEITEM hTreeItemNodeNew7 = m_showTree.InsertItem(_T("虚拟电脑7"), hTreeItemParent);
	m_showTree.SetItemData(hTreeItemNodeNew7, (DWORD_PTR)hTreeItemNodeNew7);*/
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void TreeListDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CPoint pt = GetCurrentMessage()->pt;	//获取当前鼠标点击消息的坐标点
	m_showTree.ScreenToClient(&pt);			//将鼠标的屏幕坐标，转换成树形控件的客户区坐标

	UINT uFlags = 0;
	HTREEITEM hItem = m_showTree.HitTest(pt, &uFlags);//然后做点击测试
	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))//如果点击的位置是在节点位置上面
	{
		m_showTree.SelectItem(hItem);
		CString str = m_showTree.GetItemText(hItem);
		int nDat = m_showTree.GetItemData(hItem);
		//AfxMessageBox(str);
		::EnterCriticalSection(&g_cs);
		auto iter = g_VhostInfo.begin();
		while (iter != g_VhostInfo.end())
		{
			string st = CW2A(str.GetString());
			if (iter->_Name == st)
			{
				//如果存在可以开窗
				// 进程启动信息
				KillProcess(_T("GQYViewer"));
				CString Tempcmd;
				STARTUPINFO si;
				memset(&si, 0, sizeof(STARTUPINFO));
				si.cb = sizeof(STARTUPINFO);
				si.dwFlags = STARTF_USESHOWWINDOW;
				si.wShowWindow = SW_SHOW;

				// 进程信息
				//TCHAR szCmd[] = _T(" -host=192.168.2.132 -port=5678 -x=960 -y=540 -width=960 -height=540");
				PROCESS_INFORMATION pi;
				Tempcmd.Format(_T(" -host=%s -port=%d -hostname=%s -x=%d -y=%d -width=%d -height=%d"), CString((iter->_IP).c_str()), \
					5678, CString((iter->_Name).c_str()), 480, \
					270, 960, 540);
				
			/*	TCHAR *pBuf = Tempcmd.GetBuffer(_MAX_PATH);
				CreateProcess(_T("./GQYViewer.exe"), pBuf, NULL, NULL, false, 0, NULL, NULL, &si, &pi);*/

			

				TCHAR *pBuf = Tempcmd.GetBuffer(_MAX_PATH);
				HWND temphwnd = CreateVncProcess(pBuf);

				CWnd * pWnd = NULL;

				//Sleep(1000);
				pWnd = FromHandle(temphwnd);

				//移好位置之后再发送开启加速显示消息
				pWnd->PostMessage(WM_StartDisPMESSAGE, NULL, NULL);
			}
			iter++;
		}
		::LeaveCriticalSection(&g_cs);
		
	}
	*pResult = 0;
}


// 更新列表
void TreeListDlg::UpDateTreeList()
{
	m_showTree.DeleteAllItems();
	HTREEITEM hTreeItem1;
	HTREEITEM hTreeItem2;
	hTreeItem1 = m_showTree.InsertItem(_T("远程桌面控制"));
	m_showTree.SetItemData(hTreeItem1, 0);
	/*hTreeItem2 = m_showTree.InsertItem(_T("不可控电脑信号"));
	m_showTree.SetItemData(hTreeItem2, 1);*/

	::EnterCriticalSection(&g_cs);
	auto iter = g_VhostInfo.begin();
	while (iter != g_VhostInfo.end())
	{
		string name = iter->_Name;
		string state = iter->_state;
		if (state == "OnLine")
		{
			
			HTREEITEM hTreeItemNodeNew = m_showTree.InsertItem(CString(name.c_str()), hTreeItem1);
			m_showTree.SetItemData(hTreeItemNodeNew, (DWORD_PTR)hTreeItemNodeNew);
		}
		else
		{
			/*HTREEITEM hTreeItemNodeNew = m_showTree.InsertItem(CString(name.c_str()), hTreeItem2);
			m_showTree.SetItemData(hTreeItemNodeNew, (DWORD_PTR)hTreeItemNodeNew);*/
			//去掉其他树
		}
		iter++;
	}

	::LeaveCriticalSection(&g_cs);
}


void TreeListDlg::KillProcess(TCHAR* processName)
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
				//MessageBox("进程结束成功");
			}
			else
			{
				AfxMessageBox(_T("进程结束失败"));
			}

		}
	} while (Process32Next(shot, &pe));
}
