// Cmain.cpp : 实现文件
//

#include "stdafx.h"
#include "Homework.h"
#include "Cmain.h"
#include "afxdialogex.h"


// Cmain 对话框

IMPLEMENT_DYNAMIC(Cmain, CDialogEx)

Cmain::Cmain(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cmain::IDD, pParent)
{

	//  m_sendMessage = _T("");
	m_SendMessage = _T("");
}

Cmain::~Cmain()
{
}

BOOL Cmain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//创建子线程负责接收服务器数据
	RECVPARAM *pRecvParam = new RECVPARAM;
	pRecvParam->sock = m_sock;
	pRecvParam->hwnd = m_hWnd;
	HANDLE hThread=CreateThread(NULL,0,RecvProc,(LPVOID)pRecvParam,0,NULL);
	//控件初始化
	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);// 获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; // 清除显示方式位
	lStyle |= LVS_REPORT; // 设置style
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);// 设置style
	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ）
	dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report 风格的listctrl ）
	m_list.SetExtendedStyle(dwStyle); // 设置扩展风格 
	m_list.InsertColumn( 0, "在线用户", LVCFMT_LEFT, 169 );// 插入列

	LONG lStyle1;
	lStyle1 = GetWindowLong(m_list_ChatContext.m_hWnd, GWL_STYLE);// 获取当前窗口style
	lStyle1 &= ~LVS_TYPEMASK; // 清除显示方式位
	lStyle1 |= LVS_REPORT; // 设置style
	SetWindowLong(m_list_ChatContext.m_hWnd, GWL_STYLE, lStyle1);// 设置style
	DWORD dwStyle1 = m_list_ChatContext.GetExtendedStyle();
	dwStyle1 |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ）
	//dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report 风格的listctrl ）
	m_list_ChatContext.SetExtendedStyle(dwStyle1); // 设置扩展风格 
	m_list_ChatContext.InsertColumn( 0, "聊天记录", LVCFMT_LEFT, 497 );// 插入列
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

	this->SetWindowTextA(m_name);

	CString temp;
	char sendBuf[128];
	temp.Format("%s:已加入聊天室！",m_name);
	char * ctemp = (LPSTR)(LPCSTR)temp;
	strcpy(sendBuf,ctemp);
	send(m_sock, sendBuf,128, 0);
    SetTimer(1,1000,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void Cmain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_SendMessage);
	DDV_MaxChars(pDX, m_SendMessage, 1024);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LIST2, m_list_ChatContext);
}


BEGIN_MESSAGE_MAP(Cmain, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &Cmain::OnBnClickedButtonSend)
	ON_MESSAGE(WM_RECVDATA,OnRecvData)
	ON_COMMAND(ID_OnRecord, &Cmain::OnOnrecord)
	ON_COMMAND(ID_32771, &Cmain::On32771)
	ON_WM_TIMER()
	ON_COMMAND(ID_transfile, &Cmain::Ontransfile)
END_MESSAGE_MAP()


// Cmain 消息处理程序


void Cmain::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString str = m_name + ":" + m_SendMessage;
	char sendBuf[128];
	char * temp = (LPSTR)(LPCSTR)str;
	strcpy(sendBuf,temp);
	send(m_sock, sendBuf,128, 0);
	m_SendMessage = "";
	UpdateData(false);
}

DWORD WINAPI Cmain::RecvProc(LPVOID lpParameter)
{
	SOCKET sock=((RECVPARAM*)lpParameter)->sock;
    HWND hwnd=((RECVPARAM*)lpParameter)->hwnd;
	while(1)
	{
		char recvBuf[128];
        int Irecv = recv(sock, recvBuf, 128, 0);
		if(Irecv > 0)
		::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)recvBuf);
		else
		{
			AfxMessageBox("服务器已经断开连接！");
			break;
		}
		
	}
	return 0;
}

LRESULT Cmain::OnRecvData(WPARAM wParam,LPARAM lParam)
{
	char* temp = (char *)lParam;
	CString str;
	str.Format("%s",temp);

	CString strSource,ch;
	vector <CString> vecString;
	int iPos = 0;
	CString strTmp;
	strSource.Format("%s",temp);
	ch = ",";
	strTmp = strSource.Tokenize(ch,iPos);
	while(strTmp.Trim() != _T(""))
	{
		vecString.push_back(strTmp);
		strTmp = strSource.Tokenize(ch,iPos);
	}


	if(vecString[0] == "MSG")
	{
	int nIndex1 = m_list_ChatContext.GetItemCount();  
	LV_ITEM   lvitemAdd1 = {0};  
	lvitemAdd1.mask = LVIF_TEXT;  
	lvitemAdd1.iItem = nIndex1 ;  
	lvitemAdd1.iSubItem = 0;  
	lvitemAdd1.pszText = vecString[1].GetBuffer();
	vecString[1].ReleaseBuffer();
	m_list_ChatContext.InsertItem(&lvitemAdd1);
	}
	else if(vecString[0] == "LOGIN")
	{
		int nIndex = m_list.GetItemCount();  
		LV_ITEM   lvitemAdd = {0};  
		lvitemAdd.mask = LVIF_TEXT|LVIF_IMAGE;  
		lvitemAdd.iItem = nIndex ;  
		lvitemAdd.iSubItem = 0;  
		lvitemAdd.pszText = vecString[1].GetBuffer();
		vecString[1].ReleaseBuffer();
		lvitemAdd.iImage = 0;  //(某个窗口的窗口句柄)  
		m_list.InsertItem(&lvitemAdd);
	}
	else if(vecString[0] == "LOGOUT")
	{
		for(int Iindex = 0; Iindex < m_list.GetItemCount(); Iindex++)
	  {
		str = m_list.GetItemText(Iindex,0);
		if(str == vecString[1])
		{
			m_list.DeleteItem(Iindex);
			break;
		}
	}
	}
	return 0;
}

void Cmain::OnOnrecord()
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

void Cmain::On32771()
{
	// TODO: 在此添加命令处理程序代码
	
}

void Cmain::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == 1)
	{
		char sendBuf[128] = "USERLIST";
	    m_list.DeleteAllItems();
		m_list_ChatContext.DeleteAllItems();
	    send(m_sock, sendBuf,128, 0);
		KillTimer(1);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void Cmain::Ontransfile()
{
	// TODO: 在此添加命令处理程序代码
	CString FilePathName,fileName;
    CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("All Files (*.*)|*.*||"),
        NULL);
	if(dlg.DoModal()==IDOK)
    {
        FilePathName = dlg.GetPathName(); //文件名保存在了FilePathName里
		fileName = dlg.GetFileName();
    }
    else
    {
		MessageBox("未选择文件！");
         return;
    }

    CFile transFile;
	transFile.Open(FilePathName,CFile::modeRead);
	char buff[1024];
	//int iLen = transFile.Read(buff,1024);
	while(transFile.Read(buff,1024))
	{
		if(SOCKET_ERROR == send(m_sock,buff,1024,NULL))
		{
			AfxMessageBox("文件发送失败！");
		}
		memset(buff,0,1024);
	}
	AfxMessageBox("发送成功！");
	transFile.Close();
}
