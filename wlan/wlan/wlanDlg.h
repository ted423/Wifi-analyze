
// wlanDlg.h : 头文件
//

#pragma once


// CwlanDlg 对话框
class CwlanDlg : public CDialogEx
{
// 构造
public:
	CwlanDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WLAN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void wifiquantity();
	afx_msg void disconnected();
	afx_msg void load();
	int m_quantity;
	CTreeCtrl m_display;
	CString m_State;
	afx_msg void OnBnClickedButton2();
	CString m_connectedssid;
	CString m_connectmac;
	int m_macquantity;
	CString m_phy;
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_encryption;
	CString m_connectencryption;
	CString m_tixing;
	BOOL m_disconnect;
	CTreeCtrl m_mistrustssid;
	afx_msg void OnBnClickedButton5();
	CString m_unssid;
	CTreeCtrl m_mistrustmac;
	CString m_unmac;

	afx_msg void OnBnClickedButton6();
	CString path;
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
