
// MapCreaterView.h: CMapCreaterView 类的接口
//

#pragma once
#include <mutex>
#include <map>
#include <list>
#include "CGraphicBackground.h"
#include "CGraphicAGV.h"
#include "CGraphicMark.h"
#include "CGraphicStation.h"
#include "CGraphicCharger.h"
#include "CGraphicRest.h"
#include "CGraphicTrack.h"
#include "MapCreaterDoc.h"

using namespace std;

class CMapCreaterView : public CView
{
protected: // 仅从序列化创建
	CMapCreaterView() noexcept;
	DECLARE_DYNCREATE(CMapCreaterView)

// 特性
public:
	CMapCreaterDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CMapCreaterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	mutex m_mutexLock;									/*!< 互斥锁 */
	bool m_bCtrlKey;									/*!< Control键标识 */
	HACCEL m_hAccel;									/*!< 快捷键 */
	CString m_cstrFilePath;								/*!< 文件路径 */

protected:
	CGraphicBackground *m_pBackground;					/*!< 背景图指针 */
	map<unsigned int, CGraphicAGV*> m_mapAGVs;			/*!< AGV集合 */
	map<unsigned int, CGraphicMark*> m_mapMarks;		/*!< 地标卡集合 */
	map<unsigned int, CGraphicStation*> m_mapStations;	/*!< 工位集合 */
	map<unsigned int, CGraphicCharger*> m_mapChargers;	/*!< 充电器集合 */
	map<unsigned int, CGraphicRest*> m_mapRests;		/*!< 待机位集合 */
	list<CGraphicTrack*> m_listTracks;					/*!< 轨道列表 */
	CGraphicMark *m_pMark;
	CGraphicStation *m_pStation;
	CGraphicCharger *m_pCharger;
	CGraphicRest *m_pRest;
	CGraphicTrack *m_pTrack;

protected:
	/*!
	 * @brief 取消
	 *
	 * 取消未执行完成的添加动作
	*/
	void Cancel();

	/*!
	 * @brief 添加地标卡
	 *
	 * 通知列表添加地标卡
	 * @param pMark 地标卡图形指针
	*/
	void AddMark(CGraphicMark *pMark);

	/*!
	 * @brief 删除地标卡
	 *
	 * 通知列表删除地标卡
	 * @param pMark 地标卡图形指针
	*/
	void DeleteMark(CGraphicMark *pMark);

	/*!
	 * @brief 添加工位
	 *
	 * 通知列表添加工位
	 * @param pStation 工位图形指针
	*/
	void AddStation(CGraphicStation *pStation);

	/*!
	 * @brief 删除工位
	 *
	 * 通知列表删除工位
	 * @param pStation 工位图形指针
	*/
	void DeleteStation(CGraphicStation *pStation);

	/*!
	 * @brief 添加待机位
	 *
	 * 通知列表添加待机位
	 * @param pRest 待机位图形指针
	*/
	void AddRest(CGraphicRest *pRest);

	/*!
	 * @brief 删除待机位
	 *
	 * 通知列表删除待机位
	 * @param pRest 待机位图形指针
	*/
	void DeleteRest(CGraphicRest *pRest);

	/*!
	 * @brief 添加充电器
	 *
	 * 通知列表添加充电器
	 * @param pCharger 充电器图形指针
	*/
	void AddCharger(CGraphicCharger *pCharger);

	/*!
	 * @brief 删除充电器
	 *
	 * 通知列表删除充电器
	 * @param pCharger 充电器图形指针
	*/
	void DeleteCharger(CGraphicCharger *pCharger);

	/*!
	 * @brief 导出
	 * @param cstrFilePath 文件路径
	 * @return bool 导出成功返回true,失败返回false
	*/
	bool Output(CString cstrFilePath);

	/*!
	 * @brief 导入
	 * @param cstrFilePath 文件路径
	 * @return bool 导出成功返回true,失败返回false
	*/
	bool Input(CString cstrFilePath);

	/*!
	 * @brief 导入全部
	 * @param cstrFilePath 文件路径
	 * @return bool 导出成功返回true,失败返回false
	*/
	bool OutputAll(CString cstrFilePath);

	/*!
	 * @brief 导入
	 * @param cstrFilePath 文件路径
	 * @return bool 导出成功返回true,失败返回false
	*/
	bool InputAll(CString cstrFilePath);

public:
	/*!
	 * @brief 删除地标卡
	 *
	 * 删除地标卡界面图形
	 * @param unMark 地标卡编号
	*/
	void DeleteMark(unsigned int unMark);

	/*!
	 * @brief 定位地标卡
	 * @param unMark 地标卡编号
	*/
	void LocateMark(unsigned int unMark);

	/*!
	 * @brief 删除全部地标卡
	*/
	void DeleteAllMark();

	/*!
	 * @brief 删除工位
	 *
	 * 删除工位界面图形
	 * @param unStation 工位编号
	*/
	void DeleteStation(unsigned int unStation);

	/*!
	 * @brief 定位工位
	 * @param unStation 工位编号
	*/
	void LocateStation(unsigned int unStation);

	/*!
	 * @brief 删除全部工位
	*/
	void DeleteAllStation();

	/*!
	 * @brief 更新工位属性
	 * @param unStation 工位编号
	 * @param unMark 工位所在地标卡编号
	*/
	void UpdateStation(unsigned int unStation,CString cstrName,unsigned int unMark);

	/*!
	 * @brief 删除待机位
	 *
	 * 删除待机位界面图形
	 * @param unRest 待机位编号
	*/
	void DeleteRest(unsigned int unRest);

	/*!
	 * @brief 定位待机位
	 * @param unRest 待机位编号
	*/
	void LocateRest(unsigned int unRest);

	/*!
	 * @brief 删除全部待机位
	*/
	void DeleteAllRest();

	/*!
	 * @brief 更新待机位属性
	 * @param unRest 待机位编号
	 * @param unMark 待机位所在地标卡编号
	*/
	void UpdateRest(unsigned int unRest,unsigned int unMark);

	/*!
	 * @brief 删除充电器
	 *
	 * 删除充电器界面图形
	 * @param unCharger 充电器编号
	*/
	void DeleteCharger(unsigned int unCharger);

	/*!
	 * @brief 定位充电器
	 * @param unCharger 充电器编号
	*/
	void LocateCharger(unsigned int unCharger);

	/*!
	 * @brief 删除全部充电器
	*/
	void DeleteAllCharger();

	/*!
	 * @brief 更新待机位属性
	 * @param unCharger 充电器编号
	 * @param unCtrl 充电器控制器编号
	 * @param unMark 充电器所在地标卡编号
	*/
	void UpdateCharger(unsigned int unCharger,unsigned int unCtrl, unsigned int unMark);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnMapReset();
	afx_msg void OnMapMark();
	afx_msg void OnMapStation();
	afx_msg void OnMapRest();
	afx_msg void OnMapCharger();
	afx_msg void OnTrackLine();
	afx_msg void OnTrackArc();
	afx_msg void OnMapDelete();
	afx_msg void OnMapSelectall();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFileIn();
	afx_msg void OnFileOut();
	afx_msg void OnExit();
	afx_msg void OnFileSave();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // MapCreaterView.cpp 中的调试版本
inline CMapCreaterDoc* CMapCreaterView::GetDocument() const
   { return reinterpret_cast<CMapCreaterDoc*>(m_pDocument); }
#endif

