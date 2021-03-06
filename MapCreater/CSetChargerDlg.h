#pragma once


// CSetChargerDlg 对话框

class CSetChargerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetChargerDlg)

public:
	CSetChargerDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetChargerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETCHARGER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	unsigned int m_unNo;	/*!< 编号 */
	unsigned int m_unMark;	/*!< 充电器所在地标卡编号 */
	unsigned int m_unCtrl;	/*!< 受控充电器控制器编号 */
	bool m_bEdit;			/*!< 编辑标识 */
public:
	afx_msg void OnEnChangeEditNo();
	afx_msg void OnEnChangeEditMark();
	afx_msg void OnEnChangeEditCtrl();
	virtual BOOL OnInitDialog();
};
