
// HomeworkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Homework.h"
#include "HomeworkDlg.h"
#include "afxdialogex.h"
#include "Cmain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHomeworkDlg 对话框




CHomeworkDlg::CHomeworkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHomeworkDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Port = 0;
	m_Name = _T("");
}

void CHomeworkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS2, m_ServerAddress);
	//  DDX_Control(pDX, IDC_EDIT_name, m_Name);
	DDX_Text(pDX, IDC_EDIT_Port, m_Port);
	DDV_MinMaxUInt(pDX, m_Port, 0, 100000);
	DDX_Text(pDX, IDC_EDIT_name, m_Name);
	DDV_MaxChars(pDX, m_Name, 20);
}

BEGIN_MESSAGE_MAP(CHomeworkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS2, &CHomeworkDlg::OnIpnFieldchangedIpaddress2)
	ON_BN_CLICKED(IDC_BUTTON_login, &CHomeworkDlg::OnBnClickedButtonlogin)
END_MESSAGE_MAP()


// CHomeworkDlg 消息处理程序

BOOL CHomeworkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ServerAddress.SetAddress( htonl(inet_addr("127.0.0.1")));
	m_Port=6000;
	UpdateData(false);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHomeworkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHomeworkDlg::OnPaint()
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
HCURSOR CHomeworkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHomeworkDlg::OnIpnFieldchangedIpaddress2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CHomeworkDlg::OnBnClickedButtonlogin()
{
	// TODO: 在此添加控件通知处理程序代码

	//以下是获取控件内容
	UpdateData(true);
	BYTE f0,f1,f2,f3;
	CString ip;
	m_ServerAddress.GetAddress(f0,f1,f2,f3);
	ip.Format( _T("%d.%d.%d.%d"), f0, f1, f2, f3);

	//以下是向服务器发送连接请求
	SOCKET sockClient;
	sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(INVALID_SOCKET == WSAGetLastError())
		MessageBox("Create Socket failed!");

	//2.向服务器发送连接请求
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ip);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(m_Port);
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	//3.和服务器进行通信，接收或是发送数据

	CString data;
	data.Format("Login:%s:%s:%d",m_Name,ip,m_Port);
	char * temp = data.GetBuffer(0);
	char sendBuf[30];
	data.ReleaseBuffer();
	strcpy(sendBuf,temp);
	send(sockClient, sendBuf, sizeof(sendBuf) + 1, 0);
	if(SOCKET_ERROR == WSAGetLastError())
		MessageBox("Server send data failed!");
	char recvBuf[100];
    recv(sockClient, recvBuf, 100, 0);
	if(strcmp(recvBuf,"y") == 0)
	{	
		CDialog::OnCancel();
		Cmain temp1;
		temp1.m_sock = sockClient;
		temp1.m_name = m_Name;
		//temp1.SetWindowTextA(m_Name);
		temp1.DoModal();
	
	}
	else
	{
		closesocket(sockClient);
	}
}


//该函数没有用到
DWORD WINAPI CHomeworkDlg::RecvProc(LPVOID lpParameter)        //静态类成员函数(实现)

{
	AfxMessageBox("子线程启动!");
	SOCKET sock=((RECVPARAM*)lpParameter)->sock;
    HWND hwnd=((RECVPARAM*)lpParameter)->hwnd;
	char tempBuf[300];
	//1.创建套接字
       sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
       if(INVALID_SOCKET == WSAGetLastError())
              AfxMessageBox("Create Socket failed!");
 
       //2.绑定套接字到一个端口号
       SOCKADDR_IN addrSrv;
       memset(&addrSrv, 0, sizeof(addrSrv));
       addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
       addrSrv.sin_family = AF_INET;
       addrSrv.sin_port = htons(6000);
       bind(sock, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
 
       //3.设置套接字为监听模式
       listen(sock,5);
 
       //4.等待用户的连接到来
       SOCKADDR_IN addrClient;
       int sockLen = sizeof(SOCKADDR);
       while(1)
       {
		      AfxMessageBox("监听已经启动!");
              SOCKET sockConn = accept(sock, (SOCKADDR*)&addrClient, &sockLen);
              if(INVALID_SOCKET == WSAGetLastError())
                     AfxMessageBox("Connect client failed!");
 
              //5.用返回的套接字和用户通信，发送数据或是接受数据
              char recvBuf[100];
              recv(sockConn, recvBuf, 100, 0);
              if(SOCKET_ERROR == WSAGetLastError())
                     AfxMessageBox("Server recieve data failed!");

              AfxMessageBox(recvBuf);
	   }
}