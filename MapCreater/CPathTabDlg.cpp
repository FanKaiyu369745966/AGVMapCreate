// CPathTabDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CPathTabDlg.h"
#include "afxdialogex.h"


// CPathTabDlg 对话框

IMPLEMENT_DYNAMIC(CPathTabDlg, CDialogEx)

CPathTabDlg::CPathTabDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PATHTAB_DIALOG, pParent)
{

}

CPathTabDlg::~CPathTabDlg()
{
}

void CPathTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PATH, m_tabPath);
}


BEGIN_MESSAGE_MAP(CPathTabDlg, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PATH, &CPathTabDlg::OnTcnSelchangeTabPath)
END_MESSAGE_MAP()


// CPathTabDlg 消息处理程序
void CPathTabDlg::InitTab()
{
	m_tabPath.InsertItem(TABDLG_TRAFFIC, _T("交通管制"));
	m_tabPath.InsertItem(TABDLG_RESTQUEUE, _T("待机队列"));

	m_dlgTrafficList.Create(IDD_TRAFFICLIST_DIALOG, &m_tabPath);
	m_dlgRestQueueList.Create(IDD_RESTQUEUELIST_DIALOG, &m_tabPath);

	m_dlgTrafficList.ShowWindow(SW_SHOW);
	m_dlgRestQueueList.ShowWindow(SW_HIDE);

	m_tabPath.SetCurFocus(TABDLG_TRAFFIC);
}


void CPathTabDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_tabPath.GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rcClient;
	GetClientRect(&rcClient);

	CSize sizeTabHeader;	/*!< 页头 */
	CRect rcTabItem;
	CRect rcSpace;

	m_tabPath.MoveWindow(rcClient);

	m_tabPath.GetClientRect(rcSpace);

	sizeTabHeader.cx = (rcSpace.Width() - 4) / 2;
	sizeTabHeader.cy = 25;
	m_tabPath.SetItemSize(sizeTabHeader);

	m_tabPath.GetItemRect(0, rcTabItem);

	rcSpace.top += sizeTabHeader.cy + 4;
	rcSpace.left += 4;
	rcSpace.bottom -= 4;
	rcSpace.right -= 4;

	m_dlgTrafficList.MoveWindow(rcSpace);
	m_dlgRestQueueList.MoveWindow(rcSpace);

	return;
}


BOOL CPathTabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitTab();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常:  OCX 属性页应返回 FALSE
}


BOOL CPathTabDlg::PreTranslateMessage(MSG* pMsg)
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

void CPathTabDlg::OnTcnSelchangeTabPath(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	m_dlgTrafficList.ShowWindow(SW_HIDE);
	m_dlgRestQueueList.ShowWindow(SW_HIDE);

	switch (m_tabPath.GetCurSel())
	{
	case TABDLG_TRAFFIC:
		m_dlgTrafficList.ShowWindow(SW_SHOW);

		m_dlgTrafficList.SetFocus();

		break;
	case TABDLG_RESTQUEUE:
		m_dlgRestQueueList.ShowWindow(SW_SHOW);

		m_dlgRestQueueList.SetFocus();
		break;
	default:
		m_dlgTrafficList.ShowWindow(SW_SHOW);

		m_dlgTrafficList.SetFocus();
		break;
	}

	return;
}
