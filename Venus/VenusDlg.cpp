
// VenusDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusDlg.h"
#include "afxdialogex.h"
#include <windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CVenusDlg 对话框

CVenusDlg::CVenusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVenusDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	IDC_MYINFOTEXT = 0;
}
CVenusDlg::~CVenusDlg()
{
	delete myIndex;
}

void CVenusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVenusDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_KEYDOWN()
	ON_EN_CHANGE(IDC_EDIT1, &CVenusDlg::OnChangeEdit1)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CVenusDlg 消息处理程序

bool CVenusDlg::GetShowStatus()
{
	return this->curStatus;
}

void CVenusDlg::SetShowStatus(bool status)
{
	if (status)
	{
		myIndex->InitIndex();
		this->ShowWindow(SW_SHOWNORMAL);
	}
	else
	{
		this->ShowWindow(SW_HIDE);
	}
	this->curStatus = status;
}

void CVenusDlg::RunLnkFile(LPSTR path)
{
	//WinExec(exePath, SW_SHOWNORMAL);
	//ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);   // hack: “open”引起的内存无法释放？
	ShellExecute(NULL, NULL, path, NULL, NULL, SW_SHOWNORMAL);
}

BOOL CVenusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	IDC_MYINFOTEXT = 63301;
	myIndex = new LocalPath();
	lastKey = "";
	lastInfoSize = 0;
	curStatus = false;

	// 设置窗口位置
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);  //从任务栏中去掉
	SetWindowPos(NULL, 5, 5, 600, 300, 0);

	// 窗体透明
	SetBackgroundColor(RGB(1,11,21));
	LONG ExtendedStyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE );
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,ExtendedStyle | WS_EX_LAYERED );
	::SetLayeredWindowAttributes(GetSafeHwnd(),RGB(1,11,21),0,LWA_COLORKEY);

	// 输入框字体大小
	editFont.CreatePointFont(240,"宋体");
	CEdit *myInput = (CEdit *)GetDlgItem(IDC_EDIT1);
	myInput->SetFont(&editFont);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVenusDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVenusDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// 重载按键事件
void CVenusDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CVenusDlg::OnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// 清理之前结果
	lastKey = "";
	for (int i=0; i<lastInfoSize; ++i)
	{
		myInfoLabel[i]->DestroyWindow();
		delete myInfoLabel[i];
	}
	
	CString txt;
	GetDlgItemText(IDC_EDIT1, txt);

	vector<CString> keyMatch;
	myIndex->FindIndex(txt, keyMatch);

	// 输出标签中每个字符的大小
	int labelWidth = 26;
	int labelHeight = 50;

	int rect_left = 10;
	int rect_top = 50;

	for (int i=0; i < keyMatch.size(); i++)
	{
		if (i == 0)
		{
			lastKey = keyMatch[i];
		}
		if (i >= 50)
		{
			break;
		}
		CRect rect;
		rect.left = rect_left;
		int clen = keyMatch[i].GetLength();
		rect.right = rect_left + clen * labelWidth;  // 字数动态调整宽度
		rect.top = rect_top + i * labelHeight;
		rect.bottom = rect_top + labelHeight * 0.8 + i * labelHeight;

		myInfoLabel[i] = new CLabel();
		myInfoLabel[i]->Create(keyMatch[i], WS_VISIBLE, rect, this, IDC_MYINFOTEXT+i);
		myInfoLabel[i]->SetTextColor(RGB(255, 255, 255));
		myInfoLabel[i]->SetFontName("Courier New");
		myInfoLabel[i]->SetFontSize(labelWidth-1);
		myInfoLabel[i]->SetFontBold(TRUE);
		myInfoLabel[i]->SetBkColor(RGB(10, 136, 255));
		//myInfoLabel[i]->SetTransparent(TRUE);
	}

	lastInfoSize = keyMatch.size() > 50 ? 50: keyMatch.size();
	SetWindowPos(NULL, 5, 5, 600, rect_top + (keyMatch.size()+1)*labelHeight, 0);
	//_CrtDumpMemoryLeaks();  // 检查内存
}


BOOL CVenusDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)
	{
		// 隐藏窗体
		SetShowStatus(false);

		CString txt;
		GetDlgItemText(IDC_EDIT1, txt);

		if (pMsg->wParam == VK_RETURN)
		{
			// ctrl + enter 百度搜索输入的关键字
			if (GetAsyncKeyState(VK_CONTROL)<0)
			{
				CString url;
				url.Format("www.baidu.com/s?wd=%s", txt);
				ShellExecute(NULL, _T("open"), _T(url), NULL, NULL, SW_SHOWNORMAL);
			}
			// shift + enter 查询DICT字典
			else if (GetAsyncKeyState(VK_SHIFT)<0)
			{
				CString url;
				url.Format("dict.cn/%s", txt);
				ShellExecute(NULL, _T("open"), _T(url), NULL, NULL, SW_SHOWNORMAL);
			}
			// 仅回车时执行当前匹配到快捷程序
			else
			{
				if (!lastKey.IsEmpty())
				{
					CString exePath = myIndex->GetLocalPath(lastKey);
					if (lastKey == ";lnk")
					{
						system(exePath);
					}
					else
					{
						RunLnkFile((LPSTR)(LPCSTR)exePath);
					}
				}

			}
		}

		// 清空输入内容
		CEdit *myInput = (CEdit *)GetDlgItem(IDC_EDIT1);
		myInput->SetSel(0, -1);
		myInput->Clear();

		return FALSE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CVenusDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// 改变编辑框文字颜色
	if (pWnd->GetDlgCtrlID() == IDC_EDIT1)
	{
		pDC->SetTextColor(RGB(248, 252, 255));
		pDC->SetBkColor(RGB(15, 15, 15));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
