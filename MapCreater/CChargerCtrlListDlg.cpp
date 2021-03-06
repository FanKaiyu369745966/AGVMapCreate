// CChargerCtrlListDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CChargerCtrlListDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "CSetChargerCtrlDlg.h"


// CChargerCtrlListDlg 对话框

IMPLEMENT_DYNAMIC(CChargerCtrlListDlg, CDialogEx)

CChargerCtrlListDlg::CChargerCtrlListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHARGERCTRLLIST_DIALOG, pParent)
{

}

CChargerCtrlListDlg::~CChargerCtrlListDlg()
{
}

void CChargerCtrlListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHARGERCTRL, m_listChargerCtrl);
}


BEGIN_MESSAGE_MAP(CChargerCtrlListDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CChargerCtrlListDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CChargerCtrlListDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CChargerCtrlListDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CChargerCtrlListDlg::OnBnClickedButtonDelall)
END_MESSAGE_MAP()


// CChargerCtrlListDlg 消息处理程序

void CChargerCtrlListDlg::SortList()
{
	int nRow = m_listChargerCtrl.GetItemCount();
	int nCol = 3;

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
			ppCStrList[i][l] = m_listChargerCtrl.GetItemText(i, l);
		}
	}

	/*!
	 * 根据条件进行排序
	 * 充电控制器的排序条件为控制器编号
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
			m_listChargerCtrl.SetItemText(i, j, ppCStrList[i][j]);
		}
	}

	for (int i = 0; i < nRow; i++)
	{
		delete[] ppCStrList[i];
	}

	delete[] ppCStrList;

	return;
}

bool CChargerCtrlListDlg::Output(CString cstrFilePath)
{
	TRACE(_T("开始导出充电器控制器信息.\r\n"));
	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("充电器控制器信息导出失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL)
	{
		TRACE(_T("充电器控制器信息导出失败.\r\n"));
		return false;
	}

	/*!
	 * 写入充电控制器信息
	*/
	for (int i = 0; i < m_listChargerCtrl.GetItemCount(); i++)
	{
		TiXmlElement element("ChargerCtrl");	/*!< 子节点 */
		element.SetAttribute("No", CStringA(m_listChargerCtrl.GetItemText(i, CHARGERCTRLLIST_NO)));
		element.SetAttribute("IP", CStringA(m_listChargerCtrl.GetItemText(i, CHARGERCTRLLIST_IP)));
		element.SetAttribute("Port", CStringA(m_listChargerCtrl.GetItemText(i, CHARGERCTRLLIST_PORT)));

		pXmlRoot->InsertEndChild(element);

	}

	TRACE(_T("充电器控制器信息导出完成.\r\n"));
	return xmlDoc.SaveFile(CStringA(cstrFilePath));
}

