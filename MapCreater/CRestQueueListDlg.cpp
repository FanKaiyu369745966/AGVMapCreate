﻿// CRestQueueListDlg.cpp: 实现文件
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
END_MESSAGE_MAP()


// CRestQueueListDlg 消息处理程序

bool CRestQueueListDlg::Output(CString cstrFilePath)
{
	TRACE(_T("开始导出待机队列信息.\r\n"));

	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("待机队列信息导出失败.\r\n"));
		return false;
	}

	TiXmlElement* pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL)
	{
		TRACE(_T("待机队列信息导出失败.\r\n"));
		return false;
	}

	/*!
	 * 写入待机对列信息
	*/
	TiXmlElement* pElement = nullptr;
	CString cstrQueue = _T("");	/*!< 当前队列 */

	for (int i = 0; i < m_listRestQueue.GetItemCount(); i++)
	{
		TiXmlElement element("RestQueue");	/*!< 子节点 */
		element.SetAttribute("No", CStringA(m_listRestQueue.GetItemText(i, RESTQUEUELIST_NO)));

		/*!
		 * 拆分禁行地标卡
		*/
		CString cstrStop = m_listRestQueue.GetItemText(i, RESTQUEUELIST_STOP);
		int nStart = 0;
		int nEnd = 0;
		do
		{
			TiXmlElement elementBrother("Rester");

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
			element.InsertEndChild(elementBrother);
		} while (nEnd != -1);

		pXmlRoot->InsertEndChild(element);
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

	TiXmlElement* pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL || pXmlRoot->NoChildren())
	{
		TRACE(_T("待机队列信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement* pXmlElemt = pXmlRoot->FirstChildElement(); /*!< 子节点*/

	while (pXmlElemt)
	{
		CString cstrElemtName(pXmlElemt->Value()); /*!< 子节点名称 */

		/*!
		 * 读取待机对列信息
		*/
		if (cstrElemtName == _T("RestQueue"))
		{
			TiXmlAttribute* pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			CString cstrQueue = _T("");		/*!< 编号 */
			CString cstrStop = _T("");		/*!< 禁行地标卡组 */

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
			TiXmlElement* pXmlElemtChild = pXmlElemt->FirstChildElement();

			while (pXmlElemtChild)
			{
				CString cstrElemtChildName(pXmlElemtChild->Value()); /*!< 子节点名称 */

				if (cstrElemtChildName == _T("Rester"))
				{
					TiXmlAttribute* pAttrGrandson = pXmlElemtChild->FirstAttribute(); /*!< 子节点属性 */

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

				pXmlElemtChild = pXmlElemtChild->NextSiblingElement();
			}

			int nRow = m_listRestQueue.InsertItem(m_listRestQueue.GetItemCount(), cstrQueue);
			m_listRestQueue.SetItemText(nRow, RESTQUEUELIST_STOP, cstrStop);
		}

		pXmlElemt = pXmlElemt->NextSiblingElement();
	}

	TRACE(_T("待机队列信息导入完成.\r\n"));

	return true;
}

void CRestQueueListDlg::InitList()
{
	m_listRestQueue.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listRestQueue.InsertColumn(RESTQUEUELIST_NO + 1, _T("编号"), LVCFMT_CENTER);
	m_listRestQueue.InsertColumn(RESTQUEUELIST_STOP + 1, _T("成员"), LVCFMT_CENTER);
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
	int nDis = (rect.Width() - 4 * nWidth) / 5;

	GetDlgItem(IDC_BUTTON_ADD)->MoveWindow(CRect(rect.left + nDis, rect.top, rect.left + nDis + nWidth, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_EDIT)->MoveWindow(CRect(rect.left + nDis * 2 + nWidth, rect.top, rect.left + nDis * 2 + nWidth * 2, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DEL)->MoveWindow(CRect(rect.left + nDis * 3 + nWidth * 2, rect.top, rect.left + nDis * 3 + nWidth * 3, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DELALL)->MoveWindow(CRect(rect.left + nDis * 4 + nWidth * 3, rect.top, rect.left + nDis * 4 + nWidth * 4, rect.top + nHeight));

	rect.top += nHeight + 5;
	m_listRestQueue.MoveWindow(rect);

	/*!
	 * 调整列表中列的宽度
	*/
	m_listRestQueue.GetClientRect(&rect);

	m_listRestQueue.SetColumnWidth(RESTQUEUELIST_NO, rect.Width() / 4);
	m_listRestQueue.SetColumnWidth(RESTQUEUELIST_STOP, rect.Width() * 3 / 4);

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

BOOL CRestQueueListDlg::PreTranslateMessage(MSG* pMsg)
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
			if (setRestQueueDlg.m_unQueue == _ttoi(m_listRestQueue.GetItemText(i, RESTQUEUELIST_NO)))
			{
				MessageBox(_T("已存在相同编号的待机队列!"), _T("添加失败"), MB_ICONERROR);
				return;
			}
		}

		CString cstrText = _T("");
		cstrText.Format(_T("%d"), setRestQueueDlg.m_unQueue);

		int nRow = m_listRestQueue.InsertItem(m_listRestQueue.GetItemCount(), cstrText);

		cstrText.Format(_T("%d"), setRestQueueDlg.m_unRest);
		m_listRestQueue.SetItemText(nRow, RESTQUEUELIST_STOP, setRestQueueDlg.m_cstrStop);
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
			setRestQueueDlg.m_cstrStop = m_listRestQueue.GetItemText(i, RESTQUEUELIST_STOP);
			setRestQueueDlg.m_bEdit = true;

			if (setRestQueueDlg.DoModal() == IDOK)
			{
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

	return;
}

void CRestQueueListDlg::OnBnClickedButtonDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listRestQueue.DeleteAllItems();
	return;
}
