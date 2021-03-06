// CSetMarkDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CSetMarkDlg.h"
#include "afxdialogex.h"


// CSetMarkDlg 对话框

IMPLEMENT_DYNAMIC(CSetMarkDlg, CDialogEx)

CSetMarkDlg::CSetMarkDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETMARK_DIALOG, pParent)
	, m_unNo(1)
{

}

CSetMarkDlg::~CSetMarkDlg()
{
}

void CSetMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NO, m_unNo);
	DDV_MinMaxUInt(pDX, m_unNo, 1, 65535);
}


BEGIN_MESSAGE_MAP(CSetMarkDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_NO, &CSetMarkDlg::OnEnChangeEditNo)
END_MESSAGE_MAP()


// CSetMarkDlg 消息处理程序


void CSetMarkDlg::OnEnChangeEditNo()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	return;
}