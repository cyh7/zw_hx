#pragma once
#include "CMyButton.h"

// CMonitorDlg 对话框

class CMonitorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorDlg)

public:
	CMonitorDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMonitorDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MONITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HICON m_hIconRed;
	HICON m_hIconBlue;
	HICON m_hIconYellow;
	HICON m_hIconGray;
	virtual BOOL OnInitDialog();
	CStatic m_mon_pic_plc;
	CStatic m_mon_pic_glue;
	CStatic m_mon_pic_stop;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	CBrush m_Brush;
	CLayout m_layoutMonitor;

	//HBITMAP m_hBit;
	afx_msg void OnBnClickedMonBtnOpvs();
	afx_msg void OnBnClickedMonBtnOpcad();
	afx_msg void OnBnClickedMonBtnOpdata();
	afx_msg void OnBnClickedMonBtnOpmod();
	CMyButton m_mon_btn_opvs;
	CMyButton m_mon_btn_opcad;
	CMyButton m_mon_btn_opdata;
	CMyButton m_mon_btn_opmod;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 背板型号
	CString m_mon_edit_type;
	DWORD m_mon_edit_batch;
	DOUBLE m_mon_edit_x;
	DOUBLE m_mon_edit_y;
	DOUBLE m_mon_edit_theta;
	CMyButton m_mon_btn_opmon;

	HBITMAP m_mon_hBitmapMain;
	CStatic m_mon_pic_main;
};
