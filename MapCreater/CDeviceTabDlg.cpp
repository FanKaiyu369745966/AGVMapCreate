// CDeviceTabDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CDeviceTabDlg.h"
#include "afxdialogex.h"


// CDeviceTabDlg 对话框

IMPLEMENT_DYNAMIC(CDeviceTabDlg, CDialogEx)

CDeviceTabDlg::CDeviceTabDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DEVICETAB_DIALOG, pParent)
{

}

CDeviceTabDlg::~CDeviceTabDlg()
{
}

void CDeviceTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_DEVICE, m_tabDevice);
}


BEGIN_MESSAGE_MAP(CDeviceTabDlg, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DEVICE, &CDeviceTabDlg::OnTcnSelchangeTabDevice)
END_MESSAGE_MAP()


// CDeviceTabDlg 消息处理程序
void CDeviceTabDlg::InitTab()
{
	m_tabDevice.InsertItem(TABDLG_AGV, _T("AGV信息"));
	m_tabDevice.InsertItem(TABDLG_TV, _T("显示器信息"));
	m_tabDevice.InsertItem(TABDLG_CHARGER, _T("充电控制器信息"));
	m_tabDevice.InsertItem(TABDLG_CA, _T("无线主机信息"));
	m_tabDevice.InsertItem(TABDLG_CALL, _T("无线呼叫器信息"));
	m_tabDevice.InsertItem(TABDLG_WATCH, _T("无线手表信息"));

	
	m_dlgAGVList.Create(IDD_AGVLIST_DIALOG, &m_tabDevice);
	m_dlgTVList.Create(IDD_TVLIST_DIALOG, &m_tabDevice);
	m_dlgChargerCtrlList.Create(IDD_CHARGERCTRLLIST_DIALOG, &m_tabDevice);
	m_dlgCAList.Create(IDD_CALIST_DIALOG, &m_tabDevice);
	m_dlgCallList.Create(IDD_CALLLIST_DIALOG, &m_tabDevice);
	m_dlgWatchList.Create(IDD_WATCHLIST_DIALOG, &m_tabDevice);

	m_dlgAGVList.ShowWindow(SW_SHOW);
	m_dlgTVList.ShowWindow(SW_HIDE);
	m_dlgChargerCtrlList.ShowWindow(SW_HIDE);
	m_dlgCAList.ShowWindow(SW_HIDE);
	m_dlgCallList.ShowWindow(SW_HIDE);
	m_dlgWatchList.ShowWindow(SW_HIDE);

	m_tabDevice.SetCurFocus(TABDLG_AGV);
	
}


void CDeviceTabDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_tabDevice.GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rcClient;
	GetClientRect(&rcClient);

	CSize sizeTabHeader;	/*!< 页头 */
	CRect rcTabItem;
	CRect rcSpace;

	m_tabDevice.MoveWindow(rcClient);

	m_tabDevice.GetClientRect(rcSpace);

	sizeTabHeader.cx = (rcSpace.Width() - 4) / 6;
	sizeTabHeader.cy = 25;
	m_tabDevice.SetItemSize(sizeTabHeader);

	m_tabDevice.GetItemRect(0, rcTabItem);

	rcSpace.top += sizeTabHeader.cy + 4;
	rcSpace.left += 4;
	rcSpace.bottom -= 4;
	rcSpace.right -= 4;

	m_dlgAGVList.MoveWindow(rcSpace);
	m_dlgTVList.MoveWindow(rcSpace);
	m_dlgChargerCtrlList.MoveWindow(rcSpace);
	m_dlgCAList.MoveWindow(rcSpace);
	m_dlgCallList.MoveWindow(rcSpace);
	m_dlgWatchList.MoveWindow(rcSpace);

	return;
}


BOOL CDeviceTabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitTab();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常:  OCX 属性页应返回 FALSE
}


BOOL CDeviceTabDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->wParam)
	{
	case VK_ESCAPE:
		return TRUE;
	case VK_RETURN:
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDeviceTabDlg::OnTcnSelchangeTabDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	m_dlgAGVList.ShowWindow(SW_HIDE);
	m_dlgTVList.ShowWindow(SW_HIDE);
	m_dlgChargerCtrlList.ShowWindow(SW_HIDE);
	m_dlgCAList.ShowWindow(SW_HIDE);
	m_dlgCallList.ShowWindow(SW_HIDE);
	m_dlgWatchList.ShowWindow(SW_HIDE);

	switch (m_tabDevice.GetCurSel())
	{
	case TABDLG_AGV:
		m_dlgAGVList.ShowWindow(SW_SHOW);

		m_dlgAGVList.SetFocus();

		break;
	case TABDLG_TV:
		m_dlgTVList.ShowWindow(SW_SHOW);

		m_dlgTVList.SetFocus();
		break;
	case TABDLG_CHARGER:
		m_dlgChargerCtrlList.ShowWindow(SW_SHOW);

		m_dlgChargerCtrlList.SetFocus();
		break;
	case TABDLG_CA:
		m_dlgCAList.ShowWindow(SW_SHOW);

		m_dlgCAList.SetFocus();
		break;
	case TABDLG_CALL:
		m_dlgCallList.ShowWindow(SW_SHOW);

		m_dlgCallList.SetFocus();
		break;
	case TABDLG_WATCH:
		m_dlgWatchList.ShowWindow(SW_SHOW);

		m_dlgWatchList.SetFocus();
		break;
	default:
		m_dlgAGVList.ShowWindow(SW_SHOW);

		m_dlgAGVList.SetFocus();
		break;
	}

	return;
}
