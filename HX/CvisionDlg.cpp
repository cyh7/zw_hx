// CvisionDlg.cpp: 实现文件
//

#include "pch.h"
#include "HX.h"
#include "CvisionDlg.h"
#include "afxdialogex.h"
#include "CLoginDlg.h"
#include "HXDlg.h"
#include "layoutinitVision.h"

//发送错误次数
int BadVisionNum = 0;
//当前发送的是第几个
int LocVisionNum = 0;
//判断是否识别完成
bool IdentifyDone = false;

//上一次时间
CString LastTime;
//插入数据库时的字符串

//double x y theta
//X Y Theta
double vs_x;
double vs_y;
double vs_theta;


// CvisionDlg 对话框

IMPLEMENT_DYNAMIC(CvisionDlg, CDialogEx)

CvisionDlg::CvisionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VISION, pParent)
{

}

CvisionDlg::~CvisionDlg()
{
}

void CvisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIS_BTN_OPMON, m_vs_btn_opmon);
	DDX_Control(pDX, IDC_VIS_BTN_OPCAD, m_vs_btn_opcad);
	DDX_Control(pDX, IDC_VIS_BTN_OPDATA, m_vs_btn_opdata);
	DDX_Control(pDX, IDC_VIS_BTN_OPMOD, m_vs_btn_opmod);
	DDX_Control(pDX, IDC_VIS_BTN_OPVS, m_vs_btn_opvs);
}


BEGIN_MESSAGE_MAP(CvisionDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_VIS_BTN_OPCAD, &CvisionDlg::OnBnClickedVisBtnOpcad)
	ON_BN_CLICKED(IDC_VIS_BTN_OPMOD, &CvisionDlg::OnBnClickedVisBtnOpmod)
	
	ON_WM_SIZING()
	ON_BN_CLICKED(IDC_VIS_BTN_OPDATA, &CvisionDlg::OnBnClickedVisBtnOpdata)
	ON_WM_PAINT()
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_VIS_BTN_OPMON, &CvisionDlg::OnBnClickedVisBtnOpmon)
END_MESSAGE_MAP()


// CvisionDlg 消息处理程序
BOOL CvisionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//控件大小同步
	CRect v_rect;
	GetClientRect(&v_rect);
	old_Vision.x = v_rect.right - v_rect.left;
	old_Vision.y = v_rect.bottom - v_rect.top;

	//ModifyStyle(WS_CAPTION, 0, 0);  // 如果只是要普通的全屏，不想去掉标题栏，就不用第一个语句
	//SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);

	//按钮绘制
	{
		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_VIS_BTN_OPMON)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_vs_btn_opmon.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_vs_btn_opmon.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_vs_btn_opmon.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_vs_btn_opmon.setWordSize(200);

		GetDlgItem(IDC_VIS_BTN_OPVS)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_vs_btn_opvs.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_vs_btn_opvs.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_vs_btn_opvs.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_vs_btn_opvs.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_VIS_BTN_OPCAD)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_vs_btn_opcad.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_vs_btn_opcad.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_vs_btn_opcad.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_vs_btn_opcad.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_VIS_BTN_OPDATA)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_vs_btn_opdata.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_vs_btn_opdata.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_vs_btn_opdata.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_vs_btn_opdata.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_VIS_BTN_OPMOD)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_vs_btn_opmod.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_vs_btn_opmod.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_vs_btn_opmod.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_vs_btn_opmod.setWordSize(200);
	}
	CFont* p_name = new CFont;
	p_name->CreateFont(50,      // nHeight，文字大小
		0,          // nWidth
		0,          // nEscapement
		0,          // nOrientation
		FW_BOLD,    // nWeight，加粗
		FALSE,      // bItalic
		FALSE,      // bUnderline
		0,          // cStrikeOut
		ANSI_CHARSET,               // nCharSet
		OUT_DEFAULT_PRECIS,         // nOutPrecision
		CLIP_DEFAULT_PRECIS,        // nClipPrecision
		DEFAULT_QUALITY,            // nQuality
		DEFAULT_PITCH | FF_SWISS,   // nPitchAndFamily
		_T("楷体"));       // lpszFac，字体
	GetDlgItem(IDC_VS_STATIC_NAME)->SetFont(p_name, false);
	
	CFont* p_font = new CFont;
	p_font->CreateFont(18,      // nHeight，文字大小
		0,          // nWidth
		0,          // nEscapement
		0,          // nOrientation
		FW_BOLD,    // nWeight，加粗
		FALSE,      // bItalic
		FALSE,      // bUnderline
		0,          // cStrikeOut
		ANSI_CHARSET,               // nCharSet
		OUT_DEFAULT_PRECIS,         // nOutPrecision
		CLIP_DEFAULT_PRECIS,        // nClipPrecision
		DEFAULT_QUALITY,            // nQuality
		DEFAULT_PITCH | FF_SWISS,   // nPitchAndFamily
		_T("微软雅黑"));       // lpszFac，字体

	

	InitLayoutVision(m_layoutVision, this);
	m_Brush.CreateSolidBrush(RGB(240, 240, 220));
	//全屏幕操作
	{
		WINDOWPLACEMENT m_struOldWndpl;
		//get current system resolution
		int g_iCurScreenWidth = GetSystemMetrics(SM_CXSCREEN); //1920
		int g_iCurScreenHeight = GetSystemMetrics(SM_CYSCREEN); //1080

		//for full screen while backplay
		GetWindowPlacement(&m_struOldWndpl);

		CRect rectWholeDlg;//entire client(including title bar)
		CRect rectClient;//client area(not including title bar)
		CRect rectFullScreen;
		//用于接收左上角和右下角的屏幕坐标
		GetWindowRect(&rectWholeDlg);
		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &rectClient);
		//将显示器上给定点或矩形的客户去坐标转换为屏幕坐标 新的坐标是相对于屏幕左上角的
		ClientToScreen(&rectClient);

		//-8 = 0 - 8
		rectFullScreen.left = rectWholeDlg.left - rectClient.left;
		//-28 = 0 - 28
		rectFullScreen.top = rectWholeDlg.top;
		// = 1088 + 1920 - 1080
		rectFullScreen.right = rectWholeDlg.right + g_iCurScreenWidth - rectClient.right;
		// = 639 + 1080 - 609
		rectFullScreen.bottom = rectWholeDlg.bottom + g_iCurScreenHeight - rectClient.bottom - 20;


		//enter into full screen;
		WINDOWPLACEMENT struWndpl;
		struWndpl.length = sizeof(WINDOWPLACEMENT);
		struWndpl.flags = 0;
		struWndpl.showCmd = SW_SHOWNORMAL;
		struWndpl.rcNormalPosition = rectFullScreen;
		SetWindowPlacement(&struWndpl);
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

