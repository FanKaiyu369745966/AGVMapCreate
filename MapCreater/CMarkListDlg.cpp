// CMarkListDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CMarkListDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MapCreaterView.h"


// CMarkListDlg 对话框

IMPLEMENT_DYNAMIC(CMarkListDlg, CDialogEx)

CMarkListDlg::CMarkListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MARKLIST_DIALOG, pParent)
{

}

CMarkListDlg::~CMarkListDlg()
{
}

void CMarkListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MARK, m_listMark);
}


BEGIN_MESSAGE_MAP(CMarkListDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CMarkListDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CMarkListDlg::OnBnClickedButtonDelall)
	ON_BN_CLICKED(IDC_BUTTON_LOCATION, &CMarkListDlg::OnBnClickedButtonLocation)
END_MESSAGE_MAP()


// CMarkListDlg 消息处理程序

void CMarkListDlg::Add(unsigned int unMark, CPoint ptCenter)
{
	CString cstrNo = _T("");
	CString cstrPoint = _T("");
	cstrNo.Format(_T("%d"), unMark);
	cstrPoint.Format(_T("%d,%d"), ptCenter.x, ptCenter.y);
	
	int nRow = -1;

	for (int i = 0; i < m_listMark.GetItemCount(); i++)
	{
		if (unMark == _ttoi(m_listMark.GetItemText(i, 0)))
		{
			nRow = i;
			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listMark.InsertItem(m_listMark.GetItemCount(), cstrNo);
		m_listMark.SetItemText(nRow, 1, cstrPoint);
		SortList();
	}
	else
	{
		m_listMark.SetItemText(nRow, 1, cstrPoint);
	}

	return;
}

void CMarkListDlg::DeleteItem(unsigned int unMark)
{
	for (int i = 0; i < m_listMark.GetItemCount(); i++)
	{
		if (unMark == _ttoi(m_listMark.GetItemText(i, 0)))
		{
			m_listMark.DeleteItem(i);

			return;
		}
	}

	return;
}

void CMarkListDlg::SortList()
{
	int nRow = m_listMark.GetItemCount();
	int nCol = 2;

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
			ppCStrList[i][l] = m_listMark.GetItemText(i, l);
		}
	}

	/*!
	 * 根据条件进行排序
	 * 地标卡的排序条件为地标卡号
	 * 冒泡排序
	*/
	for (int i = 1; i < nRow; i++)
	{
		for (int j = 0; j < nRow-i; j++)
		{
			if (_ttoi(ppCStrList[j][0]) > _ttoi(ppCStrList[j+1][0]))
			{
				CString *pCStrItem = ppCStrList[j];
				ppCStrList[j] = ppCStrList[j+1];
				ppCStrList[j+1] = pCStrItem;
			}
		}
	}

	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			m_listMark.SetItemText(i, j, ppCStrList[i][j]);
		}
	}

	for (int i = 0; i < nRow; i++)
	{
		delete[] ppCStrList[i];
	}

	delete[] ppCStrList;

	return;
}

void CMarkListDlg::Delete(unsigned int unMark)
{
	/*!
	 * 通知界面删除地标卡图形
	*/
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMapCreaterView* pView = (CMapCreaterView*)pMainFrame->GetActiveView();
	pView->DeleteMark(unMark);

	return;
}

void CMarkListDlg::DeleteAll()
{
	/*!
	 * 通知界面删除所有地标卡图形
	*/
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMapCreaterView* pView = (CMapCreaterView*)pMainFrame->GetActiveView();
	pView->DeleteAllMark();

	return;
}

void CMarkListDlg::Locate(unsigned int unMark)
{
	/*!
	 * 通知界面定位选中的地标卡
	*/
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMapCreaterView* pView = (CMapCreaterView*)pMainFrame->GetActiveView();
	pView->LocateMark(unMark);

	return;
}

void CMarkListDlg::InitList()
{
	m_listMark.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listMark.InsertColumn(1, _T("编号"), LVCFMT_CENTER);
	m_listMark.InsertColumn(2, _T("坐标"), LVCFMT_CENTER);
	m_listMark.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_listMark.DeleteColumn(0);

	return;
}

void CMarkListDlg::ListSize()
{
	if (GetDlgItem(IDC_BUTTON_DEL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_DELALL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_LOCATION)->GetSafeHwnd() == nullptr
		|| m_listMark.GetSafeHwnd() == nullptr)
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
	int nDis = (rect.Width() - 3 * nWidth) / 4;

	GetDlgItem(IDC_BUTTON_LOCATION)->MoveWindow(CRect(rect.left + nDis, rect.top, rect.left + nDis + nWidth, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DEL)->MoveWindow(CRect(rect.left + nDis * 2 + nWidth, rect.top, rect.left + nDis * 2 + nWidth * 2, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DELALL)->MoveWindow(CRect(rect.left + nDis * 3 + nWidth * 2, rect.top, rect.left + nDis * 3 + nWidth * 3, rect.top + nHeight));

	rect.top += nHeight + 5;
	m_listMark.MoveWindow(rect);

	/*!
	 * 调整列表中列的宽度
	*/
	m_listMark.GetClientRect(&rect);

	m_listMark.SetColumnWidth(0,rect.Width() / 2);
	m_listMark.SetColumnWidth(1, rect.Width() / 2);

	return;
}


BOOL CMarkListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMarkListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ListSize();
}

BOOL CMarkListDlg::PreTranslateMessage(MSG * pMsg)
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


void CMarkListDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	/*!
	 * 删除勾选的
	*/
	for (int i = 0; i < m_listMark.GetItemCount();)
	{
		if (m_listMark.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			Delete(_ttoi(m_listMark.GetItemText(i, 0)));
			m_listMark.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	return;
}


void CMarkListDlg::OnBnClickedButtonDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	DeleteAll();
	m_listMark.DeleteAllItems();

	return;
}


void CMarkListDlg::OnBnClickedButtonLocation()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listMark.GetItemCount(); i++)
	{
		if (m_listMark.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			Locate(_ttoi(m_listMark.GetItemText(i, 0)));

			return;
		}
	}

	return;
}
