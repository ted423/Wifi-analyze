
// wlanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "wlan.h"
#include "wlanDlg.h"
#include "afxdialogex.h"
#include "Wlanapi.h"
#include <direct.h>
#include <io.h>
#pragma comment(lib, "Wlanapi.lib") 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CwlanDlg 对话框




CwlanDlg::CwlanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CwlanDlg::IDD, pParent)
{

	char *temp;
	size_t len;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_quantity = 0;
	m_State = _T("");
	m_connectedssid = _T("");
	m_connectmac = _T("");
	m_macquantity = 0;
	m_phy = _T("");
	m_encryption = _T("");
	m_connectencryption = _T("");
	m_tixing = _T("");
	errno_t err = _dupenv_s(&temp, &len, "APPDATA");
	if (err) exit(-1);
	Cpath = temp;
	Cpath += "\\Wifi-analyze";
	free(temp);
}

void CwlanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_quantity);
	DDV_MinMaxInt(pDX, m_quantity, 0, 65525);
	DDX_Control(pDX, IDC_TREE1, m_display);
	DDX_Text(pDX, IDC_EDIT2, m_State);
	DDX_Text(pDX, IDC_EDIT3, m_connectedssid);
	DDX_Text(pDX, IDC_EDIT4, m_connectmac);
	DDX_Text(pDX, IDC_EDIT5, m_macquantity);
	DDV_MinMaxInt(pDX, m_macquantity, 0, 65535);
	DDX_Text(pDX, IDC_EDIT6, m_phy);
	DDX_Text(pDX, IDC_EDIT7, m_encryption);
	DDX_Text(pDX, IDC_EDIT8, m_connectencryption);
	DDX_Control(pDX, IDC_TREE2, m_HistoryInformation);
}

BEGIN_MESSAGE_MAP(CwlanDlg, CDialogEx)

	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CwlanDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CwlanDlg::OnBnClickedButton2)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CwlanDlg 消息处理程序

BOOL CwlanDlg::OnInitDialog()
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
	CwlanDlg::load();
	CwlanDlg::wifiquantity();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE

}

void CwlanDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CwlanDlg::OnPaint()
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
HCURSOR CwlanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CwlanDlg::wifiquantity()
{
	UpdateData(TRUE);
	m_display.DeleteAllItems();
	DWORD dwError = ERROR_SUCCESS;
	DWORD dwNegotiatedVersion;
	DWORD dwResult = 0;
	HANDLE hClientHandle = NULL;
	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
	DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;
	PWLAN_INTERFACE_INFO_LIST pInterfaceList = NULL;
	PWLAN_BSS_LIST pWlanBssList = NULL;
	DOT11_BSS_TYPE dot11BssType = dot11_BSS_type_any;
	m_State = "未连接";
	m_connectedssid = "";
	m_connectmac = "";
	m_quantity = 0;
	m_macquantity = 0;
	m_phy = "";
	m_encryption = "";
	m_connectencryption = "";
	bool disconnect = 0;
	dwError = WlanOpenHandle(WLAN_API_VERSION_1_0, NULL, &dwNegotiatedVersion, &hClientHandle);
	if (dwError != ERROR_SUCCESS)
		return;
	dwError = WlanEnumInterfaces(hClientHandle, NULL, &pInterfaceList);
	if (dwError != ERROR_SUCCESS)
	{
		WlanFreeMemory(pInterfaceList);
		return;
	}
	if (pInterfaceList[0].InterfaceInfo[0].isState == wlan_interface_state_connected) {
		dwResult = WlanQueryInterface(hClientHandle,
			&pInterfaceList[0].InterfaceInfo[0].InterfaceGuid,
			wlan_intf_opcode_current_connection,
			NULL,
			&connectInfoSize,
			(PVOID *)&pConnectInfo,
			&opCode);

		if (dwResult != ERROR_SUCCESS) {
			WlanFreeMemory(pConnectInfo);
			WlanFreeMemory(pInterfaceList);
			return;
			// You can use FormatMessage to find out why the function failed
		}
		else {

			switch (pConnectInfo->isState) {
			case wlan_interface_state_not_ready:
				m_State = "Not ready";
				break;
			case wlan_interface_state_connected:
				m_State = "Connected";
				m_connectedssid = pConnectInfo[0].strProfileName;
				m_connectmac.Format(_T("%.2X:%.2X:%.2X:%.2X:%.2X:%.2X"), pConnectInfo[0].wlanAssociationAttributes.dot11Bssid[0], pConnectInfo[0].wlanAssociationAttributes.dot11Bssid[1], pConnectInfo[0].wlanAssociationAttributes.dot11Bssid[2], pConnectInfo[0].wlanAssociationAttributes.dot11Bssid[3], pConnectInfo[0].wlanAssociationAttributes.dot11Bssid[4], pConnectInfo[0].wlanAssociationAttributes.dot11Bssid[5]);

				break;
			case wlan_interface_state_ad_hoc_network_formed:
				m_State = "First node in a ad hoc network";
				break;
			case wlan_interface_state_disconnecting:
				m_State = "Disconnecting";
				break;
			case wlan_interface_state_disconnected:
				m_State = "Not connected";
				break;
			case wlan_interface_state_associating:
				m_State = "Attempting to associate with a network";
				break;
			case wlan_interface_state_discovering:
				m_State = "Auto configuration is discovering settings for the network";
				break;
			case wlan_interface_state_authenticating:
				m_State = "In process of authenticating";
				break;
			default:
				m_State.Format(_T("Unknown state %ld"), pInterfaceList[0].InterfaceInfo[0].isState);
				break;
			}
		}
	}
	if (pInterfaceList[0].InterfaceInfo[0].isState == wlan_interface_state_connected){
		switch (pConnectInfo[0].wlanSecurityAttributes.dot11CipherAlgorithm){
		case  DOT11_CIPHER_ALGO_NONE:
			m_encryption = "未加密";
			break;
		case DOT11_CIPHER_ALGO_WEP40:
			m_encryption = "WEP40";
			break;
		case DOT11_CIPHER_ALGO_TKIP:
			m_encryption = "TKIP";
			break;
		case DOT11_CIPHER_ALGO_CCMP:
			m_encryption = "CCMP";
			break;
		case DOT11_CIPHER_ALGO_WEP104:
			m_encryption = "WEP104";
			break;
		case DOT11_CIPHER_ALGO_WPA_USE_GROUP:
			m_encryption = "WPA";
			break;
		case DOT11_CIPHER_ALGO_WEP:
			m_encryption = "WEP";
			break;
		default:
			m_encryption.Format(_T("Other (%lu)"), pConnectInfo[0].wlanSecurityAttributes.dot11CipherAlgorithm);
			break;
		}
		switch (pConnectInfo[0].wlanSecurityAttributes.dot11AuthAlgorithm){
		case DOT11_AUTH_ALGO_80211_OPEN:
			m_connectencryption = "未加密";
			break;
		case DOT11_AUTH_ALGO_80211_SHARED_KEY:
			m_connectencryption = "SHARED_KEY";
			break;
		case DOT11_AUTH_ALGO_WPA:
			m_connectencryption = "WPA";
			break;
		case DOT11_AUTH_ALGO_WPA_PSK:
			m_connectencryption = "WPA_PSK";
			break;
		case DOT11_AUTH_ALGO_WPA_NONE:
			m_connectencryption = "WPA_NONE";
			break;
		case DOT11_AUTH_ALGO_RSNA:
			m_connectencryption = "WPA2";
			break;
		case DOT11_AUTH_ALGO_RSNA_PSK:
			m_connectencryption = "WPA2_PSK";
			break;
		default:
			m_connectencryption.Format(_T("Other (%lu)"), pConnectInfo[0].wlanSecurityAttributes.dot11AuthAlgorithm);
			break;
		}
		switch (pConnectInfo[0].wlanAssociationAttributes.dot11PhyType){
		case dot11_phy_type_fhss:
			m_phy = "Frequency-hopping spread-spectrum (FHSS)";
			break;
		case dot11_phy_type_dsss:
			m_phy = "Direct sequence spread spectrum (DSSS)";
			break;
		case dot11_phy_type_irbaseband:
			m_phy = "Infrared (IR) baseband";
			break;
		case dot11_phy_type_ofdm:
			m_phy = "Orthogonal frequency division multiplexing (OFDM)";
			break;
		case dot11_phy_type_hrdsss:
			m_phy = "High-rate DSSS (HRDSSS) ";
			break;
		case dot11_phy_type_erp:
			m_phy = "Extended rate PHY type";
			break;
		case dot11_phy_type_ht:
			m_phy = "802.11n PHY type";
			break;
		default:
			m_phy = "Unknown ";
			break;

		}
	}
	GUID& guid = pInterfaceList->InterfaceInfo[0].InterfaceGuid;
	PWLAN_AVAILABLE_NETWORK_LIST pWLAN_AVAILABLE_NETWORK_LIST = NULL;
	dwError = WlanGetAvailableNetworkList(hClientHandle, &guid,
		WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_MANUAL_HIDDEN_PROFILES, NULL,
		&pWLAN_AVAILABLE_NETWORK_LIST);
	if (dwError != ERROR_SUCCESS)
	{
		WlanFreeMemory(pConnectInfo);
		WlanFreeMemory(pInterfaceList);
		WlanFreeMemory(pWLAN_AVAILABLE_NETWORK_LIST);
		return;
	}

	dwError = WlanGetNetworkBssList(hClientHandle, &pInterfaceList[0].InterfaceInfo[0].InterfaceGuid,
		NULL, dot11BssType, NULL,
		NULL, &pWlanBssList);
	if (dwError != ERROR_SUCCESS)
	{
		WlanFreeMemory(pConnectInfo);
		WlanFreeMemory(pInterfaceList);
		WlanFreeMemory(pWLAN_AVAILABLE_NETWORK_LIST);
		WlanFreeMemory(pWlanBssList);

		return;
	}
	m_quantity = pWLAN_AVAILABLE_NETWORK_LIST->dwNumberOfItems;
	m_macquantity = pWlanBssList->dwNumberOfItems;
	HTREEITEM hItem, hSubItem;
	int i = 0;
	CString temp;
	for (i = 0; i < m_quantity; i++)
	{
		int a = 0;

		temp = pWLAN_AVAILABLE_NETWORK_LIST[0].Network[i].dot11Ssid.ucSSID;
		a = pWLAN_AVAILABLE_NETWORK_LIST[0].Network[i].wlanSignalQuality;
		temp.Format(temp + "（-%d）", a);
		if (pWLAN_AVAILABLE_NETWORK_LIST[0].Network[i].bSecurityEnabled == 0)
		{
			temp = _T("[未加密]" + temp);
		}
		if (pWLAN_AVAILABLE_NETWORK_LIST[0].Network[i].bSecurityEnabled == 1)
		{
			switch (pWLAN_AVAILABLE_NETWORK_LIST[0].Network[i].dot11DefaultAuthAlgorithm) {

			case DOT11_AUTH_ALGO_80211_SHARED_KEY:
				temp = _T("[SHARED_KEY]" + temp);
				break;
			case DOT11_AUTH_ALGO_WPA:
				temp = _T("[WPA]" + temp);
				break;
			case DOT11_AUTH_ALGO_WPA_PSK:
				temp = _T("[WPA_PSK]" + temp);
				break;
			case DOT11_AUTH_ALGO_WPA_NONE:
				temp = _T("[WPA_NONE]" + temp);
				break;
			case DOT11_AUTH_ALGO_RSNA:
				temp = _T("[WPA2]" + temp);
				break;
			case DOT11_AUTH_ALGO_RSNA_PSK:
				temp = _T("[WPA2_PSK]" + temp);
				break;
			default:
				temp.Format(_T("[Other (%lu)]" + temp), pWLAN_AVAILABLE_NETWORK_LIST[0].Network[i].dot11DefaultAuthAlgorithm);
				break;
			}



		}
		hItem = m_display.InsertItem(temp, TVI_ROOT);
		temp = "";
		switch (pWLAN_AVAILABLE_NETWORK_LIST[0].Network[i].dot11PhyTypes[0]){
		case dot11_phy_type_fhss:
			temp = "Frequency-hopping spread-spectrum (FHSS)";
			break;
		case dot11_phy_type_dsss:
			temp = "Direct sequence spread spectrum (DSSS)";
			break;
		case dot11_phy_type_irbaseband:
			temp = "Infrared (IR) baseband";
			break;
		case dot11_phy_type_ofdm:
			temp = "Orthogonal frequency division multiplexing (OFDM)";
			break;
		case dot11_phy_type_hrdsss:
			temp = "High-rate DSSS (HRDSSS) ";
			break;
		case dot11_phy_type_erp:
			temp = "Extended rate PHY type";
			break;
		case dot11_phy_type_ht:
			temp = "802.11n PHY type";
			break;
		default:
			temp = "Unknown ";
			break;

		}
		hSubItem = m_display.InsertItem(temp, hItem);
		temp = "";

		a = pWLAN_AVAILABLE_NETWORK_LIST[0].Network[i].uNumberOfBssids;

		temp.Format(_T("BSSID数量：%d"), a);
		hSubItem = m_display.InsertItem(temp, hItem);
		for (unsigned int j = 0, k = 0; j < pWlanBssList[0].dwNumberOfItems; j++)
		{
			CString compare1, compare2;
			compare1 = pWlanBssList[0].wlanBssEntries[j].dot11Ssid.ucSSID;
			compare2 = pWLAN_AVAILABLE_NETWORK_LIST[0].Network[i].dot11Ssid.ucSSID;
			if (compare1 == compare2)
			{

				CString temp1;
				temp1.Format(_T("%.2X:%.2X:%.2X:%.2X:%.2X:%.2X"), pWlanBssList[0].wlanBssEntries[j].dot11Bssid[0], pWlanBssList[0].wlanBssEntries[j].dot11Bssid[1], pWlanBssList[0].wlanBssEntries[j].dot11Bssid[2], pWlanBssList[0].wlanBssEntries[j].dot11Bssid[3], pWlanBssList[0].wlanBssEntries[j].dot11Bssid[4], pWlanBssList[0].wlanBssEntries[j].dot11Bssid[5]);

				hItem = m_display.InsertItem(temp1, hSubItem);
			}
		}
	}


	WlanFreeMemory(pConnectInfo);
	WlanFreeMemory(pInterfaceList);
	WlanFreeMemory(pWLAN_AVAILABLE_NETWORK_LIST);
	WlanFreeMemory(pWlanBssList);


	WlanCloseHandle(hClientHandle, NULL);
	UpdateData(FALSE);



}
void CwlanDlg::disconnected()
{

	DWORD dwError = ERROR_SUCCESS;
	DWORD dwNegotiatedVersion;
	HANDLE hClientHandle = NULL;
	dwError = WlanOpenHandle(WLAN_API_VERSION_1_0, NULL, &dwNegotiatedVersion, &hClientHandle);
	if (dwError != ERROR_SUCCESS)
		return;
	PWLAN_INTERFACE_INFO_LIST pInterfaceList = NULL;
	dwError = WlanEnumInterfaces(hClientHandle, NULL, &pInterfaceList);
	if (dwError != ERROR_SUCCESS)
	{
		WlanFreeMemory(pInterfaceList);
		return;
	}
	GUID& guid = pInterfaceList->InterfaceInfo[0].InterfaceGuid;
	dwError = WlanDisconnect(hClientHandle, &guid, NULL);
	WlanCloseHandle(hClientHandle, NULL);
}
void CwlanDlg::OnBnClickedButton1()
{
	CwlanDlg::wifiquantity();
	SetTimer(1, 5000, NULL);
}


