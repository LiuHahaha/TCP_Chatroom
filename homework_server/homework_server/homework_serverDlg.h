
// homework_serverDlg.h : 头文件
//

#pragma once


// Chomework_serverDlg 对话框
class Chomework_serverDlg : public CDialogEx
{
// 构造
public:
	Chomework_serverDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_HOMEWORK_SERVER_DIALOG };

//自定义函数
	static DWORD WINAPI RecvProc(LPVOID lpParameter);//接收客户端信息的线程函数
	static DWORD WINAPI CreatUser(LPVOID lpParameter);//创建用户并接收客户端的线程函数
	static DWORD WINAPI RecvFile(LPVOID lpParameter);//接收文件的线程
	UINT   SendMessage_r(CString message);//循环发送数据的函数         
//自定义数据
	SOCKET m_sock;
	vector<UserInfo> user_info;
	vector<fileInfo> fileList;
	CStatusBarCtrl *m_StatBar;
	CImageList m_image;
	CString filePath;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	LRESULT afx_msg OnRecvData(WPARAM wParam,LPARAM lParam);//处理客户端连接申请的函数
	LRESULT afx_msg OnUserExit(WPARAM wParam,LPARAM lParam);//处理用户退出申请的函数
	LRESULT afx_msg OnUserEcho(WPARAM wParam,LPARAM lParam);//处理用户里聊天信息的函数
	LRESULT afx_msg OnUserList(WPARAM wParam,LPARAM lParam);//处理用户列表的函数
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void Onportset();
	afx_msg void OnBnClickedButton2();
	CListCtrl m_list;
	CListCtrl m_list_ChatContext;
	afx_msg void OnOutputChatcontext();
};
