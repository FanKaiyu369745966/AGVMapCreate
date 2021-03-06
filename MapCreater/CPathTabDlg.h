#pragma once
#include "CTrafficListDlg.h"
#include "CRestQueueListDlg.h"

// CPathTabDlg 对话框

class CPathTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPathTabDlg)

public:
	CPathTabDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPathTabDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHTAB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	enum TABDLG
	{
		TABDLG_TRAFFIC,		/*!< 交通管制列表框 */
		TABDLG_RESTQUEUE,	/*!< 待机队列列表框 */
	};
public:
	CTabCtrl m_tabPath;
	CTrafficListDlg m_dlgTrafficList;
	CRestQueueListDlg m_dlgRestQueueList;

protected:
	/*!
	 * @brief 初始化Tab
	*/
	void InitTab();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTcnSelchangeTabPath(NMHDR *pNMHDR, LRESULT *pResult);
};
