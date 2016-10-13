
// homework_serverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "homework_server.h"
#include "homework_serverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//设置全局变量
vector<SOCKET> UserSocket;
UINT m_userNum;
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
public:
	afx_msg void Onportset();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
	ON_COMMAND(ID_port_set, &CAboutDlg::Onportset)
END_MESSAGE_MAP()


// Chomework_serverDlg 对话框




Chomework_serverDlg::Chomework_serverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Chomework_serverDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Chomework_serverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LIST2, m_list_ChatContext);
}

BEGIN_MESSAGE_MAP(Chomework_serverDlg, CDialogEx)
	ON_MESSAGE(WM_RECVDATA,OnRecvData)
	ON_MESSAGE(WM_USER_EXIT,OnUserExit)
	ON_MESSAGE(WM_USER_SENDMSG,OnUserEcho)
	ON_MESSAGE(WM_USER_LIST,OnUserList)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_port_set, &Chomework_serverDlg::Onportset)
	ON_COMMAND(ID_OUTPUT_CHATCONTEXT, &Chomework_serverDlg::OnOutputChatcontext)
END_MESSAGE_MAP()


// Chomework_serverDlg 消息处理程序


BOOL Chomework_serverDlg::OnInitDialog()
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
	m_userNum = 0;
	m_StatBar=new CStatusBarCtrl;
	RECT m_Rect;
	GetClientRect(&m_Rect); //获取对话框的矩形区域
	m_Rect.top=m_Rect.bottom-20; //设置状态栏的矩形区域
	m_StatBar->Create(WS_BORDER|WS_VISIBLE|CBRS_BOTTOM,m_Rect,this,3);
	int nParts[2]= {300, 500}; //分割尺寸
	m_StatBar->SetParts(2, nParts); //分割状态栏
	m_StatBar->SetText("当前在线数为：0",0,0); //第一个分栏加入"这是第一个指示器"
	m_StatBar->SetText("当前未开启服务器",1,0); //以下类似

	filePath = ".\\Recv\\";
	//listcontrol控件初始化

	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);// 获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; // 清除显示方式位
	lStyle |= LVS_REPORT; // 设置style
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);// 设置style
	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ）
	dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report 风格的listctrl ）
	m_list.SetExtendedStyle(dwStyle); // 设置扩展风格 
	m_list.InsertColumn( 0, "在线用户", LVCFMT_LEFT, 176 );// 插入列

	LONG lStyle1;
	lStyle1 = GetWindowLong(m_list_ChatContext.m_hWnd, GWL_STYLE);// 获取当前窗口style
	lStyle1 &= ~LVS_TYPEMASK; // 清除显示方式位
	lStyle1 |= LVS_REPORT; // 设置style
	SetWindowLong(m_list_ChatContext.m_hWnd, GWL_STYLE, lStyle1);// 设置style
	DWORD dwStyle1 = m_list_ChatContext.GetExtendedStyle();
	dwStyle1 |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ）
	//dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report 风格的listctrl ）
	m_list_ChatContext.SetExtendedStyle(dwStyle1); // 设置扩展风格 
	m_list_ChatContext.InsertColumn( 0, "聊天记录", LVCFMT_LEFT, 449 );// 插入列
	//新插入的在上面
	
	m_image.Create(16,16, TRUE|ILC_COLOR24, 3, 1);
    m_list.SetImageList(&m_image,LVSIL_SMALL);
	HICON hicon=(HICON)LoadImage(
        NULL,    //handle of the instance that contains //the image
        "1.ico",//name or identifier of image
        IMAGE_ICON,//type of image-can also be IMAGE_CURSOR or MAGE_ICON
        0,0,//desired width and height
        LR_LOADFROMFILE);//load flags
	m_image.Add(hicon);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Chomework_serverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Chomework_serverDlg::OnPaint()
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
HCURSOR Chomework_serverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//菜单栏绑定的函数
void CAboutDlg::Onportset(void)
{}

void Chomework_serverDlg::Onportset()
{
	// TODO: 在此添加命令处理程序代码
	m_StatBar->SetText("当前已开启服务器",1,0); //以下类似
	RECVPARAM *pRecvParam = new RECVPARAM;
	pRecvParam->sock = m_sock;
	pRecvParam->hwnd = m_hWnd;
	HANDLE hThread=CreateThread(NULL,0,RecvProc,(LPVOID)pRecvParam,0,NULL);
	
}
//接收数据信号绑定的函数
LRESULT Chomework_serverDlg::OnRecvData(WPARAM wParam,LPARAM lParam)

