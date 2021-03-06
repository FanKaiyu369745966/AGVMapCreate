// CRestQueueListDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CRestQueueListDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "CSetRestQueueDlg.h"


// CRestQueueListDlg 对话框

IMPLEMENT_DYNAMIC(CRestQueueListDlg, CDialogEx)

CRestQueueListDlg::CRestQueueListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESTQUEUELIST_DIALOG, pParent)
{

}

CRestQueueListDlg::~CRestQueueListDlg()
{
}

void CRestQueueListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESTQUEUE, m_listRestQueue);
}


BEGIN_MESSAGE_MAP(CRestQueueListDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CRestQueueListDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CRestQueueListDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CRestQueueListDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CRestQueueListDlg::OnBnClickedButtonDelall)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CRestQueueListDlg::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CRestQueueListDlg::OnBnClickedButtonDown)
END_MESSAGE_MAP()


// CRestQueueListDlg 消息处理程序

void CRestQueueListDlg::SortList()
{
	int nRow = m_listRestQueue.GetItemCount();
	int nCol = 4;

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
			ppCStrList[i][l] = m_listRestQueue.GetItemText(i, l);
		}
	}

	/*!
	 * 根据条件进行排序
	 * 待机队列的排序条件
	 * 1、先按照队列编号进行排序
	 * 2、然后按照顺序重新填写排序号
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
			m_listRestQueue.SetItemText(i, j, ppCStrList[i][j]);
		}
	}

	for (int i = 0; i < nRow; i++)
	{
		delete[] ppCStrList[i];
	}

	delete[] ppCStrList;


	/*!
	 * 按照当前顺序重新填写排序号
	*/
	int nCurQueue = 0;	/*!< 当前排列的队列号 */
	int nCount = 1;		/*!< 当前排序号 */
	for (int i = 0; i < m_listRestQueue.GetItemCount(); i++)
	{
		if (nCurQueue == 0)
		{
			nCurQueue = _ttoi(m_listRestQueue.GetItemText(i, RESTQUEUELIST_NO));
		}
		else
		{
			if (nCurQueue != _ttoi(m_listRestQueue.GetItemText(i, RESTQUEUELIST_NO)))
			{
				nCurQueue = _ttoi(m_listRestQueue.GetItemText(i, RESTQUEUELIST_NO));
				nCount = 1;
			}
		}

		CString cstrText = _T("");
		cstrText.Format(_T("%d"), nCount);
		m_listRestQueue.SetItemText(i, RESTQUEUELIST_SORT, cstrText);

		nCount++;
	}

	return;
}

bool CRestQueueListDlg::Output(CString cstrFilePath)
{
	TRACE(_T("开始导出待机队列信息.\r\n"));

	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("待机队列信息导出失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL)
	{
		TRACE(_T("待机队列信息导出失败.\r\n"));
		return false;
	}

	/*!
	 * 写入待机对列信息
	*/
	TiXmlElement *pElement = nullptr;
	CString cstrQueue = _T("");	/*!< 当前队列 */

	for (int i = 0; i < m_listRestQueue.GetItemCount(); i++)
	{
		if (pElement == nullptr)
		{
			pElement = new TiXmlElement("Queue");
			pElement->SetAttribute("No", CStringA(m_listRestQueue.GetItemText(i, RESTQUEUELIST_NO)));
			cstrQueue = m_listRestQueue.GetItemText(i, RESTQUEUELIST_NO);
		}
		else
		{
			if (cstrQueue != m_listRestQueue.GetItemText(i, RESTQUEUELIST_NO))
			{
				pXmlRoot->InsertEndChild(*pElement);

				delete pElement;

				pElement = nullptr;

				pElement = new TiXmlElement("Queue");
				pElement->SetAttribute("No", CStringA(m_listRestQueue.GetItemText(i, RESTQUEUELIST_NO)));
				cstrQueue = m_listRestQueue.GetItemText(i, RESTQUEUELIST_NO);
			}
			
		}

		TiXmlElement elementChild("Area");	/*!< 子节点 */
		elementChild.SetAttribute("No", CStringA(m_listRestQueue.GetItemText(i, RESTQUEUELIST_SORT)));

		TiXmlElement elementGrandson("Rest");	/*!< 子节点 */
		elementGrandson.SetAttribute("No", CStringA(m_listRestQueue.GetItemText(i, RESTQUEUELIST_REST)));
		elementChild.InsertEndChild(elementGrandson);

		/*!
		 * 拆分禁行地标卡
		*/
		CString cstrStop = m_listRestQueue.GetItemText(i, RESTQUEUELIST_STOP);
		int nStart = 0;
		int nEnd = 0;
		do
		{
			TiXmlElement elementBrother("Stop");

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
				nStart = nEnd + 1;
			}

			if (cstrNo.IsEmpty())
			{
				continue;
			}

			elementBrother.SetAttribute("No", CStringA(cstrNo));
			elementChild.InsertEndChild(elementBrother);
		} while (nEnd != -1);
	
		pElement->InsertEndChild(elementChild);
	}

	if (pElement)
	{
		pXmlRoot->InsertEndChild(*pElement);
		delete pElement;
	}

	TRACE(_T("待机队列信息导出完成.\r\n"));

	return xmlDoc.SaveFile(CStringA(cstrFilePath));
}

