// CCAListDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CCAListDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "CSetCADlg.h"

// CCAListDlg 对话框

IMPLEMENT_DYNAMIC(CCAListDlg, CDialogEx)

CCAListDlg::CCAListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CALIST_DIALOG, pParent)
{

}

CCAListDlg::~CCAListDlg()
{
}

void CCAListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CA, m_listCA);
}


BEGIN_MESSAGE_MAP(CCAListDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCAListDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CCAListDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CCAListDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CCAListDlg::OnBnClickedButtonDelall)
END_MESSAGE_MAP()


// CCAListDlg 消息处理程序

void CCAListDlg::SortList()
{
	int nRow = m_listCA.GetItemCount();
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
			ppCStrList[i][l] = m_listCA.GetItemText(i, l);
		}
	}

	/*!
	 * 根据条件进行排序
	 * 无线主机的排序条件为无线主机编号
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
			m_listCA.SetItemText(i, j, ppCStrList[i][j]);
		}
	}

	for (int i = 0; i < nRow; i++)
	{
		delete[] ppCStrList[i];
	}

	delete[] ppCStrList;

	return;
}

bool CCAListDlg::Output(CString cstrFilePath)
{
	TRACE(_T("开始导出无线主机信息.\r\n"));
	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("无线主机信息导出失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL)
	{
		TRACE(_T("无线主机信息导出失败.\r\n"));
		return false;
	}

	/*!
	 * 写入无线主机信息
	*/
	for (int i = 0; i < m_listCA.GetItemCount(); i++)
	{
		TiXmlElement element("CA");	/*!< 子节点 */
		element.SetAttribute("No", CStringA(m_listCA.GetItemText(i, CALIST_NO)));
		element.SetAttribute("IP", CStringA(m_listCA.GetItemText(i, CALIST_IP)));
		element.SetAttribute("Port", CStringA(m_listCA.GetItemText(i, CALIST_PORT)));

		pXmlRoot->InsertEndChild(element);

	}

	TRACE(_T("无线主机信息导出完成.\r\n"));
	return xmlDoc.SaveFile(CStringA(cstrFilePath));
}

bool CCAListDlg::Input(CString cstrFilePath)
{
	TRACE(_T("开始导入无线主机信息.\r\n"));
	m_listCA.DeleteAllItems();

	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("无线主机信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL || pXmlRoot->NoChildren())
	{
		TRACE(_T("无线主机信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlElemt = pXmlRoot->FirstChildElement(); /*!< 子节点*/

	while (pXmlElemt)
	{
		CString cstrElemtName(pXmlElemt->Value()); /*!< 子节点名称 */

		/*!
		 * 读取无线主机信息
		*/
		if (cstrElemtName == _T("CA"))
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

			int nRow = m_listCA.InsertItem(m_listCA.GetItemCount(), cstrNo);
			m_listCA.SetItemText(nRow,CALIST_IP, cstrIp);
			m_listCA.SetItemText(nRow, CALIST_PORT, cstrPort);
		}

		pXmlElemt = pXmlElemt->NextSiblingElement();
	}

	SortList();

	TRACE(_T("无线主机信息导入完成.\r\n"));

	return true;
}

void CCAListDlg::InitList()
{
	m_listCA.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCA.InsertColumn(CALIST_NO + 1, _T("编号"), LVCFMT_CENTER);
	m_listCA.InsertColumn(CALIST_IP + 1, _T("IP地址"), LVCFMT_CENTER);
	m_listCA.InsertColumn(CALIST_PORT + 1, _T("端口"), LVCFMT_CENTER);
	m_listCA.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_listCA.DeleteColumn(0);

	return;
}

void CCAListDlg::ListSize()
{
	if (GetDlgItem(IDC_BUTTON_DEL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_DELALL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_EDIT)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_ADD)->GetSafeHwnd() == nullptr
		|| m_listCA.GetSafeHwnd() == nullptr)
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
	m_listCA.MoveWindow(rect);

	/*!
	 * 调整列表中列的宽度
	*/
	m_listCA.GetClientRect(&rect);

	m_listCA.SetColumnWidth(CALIST_NO, rect.Width() / 4);
	m_listCA.SetColumnWidth(CALIST_IP, rect.Width() * 2 / 4);
	m_listCA.SetColumnWidth(CALIST_PORT, rect.Width() / 4);

	return;
}

BOOL CCAListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CCAListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ListSize();
}

BOOL CCAListDlg::PreTranslateMessage(MSG * pMsg)
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

void CCAListDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CSetCADlg setCADlg;

	if (setCADlg.DoModal() == IDOK)
	{
		for (int i = 0; i < m_listCA.GetItemCount(); i++)
		{
			if (setCADlg.m_unNo == _ttoi(m_listCA.GetItemText(i, CALIST_NO)))
			{
				MessageBox(_T("已存在相同编号的无线主机设备!"), _T("添加失败"), MB_ICONERROR);
				return;
			}
		}

		CString cstrNo = _T("");
		cstrNo.Format(_T("%d"), setCADlg.m_unNo);

		int nRow = m_listCA.InsertItem(m_listCA.GetItemCount(), cstrNo);

		m_listCA.SetItemText(nRow, CALIST_IP, setCADlg.m_cstrIp);

		CString cstrPort = _T("");
		cstrPort.Format(_T("%d"), setCADlg.m_unPort);
		m_listCA.SetItemText(nRow, CALIST_PORT, cstrPort);

		SortList();
	}

	return;
}

void CCAListDlg::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listCA.GetItemCount(); i++)
	{
		if (m_listCA.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			CSetCADlg setCADlg;

			setCADlg.m_unNo = _ttoi(m_listCA.GetItemText(i, CALIST_NO));
			setCADlg.m_cstrIp = m_listCA.GetItemText(i, CALIST_IP);
			setCADlg.m_unPort = _ttoi(m_listCA.GetItemText(i, CALIST_PORT));
			setCADlg.m_bEdit = true;

			if (setCADlg.DoModal() == IDOK)
			{
				m_listCA.SetItemText(i, CALIST_IP, setCADlg.m_cstrIp);

				CString cstrPort = _T("");
				cstrPort.Format(_T("%d"), setCADlg.m_unPort);
				m_listCA.SetItemText(i, CALIST_PORT, cstrPort);
			}

			return;
		}
	}

	return;
}

void CCAListDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listCA.GetItemCount();)
	{
		if (m_listCA.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			m_listCA.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	return;
}

void CCAListDlg::OnBnClickedButtonDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listCA.DeleteAllItems();
	return;
}