// CvisionDlg.cpp: 实现文件
//

#include "pch.h"
#include "HX.h"
#include "CvisionDlg.h"
#include "afxdialogex.h"
#include "CLoginDlg.h"
#include "HXDlg.h"
#include "InfoFile.h"
#include "layoutinitVision.h"
#include "HalconCpp.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "core/persistence.hpp"
using namespace HalconCpp;

//发送错误次数
int BadVisionNum = 0;
//当前发送的是第几个
int LocVisionNum = 0;
//判断是否识别完成
bool IdentifyDone = false;
//识别错误次数
int IdentifyWrongNum = 0;
//上一次时间
CString LastTime;
//插入数据库时的字符串

//double x y theta
//X Y Theta
float result[3];

double vs_x;
double vs_y;
double vs_theta;
CvisionDlg* CvisionDlg::pVisiondlg = NULL;


// CvisionDlg 对话框

IMPLEMENT_DYNAMIC(CvisionDlg, CDialogEx)

//定义全局变量
HTuple hv_AcqHandleleft, hv_AcqHandleright;
HObject  ho_image_l, ho_image_r;
bool Redraw_crop_picture_box = TRUE; //是否支持移动裁剪框的标志
POINT m_startPos_l, m_endPos_l;
POINT m_startPos_r, m_endPos_r;
int m_startPos_l_x, m_startPos_l_y, m_endPos_l_x, m_endPos_l_y;

int m_startPos_r_x, m_startPos_r_y, m_endPos_r_x, m_endPos_r_y;

float Row_l, Column_l, Angle_l, Row_r, Column_r, Angle_r;//最终定位数据
HTuple	hv_Row_l, hv_Column_l, hv_Angle_l;//左相机的最终定位数据
HTuple	hv_Row_r, hv_Column_r, hv_Angle_r;//右相机的最终定位数据
int left_begin_p_x, left_begin_p_y, left_begin_p1_x, left_begin_p1_y, right_begin_p_x, right_begin_p_y, right_begin_p1_x, right_begin_p1_y,
left_end_p_x, left_end_p_y, left_end_p1_x, left_end_p1_y, right_end_p_x, right_end_p_y, right_end_p1_x, right_end_p1_y;//显示用的直线数据
HTuple l_begin_row = 0, l_begin_column = 0, l_end_row = 0, l_end_column = 0, l_begin1_row = 0, l_begin1_column = 0, l_end1_row = 0, l_end1_column = 0;//计算用的直线数据
HTuple r_begin_row = 0, r_begin_column = 0, r_end_row = 0, r_end_column = 0, r_begin1_row = 0, r_begin1_column = 0, r_end1_row = 0, r_end1_column = 0;//计算用的直线数据
int scale = 9;//定义图像显示缩放倍数
int baoguang_time = 320000;//定义左相机曝光时间,60000-1000000
int baoguang2_time = 320000;//定义左相机曝光时间,60000-1000000
int firstflag = 1;
int all_width, all_height;
int edit_frame_length = 1600;//在系统设置里对应裁剪框长度
int edit_frame_width = 1600;//在系统设置里对应裁剪框宽度
int edit_frame_left_x = 1000;//设置裁剪框初始位置
int edit_frame_left_y = 1000;//设置裁剪框初始位置
int edit_frame_right_x = 1000;//设置裁剪框初始位置
int edit_frame_right_y = 1000;//设置裁剪框初始位置
int row_l = -1;
int column_l = -1;
int angle_l = -1;
int row_r = -1;
int column_r = -1;
int angle_r = -1;
string filename;
////标准版左上角和左下角分别在各自像素坐标系下的表示
//float coordinate_pixel_left_x = 0;
//float coordinate_pixel_left_y = 0;
//float coordinate_pixel_right_x = 0;
//float coordinate_pixel_right_y = 0;
////标准版左上角和左下角分别在机器人基坐标系下的表示
//float coordinate_robot_left_x = 0;
//float coordinate_robot_left_y = 0;
//float coordinate_robot_right_x = 0;
//float coordinate_robot_right_y = 0;
////左右相机的像素尺寸和实际尺寸的比例转换,
//double left_x_proportion = 10000 / 3600;
//double left_y_proportion = 10000 / 5400;
//double right_x_proportion = 10000 / 3600;
//double right_y_proportion = 10000 / 5400;

//定义图像处理有关参数
int hv_Threshold = 200;
int hv_Filter_block_radius = 3;
int hv_closing_radius = 10;
HTuple hv_min_length_width_line = edit_frame_length * 0.30;
HTuple hv_min_length_height_line = edit_frame_width * 0.30;

int check_l_over = 1;//判断子线程是否结束,结束时置零
int check_r_over = 1;//判断子线程是否结束,结束时置零
int check_l_1_over = 1;//判断子线程是否结束,结束时置零
int check_r_1_over = 1;//判断子线程是否结束,结束时置零
int check_l_2_over = 1;//判断子线程是否结束,结束时置零
int check_r_2_over = 1;//判断子线程是否结束,结束时置零
int opencameraleft = 1, opencameraright = 1;

CvisionDlg::CvisionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VISION, pParent)
	, m_vs_edit_type(_T(""))
	, m_vs_edit_batch(0)
{

}

CvisionDlg::~CvisionDlg()
{
	CvisionDlg* pVisiondlg = CvisionDlg::pVisiondlg;
	pVisiondlg->CvisionDlg::OnCloseCam();
	OnCloseCam();

}

void CvisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIS_BTN_OPMON, m_vs_btn_opmon);
	DDX_Control(pDX, IDC_VIS_BTN_OPCAD, m_vs_btn_opcad);
	DDX_Control(pDX, IDC_VIS_BTN_OPDATA, m_vs_btn_opdata);
	DDX_Control(pDX, IDC_VIS_BTN_OPMOD, m_vs_btn_opmod);
	DDX_Control(pDX, IDC_VIS_BTN_OPVS, m_vs_btn_opvs);
	DDX_Control(pDX, IDC_VS_PIC_LOGO, m_vs_pic_logo);
	DDX_Control(pDX, IDC_VS_BTN_RESEND, m_vs_btn_resend);
	DDX_Text(pDX, IDC_VS_EDIT_TYPE, m_vs_edit_type);
	DDX_Text(pDX, IDC_VS_EDIT_BATCH, m_vs_edit_batch);
	DDX_Control(pDX, IDC_VS_PIC_LEFT, m_vs_pic_left);
	DDX_Control(pDX, IDC_VS_PIC_RIGHT, m_vs_pic_right);
	DDX_Control(pDX, IDC_VS_LIST_LOCATE, m_vs_list_location_data);
}


BEGIN_MESSAGE_MAP(CvisionDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_VIS_BTN_OPCAD, &CvisionDlg::OnBnClickedVisBtnOpcad)
	ON_BN_CLICKED(IDC_VIS_BTN_OPMOD, &CvisionDlg::OnBnClickedVisBtnOpmod)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZING()
	ON_BN_CLICKED(IDC_VIS_BTN_OPDATA, &CvisionDlg::OnBnClickedVisBtnOpdata)
	ON_WM_PAINT()
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_VIS_BTN_OPMON, &CvisionDlg::OnBnClickedVisBtnOpmon)
	ON_BN_CLICKED(IDC_VS_BTN_RESEND, &CvisionDlg::OnBnClickedVsBtnResend)
	ON_COMMAND(ID_VS_HOTKEY_LEFT_DOWN, &CvisionDlg::OnVsHotkeyLeftDown)
	ON_COMMAND(ID_VS_HOTKEY_LEFT_LEFT, &CvisionDlg::OnVsHotkeyLeftLeft)
	ON_COMMAND(ID_VS_HOTKEY_LEFT_RIGHT, &CvisionDlg::OnVsHotkeyLeftRight)
	ON_COMMAND(ID_VS_HOTKEY_LEFT_UP, &CvisionDlg::OnVsHotkeyLeftUp)
	ON_COMMAND(ID_VS_HOTKEY_RIGHT_DOWN, &CvisionDlg::OnVsHotkeyRightDown)
	ON_COMMAND(ID_VS_HOTKEY_RIGHT_LEFT, &CvisionDlg::OnVsHotkeyRightLeft)
	ON_COMMAND(ID_VS_HOTKEY_RIGHT_RIGHT, &CvisionDlg::OnVsHotkeyRightRight)
	ON_COMMAND(ID_VS_HOTKEY_RIGHT_UP, &CvisionDlg::OnVsHotkeyRightUp)
	ON_BN_CLICKED(IDC_VIS_BTN_OPVS, &CvisionDlg::OnBnClickedVisBtnOpvs)
END_MESSAGE_MAP()


