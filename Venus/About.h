#pragma once


// CAbout 对话框

class CAbout : public CDialog
{
	DECLARE_DYNAMIC(CAbout)

public:
	CAbout(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAbout();

// 对话框数据
	enum { IDD = IDD_ABOUT };

private:
	// 注册表启动项参数
	HKEY hKey;
	CString strRegPath, strMyKey;
	CButton* m_button;
	bool OpenRegeditRun();
	void CloseRegeditRun();
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	virtual BOOL OnInitDialog();
};
