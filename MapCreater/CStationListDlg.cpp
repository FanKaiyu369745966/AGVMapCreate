// CStationListDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CStationListDlg.h"
#include "afxdialogex.h"
#include "CSetStationDlg.h"
#include "MainFrm.h"
#include "MapCreaterView.h"


// CStationListDlg 对话框

IMPLEMENT_DYNAMIC(CStationListDlg, CDialogEx)

CStationListDlg::CStationListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STATIONLIST_DIALOG, pParent)
{

}

CStationListDlg::~CStationListDlg()
{
}

void CStationListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STATION, m_listStation);
}


BEGIN_MESSAGE_MAP(CStationListDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_LOCATION, &CStationListDlg::OnBnClickedButtonLocation)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CStationListDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CStationListDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CStationListDlg::OnBnClickedButtonDelall)
END_MESSAGE_MAP()


// CStationListDlg 消息处理程序
void CStationListDlg::SortList()
{
	int nRow = m_listStation.GetItemCount();
	int nCol = 4;

	if (nRow < 2)
	{
		return;
	}

	/*!
	 * 将列表成员放入数组集合
	 * 列表成员为字符串数组
	*/
	CString **ppCStrList = new CString*[nRow];

	for (int i = 0; i < nRow; i++)
	{
		ppCStrList[i] = new CString[nCol];
	}

	for (int i = 0; i < nRow; i++)
	{
		for (int l = 0; l < nCol; l++)
		{
			ppCStrList[i][l] = m_listStation.GetItemText(i, l);
		}
	}

	/*!
	 * 根据条件进行排序
	 * 工位的排序条件为工位号
	 * 冒泡排序
	*/
	for (int i = 1; i < nRow; i++)
	{
		for (int j = 0; j < nRow - i; j++)
		{
			if (_ttoi(ppCStrList[j][0]) > _ttoi(ppCStrList[j + 1][0]))
			{
				CString *pCStrItem = ppCStrList[j];
				ppCStrList[j] = ppCStrList[j + 1];
				ppCStrList[j + 1] = pCStrItem;
			}
		}
	}

	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			m_listStation.SetItemText(i, j, ppCStrList[i][j]);
		}
	}

	for (int i = 0; i < nRow; i++)
	{
		delete[] ppCStrList[i];
	}

	delete[] ppCStrList;

	return;
}

void CStationListDlg::Delete(unsigned int unStation)
{
	/*!
	 * 通知界面删除工位图形
	*/
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMapCreaterView* pView = (CMapCreaterView*)pMainFrame->GetActiveView();
	pView->DeleteStation(unStation);

	return;
}

void CStationListDlg::DeleteAll()
{
	/*!
	 * 通知界面删除所有工位图形
	*/
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMapCreaterView* pView = (CMapCreaterView*)pMainFrame->GetActiveView();
	pView->DeleteAllStation();

	return;
}

void CStationListDlg::Locate(unsigned int unStation)
{
	/*!
	 * 通知界面定位选中的工位
	*/
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMapCreaterView* pView = (CMapCreaterView*)pMainFrame->GetActiveView();
	pView->LocateStation(unStation);

	return;
}

void CStationListDlg::Update(unsigned int unStation, CString cstrName, unsigned int unMark)
{
	/*!
	 * 通知界面更新工位属性
	*/
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMapCreaterView* pView = (CMapCreaterView*)pMainFrame->GetActiveView();
	pView->UpdateStation(unStation, cstrName, unMark);

	return;
}