HBRUSH CvisionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if (nCtlColor == CTLCOLOR_STATIC)//如果当前控件属于静态文本
	{
		//pDC->SetTextColor(RGB(255, 0, 0)); //字体颜色
		pDC->SetTextColor(RGB(50, 50, 200));  //字体颜色
		pDC->SetBkColor(RGB(240, 240, 220));   //字体背景色
		pDC->SetBkMode(TRANSPARENT); //设置字体背景为透明
		//pDC->SetFont(p_font);//设置字体

		//pDC-> SetBkColor(RGB(0, 0, 255));  //字体背景色
		return (HBRUSH)m_Brush.GetSafeHandle();
	}
	return hbr;
	
}


unsigned short CvisionDlg::CRC16(unsigned char* puchMsg, unsigned short usDataLen)
{
	// TODO: 在此处添加实现代码.
	static unsigned char auchCRCHi[] =
	{
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
	};

	static unsigned char auchCRCLo[] =
	{
		0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
		0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
		0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
		0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
		0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
		0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
		0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
		0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
		0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
		0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
		0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
		0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
		0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
		0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
		0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
		0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
		0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
		0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
		0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
		0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
		0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
		0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
		0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
		0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
		0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
		0x43, 0x83, 0x41, 0x81, 0x80, 0x40
	};

	unsigned char uchCRCHi = 0xFF;
	unsigned char uchCRCLo = 0xFF;
	unsigned char uIndex;
	while (usDataLen--)
	{
		uIndex = uchCRCHi ^ *puchMsg++;
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo);
}