{   
	//获取收到的字符串
	char* temp = (char*)lParam;
	//分割字符串获取定义的参数
	CString strSource,ch;
	vector <CString> vecString;
	int iPos = 0;
	CString strTmp;
	strSource.Format("%s",temp);
	ch = ":";
	strTmp = strSource.Tokenize(ch,iPos);
	while(strTmp.Trim() != _T(""))
	{
		vecString.push_back(strTmp);
		strTmp = strSource.Tokenize(ch,iPos);
	}
	if(vecString[0] == "Login")
	{
		CString str;
		str.Format("用户：%s  请求链接服务器！",vecString[1]);
		if(IDYES==AfxMessageBox(str,MB_YESNO|MB_ICONQUESTION))
		{
			m_userNum++;
			MessageBox("已允许链接");

			//创建新的用户信息表

			UserInfo newUserInfo;
			newUserInfo.UserId = m_userNum;
			newUserInfo.time = CTime::GetCurrentTime();
			newUserInfo.UserName = vecString[1];
			newUserInfo.UserIp = vecString[2];
			newUserInfo.port = atoi(vecString[3]);
			newUserInfo.m_socket = UserSocket[m_userNum-1];
			Exint * tempInt = new Exint;
			tempInt->number = m_userNum;
			tempInt->hwnd = m_hWnd;
			newUserInfo.ThreadId = CreateThread(NULL,0,CreatUser,(LPVOID)(tempInt),0,NULL);//创建子线程进行通信
			user_info.push_back(newUserInfo);


			//以下是设置控件内容

			//设置状态栏信息
			CString temp;
			temp.Format("当前在线数为：%d",m_userNum);
			m_StatBar->SetText(temp,0,0);
			//以下是对listcontrol的更新
			int nIndex = m_list.GetItemCount();  
			LV_ITEM   lvitemAdd = {0};  
			lvitemAdd.mask = LVIF_TEXT|LVIF_IMAGE;  
			lvitemAdd.iItem = nIndex ;  
			lvitemAdd.iSubItem = 0;  
			lvitemAdd.pszText = newUserInfo.UserName.GetBuffer();
			newUserInfo.UserName.ReleaseBuffer();
			lvitemAdd.iImage = 0;  //(某个窗口的窗口句柄)  
			m_list.InsertItem(&lvitemAdd);
            /*
			int nIndex1 = m_list_ChatContext.GetItemCount();  
			LV_ITEM   lvitemAdd1 = {0};  
			lvitemAdd1.mask = LVIF_TEXT;  
			lvitemAdd1.iItem = nIndex1 ;  
			lvitemAdd1.iSubItem = 0;  
			temp.Format("%s:加入聊天室",newUserInfo.UserName);
			lvitemAdd1.pszText = temp.GetBuffer();
			temp.ReleaseBuffer();
			m_list_ChatContext.InsertItem(&lvitemAdd1);
			*/

			//发送用户信息
			char sendBuf[] = "y";
	        send(UserSocket[m_userNum-1], sendBuf, 100, 0);


			Sleep(1000);

			CString strList = "LOGIN," + newUserInfo.UserName;
			SendMessage_r(strList);

		}
		else
		{

			MessageBox("已拒绝链接！");
		}
	}
	else if(vecString[0] == "File")
	{
		SOCKET socket = UserSocket[UserSocket.size() -1];
		UserSocket.pop_back();
		CString str;
		int iFileSize = atoi(vecString[3]); 
		str.Format("用户：%s  请求发送文件！ \n 文件名：%s \n 大小;%d",vecString[1],vecString[2],iFileSize);
		if(IDYES==AfxMessageBox(str,MB_YESNO|MB_ICONQUESTION))
		{
		fileInfo* newFile = new fileInfo;
		newFile->m_From = vecString[1];
		newFile->m_fileName = vecString[2];
		newFile->m_filePatch = filePath;
		newFile->hwnd = m_hWnd;
		newFile->m_fileSocket = socket;
		if(!CreateThread(NULL,0,RecvFile,(LPVOID)(newFile),0,NULL))
		{

		}
		}
	}
	return true;
}
//接收线程函数
DWORD WINAPI Chomework_serverDlg::RecvProc(LPVOID lpParameter)        //静态类成员函数(实现)

