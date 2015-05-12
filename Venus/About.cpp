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
END_MESSAGE_MAP()


// CAbout 消息处理程序