// CvisionDlg 消息处理程序
BOOL CvisionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_VS_HOTKEY));//初始化快捷键

	pVisiondlg = this;
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

		GetDlgItem(IDC_VS_BTN_RESEND)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_vs_btn_resend.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_vs_btn_resend.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_vs_btn_resend.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_vs_btn_resend.setWordSize(200);
	}
	//改变字体
	{
		f_vs_font.CreateFontW(50,      // nHeight，文字大小
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
		GetDlgItem(IDC_VS_STATIC_NAME)->SetFont(&f_vs_font, false);


		f_vs_name.CreateFontW(18,      // nHeight，文字大小
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
		GetDlgItem(IDC_VS_STATIC_TYPE)->SetFont(&f_vs_name, false);
		GetDlgItem(IDC_VS_STATIC_BATCH)->SetFont(&f_vs_name, false);
	}
	

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

	//设置默认重新选择图片裁剪框
	//((CButton*)GetDlgItem(IDC_VS_CHECK_CAIJIAN))->SetCheck(TRUE);
	m_startPos_l = { edit_frame_left_x / scale,edit_frame_left_y / scale };
	m_endPos_l = { (edit_frame_left_x + edit_frame_length) / scale,(edit_frame_left_y + edit_frame_width) / scale };

	m_startPos_l_x = edit_frame_left_x / scale;
	m_startPos_l_y = edit_frame_left_y / scale;
	m_endPos_l_x = (edit_frame_left_x + edit_frame_length) / scale;
	m_endPos_l_y = (edit_frame_left_y + edit_frame_width) / scale;

	m_startPos_r = { edit_frame_right_x / scale,edit_frame_right_y / scale };
	m_endPos_r = { (edit_frame_right_x + edit_frame_length) / scale,(edit_frame_right_y + edit_frame_width) / scale };

	m_startPos_r_x = edit_frame_right_x / scale;
	m_startPos_r_y = edit_frame_right_y / scale;
	m_endPos_r_x = (edit_frame_right_x + edit_frame_length) / scale;
	m_endPos_r_y = (edit_frame_right_y + edit_frame_width) / scale;

	//m_vs_btn_collect.EnableWindow(FALSE); //禁用按钮
	//m_vs_btn_closecam.EnableWindow(FALSE); //禁用按钮
	//初始化数据列表

	m_vs_list_location_data.InsertColumn(0, _T("左上角行坐标"), LVCFMT_CENTER, 120);
	m_vs_list_location_data.InsertColumn(1, _T("左上角列坐标"), LVCFMT_CENTER, 120);
	m_vs_list_location_data.InsertColumn(2, _T("右边行坐标"), LVCFMT_CENTER, 120);
	m_vs_list_location_data.InsertColumn(3, _T("右边列坐标"), LVCFMT_CENTER, 120);
	m_vs_list_location_data.InsertColumn(4, _T("计算倾角"), LVCFMT_CENTER, 120);
	m_vs_list_location_data.InsertColumn(5, _T("左直角边倾角"), LVCFMT_CENTER, 120);
	m_vs_list_location_data.InsertColumn(6, _T("右直角边倾角"), LVCFMT_CENTER, 120);
	//m_vs_list_location_data.InsertColumn(6, _T("用时"), LVCFMT_CENTER, 95);

	m_vs_hBitmap_logo = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_HG), IMAGE_BITMAP, 200, 40, LR_DEFAULTCOLOR);
	m_vs_pic_logo.SetBitmap(m_vs_hBitmap_logo);

	CmodbusDlg* pdlg = CmodbusDlg::pModbusdlg;
	pdlg->OnBnClickedButtonOpen();
	Sleep(50);
	SendData(1, 75, 32767);
	Sleep(50);
	SetTimer(1, 100, NULL);



	OnOpenCam();

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
	if (pWnd->GetDlgCtrlID() == IDC_VS_EDIT_TYPE || IDC_VS_EDIT_BATCH)
	{
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(RGB(50, 50, 200));  //字体颜色
		//pDC->SetBkColor(RGB(240, 240, 220));   //字体背景色
		return (HBRUSH)m_Brush.GetSafeHandle();  // 设置背景色
		//return (HBRUSH)::GetStockObject(WHITE_BRUSH);
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

	CmodbusDlg* pdlg = CmodbusDlg::pModbusdlg;
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
	if (m_hAccel)
	{
		if (::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
		{
			return(TRUE);
		}
	}
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

int empty_location_data()
{
	left_begin_p_x = left_begin_p_y = left_begin_p1_x = left_begin_p1_y = right_begin_p_x = right_begin_p_y = right_begin_p1_x = right_begin_p1_y =
		left_end_p_x = left_end_p_y = left_end_p1_x = left_end_p1_y = right_end_p_x = right_end_p_y = right_end_p1_x = right_end_p1_y = 0;
	Row_l = Column_l = Angle_l = Row_r = Column_r = Angle_r = -1;
	return 0;
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
		//SprayBatch喷涂批次
		m_vs_edit_batch = SprayBatch;
		//backboard背板型号
		m_vs_edit_type = backboard;
		UpdateData(FALSE);

		SendOnce_Vision = true;
		SendData(0, 74, 1);

		//判断上一次发送的是否为0，为0没有触发receive则断线
		if (m_Status_T2 == 0)
		{
			//m_Status_T2 = 0; //在这里把m_Status_T2置为0
			//断线标志位
			DisconnectFlag = true;
			DisconnectNum += 1;
			//断线超过10s，即DisconnectNum=50则提示断线
			if (DisconnectNum >= 300)
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
					OnCollectAndLocate();//采集图像并定位
					//识别出来的情况
					if (Row_l != -1 && Row_r != -1)
					{

						KillTimer(1); //先终止该定时器，进行视觉处理
						vs_x = Row_l;
						vs_y = Column_l;
						vs_theta = Angle_l;
						result[0] = row_l;
						result[1] = column_l;
						result[2] = angle_l;
						/*result[0] = 500;
						result[1] = 500;
						result[2] = 0;*/

						if ((vs_x >= x_floor && vs_x <= x_ceil) && (vs_y >= x_floor && vs_y <= y_ceil) && (vs_theta >= theta_floor && vs_theta <= theta_ceil))
						{
							data_good = _T("良品");
						}
						else
						{
							data_good = _T("非良品");
						}
						IdentifyDone = true;
						insertdata = 0;
						SprayBatch += 1;
						//执行视觉识别程序 产生三个坐标，
						//执行发送函数  这里的发送函数应该是启动定时器2
						/*SetTimer(1, 100, NULL);
						IdentifyDone = true;*/
						SetTimer(2, 50, NULL);
					}
					else//没识别出来
					{
						if (IdentifyWrongNum < 2)
						{
							OnCollectAndLocate();
							if (Row_l == -1 || Row_r == -1)
							{
								IdentifyWrongNum += 1;
							}
							else
							{
								IdentifyWrongNum = 0;
							}
							
						}
						else
						{
							//SendData();
							//KillTimer(1);
							//这个标志位只用于测验时,防止重复
							IdentifyDone = true;
							IdentifyWrongNum = 0;
							AfxMessageBox(_T("视觉定位失败"));
						}
					}


				}
			}

		}
		break;
	}
	case 2:
	{
		DisconnectFlag = false;
		SendOnce_Vision = false;
		m_Vision_T1 = GetTickCount64();
		if (m_Vision_T2 != 0 && RecMsgFlag == true && OverTime_Vision == false)
		{
			//
			BadVisionNum = 0;
			m_Vision_T2 = 0;
			if (LocVisionNum < 3)
			{
				//SendData;
				SendData(1, 70 + LocVisionNum, result[LocVisionNum]);
				LocVisionNum++;
			}
			//发送完毕
			else
			{
				//SendData 这个跟轩举商量
				//发送完毕 发送数清0

				KillTimer(2);
				IdentifyDone = true;
				SendOnce_Vision = true;
				SendData(1, 73, 32767);
				LocVisionNum = 0;
				m_Vision_T2 = GetTickCount64();//这里加一个计时是防止下一组背板错误判断
				//SprayBatch += 1; //喷涂批次加一
				//重启定时器1
				DisconnectNum = 0;
				ReSetTime();
			}

		}
		else
		{
			BadVisionNum++;
			if (BadVisionNum < 4)
			{
				//第一个数据出现错误与后边的数据出现错误是一样的处理措施
				//先减1发送前一个数据
				LocVisionNum = LocVisionNum - 1;
				//寄存器地址  发送的数据根据LocVisionNum来定

				//SendData(1, , );
				SendData(1, 70 + LocVisionNum, result[LocVisionNum]);
				//发送完之后做加一处理
				LocVisionNum++;
			}
			else
			{
				//如果发送错误,就代表着背板已经到位,那么出错的话,就让退出程序,开机是自动开始通信


				//停止发送
				KillTimer(2);
				//报错
				CString msg;

				//%02X为16进制显示  %d十进制 %s 字符串
				msg.Format(_T("第%d个数据发送错误，请检查通信连接并重启！"), LocVisionNum);
				AfxMessageBox(msg);
				LocVisionNum = 0;
				BadVisionNum = 0;
			}
		}

		break;
	}
	case 3:
	{
		
	}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CvisionDlg::OnBnClickedVisBtnOpmon()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg* p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowMonitor();
}

void CvisionDlg::OnBnClickedVisBtnOpcad()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg* p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowCad();
}


void CvisionDlg::OnBnClickedVisBtnOpmod()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg* p_hxdlg = (CHXDlg*)this->GetParent();
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
	CHXDlg* p_hxdlg = (CHXDlg*)this->GetParent();
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