bool CRestQueueListDlg::Input(CString cstrFilePath)
{
	TRACE(_T("开始导入待机队列信息.\r\n"));

	m_listRestQueue.DeleteAllItems();

	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("待机队列信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL || pXmlRoot->NoChildren())
	{
		TRACE(_T("待机队列信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlElemt = pXmlRoot->FirstChildElement(); /*!< 子节点*/

	while (pXmlElemt)
	{
		CString cstrElemtName(pXmlElemt->Value()); /*!< 子节点名称 */

		/*!
		 * 读取待机对列信息
		*/
		if (cstrElemtName == _T("Queue"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			CString cstrQueue = _T("");		/*!< 编号 */

			/*!
			 * 队列属性
			*/
			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					cstrQueue = cstrAttrValue;
				}
			}

			/*!
			 * 队列排序
			*/
			TiXmlElement *pXmlElemtChild = pXmlElemt->FirstChildElement();

			while (pXmlElemtChild)
			{
				CString cstrElemtChildName(pXmlElemtChild->Value()); /*!< 子节点名称 */

				CString cstrArea = _T("");		/*!< 排列 */
				CString cstrRest = _T("");		/*!< 待机位 */
				CString cstrStop = _T("");		/*!< 禁行地标卡组 */

				if (cstrElemtChildName == _T("Area"))
				{
					TiXmlAttribute *pAttrChild = pXmlElemtChild->FirstAttribute(); /*!< 子节点属性 */

					/*!
					 * 队列排序属性
					*/
					while (pAttrChild)
					{
						CString cstrAttrName(pAttrChild->Name());	/*!< 属性名称*/
						CString cstrAttrValue(pAttrChild->Value()); /*!< 属性值 */

						pAttrChild = pAttrChild->Next();

						if (cstrAttrName == _T("No"))
						{
							cstrArea = cstrAttrValue;
						}
					}

					/*!
					 * 队列成员
					*/
					TiXmlElement *pXmlElemtGrandson = pXmlElemtChild->FirstChildElement();

					while (pXmlElemtGrandson)
					{
						CString cstrElemtGrandsonName(pXmlElemtGrandson->Value()); /*!< 子节点名称 */

						if (cstrElemtGrandsonName == _T("Rest"))
						{
							TiXmlAttribute *pAttrGrandson = pXmlElemtGrandson->FirstAttribute(); /*!< 子节点属性 */

							/*!
							 * 队列成员属性
							*/
							while (pAttrGrandson)
							{
								CString cstrAttrName(pAttrGrandson->Name());	/*!< 属性名称 */
								CString cstrAttrValue(pAttrGrandson->Value());	/*!< 属性值 */

								pAttrGrandson = pAttrGrandson->Next();

								if (cstrAttrName == _T("No"))
								{
									cstrRest = cstrAttrValue;
								}
							}
						}
						else if (cstrElemtGrandsonName == _T("Stop"))
						{
							TiXmlAttribute *pAttrGrandson = pXmlElemtGrandson->FirstAttribute(); /*!< 子节点属性 */

							/*!
							 * 队列成员属性
							*/
							while (pAttrGrandson)
							{
								CString cstrAttrName(pAttrGrandson->Name());	/*!< 属性名称 */
								CString cstrAttrValue(pAttrGrandson->Value());	/*!< 属性值 */

								pAttrGrandson = pAttrGrandson->Next();

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

						pXmlElemtGrandson = pXmlElemtGrandson->NextSiblingElement();
					}
				}

				pXmlElemtChild = pXmlElemtChild->NextSiblingElement();

				int nRow = m_listRestQueue.InsertItem(m_listRestQueue.GetItemCount(), cstrQueue);
				m_listRestQueue.SetItemText(nRow, RESTQUEUELIST_SORT, cstrArea);
				m_listRestQueue.SetItemText(nRow, RESTQUEUELIST_REST, cstrRest);
				m_listRestQueue.SetItemText(nRow, RESTQUEUELIST_STOP, cstrStop);
			}
		}

		pXmlElemt = pXmlElemt->NextSiblingElement();
	}

	SortList();

	TRACE(_T("待机队列信息导入完成.\r\n"));

	return true;
}

void CRestQueueListDlg::InitList()
{
	m_listRestQueue.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listRestQueue.InsertColumn(RESTQUEUELIST_NO + 1, _T("编号"), LVCFMT_CENTER);
	m_listRestQueue.InsertColumn(RESTQUEUELIST_SORT + 1, _T("排序"), LVCFMT_CENTER);
	m_listRestQueue.InsertColumn(RESTQUEUELIST_REST + 1, _T("待机位"), LVCFMT_CENTER);
	m_listRestQueue.InsertColumn(RESTQUEUELIST_STOP + 1, _T("禁行地标卡组"), LVCFMT_CENTER);
	m_listRestQueue.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_listRestQueue.DeleteColumn(0);

	return;
}

void CRestQueueListDlg::ListSize()
{
	if (GetDlgItem(IDC_BUTTON_DEL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_DELALL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_EDIT)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_ADD)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_UP)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_DOWN)->GetSafeHwnd() == nullptr
		|| m_listRestQueue.GetSafeHwnd() == nullptr)
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
	int nDis = (rect.Width() - 6 * nWidth) / 7;

	GetDlgItem(IDC_BUTTON_ADD)->MoveWindow(CRect(rect.left + nDis, rect.top, rect.left + nDis + nWidth, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_EDIT)->MoveWindow(CRect(rect.left + nDis * 2 + nWidth, rect.top, rect.left + nDis * 2 + nWidth * 2, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DEL)->MoveWindow(CRect(rect.left + nDis * 3 + nWidth * 2, rect.top, rect.left + nDis * 3 + nWidth * 3, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DELALL)->MoveWindow(CRect(rect.left + nDis * 4 + nWidth * 3, rect.top, rect.left + nDis * 4 + nWidth * 4, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_UP)->MoveWindow(CRect(rect.left + nDis * 5 + nWidth * 4, rect.top, rect.left + nDis * 5 + nWidth * 5, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DOWN)->MoveWindow(CRect(rect.left + nDis * 6 + nWidth * 5, rect.top, rect.left + nDis * 6 + nWidth * 6, rect.top + nHeight));

	rect.top += nHeight + 5;
	m_listRestQueue.MoveWindow(rect);

	/*!
	 * 调整列表中列的宽度
	*/
	m_listRestQueue.GetClientRect(&rect);

	m_listRestQueue.SetColumnWidth(RESTQUEUELIST_NO, rect.Width() / 6);
	m_listRestQueue.SetColumnWidth(RESTQUEUELIST_SORT, rect.Width() / 6);
	m_listRestQueue.SetColumnWidth(RESTQUEUELIST_REST, rect.Width() / 6);
	m_listRestQueue.SetColumnWidth(RESTQUEUELIST_STOP, rect.Width() * 3 / 6);

	return;
}

BOOL CRestQueueListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CRestQueueListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ListSize();
}

BOOL CRestQueueListDlg::PreTranslateMessage(MSG * pMsg)
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

void CRestQueueListDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CSetRestQueueDlg setRestQueueDlg;

	if (setRestQueueDlg.DoModal() == IDOK)
	{
		for (int i = 0; i < m_listRestQueue.GetItemCount(); i++)
		{
			if (setRestQueueDlg.m_unRest == _ttoi(m_listRestQueue.GetItemText(i, RESTQUEUELIST_REST)))
			{
				MessageBox(_T("已存在相同编号的待机位!"), _T("添加失败"), MB_ICONERROR);
				return;
			}
		}

		CString cstrText = _T("");
		cstrText.Format(_T("%d"), setRestQueueDlg.m_unQueue);

		int nRow = m_listRestQueue.InsertItem(m_listRestQueue.GetItemCount(), cstrText);

		cstrText.Format(_T("%d"), setRestQueueDlg.m_unRest);
		m_listRestQueue.SetItemText(nRow, RESTQUEUELIST_REST, cstrText);
		m_listRestQueue.SetItemText(nRow, RESTQUEUELIST_STOP, setRestQueueDlg.m_cstrStop);

		SortList();
	}

	return;
}

void CRestQueueListDlg::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listRestQueue.GetItemCount(); i++)
	{
		if (m_listRestQueue.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			CSetRestQueueDlg setRestQueueDlg;

			setRestQueueDlg.m_unQueue = _ttoi(m_listRestQueue.GetItemText(i, RESTQUEUELIST_NO));
			setRestQueueDlg.m_unRest = _ttoi(m_listRestQueue.GetItemText(i, RESTQUEUELIST_REST));
			setRestQueueDlg.m_cstrStop = m_listRestQueue.GetItemText(i, RESTQUEUELIST_STOP);
			setRestQueueDlg.m_bEdit = true;

			if (setRestQueueDlg.DoModal() == IDOK)
			{
				for (int l = 0; l < m_listRestQueue.GetItemCount(); l++)
				{
					if (l == i)
					{
						continue;
					}

					if (setRestQueueDlg.m_unRest == _ttoi(m_listRestQueue.GetItemText(l, RESTQUEUELIST_REST)))
					{
						MessageBox(_T("已存在相同编号的待机位!"), _T("添加失败"), MB_ICONERROR);
						return;
					}
				}

				CString cstrText = _T("");
				cstrText.Format(_T("%d"), setRestQueueDlg.m_unRest);
				m_listRestQueue.SetItemText(i, RESTQUEUELIST_REST, cstrText);
				m_listRestQueue.SetItemText(i, RESTQUEUELIST_STOP, setRestQueueDlg.m_cstrStop);
			}

			return;
		}
	}

	return;
}

void CRestQueueListDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listRestQueue.GetItemCount();)
	{
		if (m_listRestQueue.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			m_listRestQueue.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	SortList();

	return;
}

void CRestQueueListDlg::OnBnClickedButtonDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listRestQueue.DeleteAllItems();
	return;
}

void CRestQueueListDlg::OnBnClickedButtonUp()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCol = 4;

	for (int i = 0; i < m_listRestQueue.GetItemCount(); i++)
	{
		if (m_listRestQueue.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			/*!
			 * 在列表顶端无法上移
			*/
			if (i == 0)
			{
				MessageBox(_T("在列表顶端无法上移"), _T("移动失败"), MB_ICONERROR);
				return;
			}

			int nChange = i - 1; /*!< 交换列号 */

			CString *pCStrList = new CString[nCol];

			for (int c = 0; c < nCol; c++)
			{
				pCStrList[c] = m_listRestQueue.GetItemText(nChange, c);
			}

			/*!
			 * 无法移动至不同队列
			*/
			if (pCStrList[0] != m_listRestQueue.GetItemText(i, RESTQUEUELIST_NO))
			{
				MessageBox(_T("无法移动至不同队列中"),_T("移动失败"),MB_ICONERROR);
				return;
			}

			for (int c = 0; c < nCol; c++)
			{
				m_listRestQueue.SetItemText(nChange, c, m_listRestQueue.GetItemText(i, c));
			}

			for (int c = 0; c < nCol; c++)
			{
				m_listRestQueue.SetItemText(i, c, pCStrList[c]);
			}
			
			delete[] pCStrList;

			break;
		}
	}

	SortList();

	return;
}

void CRestQueueListDlg::OnBnClickedButtonDown()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCol = 4;

	for (int i = 0; i < m_listRestQueue.GetItemCount(); i++)
	{
		if (m_listRestQueue.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			if (i == m_listRestQueue.GetItemCount()-1)
			{
				MessageBox(_T("在列表底端无法下移"), _T("移动失败"), MB_ICONERROR);
				return;
			}

			int nChange = i + 1; /*!< 交换列号 */

			CString *pCStrList = new CString[nCol];

			for (int c = 0; c < nCol; c++)
			{
				pCStrList[c] = m_listRestQueue.GetItemText(nChange, c);
			}

			/*!
			 * 无法移动至不同队列
			*/
			if (pCStrList[0] != m_listRestQueue.GetItemText(i, RESTQUEUELIST_NO))
			{
				MessageBox(_T("无法移动至不同队列中"), _T("移动失败"), MB_ICONERROR);
				return;
			}

			for (int c = 0; c < nCol; c++)
			{
				m_listRestQueue.SetItemText(nChange, c, m_listRestQueue.GetItemText(i, c));
			}

			for (int c = 0; c < nCol; c++)
			{
				m_listRestQueue.SetItemText(i, c, pCStrList[c]);
			}

			delete[] pCStrList;

			break;
		}
	}

	SortList();

	return;
}