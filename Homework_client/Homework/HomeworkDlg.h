
// HomeworkDlg.h : 头文件
//

#pragma once


// CHomeworkDlg 对话框
class CHomeworkDlg : public CDialogEx
{
// 构造
public:
	CHomeworkDlg(CWnd* pParent = NULL);	// 标准构造函数
	static DWORD WINAPI RecvProc(LPVOID lpParameter);
// 对话框数据
	enum { IDD = IDD_HOMEWORK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIpnFieldchangedIpaddress2(NMHDR *pNMHDR, LRESULT *pResult);
	CIPAddressCtrl m_ServerAddress;
//	CEdit m_Name;
	UINT m_Port;
	CString m_Name;
	afx_msg void OnBnClickedButtonlogin();
};