void CvisionDlg::SendData(int CommTypeIn, WORD DownAdd, DWORD DownData)
{
	// TODO: 在此处添加实现代码.
		// TODO: 在此处添加实现代码.
	unsigned char SendData[200];
	unsigned short CRCData;
	//CByteArray SendArray;
	char SendArray[8];
	DWORD MidData;
	BYTE StrLength;

	SendData[0] = 01;
	if (CommTypeIn == 0)
		SendData[1] = 3;
	else
		SendData[1] = 6;
	MidData = DownAdd;
	SendData[2] = (unsigned char)(MidData >> 8);
	SendData[3] = (unsigned char)(MidData - 256 * SendData[2]);
	MidData = DownData;
	SendData[4] = (unsigned char)(MidData >> 8);
	SendData[5] = (unsigned char)(MidData - 256 * SendData[4]);
	//CRC校验
	CRCData = CRC16(SendData, 6);
	SendData[6] = CRCData >> 8;
	SendData[7] = CRCData - 256 * SendData[6];
	StrLength = 8;

	//CString msg;
	////%02X为16进制显示  %d十进制 %s 字符串
	//msg.Format(_T("%02X"), SendData[3]);
	//MessageBox(msg);

	/*SendArray.RemoveAll();
	SendArray.SetSize(StrLength);*/
	//把待发送数据存入数组中

	//CString msg;
 //   //%02X为16进制显示  %d十进制 %s 字符串
 //   msg.Format(_T("%02X"), SendData[3]);
 //   MessageBox(msg);

	for (int Circle = 0; Circle < StrLength; Circle++)
		SendArray[Circle] = SendData[Circle];

	//int len1 = SendArray.GetSize();
	//CString temp('x', len1);
	//for (int i = 0; i < len1; i++)
	//{
	//	temp.SetAt(i, SendArray.GetAt(i));
	//}

	//int n = temp.GetLength();//n= 8
	//int len = WideCharToMultiByte(CP_ACP, 0, temp, temp.GetLength(), NULL, 0, NULL, NULL);//len = 8
	//char *m_str = new char[len + 1];
	//WideCharToMultiByte(CP_ACP, 0, temp, temp.GetLength(), m_str, len, NULL, NULL);
	//m_str[len + 1] = '\0';

	CmodbusDlg *pdlg = CmodbusDlg::pModbusdlg;
	pdlg->m_SerialPort.writeData(SendArray, 8);
	

	//CPublic::m_SerialPort.writeData(SendArray, 8);
}

//每次定时器1进入时执行一个判断
//如果到达 就开始调用相机识别的函数
//相机识别完成，把一个标志位打正
//用另一个定时器每隔多少ms判断一下这个定时器
//如果判别完成就把数据发送过去


BOOL CvisionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此处添加实现代码.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F1)
		{
			OnBnClickedVisBtnOpmon();
		}
		if (pMsg->wParam == VK_F3)
		{
			OnBnClickedVisBtnOpcad();
		}
		if (pMsg->wParam == VK_F4)
		{
			OnBnClickedVisBtnOpdata();
		}
		if (pMsg->wParam == VK_F5)
		{
			OnBnClickedVisBtnOpmod();
		}
	}
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)    //屏蔽回车和ESC  
		return TRUE;
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)  //屏蔽ALT+F4
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CvisionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_layoutVision.OnSize(cx, cy);
}




void CvisionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//static CTime preTime = CTime().GetCurrentTime();
	switch (nIDEvent)
	{
		//发送命令询问背板是否到位
		case 1:
		{
			//寄存器地址95 读1位数据
			
			SendOnce_Vision = true;
			SendData(0, 95, 1);  
			
			//判断上一次发送的是否为0，为0没有触发receive则断线
			if (m_Status_T2 == 0)
			{
				//m_Status_T2 = 0; //在这里把m_Status_T2置为0
				//断线标志位
				DisconnectFlag = true;
				DisconnectNum += 1;
				//断线超过10s，即DisconnectNum=50则提示断线
				if(DisconnectNum >= 50)
				{
					KillTimer(1);
					AfxMessageBox(_T("请检查连接！"));
				}
			}
			//没有断线 通信没断线才能执行下边的程序
			else
			{
				DisconnectFlag = false;
				m_Status_T2 = 0;//如果没有断线 那么在onReceive里会更改这个值，如果断线了那就不会更改了
				//只能对上一个时间循环里的数据进行判断
				if (ArriveFlag == true)
					//如果已经到位,且未识别完成 加一个喷胶是否喷完的标志位
					//进入判断是 到位，未识别完成，归根结底是什么时候进入判断呢，是在背板到了，再进入判断
					//背板到了，就停止定时器
					//主要问题，重启之后还是true怎么，把killtime放里边
					//背板到位之后，开始判断喷胶，
					//喷胶喷完为true,在下一台背板进来的时候喷胶标志位一直为true，进入判断的条件是
					//SprayDone == false
					//在背板到达时打为false 此时ArriveFlag = true 
				{
					//IdentifyDone初始值设为false,执行完程序后设置为true，在背板离开的时候设置为false
					//Sleep()
					//Send()  发送喷胶判断
					if (IdentifyDone == false)
					{
						KillTimer(1); //先终止该定时器，进行视觉处理

						/*
						识别完成的时候就把数据读到容器里
						其实就三个数据可以换个方式
						IdentifyDone = true;
						//对比设置的界限
						//产生是否良品字符串 在这里边判断只用写一个字符串就可以了
						//当前的日期
						//坐标数据
						//三个状态 是来自收到

						//程序识别完之后，直接开始settimer
						SetTimer(2,30,Null);
						*/
						IdentifyDone = true;
						//CTime curTime = CTime().GetCurrentTime();//当前时间
						//LastTime = preTime.Format("%Y-%m-%d %H:%M:%S");
						if ((vs_x >= x_floor && vs_x <= x_ceil) && (vs_y >= x_floor && vs_y <= y_ceil) && (vs_theta >= theta_floor && vs_theta <= theta_ceil))
						{
							data_good = _T("良品");
						}
						else
						{
							data_good = _T("非良品");
						}

						//if (SprayBatch > 0)
						//{
						//	
						//	
						//	LastTime;
						//	SprayBatch;
						//	backboard;
						//	vs_x;
						//	vs_y;
						//	vs_theta;
						//	
						//	//插入数据库,插入(LastTime  1
						//	//这里判断四个flag 生成四个CString 
						//	//能进来就说明当前是正常的  要不通信状态不插入数据库
						//	//上一次的坐标对比设置 CString 良与不良  7 8 9 10
						//	//背板型号 2
						//	//喷涂批次就是当前的SprayBatch 3
						//	//X Y theta坐标   4 5 6
						//}
						//preTime = curTime;
						insertdata = 0;
						SprayBatch += 1;
						//执行视觉识别程序 产生三个坐标，
						SetTimer(2, 50, NULL);
						//执行发送函数  这里的发送函数应该是启动定时器2
					}
				}

			}
			break;
		}
		case 2:
		{
			SendOnce_Vision = false;
			m_Vision_T1 = GetTickCount();
			if (m_Vision_T2 != 0 && RecMsgFlag == true && OverTime_Vision == false)
			{
				//
				BadVisionNum = 0;
				m_Vision_T2 = 0;
				if (LocVisionNum < 3)
				{
					//SendData;
					//SendData(1, LocVisionNum + 90, testLoc[LocVisionNum]);
					LocVisionNum++;
				}
				//发送完毕
				else
				{
					//SendData 这个跟轩举商量
					//发送完毕 发送数清0
					KillTimer(2);
					LocVisionNum = 0;
					m_Vision_T2 = GetTickCount();//这里加一个计时是防止下一组背板错误判断
					//SprayBatch += 1; //喷涂批次加一
					//重启定时器1
					SetTimer(1, 200, NULL);
				}

			}
			else
			{
				BadVisionNum++;
				if (BadVisionNum < 3)
				{
					//第一个数据出现错误与后边的数据出现错误是一样的处理措施
					//先减1发送前一个数据
					LocVisionNum = LocVisionNum - 1;
					//寄存器地址  发送的数据根据LocVisionNum来定

					//SendData(1, , );
					//SendData(1, LocVisionNum + 90, testLoc[LocVisionNum]);
					//发送完之后做加一处理
					LocVisionNum++;
				}
				else
				{

					//停止发送
					KillTimer(2);
					//报错
					CString msg;

					//%02X为16进制显示  %d十进制 %s 字符串
					msg.Format(_T("第%d个数据发送错误，终止发送！"), LocVisionNum);
					AfxMessageBox(msg);

				}
			}

			break;
		}
		
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CvisionDlg::OnBnClickedVisBtnOpmon()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowMonitor();
}

void CvisionDlg::OnBnClickedVisBtnOpcad()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowCad();
}


void CvisionDlg::OnBnClickedVisBtnOpmod()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowModbus();
}





void CvisionDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
}


void CvisionDlg::OnBnClickedVisBtnOpdata()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowData();
}


void CvisionDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(rect);

	dc.FillSolidRect(rect, RGB(240, 240, 220));
	//dc.FillSolidRect(rect, RGB(125, 125, 255));

	CDialogEx::OnPaint();
					   // 不为绘图消息调用 CDialogEx::OnPaint()
}


BOOL CvisionDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnHelpInfo(pHelpInfo);
}





// 状态判断





