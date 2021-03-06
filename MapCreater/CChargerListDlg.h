#pragma once


// CChargerListDlg 对话框

class CChargerListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChargerListDlg)

public:
	CChargerListDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CChargerListDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARGERLIST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCharger;

protected:
	/*!
	 * @brief 为列表成员排序
	*/
	void SortList();

	/*!
	 * @brief 删除列表成员
	 *
	 * 通知界面删除充电器图形
	 * @param unCharger 充电器编号
	*/
	void Delete(unsigned int unCharger);

	/*!
	 * @brief 删除全部成员
	 *
	 * 通知界面删除所有充电器图形
	*/
	void DeleteAll();

	/*!
	 * @brief 定位
	 *
	 * 通知界面定位充电器图形
	 * @param unCharger 充电器编号
	*/
	void Locate(unsigned int unCharger);

	/*!
	 * @brief 更新列表成员
	 *
	 * 通知界面更新充电器图形
	 * @param unCharger 充电器编号
	 * @param unCtrl 充电器控制器编号
	 * @param unMark 充电器所在地标卡编号
	*/
	void Update(unsigned int unCharger, unsigned int unCtrl, unsigned int unMark);

public:
	/*!
	 * @brief 添加列表成员
	 * @param unCharger 充电器编号
	 * @param ptCenter 充电器中心坐标
	 * @param unCtrl 充电器控制器编号
	 * @param unMark 充电器所在地标卡编号
	*/
	void Add(unsigned int unCharger, CPoint ptCenter, unsigned int unCtrl, unsigned int unMark);

	/*!
	 * @brief 删除列表成员
	 * @param unCharger 充电器编号
	*/
	void DeleteItem(unsigned int unCharger);

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
	afx_msg void OnBnClickedButtonLocation();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonDelall();
};