void CStationListDlg::Add(unsigned int unStation, CPoint ptCenter, CString cstrName, unsigned int unMark)
{
	CString cstrNo = _T("");
	CString cstrPoint = _T("");
	CString cstrMark = _T("");
	cstrNo.Format(_T("%d"), unStation);
	cstrPoint.Format(_T("%d,%d"), ptCenter.x, ptCenter.y);
	cstrMark.Format(_T("%d"), unMark);

	int nRow = -1;

	for (int i = 0; i < m_listStation.GetItemCount(); i++)
	{
		if (unStation == _ttoi(m_listStation.GetItemText(i, 0)))
		{
			nRow = i;
			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listStation.InsertItem(m_listStation.GetItemCount(), cstrNo);
		m_listStation.SetItemText(nRow, 1, cstrPoint);
		m_listStation.SetItemText(nRow, 2, cstrMark);
		m_listStation.SetItemText(nRow, 3, cstrName);

		SortList();
	}
	else
	{
		m_listStation.SetItemText(nRow, 1, cstrPoint);
		m_listStation.SetItemText(nRow, 2, cstrMark);
		m_listStation.SetItemText(nRow, 3, cstrName);
	}

	return;
}

void CStationListDlg::DeleteItem(unsigned int unStation)
{
	for (int i = 0; i < m_listStation.GetItemCount(); i++)
	{
		if (unStation == _ttoi(m_listStation.GetItemText(i, 0)))
		{
			m_listStation.DeleteItem(i);

			return;
		}
	}

	return;
}

void CStationListDlg::InitList()
{
	m_listStation.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listStation.InsertColumn(1, _T("编号"), LVCFMT_CENTER);
	m_listStation.InsertColumn(2, _T("坐标"), LVCFMT_CENTER);
	m_listStation.InsertColumn(3, _T("地标卡"), LVCFMT_CENTER);
	m_listStation.InsertColumn(4, _T("名称"), LVCFMT_CENTER);
	m_listStation.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_listStation.DeleteColumn(0);

	return;
}

void CStationListDlg::ListSize()
{
	if (GetDlgItem(IDC_BUTTON_DEL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_DELALL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_LOCATION)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_EDIT)->GetSafeHwnd() == nullptr
		|| m_listStation.GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);

	/*!
	 * 调整对话框中的空间位置
	*/

	int nWidth = 100;	/*!< 宽 */
	int nHeight = 30;	/*!< 高 */

	rect.top += 5;
	int nDis = (rect.Width() - 4 * nWidth) / 5;

	GetDlgItem(IDC_BUTTON_LOCATION)->MoveWindow(CRect(rect.left + nDis, rect.top, rect.left + nDis + nWidth, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_EDIT)->MoveWindow(CRect(rect.left + nDis * 2 + nWidth, rect.top, rect.left + nDis * 2 + nWidth * 2, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DEL)->MoveWindow(CRect(rect.left + nDis * 3 + nWidth * 2, rect.top, rect.left + nDis * 3 + nWidth * 3, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DELALL)->MoveWindow(CRect(rect.left + nDis * 4 + nWidth * 3, rect.top, rect.left + nDis * 4 + nWidth * 4, rect.top + nHeight));

	rect.top += nHeight + 5;
	m_listStation.MoveWindow(rect);

	/*!
	 * 调整列表中列的宽度
	*/
	m_listStation.GetClientRect(&rect);

	m_listStation.SetColumnWidth(0, rect.Width() / 5);
	m_listStation.SetColumnWidth(1, rect.Width() / 5);
	m_listStation.SetColumnWidth(2, rect.Width() / 5);
	m_listStation.SetColumnWidth(3, rect.Width() * 2 / 5);

	return;
}

BOOL CStationListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CStationListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ListSize();

	return;
}

BOOL CStationListDlg::PreTranslateMessage(MSG * pMsg)
{
	switch (pMsg->wParam)
	{
	case VK_ESCAPE:
		return TRUE;
	case VK_RETURN:
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CStationListDlg::OnBnClickedButtonLocation()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listStation.GetItemCount(); i++)
	{
		if (m_listStation.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			Locate(_ttoi(m_listStation.GetItemText(i, 0)));

			return;
		}
	}

	return;
}


void CStationListDlg::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listStation.GetItemCount(); i++)
	{
		if (m_listStation.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			CSetStationDlg setStationDlg;
			setStationDlg.m_unNo = _ttoi(m_listStation.GetItemText(i, 0));
			setStationDlg.m_unMark = _ttoi(m_listStation.GetItemText(i, 2));
			setStationDlg.m_cstrName = m_listStation.GetItemText(i, 3);
			setStationDlg.m_bEdit = true;

			if (setStationDlg.DoModal() == IDOK)
			{
				CString cstrMark = _T("");
				cstrMark.Format(_T("%d"), setStationDlg.m_unMark);

				m_listStation.SetItemText(i, 2, cstrMark);
				m_listStation.SetItemText(i, 3, setStationDlg.m_cstrName);

				Update(setStationDlg.m_unNo, setStationDlg.m_cstrName, setStationDlg.m_unMark);
			}

			return;
		}
	}

	return;
}


void CStationListDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	/*!
	 * 删除勾选的
	*/
	for (int i = 0; i < m_listStation.GetItemCount();)
	{
		if (m_listStation.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			Delete(_ttoi(m_listStation.GetItemText(i, 0)));
			m_listStation.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	return;
}


void CStationListDlg::OnBnClickedButtonDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	DeleteAll();
	m_listStation.DeleteAllItems();

	return;
}
