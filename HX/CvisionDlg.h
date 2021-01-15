#pragma once
#include "CmodbusDlg.h"
#include "CLayout.h"
#include "CMyButton.h"
#include "Resource.h"

#define DATA_FOLDER "D://实际屏幕版/HX/HX/"
extern bool IdentifyDone;
//把时间全局给插入数据库用
extern CString LastTime;
extern double vs_x;
extern double vs_y;
extern double vs_theta;
extern int hv_Threshold;
extern int hv_Filter_block_radius;
extern int hv_closing_radius;

extern int edit_frame_length;
extern int edit_frame_width;

extern int edit_frame_left_x;
extern int edit_frame_left_y;
extern int edit_frame_right_x;
extern int edit_frame_right_y;

extern int baoguang_time;
extern int baoguang2_time;

// CvisionDlg 对话框

class CvisionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CvisionDlg)

public:
	CvisionDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CvisionDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VISION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	unsigned short CRC16(unsigned char* puchMsg, unsigned short usDataLen);
	void SendData(int CommTypeIn, WORD DownAdd, DWORD DownData);
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CLayout m_layoutVision;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	POINT old_Vision;
	afx_msg void OnBnClickedVisBtnOpcad();
	afx_msg void OnBnClickedVisBtnOpmod();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void OnOpenCam();
	void OnCloseCam();
	void OnCollectAndLocate();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnBnClickedVisBtnOpdata();
	CBrush m_Brush;
	CMyButton m_vs_btn_opmon;
	CMyButton m_vs_btn_opcad;
	CMyButton m_vs_btn_opdata;
	CMyButton m_vs_btn_opmod;
	afx_msg void OnPaint();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnClickedVisBtnOpmon();
	CMyButton m_vs_btn_opvs;

	CStatic m_vs_pic_logo;
	HBITMAP m_vs_hBitmap_logo;

	CFont f_vs_font;
	CFont f_vs_name;
	CMyButton m_vs_btn_resend;
	afx_msg void OnBnClickedVsBtnResend();
	// 停止定时器1
	void KillTime1();
	// 激活定时器1
	void ReSetTime();
	//void Show_edit_frame();
	static CvisionDlg *pVisiondlg;
	
	CString m_vs_edit_type;
	DWORD m_vs_edit_batch;

	//struct edit_frame_location;//定义结构体存储裁剪框位置
	//void put_in();//将裁剪框位置写入内存
	//void get_out();////从内存中读取裁剪框位置
	HACCEL m_hAccel;

	CStatic m_vs_pic_left;
	CStatic m_vs_pic_right;
	CListCtrl m_vs_list_location_data;
	afx_msg void OnVsHotkeyLeftDown();
	afx_msg void OnVsHotkeyLeftLeft();
	afx_msg void OnVsHotkeyLeftRight();
	afx_msg void OnVsHotkeyLeftUp();
	afx_msg void OnVsHotkeyRightDown();
	afx_msg void OnVsHotkeyRightLeft();
	afx_msg void OnVsHotkeyRightRight();
	afx_msg void OnVsHotkeyRightUp();
	afx_msg void OnBnClickedVisBtnOpvs();
	virtual BOOL DestroyWindow();
	virtual void PostNcDestroy();

	void Show_edit_frame();
	void Show_location_result();
	void OnLeftBmpSaveAndShow();
	void OnRightBmpSaveAndShow();
};
