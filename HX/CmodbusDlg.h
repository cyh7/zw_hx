#pragma once
//About CSerialPort start
#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"
#include "CLayout.h"
#include "CMyButton.h"

using namespace itas109;



extern int SendGlueNum;

extern bool RecMsgFlag;

extern long m_CadT1;
extern long m_CadT2;
extern bool OverTime;

extern bool SendOnce;
extern bool SendOnce_Vision;

extern long m_Vision_T1;
extern long m_Vision_T2;
extern bool OverTime_Vision;

extern long m_Status_T1;
extern long m_Status_T2;

//判断背板是否到达
extern bool ArriveFlag;
//设置界面
//背板型号
extern CString backboard;
//X上下限 Y上下限 THETA上下限
extern double x_floor;
extern double x_ceil;
extern double y_floor;
extern double y_ceil;
extern double theta_floor;
extern double theta_ceil;
//良品
extern CString data_good;
//胶机状态 false没有停机 true停机
extern bool SprayFlag;
//喷涂批次
extern DWORD SprayBatch;
//PLC正常
extern bool PlcFlag;
//急停标志位
extern bool StopFlag;
//通信状态
extern bool DisconnectFlag;
extern int DisconnectNum;
//防止识别完成后重复插入，识别完置0；插入完成置1
extern int insertdata;


// CmodbusDlg 对话框

class CmodbusDlg : public CDialogEx , public has_slots<>//About CSerialPort
{
	DECLARE_DYNAMIC(CmodbusDlg)

public:
	CmodbusDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CmodbusDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODBUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	 
	CString m_EditSend;
	CComboBox m_comb1;
	CComboBox m_comb2;
	CComboBox m_comb3;
	CComboBox m_comb4;
	CComboBox m_comb5;

private:
	CString m_EditReceive;
public:
	afx_msg void OnBnClickedButtonOpen();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSendOnce();
	unsigned short CRC16(unsigned char* puchMsg, unsigned short usDataLen);
	char HexChar(char c);
	void StringtoHex(BYTE * GB, int glen, BYTE* SB, int* slen);
	void OnReceive();
	CMyButton m_OpenCloseCtrl;
	void SendData(int CommTypeIn, WORD DownAdd, DWORD DownData);
	afx_msg void OnBnClickedButton1();
	void ReadArrive();
	void SendGlueData();
	
private:
	/*CListCtrl m_GlueList;
	CListCtrl m_IdentifyList;*/
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_EditReceiveCtrl;
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	
	
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonClean();

	CSerialPort m_SerialPort;//About CSerialPort
	static CmodbusDlg *pModbusdlg;//给别的窗口调用本窗口里的变量用的
	afx_msg void OnPaint();
	BOOL PreTranslateMessage(MSG* pMsg); //屏蔽按键
public:
	//界面绘制
	CBrush m_Brush;
	CMyButton m_btn_SendOnce;
	CMyButton m_btn_Clean;
	POINT old_mod;
	CLayout m_layoutMod;
	afx_msg void OnBnClickedModBtnOpcad();
	afx_msg void OnBnClickedModBtnOpdata();
	afx_msg void OnBnClickedModBtnOpvs();
	CMyButton m_mod_btn_opmon;
	CMyButton m_mod_btn_opvs;
	CMyButton m_mod_btn_opcad;
	CMyButton m_mod_btn_opdata;
	CString m_mod_type;
	afx_msg void OnBnClickedModBtnChange();
	CMyButton m_mod_btn_change;
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnClickedModBtnOpmon();
	CMyButton m_mod_btn_opmod;
	void JudgeStatus();
	// x下限
	double m_mod_edit_xfloor;
	// y下限
	double m_mod_edit_yfloor;
	double m_mod_edit_thetafloor;
	double m_mod_edit_xceil;
	double m_mod_edit_yceil;
	double m_mod_edit_thetaceil;
};
