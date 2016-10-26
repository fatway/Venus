// About.cpp : 实现文件
//

#include "stdafx.h"
#include "Venus.h"
#include "About.h"
#include "afxdialogex.h"


// CAbout 对话框

IMPLEMENT_DYNAMIC(CAbout, CDialog)

CAbout::CAbout(CWnd* pParent /*=NULL*/)
	: CDialog(CAbout::IDD, pParent)
{
}

CAbout::~CAbout()
{
}

void CAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAbout, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &CAbout::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &CAbout::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAbout 消息处理程序


// 打开注册表启动项
bool CAbout::OpenRegeditRun()
{
	if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 关闭注册表
void CAbout::CloseRegeditRun()
{
	RegCloseKey(hKey);
}


void CAbout::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_button->GetCheck())
	{
		if (OpenRegeditRun())
		{
			TCHAR szModule[MAX_PATH];
			GetModuleFileName(NULL, szModule, MAX_PATH);//得到本程序自身的全路径  

			RegSetValueEx(hKey, strMyKey, 0, REG_SZ, (LPBYTE)szModule, (lstrlen(szModule) + 1)*sizeof(TCHAR));

			CloseRegeditRun();
		}
		else
		{
			AfxMessageBox(_T("系统参数错误,不能随系统启动"));
		}
	}
	else
	{
		if (OpenRegeditRun())
		{
			RegDeleteValue(hKey, strMyKey);
			CloseRegeditRun();
		}
	}
}


BOOL CAbout::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 检查当前是否有自启动项
	strRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	strMyKey = "Venus";
	m_button = (CButton *)GetDlgItem(IDC_CHECK1);

	wchar_t strMyValue[MAX_PATH];
	unsigned long i,j;
	if (OpenRegeditRun())
	{
		// 查找启动项下是否已有该子项
		if (RegQueryValueEx(hKey, strMyKey, 0, &j, (LPBYTE)strMyValue, &i)==ERROR_SUCCESS)
		{
			m_button->SetCheck(1);
		}
		else
		{
			m_button->SetCheck(0);
		}
		CloseRegeditRun();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAbout::OnBnClickedButton1()
{
	// 打开安装目录
	char szFilePath[MAX_PATH + 1]={0};
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串
	char szOpera[MAX_PATH + 1] = "explorer /e \"";
	strcat_s(szOpera, szFilePath);
	strcat_s(szOpera, "\"");
	system(szOpera);
}
