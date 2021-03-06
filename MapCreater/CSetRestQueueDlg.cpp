// CSetRestQueueDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CSetRestQueueDlg.h"
#include "afxdialogex.h"


// CSetRestQueueDlg 对话框

IMPLEMENT_DYNAMIC(CSetRestQueueDlg, CDialogEx)

CSetRestQueueDlg::CSetRestQueueDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETRESTQUEUE_DIALOG, pParent)
	, m_unQueue(1)
	, m_unRest(1)
	, m_cstrStop(_T(""))
{
	m_cstrStopHelp = _T("禁行地标卡组。地标卡号与地标卡号之间使用(,)分割。");
	m_bEdit = false;
}

CSetRestQueueDlg::~CSetRestQueueDlg()
{
}

void CSetRestQueueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_QUEUE, m_unQueue);
	DDV_MinMaxUInt(pDX, m_unQueue, 1, 65535);
	DDX_Text(pDX, IDC_EDIT_REST, m_unRest);
	DDV_MinMaxUInt(pDX, m_unRest, 1, 65535);
	DDX_Text(pDX, IDC_EDIT_STOP, m_cstrStop);
}


BEGIN_MESSAGE_MAP(CSetRestQueueDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_EDIT_STOP, &CSetRestQueueDlg::OnKillfocusEditStop)
	ON_EN_SETFOCUS(IDC_EDIT_STOP, &CSetRestQueueDlg::OnSetfocusEditStop)
	ON_EN_CHANGE(IDC_EDIT_QUEUE, &CSetRestQueueDlg::OnEnChangeEditQueue)
	ON_EN_CHANGE(IDC_EDIT_REST, &CSetRestQueueDlg::OnEnChangeEditRest)
	ON_EN_CHANGE(IDC_EDIT_STOP, &CSetRestQueueDlg::OnEnChangeEditStop)
	ON_BN_CLICKED(IDOK, &CSetRestQueueDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetRestQueueDlg 消息处理程序


BOOL CSetRestQueueDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_cstrStop.IsEmpty())
	{
		GetDlgItem(IDC_EDIT_STOP)->SetWindowText(m_cstrStopHelp);
	}

	if(m_bEdit)
	{
		GetDlgItem(IDC_EDIT_QUEUE)->EnableWindow(false);
	}

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CSetRestQueueDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if (IDC_STATIC_STOPHELP == pWnd->GetDlgCtrlID())
	{
		// 红色
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	else
	{
		if (GetDlgItem(pWnd->GetDlgCtrlID())->GetSafeHwnd() == false)
		{
			return hbr;
		}

		CString cstrText = _T("");
		GetDlgItem(pWnd->GetDlgCtrlID())->GetWindowText(cstrText);

		if (cstrText == m_cstrStopHelp)
		{
			//灰色
			pDC->SetTextColor(RGB(192, 192, 192));
		}
		else
		{
			// 黑色
			pDC->SetTextColor(RGB(0, 0, 0));
		}
	}

	return hbr;
}


void CSetRestQueueDlg::OnKillfocusEditStop()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cstrText = _T("");
	GetDlgItem(IDC_EDIT_STOP)->GetWindowText(cstrText);

	if (cstrText.IsEmpty())
	{
		GetDlgItem(IDC_EDIT_STOP)->SetWindowText(m_cstrStopHelp);
	}
}


void CSetRestQueueDlg::OnSetfocusEditStop()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cstrText = _T("");
	GetDlgItem(IDC_EDIT_STOP)->GetWindowText(cstrText);

	if (cstrText == m_cstrStopHelp)
	{
		GetDlgItem(IDC_EDIT_STOP)->SetWindowText(_T(""));
	}
}


void CSetRestQueueDlg::OnEnChangeEditQueue()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
}


void CSetRestQueueDlg::OnEnChangeEditRest()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
}


void CSetRestQueueDlg::OnEnChangeEditStop()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	if (m_cstrStop == m_cstrStopHelp)
	{
		return;
	}

	for (int i = 0; i < m_cstrStop.GetLength(); i++)
	{
		WCHAR wchText = m_cstrStop.GetAt(i);

		if (wchText != _T(',')
			&& (wchText < _T('0') || wchText > _T('9')))
		{
			MessageBox(_T("仅允许输入','以及数字"), _T("错误"), MB_ICONERROR);

			m_cstrStop.Remove(wchText);

			UpdateData(false);

			((CEdit*)GetDlgItem(IDC_EDIT_STOP))->SetSel(m_cstrStop.GetLength(), m_cstrStop.GetLength(), TRUE);

			return;
		}
	}
}


void CSetRestQueueDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();

	if (m_cstrStop == m_cstrStopHelp)
	{
		m_cstrStop = _T("");
	}
}
