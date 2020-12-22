#pragma once

//#include "CSerialPort/SerialPort.h"
//#include "CSerialPort/SerialPortInfo.h"
//#include "CLayout.h"
#include "CmodbusDlg.h"
#include "CLayout.h"
#include "CMyButton.h"

//using namespace itas109;

// CcadDlg 对话框

class CcadDlg : public CDialogEx, public has_slots<>
{
	DECLARE_DYNAMIC(CcadDlg)

public:
	CcadDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CcadDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	virtual BOOL OnInitDialog();
	//CSerialPort m_Cad_SerialPort;//About CSerialPort 
	unsigned short CRC16(unsigned char* puchMsg, unsigned short usDataLen);
	void SendData(int CommTypeIn, WORD DownAdd, DWORD DownData);
	
	afx_msg void OnBnClickedButtonCadOpen();
	afx_msg void OnBnClickedButtonCadDraw();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	CListCtrl m_CadGlueList;
	
	afx_msg void OnBnClickedButtonCadSend();
	BOOL PreTranslateMessage(MSG* pMsg);

	CBrush m_Brush;
	afx_msg void OnPaint();
	CMyButton m_cad_btn_open;
	CMyButton m_cad_btn_draw;
	CMyButton m_cad_btn_send;
	POINT old_Cad;
	afx_msg void OnBnClickedCadBtnOpvs();
	afx_msg void OnBnClickedCadBtnOpmod();
	CLayout m_layoutCad;
	afx_msg void OnBnClickedCadBtnOpdata();
	CMyButton m_cad_btn_opmon;
	CMyButton m_cad_btn_opvs;
	CMyButton m_cad_btn_opdata;
	CMyButton m_cad_btn_opmod;
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnClickedCadBtnOpmon();
	CMyButton m_cad_btn_opcad;
	CStatic m_cad_pic_logo;
	HBITMAP m_cad_hBitmap_logo;

	CFont f_cad_font;
	CFont f_cad_name;
};