//鼠标按下画出裁剪框
void CvisionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{

	CRect rect, rectr;
	ClientToScreen(&point);
	CPoint point1 = { point.x + edit_frame_length / scale,point.y + edit_frame_width / scale };

	GetDlgItem(IDC_VS_PIC_LEFT)->GetClientRect(rect);
	GetDlgItem(IDC_VS_PIC_LEFT)->ClientToScreen(rect);

	GetDlgItem(IDC_VS_PIC_RIGHT)->GetClientRect(rectr);
	GetDlgItem(IDC_VS_PIC_RIGHT)->ClientToScreen(rectr);

	if (rect.PtInRect(point))
	{
		CBitmap bitmap_l;  // CBitmap对象，用于加载位图   
		HBITMAP hBmp_l;    // 保存CBitmap加载的位图的句柄
		hBmp_l = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/left.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		m_vs_pic_left.SetBitmap(hBmp_l);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

		GetDlgItem(IDC_VS_PIC_LEFT)->ScreenToClient(&point);

		if (rect.PtInRect(point1))
		{
			m_startPos_l_x = point.x;
			m_startPos_l_y = point.y;
			m_endPos_l_x = point.x + edit_frame_length / scale;
			m_endPos_l_y = point.y + edit_frame_width / scale;

		}
		if (point1.x > rect.BottomRight().x && point1.y > rect.BottomRight().y)
		{
			m_endPos_l_y = all_height / scale;
			m_endPos_l_x = all_width / scale;
			m_startPos_l_y = m_endPos_l_y - edit_frame_width / scale;
			m_startPos_l_x = m_endPos_l_x - edit_frame_length / scale;

		}
		if (point1.x < rect.BottomRight().x && point1.y > rect.BottomRight().y)
		{
			m_endPos_l_y = all_height / scale;
			m_endPos_l_x = point.x + edit_frame_length / scale;
			m_startPos_l_y = m_endPos_l_y - edit_frame_width / scale;
			m_startPos_l_x = m_endPos_l_x - edit_frame_length / scale;

		}
		if (point1.x > rect.BottomRight().x && point1.y < rect.BottomRight().y)
		{
			m_endPos_l_y = point.y + edit_frame_width / scale;
			m_endPos_l_x = all_width / scale;
			m_startPos_l_y = m_endPos_l_y - edit_frame_width / scale;
			m_startPos_l_x = m_endPos_l_x - edit_frame_length / scale;

		}
		CString aaa = L"成功";
		edit_frame_left_x = m_startPos_l_x * scale;
		edit_frame_left_y = m_startPos_l_y * scale;
		CInfoFile file;
		file.WirteEditFrameLocation(aaa, edit_frame_left_x, edit_frame_left_y, edit_frame_right_x, edit_frame_right_y);


		CPen pen(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
		CWnd* pwnd = GetDlgItem(IDC_VS_PIC_LEFT);
		CClientDC dc(pwnd);
		dc.SelectObject(&pen);//选择画笔
		POINT m_startPos_l = { m_startPos_l_x,m_startPos_l_y };
		POINT m_endPos_l = { m_endPos_l_x,m_endPos_l_y };
		dc.MoveTo(m_startPos_l);
		dc.LineTo(m_endPos_l.x, m_startPos_l.y);
		dc.LineTo(m_endPos_l.x, m_endPos_l.y);
		dc.LineTo(m_startPos_l.x, m_endPos_l.y);
		dc.LineTo(m_startPos_l.x, m_startPos_l.y);
	}
	///////////////////////////////////////////////////////////////////////

	if (rectr.PtInRect(point))
	{
		CBitmap bitmap_r;  // CBitmap对象，用于加载位图   
		HBITMAP hBmp_r;    // 保存CBitmap加载的位图的句柄
		
		hBmp_r = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/right.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		m_vs_pic_right.SetBitmap(hBmp_r);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

		GetDlgItem(IDC_VS_PIC_RIGHT)->ScreenToClient(&point);

		if (rectr.PtInRect(point1))
		{
			m_startPos_r_x = point.x;
			m_startPos_r_y = point.y;
			m_endPos_r_x = point.x + edit_frame_length / scale;
			m_endPos_r_y = point.y + edit_frame_width / scale;
		}
		if (point1.x > rectr.BottomRight().x && point1.y > rectr.BottomRight().y)
		{
			m_endPos_r_y = all_height / scale;
			m_endPos_r_x = all_width / scale;
			m_startPos_r_y = m_endPos_r_y - edit_frame_width / scale;
			m_startPos_r_x = m_endPos_r_x - edit_frame_length / scale;

		}
		if (point1.x < rectr.BottomRight().x && point1.y > rectr.BottomRight().y)
		{
			m_endPos_r_y = all_height / scale;
			m_endPos_r_x = point.x + edit_frame_length / scale;
			m_startPos_r_y = m_endPos_r_y - edit_frame_width / scale;
			m_startPos_r_x = m_endPos_r_x - edit_frame_length / scale;
		}
		if (point1.x > rectr.BottomRight().x && point1.y < rectr.BottomRight().y)
		{
			m_endPos_r_y = point.y + edit_frame_width / scale;
			m_endPos_r_x = all_width / scale;
			m_startPos_r_y = m_endPos_r_y - edit_frame_width / scale;
			m_startPos_r_x = m_endPos_r_x - edit_frame_length / scale;
		}
		CString aaa = L"成功";
		edit_frame_right_x = m_startPos_r_x * scale;
		edit_frame_right_y = m_startPos_r_y * scale;
		CInfoFile file;
		file.WirteEditFrameLocation(aaa, edit_frame_left_x, edit_frame_left_y, edit_frame_right_x, edit_frame_right_y);

		CPen penR(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
		CWnd* pwndR = GetDlgItem(IDC_VS_PIC_RIGHT);
		CClientDC dcR(pwndR);
		dcR.SelectObject(&penR);//选择画笔
		POINT m_startPos_r = { m_startPos_r_x,m_startPos_r_y };
		POINT m_endPos_r = { m_endPos_r_x,m_endPos_r_y };

		dcR.MoveTo(m_startPos_r);
		dcR.LineTo(m_endPos_r.x, m_startPos_r.y);
		dcR.LineTo(m_endPos_r.x, m_endPos_r.y);
		dcR.LineTo(m_startPos_r.x, m_endPos_r.y);
		dcR.LineTo(m_startPos_r.x, m_startPos_r.y);
	}
	CDialog::OnLButtonDown(nFlags, point);

}


// 状态判断




void CvisionDlg::OnBnClickedVsBtnResend()
{
	// TODO: 在此添加控件通知处理程序代码
	//float result[4];
	result[0] = row_l;
	result[1] = column_l;
	result[2] = angle_l;


	OnCollectAndLocate();
	//ArriveFlag = true;
	//SetTimer(2, 50, NULL);
}


// 停止定时器1
void CvisionDlg::KillTime1()
{
	// TODO: 在此处添加实现代码.
	DisconnectNum = 0;
	IdentifyWrongNum = 0;
	KillTimer(1);
}


// 激活定时器1
void CvisionDlg::ReSetTime()
{
	// TODO: 在此处添加实现代码.
	DisconnectNum = 0;
	SendOnce = true;
	SendOnce_Vision = true;
	SetTimer(1, 100, NULL);
}

int locateleft()
//UINT locateleft(LPVOID pParam)
{
	check_l_over = 1;
	// Local iconic variables

	HObject  ho_image, ho_ImagePart, ho_ObjectSelected;
	HObject  ho_GrayImage, ho_Regions, ho_BinImage1, ho_ImageMedian;
	HObject  ho_Region, ho_RegionClosing, ho_ConnectedRegions, ho_RegionDilation1, ho_RegionDifference;
	HObject  ho_RegionFillUp, ho_SelectedRegions, ho_RegionTrans;
	HObject  ho_BinImage, ho_Edges, ho_ContoursSplit, ho_SelectedXLD1, ho_Region1;
	HObject  ho_SelectedXLD, ho_RegionLines1, ho_RegionLines, ho_RegionBorder, ho_RegionDilation, ho_ContCircle;
	HObject  ho_Image1, ho_image2, ho_ImageZoom, ho_ConnectedRegions1, ho_SelectedRegions1, ho_BinImage2;
	HTuple hv_Row1, hv_Column1, hv_Row2, hv_Column2;
	// Local control variables

	HTuple  hv_WindowHandle, hv_Width, hv_Height, hv_Number, hv_Attrib, hv_Radius, hv_StartPhi, hv_EndPhi, hv_PointOrder;
	HTuple  hv_Row11, hv_Column11, hv_Row22, hv_Column22, hv_Width1;
	HTuple  hv_Height1, hv_RowBegin, hv_ColBegin, hv_RowEnd;
	HTuple  hv_ColEnd, hv_Nr1, hv_Nc1, hv_Dist1, hv_Phi, hv_LineLength;
	HTuple  hv_RowBegin1, hv_ColBegin1, hv_RowEnd1, hv_ColEnd1;
	HTuple  hv_Nr, hv_Nc, hv_Dist, hv_Phi1, hv_Row_left, hv_Column_left, hv_Length, hv_Length1;
	HTuple  hv_IsOverlapping, hv_Angle;
	HTuple	Width, Height, hv_Area, hv_Row, hv_Column, hv_ROW, hv_COL, hv_Rad, hv_n, hv_i;
	//HTuple  hv_AcqHandleleft;

	//Image Acquisition 01: Code generated by Image Acquisition 01
	/*if (opencameraleft)
	{
		OpenFramegrabber("GigEVision2", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
			-1, "false", "default", "002149024924_DahengImaging_MER20005GM", 0, -1, &hv_AcqHandleleft);
		GrabImageStart(hv_AcqHandleleft, -1);
		opencameraleft = 0;
		//AfxMessageBox(L"左相机打开了");
	}*/

	GrabImage(&ho_image_l, hv_AcqHandleleft);

	//在视觉处理的窗口中显示采集的图像以及默认裁剪框
	try
	{
		GetImageSize(ho_image_l, &hv_Width, &hv_Height);
	}
	catch (const HalconCpp::HOperatorException& e5)
	{
		AfxMessageBox(L"左相机异常,请尝试重连相机");
	}
	
	//HTuple  hv_WindowHandle;
	ZoomImageSize(ho_image_l, &ho_ImageZoom, hv_Width / scale, hv_Height / scale, "constant");
	WriteImage(ho_ImageZoom, "bmp", 0, "left.bmp");
	//获取图像尺寸


	//更新裁剪框的坐标
	hv_Row1 = scale * m_startPos_l_y;
	hv_Column1 = scale * m_startPos_l_x;
	hv_Row2 = scale * m_endPos_l_y;
	hv_Column2 = scale * m_endPos_l_x;

	CropPart(ho_image_l, &ho_ImagePart, hv_Row1, hv_Column1, hv_Column2 - hv_Column1,
		hv_Row2 - hv_Row1);
	GetImageSize(ho_ImagePart, &hv_Width1, &hv_Height1);
	Rgb1ToGray(ho_ImagePart, &ho_GrayImage);
	Threshold(ho_ImagePart, &ho_Regions, (HTuple)hv_Threshold, 255);
	AreaCenter(ho_Regions, &hv_Area, &hv_Row, &hv_Column);

	if (hv_Area < 0.20 * (hv_Row2 - hv_Row1) * (hv_Column2 - hv_Column1))
	{
		check_l_over = 0;
		empty_location_data();
		AfxMessageBox(_T("电视机背板在裁剪框中所占比例太小,或者是相机被遮挡, 影响工件定位,请调整裁剪框位置,或移开障碍物"));
		return 0;
	}

	Connection(ho_Regions, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);

	Boundary(ho_SelectedRegions, &ho_RegionBorder, "inner");
	//*膨胀
	DilationCircle(ho_RegionBorder, &ho_RegionDilation, 2.5);
	RegionToBin(ho_RegionDilation, &ho_BinImage, 255, 0, hv_Width, hv_Height);

	//在弧形区域进行边缘检测
	EdgesSubPix(ho_BinImage, &ho_Edges, "canny", 1, 20, 40);
	//分割边缘：线和圆
	//对检测的边缘进行分割，识别线或者圆'lines_circles'，
	SegmentContoursXld(ho_Edges, &ho_RegionDilation, "lines_circles", 5, 5, 8);
	//统计识别出圆或线的数量
	CountObj(ho_RegionDilation, &hv_Number);
	//储存拟合圆的圆心坐标和半径
	hv_ROW = HTuple();
	hv_COL = HTuple();
	hv_Rad = HTuple();
	hv_n = 0;
	{
		HTuple end_val40 = hv_Number;
		HTuple step_val40 = 1;
		for (hv_i = 1; hv_i.Continue(end_val40, step_val40); hv_i += step_val40)
		{

			//选择轮廓并根据特性确定是否拟合圆：* Attrib = -1 线段 0 椭圆 1圆
			SelectObj(ho_RegionDilation, &ho_ObjectSelected, hv_i);
			GetContourGlobalAttribXld(ho_ObjectSelected, "cont_approx", &hv_Attrib);
			if (0 != (hv_Attrib > 0))
			{
				//逼近结果生成一个圆轮廓
				FitCircleContourXld(ho_ObjectSelected, "ahuber", -1, 2, 0, 3, 2, &hv_Row, &hv_Column,
					&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
				//这里会生成大量的拟合圆，通过添加条件，选取自己需要的圆，这里
				//条件为半径，（可以自己注释条件，查看所有拟合圆结果 ）
				if (0 != (HTuple(hv_Radius < 100).TupleAnd(hv_Radius > 10)))
				{
					//生成轮廓
					GenCircleContourXld(&ho_ContCircle, hv_Row, hv_Column, hv_Radius, 0, HTuple(360).TupleRad(),
						"positive", 1.0);
					//记录圆的圆心坐标和半径信息
					hv_ROW[hv_n] = hv_Row;
					hv_COL[hv_n] = hv_Column;
					hv_Rad[hv_n] = hv_Radius;
					hv_n += 1;
					if (HDevWindowStack::IsOpen())
						DispObj(ho_ContCircle, HDevWindowStack::GetActive());
					GenRegionContourXld(ho_ContCircle, &ho_Region, "filled");
					DilationCircle(ho_Region, &ho_RegionDilation1, 5.5);
					Difference(ho_SelectedRegions, ho_RegionDilation1, &ho_RegionDifference);
					ho_SelectedRegions = ho_RegionDifference;
				}
			}
		}
	}
	//求凸包

	RegionToBin(ho_SelectedRegions, &ho_BinImage1, 255, 0, hv_Width, hv_Height);
	MedianImage(ho_BinImage1, &ho_ImageMedian, "square", hv_Filter_block_radius, "mirrored");
	Threshold(ho_ImageMedian, &ho_Region1, 128, 255);

	ShapeTrans(ho_Region1, &ho_RegionTrans, "convex");
	RegionToBin(ho_RegionTrans, &ho_BinImage, 255, 0, hv_Width1, hv_Height1);

	
	//检测亚像素边缘
	EdgesSubPix(ho_BinImage, &ho_Edges, "canny", 1, 20, 40);
	SegmentContoursXld(ho_Edges, &ho_ContoursSplit, "lines_circles", 0, 4, 2);

	SelectShapeXld(ho_ContoursSplit, &ho_SelectedXLD1, "height", "and", hv_min_length_width_line,
		5000);


	SelectShapeXld(ho_ContoursSplit, &ho_SelectedXLD, "width", "and", hv_min_length_height_line,
		5000);

	FitLineContourXld(ho_SelectedXLD, "tukey", -1, 0, 5, 2, &hv_RowBegin, &hv_ColBegin,
		&hv_RowEnd, &hv_ColEnd, &hv_Nr1, &hv_Nc1, &hv_Dist1);
	TupleLength(hv_RowBegin, &hv_Length);
	//检查识别出了几条直线

	if (hv_Length ==0||hv_Length>3)
	{
		check_l_over = 0;
		empty_location_data();
		AfxMessageBox(_T("左边没找到横直线! 定位失败,检测直角失败,请框选图片裁剪框时尽量选择直角附近光照对比明显的区域!"));
		return 0;
	}
	if (hv_Length == 2)
	{
		int a, b;
		if (hv_ColBegin[0] > hv_ColEnd[0])
			a = hv_ColEnd[0].D();
		else
			a = hv_ColBegin[0].D();
		if (hv_RowBegin1[1] > hv_RowEnd1[1])
			b = hv_RowEnd1[1].D();
		else
			b = hv_RowBegin1[1].D();//发
		if (a < b)
		{
			hv_RowBegin1 = hv_RowBegin1[0].D();
			hv_ColBegin1 = hv_ColBegin1[0].D();
			hv_RowEnd1 = hv_RowEnd1[0].D();
			hv_ColEnd1 = hv_ColEnd1[0].D();
		}
		else
		{
			hv_RowBegin1 = hv_RowBegin1[1].D();
			hv_ColBegin1 = hv_ColBegin1[1].D();
			hv_RowEnd1 = hv_RowEnd1[1].D();
			hv_ColEnd1 = hv_ColEnd1[1].D();
		}
	}
	

	//计算该线段的延长线
	LineOrientation(hv_RowBegin, hv_ColBegin, hv_RowEnd, hv_ColEnd, &hv_Phi);
	hv_LineLength = 1000;
	if (0 != (hv_ColBegin < hv_ColEnd))
	{
		hv_RowBegin += (hv_Phi.TupleSin()) * hv_LineLength;
		hv_ColBegin = hv_ColBegin - (((hv_Phi.TupleCos()).TupleFabs()) * hv_LineLength);
	}
	else
	{
		hv_RowEnd += (hv_Phi.TupleSin()) * hv_LineLength;
		hv_ColEnd = hv_ColEnd - (((hv_Phi.TupleCos()).TupleFabs()) * hv_LineLength);
	}


	GenRegionLine(&ho_RegionLines1, hv_RowBegin, hv_ColBegin, hv_RowEnd, hv_ColEnd);
	FitLineContourXld(ho_SelectedXLD1, "tukey", -1, 0, 5, 2, &hv_RowBegin1, &hv_ColBegin1,
		&hv_RowEnd1, &hv_ColEnd1, &hv_Nr, &hv_Nc, &hv_Dist);
	TupleLength(hv_RowBegin1, &hv_Length1);
	//检查识别出了几条直线
	if (hv_Length1==0|| hv_Length1>2)
	{
		check_l_over = 0;
		empty_location_data();
		AfxMessageBox(_T("左边没找到竖直线! 定位失败,检测直角失败,请框选图片裁剪框时尽量选择直角附近光照对比明显的区域"));
		return 0;
	}
	if (hv_Length1 == 2)
	{
		int a, b;
		if (hv_RowBegin1[0] > hv_RowEnd1[0])
			a = hv_RowEnd1[0].D();
		else
			a = hv_RowBegin1[0].D();
		if (hv_RowBegin1[1] > hv_RowEnd1[1])
			b = hv_RowEnd1[1].D();
		else
			b = hv_RowBegin1[1].D();
		if (a < b)
		{
			hv_RowBegin1 = hv_RowBegin1[0].D();
			hv_ColBegin1 = hv_ColBegin1[0].D();
			hv_RowEnd1 = hv_RowEnd1[0].D();
			hv_ColEnd1 = hv_ColEnd1[0].D();
		}
		else
		{
			hv_RowBegin1 = hv_RowBegin1[1].D();
			hv_ColBegin1 = hv_ColBegin1[1].D();
			hv_RowEnd1 = hv_RowEnd1[1].D();
			hv_ColEnd1 = hv_ColEnd1[1].D();
		}
	}


	//计算该线段的延长线
	LineOrientation(hv_RowBegin1, hv_ColBegin1, hv_RowEnd1, hv_ColEnd1, &hv_Phi1);
	if (0 != (hv_RowBegin1 > hv_RowEnd1))
	{
		hv_RowEnd1 = hv_RowEnd1 - (((hv_Phi1.TupleFabs()).TupleSin()) * hv_LineLength);
		hv_ColEnd1 += ((hv_Phi1 / (hv_Phi1.TupleFabs())) * (hv_Phi1.TupleCos())) * hv_LineLength;
	}
	else
	{
		hv_RowBegin1 = hv_RowBegin1 - (((hv_Phi1.TupleFabs()).TupleSin()) * hv_LineLength);
		hv_ColBegin1 += ((hv_Phi1 / (hv_Phi1.TupleFabs())) * (hv_Phi1.TupleCos())) * hv_LineLength;
	}
	GenRegionLine(&ho_RegionLines, hv_RowBegin1, hv_ColBegin1, hv_RowEnd1, hv_ColEnd1);

	IntersectionLines(hv_RowBegin1, hv_ColBegin1, hv_RowEnd1, hv_ColEnd1, hv_RowBegin,
		hv_ColBegin, hv_RowEnd, hv_ColEnd, &hv_Row_l, &hv_Column_l, &hv_IsOverlapping);
	hv_RowBegin += hv_Row1;
	hv_RowBegin1 += hv_Row1;
	hv_RowEnd += hv_Row1;
	hv_RowEnd1 += hv_Row1;
	hv_ColBegin += hv_Column1;
	hv_ColBegin1 += hv_Column1;
	hv_ColEnd += hv_Column1;
	hv_ColEnd1 += hv_Column1;
	hv_Row_l += hv_Row1;
	hv_Column_l += hv_Column1;
	//将直角边端点写入全局变量中
	int RowBegin = hv_RowBegin[0].D() / scale;
	int ColBegin = hv_ColBegin[0].D() / scale;
	int RowEnd = hv_RowEnd[0].D() / scale;
	int ColEnd = hv_ColEnd[0].D() / scale;

	int RowBegin1 = hv_RowBegin1[0].D() / scale;
	int ColBegin1 = hv_ColBegin1[0].D() / scale;
	int RowEnd1 = hv_RowEnd1[0].D() / scale;
	int ColEnd1 = hv_ColEnd1[0].D() / scale;


	left_begin_p_x = ColBegin;
	left_begin_p_y = RowBegin;
	left_end_p_x = ColEnd;
	left_end_p_y = RowEnd;
	left_begin_p1_x = ColBegin1;
	left_begin_p1_y = RowBegin1;
	left_end_p1_x = ColEnd1;
	left_end_p1_y = RowEnd1;

	Row_l = hv_Row_l[0].D();
	Column_l = hv_Column_l[0].D();

	//AfxMessageBox(_T("左边相机定位结束"));
	check_l_over = 0;
	return 0;
}

int locateright()
//UINT locateright(LPVOID pParam)
{
	check_r_over = 1;
	// Local iconic variables
	HObject  ho_image, ho_ImagePart, ho_ObjectSelected;
	HObject  ho_GrayImage, ho_Regions, ho_BinImage1, ho_ImageMedian;
	HObject  ho_Region, ho_RegionClosing, ho_ConnectedRegions, ho_RegionDilation1, ho_RegionDifference;
	HObject  ho_RegionFillUp, ho_SelectedRegions, ho_RegionTrans;
	HObject  ho_BinImage, ho_Edges, ho_ContoursSplit, ho_SelectedXLD1, ho_Region1;
	HObject  ho_SelectedXLD, ho_RegionLines1, ho_RegionLines, ho_RegionBorder, ho_RegionDilation, ho_ContCircle;
	HObject  ho_Image1, ho_image2, ho_ImageZoom, ho_ConnectedRegions1, ho_SelectedRegions1, ho_BinImage2;
	HTuple hv_Row1, hv_Column1, hv_Row2, hv_Column2;
	// Local control variables

	HTuple  hv_WindowHandle, hv_Width, hv_Height, hv_Number, hv_Attrib, hv_Radius, hv_StartPhi, hv_EndPhi, hv_PointOrder;
	HTuple  hv_Row11, hv_Column11, hv_Row22, hv_Column22, hv_Width1;
	HTuple  hv_Height1, hv_RowBegin, hv_ColBegin, hv_RowEnd;
	HTuple  hv_ColEnd, hv_Nr1, hv_Nc1, hv_Dist1, hv_Phi, hv_LineLength;
	HTuple  hv_RowBegin1, hv_ColBegin1, hv_RowEnd1, hv_ColEnd1;
	HTuple  hv_Nr, hv_Nc, hv_Dist, hv_Phi1, hv_Row_left, hv_Column_left, hv_Length, hv_Length1;
	HTuple  hv_IsOverlapping, hv_Angle;
	HTuple	Width, Height, hv_Area, hv_Row, hv_Column, hv_ROW, hv_COL, hv_Rad, hv_n, hv_i;
	//HTuple  hv_AcqHandleright;
	//Image Acquisition 01: Code generated by Image Acquisition 01
	/*if (opencameraleft)
	{
		OpenFramegrabber("GigEVision2", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
			-1, "false", "default", "002149024925_DahengImaging_MER20005GM", 0, -1, &hv_AcqHandleright);
		GrabImageStart(hv_AcqHandleright, -1);
		opencameraleft = 0;
		//AfxMessageBox(L"右相机打开了");
	}*/
	try
	{
		GrabImage(&ho_image, hv_AcqHandleright);
	}
	catch (const HalconCpp::HOperatorException& e4)
	{
		AfxMessageBox(L"右相机异常,请尝试重连相机");
	}
	

	//在视觉处理的窗口中显示采集的图像以及默认裁剪框
	GetImageSize(ho_image, &hv_Width, &hv_Height);
	//HTuple  hv_WindowHandle;
	ZoomImageSize(ho_image, &ho_ImageZoom, hv_Width / scale, hv_Height / scale, "constant");

	WriteImage(ho_ImageZoom, "bmp", 0, "right.bmp");
	//获取图像尺寸

	//更新裁剪框的坐标
	hv_Row1 = scale * m_startPos_r_y;
	hv_Column1 = scale * m_startPos_r_x;
	hv_Row2 = scale * m_endPos_r_y;
	hv_Column2 = scale * m_endPos_r_x;

	CropPart(ho_image, &ho_ImagePart, hv_Row1, hv_Column1, hv_Column2 - hv_Column1,
		hv_Row2 - hv_Row1);
	GetImageSize(ho_ImagePart, &hv_Width1, &hv_Height1);
	
	Threshold(ho_ImagePart, &ho_Regions, (HTuple)hv_Threshold, 255);
	AreaCenter(ho_Regions, &hv_Area, &hv_Row, &hv_Column);

	if (hv_Area < 0.20 * (hv_Row2 - hv_Row1) * (hv_Column2 - hv_Column1))
	{
		check_r_over = 0;
		empty_location_data();
		AfxMessageBox(_T("电视机背板在裁剪框中所占比例太小,或者是相机被遮挡, 影响工件定位,请调整裁剪框位置,或移开障碍物"));
		return -1;
	}


	Connection(ho_Regions, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);

	Boundary(ho_SelectedRegions, &ho_RegionBorder, "inner");
	//*膨胀
	DilationCircle(ho_RegionBorder, &ho_RegionDilation, 2.5);
	RegionToBin(ho_RegionDilation, &ho_BinImage, 255, 0, hv_Width, hv_Height);

	//在弧形区域进行边缘检测
	EdgesSubPix(ho_BinImage, &ho_Edges, "canny", 1, 20, 40);
	//分割边缘：线和圆
	//对检测的边缘进行分割，识别线或者圆'lines_circles'，
	SegmentContoursXld(ho_Edges, &ho_RegionDilation, "lines_circles", 5, 5, 8);
	//统计识别出圆或线的数量
	CountObj(ho_RegionDilation, &hv_Number);
	//储存拟合圆的圆心坐标和半径
	hv_ROW = HTuple();
	hv_COL = HTuple();
	hv_Rad = HTuple();
	hv_n = 0;
	{
		HTuple end_val40 = hv_Number;
		HTuple step_val40 = 1;
		for (hv_i = 1; hv_i.Continue(end_val40, step_val40); hv_i += step_val40)
		{

			//选择轮廓并根据特性确定是否拟合圆：* Attrib = -1 线段 0 椭圆 1圆
			SelectObj(ho_RegionDilation, &ho_ObjectSelected, hv_i);
			GetContourGlobalAttribXld(ho_ObjectSelected, "cont_approx", &hv_Attrib);
			if (0 != (hv_Attrib > 0))
			{
				//逼近结果生成一个圆轮廓
				FitCircleContourXld(ho_ObjectSelected, "ahuber", -1, 2, 0, 3, 2, &hv_Row, &hv_Column,
					&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
				//这里会生成大量的拟合圆，通过添加条件，选取自己需要的圆，这里
				//条件为半径，（可以自己注释条件，查看所有拟合圆结果 ）
				if (0 != (HTuple(hv_Radius < 100).TupleAnd(hv_Radius > 10)))
				{
					//生成轮廓
					GenCircleContourXld(&ho_ContCircle, hv_Row, hv_Column, hv_Radius, 0, HTuple(360).TupleRad(),
						"positive", 1.0);
					//记录圆的圆心坐标和半径信息
					hv_ROW[hv_n] = hv_Row;
					hv_COL[hv_n] = hv_Column;
					hv_Rad[hv_n] = hv_Radius;
					hv_n += 1;
					if (HDevWindowStack::IsOpen())
						DispObj(ho_ContCircle, HDevWindowStack::GetActive());
					GenRegionContourXld(ho_ContCircle, &ho_Region, "filled");
					DilationCircle(ho_Region, &ho_RegionDilation1, 5.5);
					Difference(ho_SelectedRegions, ho_RegionDilation1, &ho_RegionDifference);
					ho_SelectedRegions = ho_RegionDifference;
				}
			}
		}
	}


	//求凸包

	RegionToBin(ho_SelectedRegions, &ho_BinImage1, 255, 0, hv_Width, hv_Height);
	MedianImage(ho_BinImage1, &ho_ImageMedian, "square", hv_Filter_block_radius, "mirrored");
	Threshold(ho_ImageMedian, &ho_Region1, 128, 255);

	ShapeTrans(ho_Region1, &ho_RegionTrans, "convex");
	RegionToBin(ho_RegionTrans, &ho_BinImage, 255, 0, hv_Width1, hv_Height1);

	//检测亚像素边缘
	EdgesSubPix(ho_BinImage, &ho_Edges, "canny", 1, 20, 40);
	SegmentContoursXld(ho_Edges, &ho_ContoursSplit, "lines_circles", 0, 4, 2);

	SelectShapeXld(ho_ContoursSplit, &ho_SelectedXLD1, "height", "and", hv_min_length_width_line,
		5000);


	SelectShapeXld(ho_ContoursSplit, &ho_SelectedXLD, "width", "and", hv_min_length_height_line,
		5000);

	FitLineContourXld(ho_SelectedXLD, "tukey", -1, 0, 5, 2, &hv_RowBegin, &hv_ColBegin,
		&hv_RowEnd, &hv_ColEnd, &hv_Nr1, &hv_Nc1, &hv_Dist1);
	TupleLength(hv_RowBegin, &hv_Length);
	//检查识别出了几条直线

	if (hv_Length !=1)
	{
		check_r_over = 0;
		empty_location_data();
		AfxMessageBox(_T("右边没找到横直线! 定位失败,检测直角失败,请框选图片裁剪框时尽量选择直角附近光照对比明显的区域!"));
		return 0;
	}
	
	//计算该线段的延长线
	LineOrientation(hv_RowBegin, hv_ColBegin, hv_RowEnd, hv_ColEnd, &hv_Phi);
	hv_LineLength = 1000;
	if (0 != (hv_ColBegin < hv_ColEnd))
	{
		hv_RowEnd = hv_RowEnd - ((hv_Phi.TupleSin()) * hv_LineLength);
		hv_ColEnd += (hv_Phi.TupleCos()) * hv_LineLength;
	}
	else
	{
		hv_RowBegin = hv_RowBegin - ((hv_Phi.TupleSin()) * hv_LineLength);
		hv_ColBegin += (hv_Phi.TupleCos()) * hv_LineLength;
	}
	GenRegionLine(&ho_RegionLines1, hv_RowBegin, hv_ColBegin, hv_RowEnd, hv_ColEnd);
	FitLineContourXld(ho_SelectedXLD1, "tukey", -1, 0, 5, 2, &hv_RowBegin1, &hv_ColBegin1,
		&hv_RowEnd1, &hv_ColEnd1, &hv_Nr, &hv_Nc, &hv_Dist);
	TupleLength(hv_RowBegin1, &hv_Length1);
	//检查识别出了几条直线
	if (hv_Length1 ==0|| hv_Length1>2)
	{
		check_r_over = 0;
		empty_location_data();
		AfxMessageBox(_T("右边没找到竖直线! 定位失败,检测直角失败,请框选图片裁剪框时尽量选择直角附近光照对比明显的区域"));
		return 0;
	}
	if (hv_Length1 == 2)
	{
		int a,b;
		if (hv_RowBegin1[0] > hv_RowEnd1[0])
			a = hv_RowEnd1[0].D();
		else
			a = hv_RowBegin1[0].D();
		if (hv_RowBegin1[1] > hv_RowEnd1[1])
			b = hv_RowEnd1[1].D();
		else
			b = hv_RowBegin1[1].D();
		if (a < b)
		{
			hv_RowBegin1 = hv_RowBegin1[0].D();
			hv_ColBegin1 = hv_ColBegin1[0].D();
			hv_RowEnd1 = hv_RowEnd1[0].D();
			hv_ColEnd1 = hv_ColEnd1[0].D();
		}
		else
		{
			hv_RowBegin1 = hv_RowBegin1[1].D();
			hv_ColBegin1 = hv_ColBegin1[1].D();
			hv_RowEnd1 = hv_RowEnd1[1].D();
			hv_ColEnd1 = hv_ColEnd1[1].D();
		}
	}

	//计算该线段的延长线
	LineOrientation(hv_RowBegin1, hv_ColBegin1, hv_RowEnd1, hv_ColEnd1, &hv_Phi1);
	if (0 != (hv_RowBegin1 > hv_RowEnd1))
	{
		hv_RowEnd1 = hv_RowEnd1 - (((hv_Phi1.TupleFabs()).TupleSin()) * hv_LineLength);
		hv_ColEnd1 += ((hv_Phi1 / (hv_Phi1.TupleFabs())) * (hv_Phi1.TupleCos())) * hv_LineLength;
	}
	else
	{
		hv_RowBegin1 = hv_RowBegin1 - (((hv_Phi1.TupleFabs()).TupleSin()) * hv_LineLength);
		hv_ColBegin1 += ((hv_Phi1 / (hv_Phi1.TupleFabs())) * (hv_Phi1.TupleCos())) * hv_LineLength;
	}
	GenRegionLine(&ho_RegionLines, hv_RowBegin1, hv_ColBegin1, hv_RowEnd1, hv_ColEnd1);

	IntersectionLines(hv_RowBegin1, hv_ColBegin1, hv_RowEnd1, hv_ColEnd1, hv_RowBegin,
		hv_ColBegin, hv_RowEnd, hv_ColEnd, &hv_Row_r, &hv_Column_r, &hv_IsOverlapping);

	hv_RowBegin += hv_Row1;
	hv_RowBegin1 += hv_Row1;
	hv_RowEnd += hv_Row1;
	hv_RowEnd1 += hv_Row1;
	hv_ColBegin += hv_Column1;
	hv_ColBegin1 += hv_Column1;
	hv_ColEnd += hv_Column1;
	hv_ColEnd1 += hv_Column1;
	hv_Row_r += hv_Row1;
	hv_Column_r += hv_Column1;
	//将直角边端点写入全局变量中
	int RowBegin = hv_RowBegin[0].D() / scale;
	int ColBegin = hv_ColBegin[0].D() / scale;
	int RowEnd = hv_RowEnd[0].D() / scale;
	int ColEnd = hv_ColEnd[0].D() / scale;

	int RowBegin1 = hv_RowBegin1[0].D() / scale;
	int ColBegin1 = hv_ColBegin1[0].D() / scale;
	int RowEnd1 = hv_RowEnd1[0].D() / scale;
	int ColEnd1 = hv_ColEnd1[0].D() / scale;


	right_begin_p_x = ColBegin;
	right_begin_p_y = RowBegin;
	right_end_p_x = ColEnd;
	right_end_p_y = RowEnd;
	right_begin_p1_x = ColBegin1;
	right_begin_p1_y = RowBegin1;
	right_end_p1_x = ColEnd1;
	right_end_p1_y = RowEnd1;

	Row_r = hv_Row_r[0].D();
	Column_r = hv_Column_r[0].D();
	

	//AfxMessageBox(_T("右边相机定位结束"));

	check_r_over = 0;

	return 0;
}
void CvisionDlg::OnCollectAndLocate()
{
	// TODO: 在此处添加实现代码.
	CString str;
	//long t1 = GetTickCount64(); 

	// TODO: 在此添加控件通知处理程序代码
	locateleft();
	locateright();
	//AfxBeginThread(*locateleft, NULL);
	//AfxBeginThread(*locateright, NULL);
	//等待子线程结束
	/*do
	{
		Sleep(1);
	} while (check_l_over || check_r_over);*/


	//不管是否定位成功,都在界面显示图片
	HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄
	hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"./left.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_left.SetBitmap(hBmp);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

	HBITMAP hBmp_r;    // 保存CBitmap加载的位图的句柄
	hBmp_r = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"./right.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_right.SetBitmap(hBmp_r);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

	if (Row_l == -1 || Row_r == -1)
	{
		empty_location_data();//只要有一幅图定位失败,再次将数据置为-1.防止线程导致的错误
	}
	/********************************在界面中显示左相机定位结果*****************************************************/
	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
	CWnd* pwnd = GetDlgItem(IDC_VS_PIC_LEFT);
	CClientDC dc(pwnd);
	dc.SelectObject(&pen);//选择画笔

	POINT m_startPos_l = { m_startPos_l_x,m_startPos_l_y };
	POINT m_endPos_l = { m_endPos_l_x,m_endPos_l_y };
	dc.MoveTo(m_startPos_l);
	dc.LineTo(m_endPos_l.x, m_startPos_l.y);
	dc.LineTo(m_endPos_l.x, m_endPos_l.y);
	dc.LineTo(m_startPos_l.x, m_endPos_l.y);
	dc.LineTo(m_startPos_l.x, m_startPos_l.y);






	CPen pen1(PS_SOLID, 2, RGB(0, 255, 255));//创建一个画笔工具
	//CWnd* pwnd = GetDlgItem(IDC_VS_PIC_LEFT);
	//CClientDC dc(pwnd);
	dc.SelectObject(&pen1);//选择画笔
	POINT left_begin_p = { left_begin_p_x,left_begin_p_y };
	POINT left_end_p = { left_end_p_x,left_end_p_y };
	POINT left_begin_p1 = { left_begin_p1_x,left_begin_p1_y };
	POINT left_end_p1 = { left_end_p1_x,left_end_p1_y };
	dc.MoveTo(left_begin_p);
	dc.LineTo(left_end_p.x, left_end_p.y);
	dc.MoveTo(left_begin_p1);
	dc.LineTo(left_end_p1.x, left_end_p1.y);
	/********************************在界面中显示左相机定位结果   结束*****************************************************/



	/********************************在界面中显示右相机定位结果   开始*****************************************************/


	//CPen pen(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
	CWnd* pwnd_r = GetDlgItem(IDC_VS_PIC_RIGHT);
	CClientDC dc_r(pwnd_r);
	dc_r.SelectObject(&pen);//选择画笔
	POINT m_startPos_r = { m_startPos_r_x,m_startPos_r_y };
	POINT m_endPos_r = { m_endPos_r_x,m_endPos_r_y };
	dc_r.MoveTo(m_startPos_r);
	dc_r.LineTo(m_endPos_r.x, m_startPos_r.y);
	dc_r.LineTo(m_endPos_r.x, m_endPos_r.y);
	dc_r.LineTo(m_startPos_r.x, m_endPos_r.y);
	dc_r.LineTo(m_startPos_r.x, m_startPos_r.y);


	CPen pen11(PS_SOLID, 2, RGB(0, 255, 255));//创建一个画笔工具
	dc_r.SelectObject(&pen11);//选择画笔
	POINT right_begin_p = { right_begin_p_x,right_begin_p_y };
	POINT right_end_p = { right_end_p_x,right_end_p_y };
	POINT right_begin_p1 = { right_begin_p1_x,right_begin_p1_y };
	POINT right_end_p1 = { right_end_p1_x,right_end_p1_y };
	dc_r.MoveTo(right_begin_p);
	dc_r.LineTo(right_end_p.x, right_end_p.y);
	dc_r.MoveTo(right_begin_p1);
	dc_r.LineTo(right_end_p1.x, right_end_p1.y);
	//m_vs_btn_collect.EnableWindow(TRUE); //使能按钮



	//只有双相机定位成功时才计算定位数据,并在表格中显示定位数据
	if (Row_l != -1 && Row_r != -1)
	{

		filename = DATA_FOLDER + string("24_left_cam_calib.xml");
		//用相机标定文件来将图象处理得到的像素角点坐标转换为机器人基坐标系下的坐标
		cv::FileStorage fs(filename, cv::FileStorage::READ);
		cv::Mat cam_matrix_left, r_left, t_left;
		double s_left;
		fs["cameraMatrix"] >> cam_matrix_left;//读num1 其中["num1"]为1.yml内标识作用
		fs["rvec"] >> r_left;
		fs["tvec"] >> t_left;
		//fs["s"] >> s_left;
		fs.release();

		cv::Mat left_pixel_coordinate = cv::Mat::ones(cv::Size(3, 1), CV_64FC1);
		//left_pixel_coordinate.at<float>(0, 0) = 4916;
		//left_pixel_coordinate.at<float>(0, 1) = 3227;
		left_pixel_coordinate.at<double>(0, 0) = Column_l;
		left_pixel_coordinate.at<double>(0, 1) = Row_l;
		left_pixel_coordinate.at<double>(0, 2) = 1.f;
		left_pixel_coordinate = left_pixel_coordinate.t();

		//计算标定中的比例系数s_right
		cv::Mat tempMat3, tempMat4;
		tempMat3 = r_left.inv() * cam_matrix_left.inv() * left_pixel_coordinate;
		tempMat4 = r_left.inv() * t_left;
		s_left = tempMat4.at<double>(2, 0) / tempMat3.at<double>(2, 0);

		cv::Mat left_robot_coordinate;
		left_robot_coordinate = r_left.inv() * (cam_matrix_left.inv() * s_left * left_pixel_coordinate - t_left);//从标定文件中读标定数据,将像素坐标转换为世界坐标
		double row_ll = left_robot_coordinate.at<double>(0, 0);
		double column_ll = left_robot_coordinate.at<double>(1, 0);



		cv::FileStorage fss("26_right_cam_calib.xml", cv::FileStorage::READ);
		cv::Mat cam_matrix_right, r_right, t_right;
		float s_right;
		fss["cameraMatrix"] >> cam_matrix_right;//读num1 其中["num1"]为1.yml内标识作用
		fss["rvec"] >> r_right;
		fss["tvec"] >> t_right;

		fss.release();



		cv::Mat right_pixel_coordinate = cv::Mat::ones(cv::Size(3, 1), CV_64FC1);
		right_pixel_coordinate.at<double>(0, 0) = Column_r;
		right_pixel_coordinate.at<double>(0, 1) = Row_r;
		right_pixel_coordinate.at<double>(0, 2) = 1.00000;
		right_pixel_coordinate = right_pixel_coordinate.t();
		//计算标定中的比例系数s_right
		cv::Mat tempMat, tempMat2;
		tempMat = r_right.inv() * cam_matrix_right.inv() * right_pixel_coordinate;
		tempMat2 = r_right.inv() * t_right;
		s_right = tempMat2.at<double>(2, 0) / tempMat.at<double>(2, 0);


		cv::Mat right_robot_coordinate;
		right_robot_coordinate = r_right.inv() * (cam_matrix_right.inv() * s_right * right_pixel_coordinate - t_right);
		double row_rr = right_robot_coordinate.at<double>(0, 0);
		double column_rr = right_robot_coordinate.at<double>(1, 0);
		//用定标后的左右两个点来计算倾角
		double angle_ll = -atan((row_rr - row_ll) / (column_rr - column_ll)) / 3.1415926535 * 180;

		//计算左边图象横向直线开始点的世界坐标
		cv::Mat left_line_begin_pixel_coordinate = cv::Mat::ones(cv::Size(3, 1), CV_64FC1);
		left_line_begin_pixel_coordinate.at<double>(0, 0) = left_begin_p_x;
		left_line_begin_pixel_coordinate.at<double>(0, 1) = left_begin_p_y;
		left_line_begin_pixel_coordinate.at<double>(0, 2) = 1.00000;
		left_line_begin_pixel_coordinate = left_line_begin_pixel_coordinate.t();
		//计算标定中的比例系数s_right
		cv::Mat tempMat66, tempMat266;
		tempMat66 = r_left.inv() * cam_matrix_left.inv() * left_line_begin_pixel_coordinate;
		tempMat266 = r_left.inv() * t_left;
		s_left = tempMat266.at<double>(2, 0) / tempMat66.at<double>(2, 0);

		cv::Mat left_line_begin_robot_coordinate;
		left_line_begin_robot_coordinate = r_left.inv() * (cam_matrix_left.inv() * s_left * left_line_begin_pixel_coordinate - t_left);
		double left_line_begin_robot_coordinate_x = left_line_begin_robot_coordinate.at<double>(0, 0);
		double left_line_begin_robot_coordinate_y = left_line_begin_robot_coordinate.at<double>(1, 0);

		//计算左边图象横向直线结束点的世界坐标
		cv::Mat left_line_end_pixel_coordinate = cv::Mat::ones(cv::Size(3, 1), CV_64FC1);
		left_line_end_pixel_coordinate.at<double>(0, 0) = left_end_p_x;
		left_line_end_pixel_coordinate.at<double>(0, 1) = left_end_p_y;
		left_line_end_pixel_coordinate.at<double>(0, 2) = 1.00000;
		left_line_end_pixel_coordinate = left_line_end_pixel_coordinate.t();
		//计算标定中的比例系数s_right
		cv::Mat tempMat33, tempMat233;
		tempMat33 = r_left.inv() * cam_matrix_left.inv() * left_line_end_pixel_coordinate;
		tempMat233 = r_left.inv() * t_left;
		s_left = tempMat233.at<double>(2, 0) / tempMat33.at<double>(2, 0);

		cv::Mat left_line_end_robot_coordinate;
		left_line_end_robot_coordinate = r_left.inv() * (cam_matrix_left.inv() * s_left * left_line_end_pixel_coordinate - t_left);
		double left_line_end_robot_coordinate_x = left_line_end_robot_coordinate.at<double>(0, 0);
		double left_line_end_robot_coordinate_y = left_line_end_robot_coordinate.at<double>(1, 0);

		double angle_ll_line = -atan((left_line_end_robot_coordinate_x - left_line_begin_robot_coordinate_x) / (left_line_end_robot_coordinate_y - left_line_begin_robot_coordinate_y)) / 3.1415926535 * 180;


		//计算右边图象横向直线开始点的世界坐标
		cv::Mat right_line_begin_pixel_coordinate = cv::Mat::ones(cv::Size(3, 1), CV_64FC1);
		right_line_begin_pixel_coordinate.at<double>(0, 0) = right_begin_p_x;
		right_line_begin_pixel_coordinate.at<double>(0, 1) = right_begin_p_y;
		right_line_begin_pixel_coordinate.at<double>(0, 2) = 1.00000;
		right_line_begin_pixel_coordinate = right_line_begin_pixel_coordinate.t();
		//计算标定中的比例系数s_right
		cv::Mat tempMat66right, tempMat266right;
		tempMat66right = r_right.inv() * cam_matrix_right.inv() * right_line_begin_pixel_coordinate;
		tempMat266right = r_right.inv() * t_right;
		s_right = tempMat266right.at<double>(2, 0) / tempMat66right.at<double>(2, 0);

		cv::Mat right_line_begin_robot_coordinate;
		right_line_begin_robot_coordinate = r_right.inv() * (cam_matrix_right.inv() * s_right * right_line_begin_pixel_coordinate - t_right);
		double right_line_begin_robot_coordinate_x = right_line_begin_robot_coordinate.at<double>(0, 0);
		double right_line_begin_robot_coordinate_y = right_line_begin_robot_coordinate.at<double>(1, 0);

		//计算右边图象横向直线结束点的世界坐标
		cv::Mat right_line_end_pixel_coordinate = cv::Mat::ones(cv::Size(3, 1), CV_64FC1);
		right_line_end_pixel_coordinate.at<double>(0, 0) = right_end_p_x;
		right_line_end_pixel_coordinate.at<double>(0, 1) = right_end_p_y;
		right_line_end_pixel_coordinate.at<double>(0, 2) = 1.00000;
		right_line_end_pixel_coordinate = right_line_end_pixel_coordinate.t();
		//计算标定中的比例系数s_right
		cv::Mat tempMat33right, tempMat233right;
		tempMat33right = r_right.inv() * cam_matrix_right.inv() * right_line_end_pixel_coordinate;
		tempMat233right = r_right.inv() * t_right;
		s_right = tempMat233right.at<double>(2, 0) / tempMat33right.at<double>(2, 0);

		cv::Mat right_line_end_robot_coordinate;
		right_line_end_robot_coordinate = r_right.inv() * (cam_matrix_right.inv() * s_right * right_line_end_pixel_coordinate - t_right);
		double right_line_end_robot_coordinate_x = right_line_end_robot_coordinate.at<double>(0, 0);
		double right_line_end_robot_coordinate_y = right_line_end_robot_coordinate.at<double>(1, 0);

		double angle_rr_line = -atan((right_line_end_robot_coordinate_x - right_line_begin_robot_coordinate_x) / (right_line_end_robot_coordinate_y - right_line_begin_robot_coordinate_y)) / 3.1415926535 * 180;





		//////*将定位信息显示到表格中*/////////
		CString Column_li;
		Column_li.Format(_T("%f"), row_ll - 1564.3459);
		//Column_li.Format(_T("%f"), Row_l);
		m_vs_list_location_data.InsertItem(0, Column_li);

		CString Row_leftt;
		Row_leftt.Format(_T("%f"), column_ll - 452.5519);
		//Row_leftt.Format(_T("%f"), Column_l);
		m_vs_list_location_data.SetItemText(0, 1, Row_leftt);


		CString Column_ri;
		Column_ri.Format(_T("%f"), row_rr);
		//Column_li.Format(_T("%f"), Row_l);
		m_vs_list_location_data.SetItemText(0, 2, Column_ri);

		CString Row_rightt;
		Row_rightt.Format(_T("%f"), column_rr);
		//Row_leftt.Format(_T("%f"), Column_l);
		m_vs_list_location_data.SetItemText(0, 3, Row_rightt);


		CString Anglee;
		Anglee.Format(_T("%f"), angle_ll);
		m_vs_list_location_data.SetItemText(0, 4, Anglee);


		CString Angleleftline;
		Angleleftline.Format(_T("%f"), angle_ll_line);
		m_vs_list_location_data.SetItemText(0, 5, Angleleftline);


		CString Anglerightline;
		Anglerightline.Format(_T("%f"), angle_rr_line);
		m_vs_list_location_data.SetItemText(0, 6, Anglerightline);

		row_l =(int)( row_ll * 10 -15643);
		column_l =(int)( column_ll * 10-4526 );
		angle_l =(int)( angle_ll * 1000);
	}
	else
	{
		row_l = -1;
		column_l = -1;
		angle_l = -1;
	}

	OnLeftBmpSaveAndShow();
	OnRightBmpSaveAndShow();

	//CString Angleee;
	//Angleee.Format(_T("%f"), Angle_r);
	//m_vs_list_location_data.SetItemText(0, 5, Angleee);



	//////*将定位信息显示到表格中*/////////
	//OnLeftBmpSaveAndShow();
	//OnRightBmpSaveAndShow();


	/*long t2 = GetTickCount64();
	str.Format(L"time:%dms", t2 - t1);
	AfxMessageBox(str);*/
	//如果定位失败,跳出循环,提示重新画框,画完框之后再进入循环
	DeleteObject(hBmp);
	DeleteObject(hBmp_r);
}

void  CvisionDlg::OnCloseCam()
{
	CloseFramegrabber(hv_AcqHandleleft);
	CloseFramegrabber(hv_AcqHandleright);

	//m_vs_btn_opcam.EnableWindow(TRUE); //使能按钮
	//m_vs_btn_closecam.EnableWindow(FALSE); //禁用按钮
	//m_vs_btn_collect.EnableWindow(FALSE); //禁用按钮
	opencameraleft = 1;
	opencameraright = 1;
}

void  CvisionDlg::OnOpenCam()
{
	/********双相机初始化显示    开始********/

	//HTuple hv_AcqHandleright, hv_AcqHandleleft;
	try
	{
		//连接相机
		OpenFramegrabber("GigEVision2", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
			-1, "false", "default", "002149024924_DahengImaging_MER20005GM", 0, -1, &hv_AcqHandleleft);
		//AfxMessageBox(L"第一次调用打开相机函数时打开了相机");
	}
	catch (const HalconCpp::HOperatorException& e)
	{
		try
		{
			//连接相机
			OpenFramegrabber("GigEVision2", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
				-1, "false", "default", "002149024924_DahengImaging_MER20005GM", 0, -1, &hv_AcqHandleleft);
			//AfxMessageBox(L"第二次调用打开相机函数时打开了相机");
		}
		catch (const HalconCpp::HOperatorException& ee)
		{
			try
			{
				CloseFramegrabber(hv_AcqHandleleft);
				OpenFramegrabber("GigEVision2", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
					-1, "false", "default", "002149024924_DahengImaging_MER20005GM", 0, -1, &hv_AcqHandleleft);
				//AfxMessageBox(L"第三次调用打开相机函数时打开了相机");
			}
			catch (const HalconCpp::HOperatorException& eee)
			{
				AfxMessageBox(L"打开相机失败,可能是相机电源,网口未连接,相机被占用等原因,可尝试将相机重新连接电源");
				return;
			}


		}
	}

	try
	{
		//连接相机
		OpenFramegrabber("GigEVision2", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
			-1, "false", "default", "002149024926_DahengImaging_MER20005GM", 0, -1, &hv_AcqHandleright);
		//AfxMessageBox(L"第一次调用打开相机函数时打开了相机");
	}
	catch (const HalconCpp::HOperatorException& e)
	{
		try
		{
			//连接相机
			OpenFramegrabber("GigEVision2", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
				-1, "false", "default", "002149024926_DahengImaging_MER20005GM", 0, -1, &hv_AcqHandleright);
			//AfxMessageBox(L"第二次调用打开相机函数时打开了相机");
		}
		catch (const HalconCpp::HOperatorException& ee)
		{
			try
			{
				CloseFramegrabber(hv_AcqHandleright);
				OpenFramegrabber("GigEVision2", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
					-1, "false", "default", "002149024926_DahengImaging_MER20005GM", 0, -1, &hv_AcqHandleright);
				//AfxMessageBox(L"第三次调用打开相机函数时打开了相机");
			}
			catch (const HalconCpp::HOperatorException& eee)
			{
				
				AfxMessageBox(L"打开相机失败,可能是相机电源,网口未连接,相机被占用等原因,可尝试将相机重新连接电源");
				return;
			}
		}
	}
	SetFramegrabberParam(hv_AcqHandleleft, "ExposureTime", baoguang_time);//设置左相机曝光时间
	SetFramegrabberParam(hv_AcqHandleright, "ExposureTime", baoguang2_time);//设置右相机曝光时间
	GrabImageStart(hv_AcqHandleleft, -1);
	GrabImageStart(hv_AcqHandleright, -1);


	//m_vs_btn_opcam.EnableWindow(FALSE); //禁用按钮


	HObject ho_image_l, ho_ImageRotate_l;
	HObject ho_image_r, ho_ImageRotate_r;
	HTuple hv_Width, hv_Height;
	GrabImage(&ho_image_l, hv_AcqHandleleft);
	GrabImage(&ho_image_r, hv_AcqHandleright);
	//从内存中读取裁剪框位置
	CString aaa = L"成功";
	CInfoFile file;
	file.ReadEditFrameLocation(aaa, edit_frame_left_x, edit_frame_left_y, edit_frame_right_x, edit_frame_right_y);

	m_startPos_l_x = edit_frame_left_x / scale;
	m_startPos_l_y = edit_frame_left_y / scale;
	m_endPos_l_x = m_startPos_l_x + edit_frame_length / scale;
	m_endPos_l_y = m_startPos_l_y + edit_frame_width / scale;


	m_startPos_r_x = edit_frame_right_x / scale;
	m_startPos_r_y = edit_frame_right_y / scale;
	m_endPos_r_x = m_startPos_r_x + edit_frame_length / scale;
	m_endPos_r_y = m_startPos_r_y + edit_frame_width / scale;

	//在视觉处理的窗口中显示采集的图像以及默认裁剪框

	GetImageSize(ho_image_l, &hv_Width, &hv_Height);
	HObject  ho_GrayImagee_l, ho_ImageZoom_l;
	HObject  ho_GrayImagee_r, ho_ImageZoom_r;
	/********************************左相机初始化显示  开始************************************************************/
	Rgb1ToGray(ho_image_l, &ho_GrayImagee_l);
	GetImageSize(ho_GrayImagee_l, &hv_Width, &hv_Height);
	ZoomImageSize(ho_GrayImagee_l, &ho_ImageZoom_l, hv_Width / scale, hv_Height / scale, "constant");
	WriteImage(ho_ImageZoom_l, "bmp", 0, "left.bmp");

	CBitmap bitmap_l;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp_l;    // 保存CBitmap加载的位图的句柄
	hBmp_l = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/left.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_left.SetBitmap(hBmp_l);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  



	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
	CWnd* pwnd = GetDlgItem(IDC_VS_PIC_LEFT);
	CClientDC dc(pwnd);
	dc.SelectObject(&pen);//选择画笔
	POINT m_startPos_l = { edit_frame_left_x / scale,edit_frame_left_y / scale };
	POINT m_endPos_l = { (edit_frame_left_x + edit_frame_length) / scale,(edit_frame_left_y + edit_frame_width) / scale };
	dc.MoveTo(m_startPos_l);
	dc.LineTo(m_endPos_l.x, m_startPos_l.y);
	dc.LineTo(m_endPos_l.x, m_endPos_l.y);
	dc.LineTo(m_startPos_l.x, m_endPos_l.y);
	dc.LineTo(m_startPos_l.x, m_startPos_l.y);


	/********************************左相机初始化显示  结束************************************************************/


	/********************************右相机初始化显示  开始************************************************************/
	Rgb1ToGray(ho_image_r, &ho_GrayImagee_r);
	GetImageSize(ho_GrayImagee_r, &hv_Width, &hv_Height);
	all_width = hv_Width;
	all_height = hv_Height;
	ZoomImageSize(ho_GrayImagee_r, &ho_ImageZoom_r, hv_Width / scale, hv_Height / scale, "constant");
	WriteImage(ho_ImageZoom_r, "bmp", 0, "right.bmp");

	CBitmap bitmap_r;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp_r;    // 保存CBitmap加载的位图的句柄

	hBmp_r = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/right.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_right.SetBitmap(hBmp_r);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

	CPen penR(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
	CWnd* pwndR = GetDlgItem(IDC_VS_PIC_RIGHT);
	CClientDC dcR(pwndR);
	dcR.SelectObject(&penR);//选择画笔
	POINT m_startPos_r = { edit_frame_right_x / scale,edit_frame_right_y / scale };
	POINT m_endPos_r = { (edit_frame_right_x + edit_frame_length) / scale,(edit_frame_right_y + edit_frame_width) / scale };
	dcR.MoveTo(m_startPos_r);
	dcR.LineTo(m_endPos_r.x, m_startPos_r.y);
	dcR.LineTo(m_endPos_r.x, m_endPos_r.y);
	dcR.LineTo(m_startPos_r.x, m_endPos_r.y);
	dcR.LineTo(m_startPos_r.x, m_startPos_r.y);

	OnLeftBmpSaveAndShow();
	OnRightBmpSaveAndShow();
}






void CvisionDlg::OnVsHotkeyLeftDown()
{
	// TODO: 在此添加命令处理程序代码
	if ((m_endPos_l_y + 1) < (all_height / scale))
	{
		m_endPos_l_y += 1;
		m_startPos_l_y = m_endPos_l_y - edit_frame_width / scale;
	}

	else
	{
		m_endPos_l_y = all_height / scale;
		m_startPos_l_y = m_endPos_l_y - edit_frame_width / scale;
	}
	edit_frame_left_x = m_startPos_l_x * scale;
	edit_frame_left_y = m_startPos_l_y * scale;
	CString aaa = L"成功";
	CInfoFile file;
	file.WirteEditFrameLocation(aaa, edit_frame_left_x, edit_frame_left_y, edit_frame_right_x, edit_frame_right_y);

	CBitmap bitmap_l;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp_l;    // 保存CBitmap加载的位图的句柄
	hBmp_l = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/left.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_left.SetBitmap(hBmp_l);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
	CWnd* pwnd = GetDlgItem(IDC_VS_PIC_LEFT);
	CClientDC dc(pwnd);
	dc.SelectObject(&pen);//选择画笔
	POINT m_startPos_l = { m_startPos_l_x,m_startPos_l_y };
	POINT m_endPos_l = { m_endPos_l_x,m_endPos_l_y };
	dc.MoveTo(m_startPos_l);
	dc.LineTo(m_startPos_l.x, m_endPos_l.y);
	dc.LineTo(m_endPos_l.x, m_endPos_l.y);
	dc.LineTo(m_endPos_l.x, m_startPos_l.y);
	dc.LineTo(m_startPos_l.x, m_startPos_l.y);
}


void CvisionDlg::OnVsHotkeyLeftLeft()
{
	// TODO: 在此添加命令处理程序代码
	if ((m_startPos_l_x - 1) > 0)
	{
		m_startPos_l_x -= 1;
		m_endPos_l_x = m_startPos_l_x + edit_frame_length / scale;
	}

	else
	{
		m_startPos_l_x = 0;
		m_endPos_l_x = m_startPos_l_x + edit_frame_length / scale;
	}
	CString aaa = L"成功";
	edit_frame_left_x = m_startPos_l_x * scale;
	edit_frame_left_y = m_startPos_l_y * scale;
	CInfoFile file;
	file.WirteEditFrameLocation(aaa, edit_frame_left_x, edit_frame_left_y, edit_frame_right_x, edit_frame_right_y);

	CBitmap bitmap_l;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp_l;    // 保存CBitmap加载的位图的句柄
	hBmp_l = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/left.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_left.SetBitmap(hBmp_l);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
	CWnd* pwnd = GetDlgItem(IDC_VS_PIC_LEFT);
	CClientDC dc(pwnd);
	dc.SelectObject(&pen);//选择画笔
	POINT m_startPos_l = { m_startPos_l_x,m_startPos_l_y };
	POINT m_endPos_l = { m_endPos_l_x,m_endPos_l_y };
	dc.MoveTo(m_startPos_l);
	dc.LineTo(m_startPos_l.x, m_endPos_l.y);
	dc.LineTo(m_endPos_l.x, m_endPos_l.y);
	dc.LineTo(m_endPos_l.x, m_startPos_l.y);
	dc.LineTo(m_startPos_l.x, m_startPos_l.y);
}


void CvisionDlg::OnVsHotkeyLeftRight()
{
	// TODO: 在此添加命令处理程序代码

	if ((m_endPos_l_x + 1) < (all_width / scale))
	{
		m_endPos_l_x += 1;
		m_startPos_l_x = m_endPos_l_x - edit_frame_length / scale;
	}

	else
	{
		m_endPos_l_x = all_width / scale;
		m_startPos_l_x = m_endPos_l_x - edit_frame_length / scale;
	}
	CString aaa = L"成功";
	edit_frame_left_x = m_startPos_l_x * scale;
	edit_frame_left_y = m_startPos_l_y * scale;
	CInfoFile file;
	file.WirteEditFrameLocation(aaa, edit_frame_left_x, edit_frame_left_y, edit_frame_right_x, edit_frame_right_y);
	CBitmap bitmap_l;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp_l;    // 保存CBitmap加载的位图的句柄
	hBmp_l = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/left.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_left.SetBitmap(hBmp_l);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
	CWnd* pwnd = GetDlgItem(IDC_VS_PIC_LEFT);
	CClientDC dc(pwnd);
	dc.SelectObject(&pen);//选择画笔
	POINT m_startPos_l = { m_startPos_l_x,m_startPos_l_y };
	POINT m_endPos_l = { m_endPos_l_x,m_endPos_l_y };
	dc.MoveTo(m_startPos_l);
	dc.LineTo(m_startPos_l.x, m_endPos_l.y);
	dc.LineTo(m_endPos_l.x, m_endPos_l.y);
	dc.LineTo(m_endPos_l.x, m_startPos_l.y);
	dc.LineTo(m_startPos_l.x, m_startPos_l.y);
}


void CvisionDlg::OnVsHotkeyLeftUp()
{
	// TODO: 在此添加命令处理程序代码
	if ((m_startPos_l_y - 1) > 0)
	{
		m_startPos_l_y -= 1;
		m_endPos_l_y = m_startPos_l_y + edit_frame_width / scale;
	}

	else
	{
		m_startPos_l_y -= 0;
		m_endPos_l_y = m_startPos_l_y + edit_frame_width / scale;
	}
	CString aaa = L"成功";
	edit_frame_left_x = m_startPos_l_x * scale;
	edit_frame_left_y = m_startPos_l_y * scale;
	CInfoFile file;
	file.WirteEditFrameLocation(aaa, edit_frame_left_x, edit_frame_left_y, edit_frame_right_x, edit_frame_right_y);
	CBitmap bitmap_l;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp_l;    // 保存CBitmap加载的位图的句柄
	hBmp_l = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/left.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_left.SetBitmap(hBmp_l);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
	CWnd* pwnd = GetDlgItem(IDC_VS_PIC_LEFT);
	CClientDC dc(pwnd);
	dc.SelectObject(&pen);//选择画笔
	POINT m_startPos_l = { m_startPos_l_x,m_startPos_l_y };
	POINT m_endPos_l = { m_endPos_l_x,m_endPos_l_y };
	dc.MoveTo(m_startPos_l);
	dc.LineTo(m_startPos_l.x, m_endPos_l.y);
	dc.LineTo(m_endPos_l.x, m_endPos_l.y);
	dc.LineTo(m_endPos_l.x, m_startPos_l.y);
	dc.LineTo(m_startPos_l.x, m_startPos_l.y);
}


void CvisionDlg::OnVsHotkeyRightDown()
{
	// TODO: 在此添加命令处理程序代码
	if ((m_endPos_r_y + 1) < (all_height / scale))
	{
		m_endPos_r_y += 1;
		m_startPos_r_y = m_endPos_r_y - edit_frame_width / scale;
	}

	else
	{
		m_endPos_r_y = all_height / scale;
		m_startPos_r_y = m_endPos_r_y - edit_frame_width / scale;
	}
	CString aaa = L"成功";
	edit_frame_right_x = m_startPos_r_x * scale;
	edit_frame_right_y = m_startPos_r_y * scale;
	CInfoFile file;
	file.WirteEditFrameLocation(aaa, edit_frame_left_x, edit_frame_left_y, edit_frame_right_x, edit_frame_right_y);
	CBitmap bitmap_r;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp_r;    // 保存CBitmap加载的位图的句柄
	hBmp_r = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/right.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_right.SetBitmap(hBmp_r);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
	CWnd* pwnd = GetDlgItem(IDC_VS_PIC_RIGHT);
	CClientDC dc(pwnd);
	dc.SelectObject(&pen);//选择画笔
	POINT m_startPos_r = { m_startPos_r_x,m_startPos_r_y };
	POINT m_endPos_r = { m_endPos_r_x,m_endPos_r_y };
	dc.MoveTo(m_startPos_r);
	dc.LineTo(m_startPos_r.x, m_endPos_r.y);
	dc.LineTo(m_endPos_r.x, m_endPos_r.y);
	dc.LineTo(m_endPos_r.x, m_startPos_r.y);
	dc.LineTo(m_startPos_r.x, m_startPos_r.y);
}


void CvisionDlg::OnVsHotkeyRightLeft()
{
	// TODO: 在此添加命令处理程序代码

	if ((m_startPos_r_x - 1) > 0)
	{
		m_startPos_r_x -= 1;
		m_endPos_r_x = m_startPos_r_x + edit_frame_length / scale;
	}

	else
	{
		m_startPos_r_x = 0;
		m_endPos_r_x = m_startPos_r_x + edit_frame_length / scale;
	}
	CString aaa = L"成功";
	edit_frame_right_x = m_startPos_r_x * scale;
	edit_frame_right_y = m_startPos_r_y * scale;
	CInfoFile file;
	file.WirteEditFrameLocation(aaa, edit_frame_left_x, edit_frame_left_y, edit_frame_right_x, edit_frame_right_y);
	// TODO: 在此添加命令处理程序代码
	CBitmap bitmap_r;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp_r;    // 保存CBitmap加载的位图的句柄
	hBmp_r = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/right.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_right.SetBitmap(hBmp_r);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
	CWnd* pwnd = GetDlgItem(IDC_VS_PIC_RIGHT);
	CClientDC dc(pwnd);
	dc.SelectObject(&pen);//选择画笔
	POINT m_startPos_r = { m_startPos_r_x,m_startPos_r_y };
	POINT m_endPos_r = { m_endPos_r_x,m_endPos_r_y };
	dc.MoveTo(m_startPos_r);
	dc.LineTo(m_startPos_r.x, m_endPos_r.y);
	dc.LineTo(m_endPos_r.x, m_endPos_r.y);
	dc.LineTo(m_endPos_r.x, m_startPos_r.y);
	dc.LineTo(m_startPos_r.x, m_startPos_r.y);
}


void CvisionDlg::OnVsHotkeyRightRight()
{
	// TODO: 在此添加命令处理程序代码
	if ((m_endPos_r_x + 1) < (all_width / scale))
	{
		m_endPos_r_x += 1;
		m_startPos_r_x = m_endPos_r_x - edit_frame_length / scale;
	}

	else
	{
		m_endPos_r_x = all_width / scale;
		m_startPos_r_x = m_endPos_r_x - edit_frame_length / scale;
	}
	CString aaa = L"成功";
	edit_frame_right_x = m_startPos_r_x * scale;
	edit_frame_right_y = m_startPos_r_y * scale;
	CInfoFile file;
	file.WirteEditFrameLocation(aaa, edit_frame_left_x, edit_frame_left_y, edit_frame_right_x, edit_frame_right_y);
	CBitmap bitmap_r;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp_r;    // 保存CBitmap加载的位图的句柄
	hBmp_r = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/right.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_right.SetBitmap(hBmp_r);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
	CWnd* pwnd = GetDlgItem(IDC_VS_PIC_RIGHT);
	CClientDC dc(pwnd);
	dc.SelectObject(&pen);//选择画笔
	POINT m_startPos_r = { m_startPos_r_x,m_startPos_r_y };
	POINT m_endPos_r = { m_endPos_r_x,m_endPos_r_y };
	dc.MoveTo(m_startPos_r);
	dc.LineTo(m_startPos_r.x, m_endPos_r.y);
	dc.LineTo(m_endPos_r.x, m_endPos_r.y);
	dc.LineTo(m_endPos_r.x, m_startPos_r.y);
	dc.LineTo(m_startPos_r.x, m_startPos_r.y);
}


void CvisionDlg::OnVsHotkeyRightUp()
{
	// TODO: 在此添加命令处理程序代码
	if ((m_startPos_r_y - 1) > 0)
	{
		m_startPos_r_y -= 1;
		m_endPos_r_y = m_startPos_r_y + edit_frame_width / scale;
	}

	else
	{
		m_startPos_r_y -= 0;
		m_endPos_r_y = m_startPos_r_y + edit_frame_width / scale;
	}
	CString aaa = L"成功";
	edit_frame_right_x = m_startPos_r_x * scale;
	edit_frame_right_y = m_startPos_r_y * scale;
	CInfoFile file;
	file.WirteEditFrameLocation(aaa, edit_frame_left_x, edit_frame_left_y, edit_frame_right_x, edit_frame_right_y);

	CBitmap bitmap_r;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp_r;    // 保存CBitmap加载的位图的句柄
	hBmp_r = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/right.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_right.SetBitmap(hBmp_r);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
	CWnd* pwnd = GetDlgItem(IDC_VS_PIC_RIGHT);
	CClientDC dc(pwnd);
	dc.SelectObject(&pen);//选择画笔
	POINT m_startPos_r = { m_startPos_r_x,m_startPos_r_y };
	POINT m_endPos_r = { m_endPos_r_x,m_endPos_r_y };
	dc.MoveTo(m_startPos_r);
	dc.LineTo(m_startPos_r.x, m_endPos_r.y);
	dc.LineTo(m_endPos_r.x, m_endPos_r.y);
	dc.LineTo(m_endPos_r.x, m_startPos_r.y);
	dc.LineTo(m_startPos_r.x, m_startPos_r.y);
}


void CvisionDlg::OnBnClickedVisBtnOpvs()
{
	// TODO: 在此添加控件通知处理程序代码
}




BOOL CvisionDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnCloseCam();
	return CDialogEx::DestroyWindow();
}


void CvisionDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnCloseCam();
	CDialogEx::PostNcDestroy();
}


void CvisionDlg::Show_edit_frame()
{
	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));//创建一个画笔工具
	CWnd* pwnd = GetDlgItem(IDC_VS_PIC_RIGHT);
	CClientDC dc(pwnd);
	dc.SelectObject(&pen);//选择画笔
	POINT m_startPos_r = { m_startPos_r_x,m_startPos_r_y };
	POINT m_endPos_r = { m_endPos_r_x,m_endPos_r_y };
	dc.MoveTo(m_startPos_r);
	dc.LineTo(m_startPos_r.x, m_endPos_r.y);
	dc.LineTo(m_endPos_r.x, m_endPos_r.y);
	dc.LineTo(m_endPos_r.x, m_startPos_r.y);
	dc.LineTo(m_startPos_r.x, m_startPos_r.y);

	CWnd* pwndd = GetDlgItem(IDC_VS_PIC_LEFT);
	CClientDC dcc(pwndd);
	dcc.SelectObject(&pen);//选择画笔
	POINT m_startPos_l = { m_startPos_l_x,m_startPos_l_y };
	POINT m_endPos_l = { m_endPos_l_x,m_endPos_l_y };
	dcc.MoveTo(m_startPos_l);
	dcc.LineTo(m_startPos_l.x, m_endPos_l.y);
	dcc.LineTo(m_endPos_l.x, m_endPos_l.y);
	dcc.LineTo(m_endPos_l.x, m_startPos_l.y);
	dcc.LineTo(m_startPos_l.x, m_startPos_l.y);
}


//用Cpen在图片控件上画直线在切换对话框时会被擦除,此函数用来解决这个问题,使得画的线在下一次覆盖显示之前不会被擦除
void CvisionDlg::OnLeftBmpSaveAndShow()
{
	CWnd* pwnd = GetDlgItem(IDC_VS_PIC_LEFT);
	CClientDC dc(pwnd);
	CRect rect;
	GetDlgItem(IDC_VS_PIC_LEFT)->GetClientRect(rect);
	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);
	HDC hdc = CreateCompatibleDC(dc);
	HBITMAP holdmap = (HBITMAP)SelectObject(hdc, hbitmap);
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);
	CImage image;
	image.Attach(hbitmap);
	CString strFileName;

	//strFileName = "left_show.bmp";
	strFileName = "D://实际屏幕版/HX/HX/left_show.bmp";
	HRESULT hsesult = image.Save(strFileName);
	image.Detach();
	SelectObject(hdc, holdmap);

	CBitmap bitmap;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄
	hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/left_show.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_left.SetBitmap(hBmp);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  
	DeleteObject(hBmp);
	
}

//用Cpen在图片控件上画直线在切换对话框时会被擦除,此函数用来解决这个问题,使得画的线在下一次覆盖显示之前不会被擦除
void CvisionDlg::OnRightBmpSaveAndShow()
{
	CWnd* pwnd = GetDlgItem(IDC_VS_PIC_RIGHT);
	CClientDC dc(pwnd);
	CRect rect;
	GetDlgItem(IDC_VS_PIC_RIGHT)->GetClientRect(rect);
	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);
	HDC hdc = CreateCompatibleDC(dc);
	HBITMAP holdmap = (HBITMAP)SelectObject(hdc, hbitmap);
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);
	CImage image;
	image.Attach(hbitmap);
	CString strFileName;
	strFileName = "D://实际屏幕版/HX/HX/right_show.bmp";
	HRESULT hsesult = image.Save(strFileName);
	image.Detach();
	SelectObject(hdc, holdmap);

	CBitmap bitmap;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄
	hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"D://实际屏幕版/HX/HX/right_show.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_right.SetBitmap(hBmp);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  
	DeleteObject(hBmp);
}
