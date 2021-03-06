#pragma once


// CSetRestDlg 对话框

class CSetRestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetRestDlg)

public:
	CSetRestDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetRestDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETREST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	unsigned int m_unNo;	/*!< 编号 */
	unsigned int m_unMark;	/*!< 待机位所在地标卡编号*/
	bool m_bEdit;			/*!< 编辑标识 */

public:
	afx_msg void OnEnChangeEditNo();
	afx_msg void OnEnChangeEditMark();
	virtual BOOL OnInitDialog();
};