void CwlanDlg::OnBnClickedButton2()
{
	CwlanDlg::disconnected();
}

void CwlanDlg::load()
{
	UpdateData(TRUE);

	int i = _access_s((char *)(LPCTSTR)Cpath, 0);
	if (i == 0){
		CFile file(Cpath + "\\ssid.dat", CFile::modeReadWrite);
		CFile mfile(Cpath + "\\mac.dat", CFile::modeReadWrite);
		char pbuf[999];
		memset(pbuf, 0, sizeof(pbuf));
		file.Read(pbuf, 999);
		int i = 0;
		HTREEITEM hItem;
		CString temp;
		hItem = m_HistoryInformation.GetRootItem();
		while (pbuf[i])
		{
			if (pbuf[i] != 59){
				temp += pbuf[i];
			}
			else if (temp != ""){
				hItem = m_HistoryInformation.InsertItem(temp, TVI_ROOT);
				temp = "";
			}
			i++;

		}
		memset(pbuf, 0, sizeof(pbuf));
		mfile.Read(pbuf, 999);
		i = 0;
		hItem = m_HistoryInformation.GetRootItem();
		while (pbuf[i])
		{
			if (pbuf[i] != 59){
				temp += pbuf[i];
			}
			else if (temp != "")
			{

				hItem = m_HistoryInformation.InsertItem(temp, TVI_ROOT);
				temp = "";
			}
			i++;

		}
	}
	UpdateData(false);
}



void CwlanDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 700;
	lpMMI->ptMinTrackSize.y = 400;
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}
