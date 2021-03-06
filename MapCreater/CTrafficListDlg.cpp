// CTrafficListDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CTrafficListDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "CSetTrafficDlg.h"

// CTrafficListDlg 对话框

IMPLEMENT_DYNAMIC(CTrafficListDlg, CDialogEx)

CTrafficListDlg::CTrafficListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAFFICLIST_DIALOG, pParent)
{

}

CTrafficListDlg::~CTrafficListDlg()
{
}

void CTrafficListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TRAFFIC, m_listTraffic);
}


BEGIN_MESSAGE_MAP(CTrafficListDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CTrafficListDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CTrafficListDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CTrafficListDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CTrafficListDlg::OnBnClickedButtonDelall)
END_MESSAGE_MAP()


// CTrafficListDlg 消息处理程序

void CTrafficListDlg::SortList()
{
	/*!
	 * 根据条件进行排序
	 * 交通管制的排序条件为顺序排序
	*/
	for (int i = 0; i < m_listTraffic.GetItemCount(); i++)
	{
		CString cstrNo = _T("");	/*!< 序号 */
		cstrNo.Format(_T("%d"), i + 1);
		m_listTraffic.SetItemText(i, TRAFFICLIST_NO, cstrNo);
	}

	return;
}

bool CTrafficListDlg::Output(CString cstrFilePath)
{
	TRACE(_T("开始导出交通管制信息.\r\n"));
	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("交通管制信息导出失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL)
	{
		TRACE(_T("交通管制信息导出失败.\r\n"));
		return false;
	}

	/*!
	 * 写入交通管制信息
	*/
	for (int i = 0; i < m_listTraffic.GetItemCount(); i++)
	{
		TiXmlElement element("Traffic");	/*!< 子节点 */
		element.SetAttribute("No", CStringA(m_listTraffic.GetItemText(i, TRAFFICLIST_NO)));

		/*!
		 * 拆分管制地标卡
		*/
		CString cstrCtrl = m_listTraffic.GetItemText(i, TRAFFICLIST_CTRL);
		int nStart = 0;
		int nEnd = 0;
		do 
		{
			TiXmlElement elementChild("Ctrl");

			nEnd = cstrCtrl.Find(_T(','), nStart);

			CString cstrNo = _T("");

			// 未找到(,)
			if (nEnd == -1)
			{
				// 不存在管制地标卡
				if (cstrCtrl.IsEmpty())
				{
					break;
				}
				// 仅存在1张管制地标卡
				else
				{
					cstrNo = cstrCtrl.Mid(nStart, cstrCtrl.GetLength() - nStart);
				}
			}
			else
			{
				cstrNo = cstrCtrl.Mid(nStart, nEnd - nStart);
				nStart = nEnd+1;
			}

			if (cstrNo.IsEmpty())
			{
				continue;
			}

			elementChild.SetAttribute("No", CStringA(cstrNo));
			element.InsertEndChild(elementChild);
		} while (nEnd != -1);
		
		/*!
		 * 拆分禁行地标卡
		*/
		CString cstrStop = m_listTraffic.GetItemText(i, TRAFFICLIST_STOP);
		nStart = 0;
		nEnd = 0;
		do
		{
			TiXmlElement elementChild("Stop");

			nEnd = cstrStop.Find(_T(','), nStart);

			CString cstrNo = _T("");

			// 未找到(,)
			if (nEnd == -1)
			{
				// 不存在管制地标卡
				if (cstrStop.IsEmpty())
				{
					break;
				}
				// 仅存在1张管制地标卡
				else
				{
					cstrNo = cstrStop.Mid(nStart, cstrStop.GetLength() - nStart);
				}
			}
			else
			{
				cstrNo = cstrStop.Mid(nStart, nEnd - nStart);
				nStart = nEnd+1;
			}

			if (cstrNo.IsEmpty())
			{
				continue;
			}

			elementChild.SetAttribute("No", CStringA(cstrNo));
			element.InsertEndChild(elementChild);
		} while (nEnd != -1);

		pXmlRoot->InsertEndChild(element);

	}

	TRACE(_T("交通管制信息导出完成.\r\n"));
	return xmlDoc.SaveFile(CStringA(cstrFilePath));
}

bool CTrafficListDlg::Input(CString cstrFilePath)
{
	TRACE(_T("开始导入交通管制信息.\r\n"));
	m_listTraffic.DeleteAllItems();

	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("交通管制信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL || pXmlRoot->NoChildren())
	{
		TRACE(_T("交通管制信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlElemt = pXmlRoot->FirstChildElement(); /*!< 子节点*/

	while (pXmlElemt)
	{
		CString cstrElemtName(pXmlElemt->Value()); /*!< 子节点名称 */

		/*!
		 * 读取交通管制信息
		*/
		if (cstrElemtName == _T("Traffic"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */
			CString cstrNo = _T("0");	/*!< 编号 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					cstrNo = cstrAttrValue;
				}
			}

			TiXmlElement *pXmlElemtChild = pXmlElemt->FirstChildElement();

			CString cstrCtrl = _T("");
			CString cstrStop = _T("");

			while (pXmlElemtChild)
			{
				CString cstrElemtChildName(pXmlElemtChild->Value()); /*!< 子节点名称 */

				if (cstrElemtChildName == _T("Ctrl"))
				{
					TiXmlAttribute *pAttrChild = pXmlElemtChild->FirstAttribute(); /*!< 子节点属性 */

					while (pAttrChild)
					{
						CString cstrAttrName(pAttrChild->Name());	/*!< 属性名称*/
						CString cstrAttrValue(pAttrChild->Value()); /*!< 属性值 */

						pAttrChild = pAttrChild->Next();

						if (cstrAttrName == _T("No"))
						{
							if (cstrCtrl.IsEmpty() == false)
							{
								cstrCtrl += _T(',');
							}

							cstrCtrl += cstrAttrValue;
						}
					}
				}
				else if (cstrElemtChildName == _T("Stop"))
				{
					TiXmlAttribute *pAttrChild = pXmlElemtChild->FirstAttribute(); /*!< 子节点属性 */

					while (pAttrChild)
					{
						CString cstrAttrName(pAttrChild->Name());	/*!< 属性名称*/
						CString cstrAttrValue(pAttrChild->Value()); /*!< 属性值 */

						pAttrChild = pAttrChild->Next();

						if (cstrAttrName == _T("No"))
						{
							if (cstrStop.IsEmpty() == false)
							{
								cstrStop += _T(',');
							}

							cstrStop += cstrAttrValue;
						}
					}
				}

				pXmlElemtChild = pXmlElemtChild->NextSiblingElement();
			}
	
			int nRow = m_listTraffic.InsertItem(m_listTraffic.GetItemCount(), cstrNo);
			m_listTraffic.SetItemText(nRow, TRAFFICLIST_CTRL, cstrCtrl);
			m_listTraffic.SetItemText(nRow, TRAFFICLIST_STOP, cstrStop);
		}

		pXmlElemt = pXmlElemt->NextSiblingElement();
	}

	SortList();

	TRACE(_T("交通管制信息导入完成.\r\n"));

	return true;
}

void CTrafficListDlg::InitList()
{
	m_listTraffic.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listTraffic.InsertColumn(TRAFFICLIST_NO + 1, _T("序号"), LVCFMT_CENTER);
	m_listTraffic.InsertColumn(TRAFFICLIST_CTRL + 1, _T("管制地标卡组"), LVCFMT_CENTER);
	m_listTraffic.InsertColumn(TRAFFICLIST_STOP + 1, _T("禁行地标卡组"), LVCFMT_CENTER);
	m_listTraffic.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_listTraffic.DeleteColumn(0);

	return;
}

void CTrafficListDlg::ListSize()
{
	if (GetDlgItem(IDC_BUTTON_DEL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_DELALL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_EDIT)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_ADD)->GetSafeHwnd() == nullptr
		|| m_listTraffic.GetSafeHwnd() == nullptr)
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
	m_listTraffic.MoveWindow(rect);

	/*!
	 * 调整列表中列的宽度
	*/
	m_listTraffic.GetClientRect(&rect);

	m_listTraffic.SetColumnWidth(TRAFFICLIST_NO, rect.Width() / 9);
	m_listTraffic.SetColumnWidth(TRAFFICLIST_CTRL, rect.Width() * 4 / 9);
	m_listTraffic.SetColumnWidth(TRAFFICLIST_STOP, rect.Width() * 4/ 9);

	return;
}

BOOL CTrafficListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CTrafficListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ListSize();
}

BOOL CTrafficListDlg::PreTranslateMessage(MSG * pMsg)
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

void CTrafficListDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CSetTrafficDlg setTrafficDlg;

	if (setTrafficDlg.DoModal() == IDOK)
	{
		for (int i = 0; i < m_listTraffic.GetItemCount(); i++)
		{
			if (setTrafficDlg.m_cstrCtrl == _ttoi(m_listTraffic.GetItemText(i, TRAFFICLIST_CTRL)))
			{
				MessageBox(_T("与序号：") + m_listTraffic.GetItemText(i, TRAFFICLIST_NO) + _T("发生冲突！具有完全相同的管制地表卡组。"), _T("添加失败"), MB_ICONERROR);
				return;
			}
		}

		CString cstrNo = _T("");
		cstrNo.Format(_T("%d"), m_listTraffic.GetItemCount());

		int nRow = m_listTraffic.InsertItem(m_listTraffic.GetItemCount(), cstrNo);

		m_listTraffic.SetItemText(nRow, TRAFFICLIST_CTRL, setTrafficDlg.m_cstrCtrl);
		m_listTraffic.SetItemText(nRow, TRAFFICLIST_STOP, setTrafficDlg.m_cstrStop);

		SortList();
	}

	return;
}

void CTrafficListDlg::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listTraffic.GetItemCount(); i++)
	{
		if (m_listTraffic.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			CSetTrafficDlg setTrafficDlg;

			setTrafficDlg.m_cstrCtrl = m_listTraffic.GetItemText(i, TRAFFICLIST_CTRL);
			setTrafficDlg.m_cstrStop = m_listTraffic.GetItemText(i, TRAFFICLIST_STOP);

			if (setTrafficDlg.DoModal() == IDOK)
			{
				for (int l = 0; l < m_listTraffic.GetItemCount(); l++)
				{
					if(l == i)
					{
						continue;
					}

					if (setTrafficDlg.m_cstrCtrl == _ttoi(m_listTraffic.GetItemText(l, TRAFFICLIST_CTRL)))
					{
						MessageBox(_T("与序号：") + m_listTraffic.GetItemText(l, TRAFFICLIST_NO) + _T("发生冲突！具有完全相同的管制地表卡组。"), _T("添加失败"), MB_ICONERROR);
						return;
					}
				}

				m_listTraffic.SetItemText(i, TRAFFICLIST_CTRL, setTrafficDlg.m_cstrCtrl);
				m_listTraffic.SetItemText(i, TRAFFICLIST_STOP, setTrafficDlg.m_cstrStop);
			}

			return;
		}
	}

	return;
}

void CTrafficListDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listTraffic.GetItemCount();)
	{
		if (m_listTraffic.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			m_listTraffic.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	SortList();

	return;
}

void CTrafficListDlg::OnBnClickedButtonDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listTraffic.DeleteAllItems();
	return;
}