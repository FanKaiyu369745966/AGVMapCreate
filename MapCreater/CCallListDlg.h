#pragma once


// CCallListDlg 对话框

class CCallListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCallListDlg)

public:
	CCallListDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCallListDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALLLIST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	enum CALLLIST
	{
		CALLLIST_NO,		/*!< 编号 */
		CALLLIST_STATION,	/*!< 工位 */
		CALLLIST_RESTQUEUE,	/*!< 待机队列 */
		CALLLIST_AGV,		/*!< AGV类型 */
		CALLLIST_MSG,		/*!< 附带信息 */
	};

public:
	CListCtrl m_listCall;	/*!< 无线呼叫器列表 */

protected:
	/*!
	 * @brief 为列表成员排序
	*/
	void SortList();

public:
	/*!
	 * @brief 导出
	 *
	 * 将配置信息导出至文件
	 * @param cstrFilePath 文件路径
	 * @return bool 导出成功返回true,失败返回false
	 */
	bool Output(CString cstrFilePath);

	/*!
	 * @brief 导入
	 *
	 * 将文件中的配置信息导入
	 * @param cstrFilePath 文件路径
	 * @return bool 导入成功返回true,失败返回false
	*/
	bool Input(CString cstrFilePath);

protected:
	/*!
	 * @brief 初始化列表
	*/
	void InitList();

	/*!
	 * @brief 列表尺寸变更
	*/
	void ListSize();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonDelall();
};
