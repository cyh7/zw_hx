#pragma once
#include "CmodbusDlg.h"
#include "CLayout.h"
#include "CMyButton.h"

extern bool IdentifyDone;
//把时间全局给插入数据库用
extern CString LastTime;
extern double vs_x;
extern double vs_y;
extern double vs_theta;
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


};
