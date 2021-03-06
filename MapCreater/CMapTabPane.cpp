// CMapTabPane.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CMapTabPane.h"


// CMapTabPane

IMPLEMENT_DYNAMIC(CMapTabPane, CDockablePane)

CMapTabPane::CMapTabPane()
{

}

CMapTabPane::~CMapTabPane()
{
}


BEGIN_MESSAGE_MAP(CMapTabPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CMapTabPane 消息处理程序
int CMapTabPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgMapTab.Create(IDD_MAPTAB_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}
	m_dlgMapTab.ShowWindow(SW_SHOW);
	return 0;
}


void CMapTabPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgMapTab.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgMapTab.MoveWindow(rect);
	}
}


void CMapTabPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgMapTab.DestroyWindow();
}


void CMapTabPane::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
}

