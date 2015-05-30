
// VenusDlg.h : 头文件
//

#pragma once
#include <Windows.h>
#include "Label.h"
#include "LocalPath.h"

// CVenusDlg 对话框
class CVenusDlg : public CDialogEx
{
// 构造
public:
	CVenusDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CVenusDlg();

// 对话框数据
	enum { IDD = IDD_VENUS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	LocalPath *myIndex;
	CLabel *myInfoLabel[50];
	CFont editFont;
	CString lastKey;
	int lastInfoSize;
	bool curStatus;

public:
	bool GetShowStatus();
	void SetShowStatus(bool status);
	void RunLnkFile(LPSTR path);


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChangeEdit1();
	int IDC_MYINFOTEXT;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
