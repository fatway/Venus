
// VenusDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusDlg.h"
#include "afxdialogex.h"
#include "Label.h"

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

	// 设置窗口位置
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);  //从任务栏中去掉
	SetWindowPos(NULL, 5, 5, 600, 300, 0);

	// 窗体透明
	SetBackgroundColor(RGB(1,11,21));
	LONG ExtendedStyle = GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE );
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,ExtendedStyle | WS_EX_LAYERED );
	::SetLayeredWindowAttributes(GetSafeHwnd(),RGB(1,11,21),0,LWA_COLORKEY);

	// 输入框字体大小
	editFont.CreatePointFont(260,"宋体");
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

	// TODO:  在此添加控件通知处理程序代码

	// 清理之前结果
	lastKey = "";
	for (int i=0; i< 100; i++)
	{
		CStatic *pOldInfo = (CStatic *)GetDlgItem(IDC_MYINFOTEXT+i);
		delete pOldInfo;
	}
	
	CString txt;
	GetDlgItemText(IDC_EDIT1, txt);

	vector<CString> keyMatch;
	myIndex->FindIndex(txt, keyMatch);

	// 输出标签中每个字符的大小
	int labelWidth = 30;
	int labelHeight = 60;

	int rect_left = 10;
	int rect_top = 60;

	for (int i=0; i < keyMatch.size(); i++)
	{
		if (i == 0)
		{
			lastKey = keyMatch[i];
		}
		//CStatic *pInfo = new CStatic();
		CRect rect;
		rect.left = rect_left;
		int clen = keyMatch[i].GetLength();
		rect.right = rect_left + clen * labelWidth;  // 字数
		rect.top = rect_top + i * labelHeight;
		rect.bottom = rect_top + labelHeight * 0.8 + i * labelHeight;

		CLabel *pInfo = new CLabel();
		pInfo->Create(keyMatch[i], WS_VISIBLE, rect, this, IDC_MYINFOTEXT+i);
		pInfo->SetTextColor(RGB(255, 255, 255));
		pInfo->SetFontName("Fixedsys");
		pInfo->SetFontSize(labelWidth-1);
		pInfo->SetFontBold(TRUE);
		pInfo->SetBkColor(RGB(10, 136, 255));
	}

	SetWindowPos(NULL, 5, 5, 600, rect_top + (keyMatch.size()+1)*labelHeight, 0);
}


BOOL CVenusDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->wParam==VK_RETURN)
	{
		if (lastKey.IsEmpty())
		{
			//MessageBoxA(_T("没办法执行，结果为空"));
			return TRUE;
		}
		else
		{
			//MessageBoxA(myIndex->GetLocalPath(lastKey));
			CString exePath = myIndex->GetLocalPath(lastKey);
			WinExec(exePath, SW_SHOWNORMAL);
			//_execl(exePath);
		}
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
		pDC->SetTextColor(RGB(240, 240, 240));
		pDC->SetBkColor(RGB(10, 10, 10));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