bool CChargerCtrlListDlg::Input(CString cstrFilePath)
{
	TRACE(_T("开始导入充电器控制器信息.\r\n"));
	m_listChargerCtrl.DeleteAllItems();

	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("充电器控制器信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL || pXmlRoot->NoChildren())
	{
		TRACE(_T("充电器控制器信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlElemt = pXmlRoot->FirstChildElement(); /*!< 子节点*/

	while (pXmlElemt)
	{
		CString cstrElemtName(pXmlElemt->Value()); /*!< 子节点名称 */

		/*!
		 * 读取充电控制器信息
		*/
		if (cstrElemtName == _T("ChargerCtrl"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			CString cstrNo = _T("0");	/*!< 编号 */
			CString cstrIp = _T("0.0.0.0");	/*!< IP地址 */
			CString cstrPort = _T("0");	/*!< 端口 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					cstrNo = cstrAttrValue;
				}
				else if (cstrAttrName == _T("IP"))
				{
					cstrIp = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Port"))
				{
					cstrPort = cstrAttrValue;
				}
			}

			int nRow = m_listChargerCtrl.InsertItem(m_listChargerCtrl.GetItemCount(), cstrNo);
			m_listChargerCtrl.SetItemText(nRow, CHARGERCTRLLIST_IP, cstrIp);
			m_listChargerCtrl.SetItemText(nRow, CHARGERCTRLLIST_PORT, cstrPort);
		}

		pXmlElemt = pXmlElemt->NextSiblingElement();
	}

	SortList();

	TRACE(_T("充电器控制器信息导入完成.\r\n"));
	return true;
}

void CChargerCtrlListDlg::InitList()
{
	m_listChargerCtrl.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listChargerCtrl.InsertColumn(CHARGERCTRLLIST_NO + 1, _T("编号"), LVCFMT_CENTER);
	m_listChargerCtrl.InsertColumn(CHARGERCTRLLIST_IP + 1, _T("IP地址"), LVCFMT_CENTER);
	m_listChargerCtrl.InsertColumn(CHARGERCTRLLIST_PORT + 1, _T("端口"), LVCFMT_CENTER);
	m_listChargerCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_listChargerCtrl.DeleteColumn(0);

	return;
}

void CChargerCtrlListDlg::ListSize()
{
	if (GetDlgItem(IDC_BUTTON_DEL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_DELALL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_EDIT)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_ADD)->GetSafeHwnd() == nullptr
		|| m_listChargerCtrl.GetSafeHwnd() == nullptr)
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

	GetDlgItem(IDC_BUTTON_ADD)->MoveWindow(CRect(rect.left + nDis, rect.top, rect.left + nDis + nWidth, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_EDIT)->MoveWindow(CRect(rect.left + nDis * 2 + nWidth, rect.top, rect.left + nDis * 2 + nWidth * 2, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DEL)->MoveWindow(CRect(rect.left + nDis * 3 + nWidth * 2, rect.top, rect.left + nDis * 3 + nWidth * 3, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DELALL)->MoveWindow(CRect(rect.left + nDis * 4 + nWidth * 3, rect.top, rect.left + nDis * 4 + nWidth * 4, rect.top + nHeight));

	rect.top += nHeight + 5;
	m_listChargerCtrl.MoveWindow(rect);

	/*!
	 * 调整列表中列的宽度
	*/
	m_listChargerCtrl.GetClientRect(&rect);

	m_listChargerCtrl.SetColumnWidth(CHARGERCTRLLIST_NO, rect.Width() / 4);
	m_listChargerCtrl.SetColumnWidth(CHARGERCTRLLIST_IP, rect.Width() * 2 / 4);
	m_listChargerCtrl.SetColumnWidth(CHARGERCTRLLIST_PORT, rect.Width() / 4);

	return;
}

BOOL CChargerCtrlListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CChargerCtrlListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ListSize();
}

BOOL CChargerCtrlListDlg::PreTranslateMessage(MSG * pMsg)
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

void CChargerCtrlListDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CSetChargerCtrlDlg setChargerCtrlDlg;

	if (setChargerCtrlDlg.DoModal() == IDOK)
	{
		for (int i = 0; i < m_listChargerCtrl.GetItemCount(); i++)
		{
			if (setChargerCtrlDlg.m_unNo == _ttoi(m_listChargerCtrl.GetItemText(i, CHARGERCTRLLIST_NO)))
			{
				MessageBox(_T("已存在相同编号的充电控制器设备!"), _T("添加失败"), MB_ICONERROR);
				return;
			}
		}

		CString cstrNo = _T("");
		cstrNo.Format(_T("%d"), setChargerCtrlDlg.m_unNo);

		int nRow = m_listChargerCtrl.InsertItem(m_listChargerCtrl.GetItemCount(), cstrNo);

		m_listChargerCtrl.SetItemText(nRow, CHARGERCTRLLIST_IP, setChargerCtrlDlg.m_cstrIp);

		CString cstrPort = _T("");
		cstrPort.Format(_T("%d"), setChargerCtrlDlg.m_unPort);
		m_listChargerCtrl.SetItemText(nRow, CHARGERCTRLLIST_PORT, cstrPort);

		SortList();
	}

	return;
}

void CChargerCtrlListDlg::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listChargerCtrl.GetItemCount(); i++)
	{
		if (m_listChargerCtrl.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			CSetChargerCtrlDlg setChargerCtrlDlg;

			setChargerCtrlDlg.m_unNo = _ttoi(m_listChargerCtrl.GetItemText(i, CHARGERCTRLLIST_NO));
			setChargerCtrlDlg.m_cstrIp = m_listChargerCtrl.GetItemText(i, CHARGERCTRLLIST_IP);
			setChargerCtrlDlg.m_unPort = _ttoi(m_listChargerCtrl.GetItemText(i, CHARGERCTRLLIST_PORT));
			setChargerCtrlDlg.m_bEdit = true;

			if (setChargerCtrlDlg.DoModal() == IDOK)
			{
				m_listChargerCtrl.SetItemText(i, CHARGERCTRLLIST_IP, setChargerCtrlDlg.m_cstrIp);

				CString cstrPort = _T("");
				cstrPort.Format(_T("%d"), setChargerCtrlDlg.m_unPort);
				m_listChargerCtrl.SetItemText(i, CHARGERCTRLLIST_PORT, cstrPort);
			}

			return;
		}
	}

	return;
}

void CChargerCtrlListDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listChargerCtrl.GetItemCount();)
	{
		if (m_listChargerCtrl.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			m_listChargerCtrl.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	return;
}

void CChargerCtrlListDlg::OnBnClickedButtonDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listChargerCtrl.DeleteAllItems();
	return;
}