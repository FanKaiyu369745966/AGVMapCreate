#pragma once


// CAGVListDlg 对话框

class CAGVListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAGVListDlg)

public:
	CAGVListDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAGVListDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AGVLIST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	enum AGVLIST
	{
		AGVLIST_NO,		/*!< 编号 */
		AGVLIST_TYPE,	/*!< 类型 */
		AGVLIST_MOVE,	/*!< 可运动方向 */
		AGVLIST_IP,		/*!< IP地址 */
		AGVLIST_PORT,	/*!< 端口 */
	};

public:
	CListCtrl m_listAGV;

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
