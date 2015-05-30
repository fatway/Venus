// DlgStart.cpp : 实现文件
//

#include "stdafx.h"
#include "Venus.h"
#include "DlgStart.h"
#include "afxdialogex.h"
#include "About.h"
#include "resource.h"


// CDlgStart 对话框

IMPLEMENT_DYNAMIC(CDlgStart, CDialogEx)

CDlgStart::CDlgStart(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgStart::IDD, pParent)
{

}

CDlgStart::~CDlgStart()
{
}

void CDlgStart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgStart, CDialogEx)
	ON_MESSAGE ( WM_HOTKEY, OnHotKey )
	ON_MESSAGE ( WM_SHOW_TASK, OnShowTask )
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_32771, &CDlgStart::On32771)
	ON_COMMAND(ID_32772, &CDlgStart::On32772)
END_MESSAGE_MAP()


// CDlgStart 消息处理程序


BOOL CDlgStart::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	pDlg = new CVenusDlg(this);
	pDlg->Create(IDD_VENUS_DIALOG);
	pDlg->SetShowStatus(false);

	// 注册热键
	UINT iHotKey = VK_SPACE;
#ifdef _DEBUG
	iHotKey = VK_F1;
#endif
	if (FALSE == ::RegisterHotKey(this->GetSafeHwnd(), ID_OPEN_USER_DLG, MOD_ALT, iHotKey))
	{
		MessageBoxA("注册Alt+Space热键失败，可能被其他程序占用，请检查。", "启明星", MB_ICONWARNING);
	}

	// 启动时不显示窗体
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);  //从任务栏中去掉

	// 显示托盘图标
	ShowSys2Task();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


LRESULT CDlgStart::OnHotKey ( WPARAM wPARAM, LPARAM lPARAM )
{
	if (wPARAM == ID_OPEN_USER_DLG)
	{
		if ( !pDlg->GetShowStatus() )
		{
			pDlg->SetShowStatus(true);

			// 使新窗口获取系统焦点
			typedef void (WINAPI *PSWITCHTOTHISWINDOW) (HWND,BOOL);
			PSWITCHTOTHISWINDOW SwitchToThisWindow;
			HMODULE hUser32=GetModuleHandle(_T("user32"));
			SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
			SwitchToThisWindow(pDlg->GetSafeHwnd(), TRUE);
		}
		else
		{
			pDlg->SetShowStatus(false);
		}
	}
	return 0;
}

LRESULT CDlgStart::OnShowTask(WPARAM wParam, LPARAM lParam)   
{   
	if(wParam != IDR_MAINFRAME)   
		return 1;
	if(WM_LBUTTONDBLCLK == lParam)  
	{
		CAbout about;
		about.DoModal();
	}
	if (WM_RBUTTONDOWN == lParam)
	{
		LPPOINT point=new tagPOINT; 
		::GetCursorPos(point);     //得到鼠标位置

		CMenu menu;
		menu.LoadMenu(IDR_MENU1);//加载菜单资源

		CMenu *pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);

		delete point;
	}
	return 0;
}

// 重载系统操作
void CDlgStart::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (nID == SC_CLOSE) // 关闭窗口
	{
		NOTIFYICONDATA nd;
		nd.cbSize    = sizeof (NOTIFYICONDATA);
		nd.hWnd    = this->m_hWnd;
		nd.uID   =   IDR_MAINFRAME;
		Shell_NotifyIcon(NIM_DELETE, &nd);    // 托盘图标不显示
	}

	CDialogEx::OnSysCommand(nID, lParam);
}

void CDlgStart::ShowSys2Task()
{
	NOTIFYICONDATA nid; 
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd = this->m_hWnd; 
	nid.uID = IDR_MAINFRAME; 
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP ; 
	nid.uCallbackMessage = WM_SHOW_TASK;        // 自定义的消息名称 
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)); 
	strcpy_s(nid.szTip, "启明星");              // 信息提示条
	Shell_NotifyIcon(NIM_ADD, &nid);            // 在托盘区添加图标 
}

void CDlgStart::On32771()
{
	// TODO: 在此添加命令处理程序代码
	CAbout about;
	about.DoModal();
}

void CDlgStart::On32772()
{
	// TODO: 在此添加命令处理程序代码
	//this->DestroyWindow();
	this->OnOK();
}