{
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
              SOCKET sockConn = accept(sock, (SOCKADDR*)&addrClient, &sockLen);
              if(INVALID_SOCKET == WSAGetLastError())
                     AfxMessageBox("Connect client failed!");
 
              //5.用返回的套接字和用户通信，发送数据或是接受数据
              char recvBuf[128];
              recv(sockConn, recvBuf, 128, 0);
              if(SOCKET_ERROR == WSAGetLastError())
                     AfxMessageBox("Server recieve data failed!");

              ::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)recvBuf);
			  UserSocket.push_back(sockConn);
       }
	 
	return 0;
}
//创建用户信息函数
DWORD WINAPI Chomework_serverDlg::CreatUser(LPVOID lpParameter)
{
	UINT num = ((Exint *)lpParameter)->number;
	HWND m_hwnd = ((Exint *)lpParameter)->hwnd;
	SOCKET clientSocket = UserSocket[num-1];
	while(1)
	{
		char recvBuf[128];
        int Irecv = recv(clientSocket, recvBuf, 128, 0);
		//AfxMessageBox(recvBuf);
		if(Irecv <= 0)
		{
			closesocket(clientSocket);
			std::vector<SOCKET>::iterator it = UserSocket.begin();
			for(it = UserSocket.begin();it != UserSocket.end();it++)
			{
				if((*it) == clientSocket)
				{
					UserSocket.erase(it);
					break;
				}
			}
			int * Pnum = new int;
			*Pnum = num;
			::PostMessage(m_hwnd,WM_USER_EXIT,0,(LPARAM)Pnum);
			m_userNum--;
			break;
		}
		else if(strcmp(recvBuf,"EXIT") == 0)
		{
			closesocket(clientSocket);
			std::vector<SOCKET>::iterator it = UserSocket.begin() + num - 1;
			for(it = UserSocket.begin();it != UserSocket.end();it++)
			{
				if( (*it) == clientSocket )
				{
						UserSocket.erase(it);
						break;
				}
			}
			int * Pnum = new int;
			*Pnum = num;
			::PostMessage(m_hwnd,WM_USER_EXIT,0,(LPARAM)Pnum);
			m_userNum--;
			break;
		}
		else if(strcmp(recvBuf,"USERLIST") == 0)
		{
			int * Pnum = new int;
			*Pnum = num;
			::PostMessage(m_hwnd,WM_USER_LIST,0,(LPARAM)Pnum);

		}
		else
		{
			::PostMessage(m_hwnd,WM_USER_SENDMSG,0,(LPARAM)recvBuf);
		}
	}
	return 0;
}
//用户退出的函数
LRESULT Chomework_serverDlg::OnUserExit(WPARAM wParam,LPARAM lParam)
{


    int User_num = *(int *)lParam;
	int ii = 0,Iindex = 0;	
    CString str,temp;
	for(ii = 0; ii < user_info.size();ii++)
	{
		if(user_info[ii].UserId == User_num)
			break;
	}
	vector<UserInfo>::iterator it = user_info.begin() + ii ;
	//将用户登录情况写入日志。。。
    CString Fpatch = ".\\log\\Userinfo.txt";
	CTime tempTime = CTime::GetCurrentTime();
	CTimeSpan span = tempTime - user_info[ii].time;

	CString str1 = "用户:"+ user_info[ii].UserName + "\n";
	CString str2 = "IP地址为: " + user_info[ii].UserIp + "\n";
	CString str3 = "登录时间为：" + user_info[ii].time.Format("%Y-%m-%d %H:%M:%S %W-%A") + "\n";
    CString str4 = "退出时间为: " + tempTime.Format("%Y-%m-%d %H:%M:%S %W-%A") + "\n";
	CString str5 = "在线时间为：" + span.Format("%D天-%H小时-%M分钟-%S秒") + "\n" + "\n";
	CString WriteStr = str1 + str2 + str3 + str4 + str5;

	CStdioFile Flog(Fpatch,CFile::typeText|CFile::modeNoTruncate|CFile::modeCreate|CFile::modeReadWrite);
	Flog.SeekToEnd();
	Flog.WriteString(WriteStr);
	Flog.Close();

	//设置控件内容

	str.Format("当前在线数为：%d",m_userNum);
	m_StatBar->SetText(str,0,0);
	//设置listcontrol的内容
	int nIndex1 = m_list_ChatContext.GetItemCount();  
	LV_ITEM   lvitemAdd1 = {0};  
	lvitemAdd1.mask = LVIF_TEXT;  
	lvitemAdd1.iItem = nIndex1 ;  
	lvitemAdd1.iSubItem = 0;  
	temp.Format("%s:离开聊天室",user_info[ii].UserName);
	lvitemAdd1.pszText = temp.GetBuffer();
	temp.ReleaseBuffer();
	m_list_ChatContext.InsertItem(&lvitemAdd1);

	//向客户端发送数据
	CString strTemp = "MSG," + temp;
	SendMessage_r(strTemp);

	Sleep(1000);
	CString strList;
	strList = "LOGOUT," + user_info[ii].UserName;
	SendMessage_r(strList);

	//删除m_list中对应的用户

	for(Iindex = 0; Iindex < m_list.GetItemCount(); Iindex++)
	{
		str = m_list.GetItemText(Iindex,0);
		if(str == user_info[ii].UserName)
		{
			m_list.DeleteItem(Iindex);
			break;
		}
	}
	user_info.erase(it);
	return 0;
}
//循环发送数据函数，负责发送给客户端发送数据
UINT Chomework_serverDlg::SendMessage_r(CString message)
{
	if(message.GetLength() > 128)
		return 0;
	int ii = 0;
	char* sendBuf = (LPSTR)(LPCTSTR)message;
	char Buff[128];
	strcpy(Buff,sendBuf);
	for(ii = 0; ii < UserSocket.size(); ii++)
	{
		send(UserSocket[ii], Buff,128, 0);
	}
	return 1;
}
//负责对客户端发出的信息广播出去
LRESULT Chomework_serverDlg::OnUserEcho(WPARAM wParam,LPARAM lParam)
{
	char* temp = (char* ) lParam;
	CString str;
	str.Format("MSG,%s",temp);
	CString strmsg;
	strmsg.Format("%s",temp);
	SendMessage_r(str);
	int nIndex1 = m_list_ChatContext.GetItemCount();  
	LV_ITEM   lvitemAdd1 = {0};  
	lvitemAdd1.mask = LVIF_TEXT;  
	lvitemAdd1.iItem = nIndex1 ;  
	lvitemAdd1.iSubItem = 0;  
	lvitemAdd1.pszText = strmsg.GetBuffer();
	strmsg.ReleaseBuffer();
	m_list_ChatContext.InsertItem(&lvitemAdd1);
	
    return 0;
}
//记录聊天记录的程序
void Chomework_serverDlg::OnOutputChatcontext()
{
	// TODO: 在此添加命令处理程序代码

	CTime tm = CTime::GetCurrentTime();
	CString str = tm.Format("%Y%m%d%H%M%S%W%A");
	CString Fpatch = ".\\log\\" + str + ".txt";
	CStdioFile Flog(Fpatch,CFile::typeText|CFile::modeCreate|CFile::modeReadWrite);
	int iIndex = m_list_ChatContext.GetItemCount();
	for(int i = 0;i < iIndex; i++)
	{
	CString s = m_list_ChatContext.GetItemText(i,0);
	s += "\n";
	Flog.WriteString(s);
	}
	Flog.Close();
}
//处理用户在线列表的函数
LRESULT Chomework_serverDlg::OnUserList(WPARAM wParam,LPARAM lParam)
{
	int number = *(int *)lParam;
	int Iindex = m_list.GetItemCount();
	SOCKET clientSocket;
	for(int i = 0;i < user_info.size();i++)
	{
		if(user_info[i].UserId == number)
		{
			clientSocket = user_info[i].m_socket;
			break;
		}
	}
	for(int ii = 0;ii < Iindex; ii++)
	{
		CString message = "LOGIN," + m_list.GetItemText(ii,0);
		char* sendBuf = (LPSTR)(LPCTSTR)message;
	    char Buff[128];
	    strcpy(Buff,sendBuf);
		Sleep(20);
		send(clientSocket, Buff,128, 0);
	}

	for(int i = 0;i < m_list_ChatContext.GetItemCount();i++)
	{
		CString message = "MSG," + m_list_ChatContext.GetItemText(i,0);
		char* sendBuf = (LPSTR)(LPCTSTR)message;
	    char Buff[128];
	    strcpy(Buff,sendBuf);
		Sleep(20);
		send(clientSocket, Buff,128, 0);
	}
	return 0;
}
//创建接收文件线程
DWORD WINAPI Chomework_serverDlg::RecvFile(LPVOID lpParameter)
{
	fileInfo newFileInfo = *(fileInfo*) lpParameter;
	CFile newFile;
	char recvBuf[1024];
	newFile.Open(newFileInfo.m_fileName,CFile::modeCreate|CFile::modeWrite);
	while(1)
	{
		int Irecv = recv(newFileInfo.m_fileSocket, recvBuf, 1024, 0);
		if(Irecv>0)
		{
			    newFile.Write(recvBuf,Irecv);
				char buff[128] = "y";
				send(newFileInfo.m_fileSocket,buff,128,0);

		}
		else
		{
			break;
		}

	}
	newFile.Close();
	return 0;
}