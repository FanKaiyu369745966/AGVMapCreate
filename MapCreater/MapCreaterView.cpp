
// MapCreaterView.cpp: CMapCreaterView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MapCreater.h"
#endif

#include "MapCreaterDoc.h"
#include "MapCreaterView.h"
#include "CSetMarkDlg.h"
#include "CSetStationDlg.h"
#include "CSetRestDlg.h"
#include "CSetChargerDlg.h"
#include "MainFrm.h"
#include "Xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapCreaterView

IMPLEMENT_DYNCREATE(CMapCreaterView, CView)

BEGIN_MESSAGE_MAP(CMapCreaterView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_ZOOM_IN, &CMapCreaterView::OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, &CMapCreaterView::OnZoomOut)
	ON_COMMAND(ID_MAP_RESET, &CMapCreaterView::OnMapReset)
	ON_COMMAND(ID_MAP_MARK, &CMapCreaterView::OnMapMark)
	ON_COMMAND(ID_MAP_STATION, &CMapCreaterView::OnMapStation)
	ON_COMMAND(ID_MAP_CHARGER, &CMapCreaterView::OnMapCharger)
	ON_COMMAND(ID_MAP_REST, &CMapCreaterView::OnMapRest)
	ON_COMMAND(ID_TRACK_LINE, &CMapCreaterView::OnTrackLine)
	ON_COMMAND(ID_TRACK_ARC, &CMapCreaterView::OnTrackArc)
	ON_COMMAND(ID_MAP_DELETE, &CMapCreaterView::OnMapDelete)
	ON_COMMAND(ID_MAP_SELECTALL, &CMapCreaterView::OnMapSelectall)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_FILE_IN, &CMapCreaterView::OnFileIn)
	ON_COMMAND(ID_FILE_OUT, &CMapCreaterView::OnFileOut)
	ON_COMMAND(ID_EXIT, &CMapCreaterView::OnExit)
	ON_COMMAND(ID_FILE_SAVE, &CMapCreaterView::OnFileSave)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMapCreaterView 构造/析构

CMapCreaterView::CMapCreaterView() noexcept
{
	// TODO: 在此处添加构造代码
	m_bCtrlKey = false;
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

	m_pMark = nullptr;
	m_pStation = nullptr;
	m_pCharger = nullptr;
	m_pRest = nullptr;
	m_pTrack = nullptr;

	m_pBackground = new CGraphicBackground();

	m_cstrFilePath = _T("");
}

CMapCreaterView::~CMapCreaterView()
{
	delete m_pBackground;
	
	Cancel();
	
	for (map<unsigned int, CGraphicAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end();)
	{
		delete it->second;
		it = m_mapAGVs.erase(it);
	}

	for (map<unsigned int, CGraphicMark*>::iterator it = m_mapMarks.begin(); it != m_mapMarks.end();)
	{
		delete it->second;
		it = m_mapMarks.erase(it);
	}

	for (map<unsigned int, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end();)
	{
		delete it->second;
		it = m_mapStations.erase(it);
	}

	for (map<unsigned int, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end();)
	{
		delete it->second;
		it = m_mapChargers.erase(it);
	}

	for (map<unsigned int, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end();)
	{
		delete it->second;
		it = m_mapRests.erase(it);
	}

	for (list<CGraphicTrack*>::iterator it = m_listTracks.begin(); it != m_listTracks.end();)
	{
		delete *it;
		it = m_listTracks.erase(it);
	}
}

BOOL CMapCreaterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMapCreaterView 绘图

void CMapCreaterView::OnDraw(CDC* pDC)
{
	CMapCreaterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	if (m_mutexLock.try_lock() == false)
	{
		return;
	}

	// 不要为绘制消息而调用 CWnd::OnPaint()
	// CDC *pDC = &dc;

	//设置兼容区域，一般为当前窗口大小
	CRect RectCompatible;

	//建立兼容DC，即后面那块画布
	CDC Memdc;
	CBitmap Bitmap;

	//得到当前窗口的大小
	GetClientRect(&RectCompatible);
	Memdc.CreateCompatibleDC(pDC);

	//创建兼容位图
	Bitmap.CreateCompatibleBitmap(pDC, RectCompatible.Width(), RectCompatible.Height());

	//把位图选到兼容DC中，这样才可以开始画
	Memdc.SelectObject(Bitmap);
	Memdc.FillSolidRect(RectCompatible.left, RectCompatible.top, RectCompatible.Width(), RectCompatible.Height(), RGB(255, 255, 255));//用白色先刷新整个兼容DC，这相当于原来的刷新背景

	if (m_pBackground)
	{
		m_pBackground->DrawBackground(&Memdc);
		m_pBackground->DrawLine(&Memdc);
		m_pBackground->DrawPoint(&Memdc);
	}

	for (list<CGraphicTrack*>::iterator it = m_listTracks.begin(); it != m_listTracks.end(); it++)
	{
		(*it)->Draw(&Memdc);
	}

	for (map<unsigned int, CGraphicMark*>::iterator it = m_mapMarks.begin(); it != m_mapMarks.end(); it++)
	{
		it->second->Draw(&Memdc);
	}	

	for (map<unsigned int, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end(); it++)
	{
		it->second->Draw(&Memdc);
	}

	for (map<unsigned int, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end();it++)
	{
		it->second->Draw(&Memdc);
	}

	for (map<unsigned int, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end();it++)
	{
		it->second->Draw(&Memdc);
	}

	for (map<unsigned int, CGraphicAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end();it++)
	{
		it->second->Draw(&Memdc);
	}

	if (m_pMark)
	{
		m_pMark->Draw(&Memdc);
	}

	if (m_pStation)
	{
		m_pStation->Draw(&Memdc);
	}

	if (m_pCharger)
	{
		m_pCharger->Draw(&Memdc);
	}

	if (m_pRest)
	{
		m_pRest->Draw(&Memdc);
	}

	if (m_pTrack)
	{
		m_pTrack->Draw(&Memdc);
	}

	//位图中图片加载到画布上
	//pDC->BitBlt(RectCompatible.left, RectCompatible.top, RectCompatible.Width(), RectCompatible.Height(), &Memdc, RectCompatible.left, RectCompatible.top, SRCCOPY);//最后把兼容DC拷贝到pDC
	pDC->BitBlt(RectCompatible.left, RectCompatible.top, RectCompatible.Width(), RectCompatible.Height(), &Memdc, RectCompatible.left, RectCompatible.top, SRCCOPY);//最后把兼容DC拷贝到pDC
																																									//实现前后交替，便不会闪动
	Memdc.DeleteDC();
	Bitmap.DeleteObject();

	m_mutexLock.unlock();

	return;
}

void CMapCreaterView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	if (m_pBackground)
	{
		m_pBackground->Cancel();
	}

	for (list<CGraphicTrack*>::iterator it = m_listTracks.begin(); it != m_listTracks.end(); it++)
	{
		(*it)->Cancel();
	}

	for (map<unsigned int, CGraphicMark*>::iterator it = m_mapMarks.begin(); it != m_mapMarks.end(); it++)
	{
		it->second->Cancel();
	}

	for (map<unsigned int, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end(); it++)
	{
		it->second->Cancel();
	}

	for (map<unsigned int, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); it++)
	{
		it->second->Cancel();
	}

	for (map<unsigned int, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); it++)
	{
		it->second->Cancel();
	}

	Cancel();

	ClientToScreen(&point);
	//OnContextMenu(this, point);

	return;
}

void CMapCreaterView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMapCreaterView 诊断

#ifdef _DEBUG
void CMapCreaterView::AssertValid() const
{
	CView::AssertValid();
}

void CMapCreaterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapCreaterDoc* CMapCreaterView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapCreaterDoc)));
	return (CMapCreaterDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapCreaterView 消息处理程序


void CMapCreaterView::Cancel()
{
	lock_guard<mutex> lock(m_mutexLock);

	if (m_pMark)
	{
		delete m_pMark;
		m_pMark = nullptr;
	}

	if (m_pStation)
	{
		delete m_pStation;
		m_pStation = nullptr;
	}

	if (m_pCharger)
	{
		delete m_pCharger;
		m_pCharger = nullptr;
	}

	if (m_pRest)
	{
		delete m_pRest;
		m_pRest = nullptr;
	}

	if (m_pTrack)
	{
		delete m_pTrack;
		m_pTrack = nullptr;
	}

	return;
}

void CMapCreaterView::AddMark(CGraphicMark * pMark)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	/*!< 框架指针 */
	pMainFrame->m_paneMapTab.m_dlgMapTab.m_dlgMarkList.Add(pMark->GetNo(), pMark->GetCenterPoint());

	return;
}

void CMapCreaterView::DeleteMark(CGraphicMark * pMark)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	/*!< 框架指针 */
	pMainFrame->m_paneMapTab.m_dlgMapTab.m_dlgMarkList.DeleteItem(pMark->GetNo());

	return;
}

void CMapCreaterView::AddStation(CGraphicStation * pStation)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	/*!< 框架指针 */
	pMainFrame->m_paneMapTab.m_dlgMapTab.m_dlgStationList.Add(pStation->GetNo(), pStation->GetCenterPoint(), pStation->GetName(), pStation->GetMark());

	return;
}

void CMapCreaterView::DeleteStation(CGraphicStation * pStation)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	/*!< 框架指针 */
	pMainFrame->m_paneMapTab.m_dlgMapTab.m_dlgStationList.DeleteItem(pStation->GetNo());

	return;
}

void CMapCreaterView::AddRest(CGraphicRest * pRest)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	/*!< 框架指针 */
	pMainFrame->m_paneMapTab.m_dlgMapTab.m_dlgRestList.Add(pRest->GetNo(), pRest->GetCenterPoint(), pRest->GetMark());

	return;
}

void CMapCreaterView::DeleteRest(CGraphicRest * pRest)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	/*!< 框架指针 */
	pMainFrame->m_paneMapTab.m_dlgMapTab.m_dlgRestList.DeleteItem(pRest->GetNo());

	return;
}

void CMapCreaterView::AddCharger(CGraphicCharger * pCharger)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	/*!< 框架指针 */
	pMainFrame->m_paneMapTab.m_dlgMapTab.m_dlgChargerList.Add(pCharger->GetNo(), pCharger->GetCenterPoint(), pCharger->GetCtrl(), pCharger->GetMark());

	return;
}

void CMapCreaterView::DeleteCharger(CGraphicCharger * pCharger)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	/*!< 框架指针 */
	pMainFrame->m_paneMapTab.m_dlgMapTab.m_dlgChargerList.DeleteItem(pCharger->GetNo());

	return;
}

bool CMapCreaterView::Output(CString cstrFilePath)
{
	TRACE(_T("开始导出地图信息.\r\n"));
	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("地图信息导出失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL)
	{
		TRACE(_T("地图信息导出失败.\r\n"));
		return false;
	}

	TRACE(_T("开始导出背景图信息.\r\n"));
	/*!
	 * 写入背景图信息
	*/
	if (m_pBackground)
	{
		TiXmlElement element("Background");	/*!< 子节点 */

		CStringA cstraArr = "";
		cstraArr.Format("%.2f", CGraph::GetScale());
		element.SetAttribute("Scale", cstraArr);

		cstraArr.Format("%d", CGraph::GetOffset().x);
		element.SetAttribute("OffsetX", cstraArr);

		cstraArr.Format("%d", CGraph::GetOffset().y);
		element.SetAttribute("OffsetY", cstraArr);

		pXmlRoot->InsertEndChild(element);
	}
	TRACE(_T("背景图信息导出完成.\r\n"));

	TRACE(_T("开始导出轨道信息.\r\n"));
	/*!
	 * 写入轨道信息
	*/
	for (list<CGraphicTrack*>::iterator it = m_listTracks.begin(); it != m_listTracks.end(); it++)
	{
		TiXmlElement element("Track");	/*!< 子节点 */

		CStringA cstraArr = "";

		cstraArr.Format("%d", (*it)->GetStartPoint().x);
		element.SetAttribute("StartX", cstraArr);

		cstraArr.Format("%d", (*it)->GetStartPoint().y);
		element.SetAttribute("StartY", cstraArr);

		cstraArr.Format("%d", (*it)->GetEndPoint().x);
		element.SetAttribute("EndX", cstraArr);

		cstraArr.Format("%d", (*it)->GetEndPoint().y);
		element.SetAttribute("EndY", cstraArr);

		cstraArr.Format("%d",(*it)->GetType());
		element.SetAttribute("Arc", cstraArr);

		pXmlRoot->InsertEndChild(element);
	}

	TRACE(_T("轨道信息导出完成.\r\n"));

	TRACE(_T("开始导出地标卡信息.\r\n"));
	/*!
	 * 写入地标卡信息
	*/
	for (map<unsigned int, CGraphicMark*>::iterator it = m_mapMarks.begin(); it != m_mapMarks.end(); it++)
	{
		TiXmlElement element("Mark");	/*!< 子节点 */

		CStringA cstraArr = "";

		cstraArr.Format("%d", it->second->GetNo());
		element.SetAttribute("No", cstraArr);

		cstraArr.Format("%d", it->second->GetCenterPoint().x);
		element.SetAttribute("X", cstraArr);

		cstraArr.Format("%d", it->second->GetCenterPoint().y);
		element.SetAttribute("Y", cstraArr);

		pXmlRoot->InsertEndChild(element);
	}

	TRACE(_T("地标卡信息导出完成.\r\n"));

	TRACE(_T("开始导出工位信息.\r\n"));
	/*!
	 * 写入工位信息
	*/
	for (map<unsigned int, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end(); it++)
	{
		TiXmlElement element("Station");	/*!< 子节点 */

		CStringA cstraArr = "";

		cstraArr.Format("%d", it->second->GetNo());
		element.SetAttribute("No", cstraArr);

		cstraArr.Format("%d", it->second->GetMark());
		element.SetAttribute("Mark", cstraArr);

		cstraArr = CW2A(it->second->GetName(), CP_UTF8);
		element.SetAttribute("Name", cstraArr);

		cstraArr.Format("%d", it->second->GetCenterPoint().x);
		element.SetAttribute("X", cstraArr);

		cstraArr.Format("%d", it->second->GetCenterPoint().y);
		element.SetAttribute("Y", cstraArr);

		pXmlRoot->InsertEndChild(element);
	}

	TRACE(_T("工位信息导出完成.\r\n"));


	TRACE(_T("开始导出充电器信息.\r\n"));
	/*!
	 * 写入充电器信息
	*/
	for (map<unsigned int, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); it++)
	{
		TiXmlElement element("Charger");	/*!< 子节点 */
		CStringA cstraArr = "";

		cstraArr.Format("%d", it->second->GetNo());
		element.SetAttribute("No", cstraArr);

		cstraArr.Format("%d", it->second->GetMark());
		element.SetAttribute("Mark", cstraArr);

		cstraArr.Format("%d", it->second->GetCtrl());
		element.SetAttribute("Ctrl", cstraArr);

		cstraArr.Format("%d", it->second->GetCenterPoint().x);
		element.SetAttribute("X", cstraArr);

		cstraArr.Format("%d", it->second->GetCenterPoint().y);
		element.SetAttribute("Y", cstraArr);

		pXmlRoot->InsertEndChild(element);
	}

	TRACE(_T("充电器信息导出完成.\r\n"));

	TRACE(_T("开始导出待机位信息.\r\n"));
	/*!
	 * 写入待机位信息
	*/
	for (map<unsigned int, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); it++)
	{
		TiXmlElement element("Rest");	/*!< 子节点 */

		CStringA cstraArr = "";

		cstraArr.Format("%d", it->second->GetNo());
		element.SetAttribute("No", cstraArr);

		cstraArr.Format("%d", it->second->GetMark());
		element.SetAttribute("Mark", cstraArr);

		cstraArr.Format("%d", it->second->GetCenterPoint().x);
		element.SetAttribute("X", cstraArr);

		cstraArr.Format("%d", it->second->GetCenterPoint().y);
		element.SetAttribute("Y", cstraArr);

		pXmlRoot->InsertEndChild(element);
	}

	TRACE(_T("待机位信息导出完成.\r\n"));

	TRACE(_T("地图信息导出完成.\r\n"));

	return xmlDoc.SaveFile(CStringA(cstrFilePath));
}

bool CMapCreaterView::Input(CString cstrFilePath)
{
	TRACE(_T("开始导入地图信息.\r\n"));

	Cancel();

	lock_guard<mutex> lock(m_mutexLock);

	for (map<unsigned int, CGraphicMark*>::iterator it = m_mapMarks.begin(); it != m_mapMarks.end();)
	{
		DeleteMark(it->second);
		delete it->second;
		it = m_mapMarks.erase(it);
	}

	for (map<unsigned int, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end();)
	{
		DeleteStation(it->second);
		delete it->second;
		it = m_mapStations.erase(it);
	}

	for (map<unsigned int, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end();)
	{
		DeleteCharger(it->second);
		delete it->second;
		it = m_mapChargers.erase(it);
	}

	for (map<unsigned int, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end();)
	{
		DeleteRest(it->second);
		delete it->second;
		it = m_mapRests.erase(it);
	}

	for (list<CGraphicTrack*>::iterator it = m_listTracks.begin(); it != m_listTracks.end();)
	{
		delete *it;
		it = m_listTracks.erase(it);
	}

	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("地图信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL || pXmlRoot->NoChildren())
	{
		TRACE(_T("地图信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlElemt = pXmlRoot->FirstChildElement(); /*!< 子节点*/

	while (pXmlElemt)
	{
		CString cstrElemtName(pXmlElemt->Value()); /*!< 子节点名称 */

		/*!
		 * 读取背景信息
		*/
		if (cstrElemtName == _T("Background"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			float fScale = 0.1f;	/*!< 缩放比例 */
			CPoint ptOffset = 0;	/*!< 偏移量 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("Scale"))
				{
					fScale = _ttof(cstrAttrValue);
				}
				else if (cstrAttrName == _T("OffsetX"))
				{
					ptOffset.x= _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("OffsetY"))
				{
					ptOffset.y = _ttoi(cstrAttrValue);
				}
			}

			CGraph::SetScale(fScale);
			CGraph::SetOffset(ptOffset);
		}

		/*!
		 * 读取地标卡信息
		*/
		else if (cstrElemtName == _T("Mark"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			unsigned int unNo = 0;	/*!< 编号 */
			CPoint ptCenter = 0;	/*!< 中心坐标 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					unNo = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("X"))
				{
					ptCenter.x = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("Y"))
				{
					ptCenter.y = _ttoi(cstrAttrValue);
				}
			}

			m_mapMarks[unNo] = new CGraphicMark(unNo);
			m_mapMarks[unNo]->SetCenterPoint(ptCenter);
			AddMark(m_mapMarks[unNo]);
		}

		/*!
		 * 读取工位信息
		*/
		else if (cstrElemtName == _T("Station"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			unsigned int unNo = 0;	/*!< 编号 */
			CPoint ptCenter = 0;	/*!< 中心坐标 */
			unsigned int unMark = 0;	/*!< 地标卡 */
			CString cstrName = _T("");	/*!< 名称 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(UTF8toUnicode(pAttr->Value())); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					unNo = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("X"))
				{
					ptCenter.x = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("Y"))
				{
					ptCenter.y = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("Mark"))
				{
					unMark = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("Name"))
				{
					cstrName = cstrAttrValue;
				}
			}

			m_mapStations[unNo] = new CGraphicStation(unNo,unMark,cstrName);
			m_mapStations[unNo]->SetCenterPoint(ptCenter);
			AddStation(m_mapStations[unNo]);
		}

		/*!
		 * 读取待机位信息
		*/
		else if (cstrElemtName == _T("Rest"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			unsigned int unNo = 0;	/*!< 编号 */
			CPoint ptCenter = 0;	/*!< 中心坐标 */
			unsigned int unMark = 0;	/*!< 地标卡 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					unNo = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("X"))
				{
					ptCenter.x = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("Y"))
				{
					ptCenter.y = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("Mark"))
				{
					unMark = _ttoi(cstrAttrValue);
				}
			}

			m_mapRests[unNo] = new CGraphicRest(unNo, unMark);
			m_mapRests[unNo]->SetCenterPoint(ptCenter);
			AddRest(m_mapRests[unNo]);
		}

		/*!
		 * 读取充电器信息
		*/
		else if (cstrElemtName == _T("Charger"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			unsigned int unNo = 0;	/*!< 编号 */
			CPoint ptCenter = 0;	/*!< 中心坐标 */
			unsigned int unMark = 0;	/*!< 地标卡 */
			unsigned int unCtrl = 0;	/*!< 控制器编号 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					unNo = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("X"))
				{
					ptCenter.x = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("Y"))
				{
					ptCenter.y = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("Mark"))
				{
					unMark = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("Ctrl"))
				{
					unCtrl = _ttoi(cstrAttrValue);
				}
			}

			m_mapChargers[unNo] = new CGraphicCharger(unNo, unMark, unCtrl);
			m_mapChargers[unNo]->SetCenterPoint(ptCenter);
			AddCharger(m_mapChargers[unNo]);
		}

		/*!
		 * 读取轨道信息
		*/
		else if (cstrElemtName == _T("Track"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			bool bArc = false; /*!< 弧形标识 */
			CPoint ptStart = 0; /*!< 起点坐标 */
			CPoint ptEnd = 0; /*!< 终点坐标 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("StartX"))
				{
					ptStart.x = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("StartY"))
				{
					ptStart.y = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("EndX"))
				{
					ptEnd.x = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("EndY"))
				{
					ptEnd.y = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("Arc"))
				{
					bArc = _ttoi(cstrAttrValue);
				}
			}

			CGraphicTrack *pTrack = new CGraphicTrack(bArc);
			pTrack->SetStartPoint(ptStart);
			pTrack->SetEndPoint(ptEnd);
			m_listTracks.push_back(pTrack);
		}

		pXmlElemt = pXmlElemt->NextSiblingElement();
	}

	TRACE(_T("地图信息导入完成.\r\n"));
	return true;
}

bool CMapCreaterView::OutputAll(CString cstrFilePath)
{
	CFile file;
	file.Open(m_cstrFilePath, CFile::modeCreate | CFile::shareExclusive);
	file.Close();

	TiXmlDocument doc;//创建XML文档对象
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "utf-8", "");
	doc.LinkEndChild(decl);
	TiXmlElement root("Map");//创建根节点
	doc.InsertEndChild(root);//把根节点放到文档里
	bool bRes = doc.SaveFile(CStringA(m_cstrFilePath));

	Output(cstrFilePath);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	/*!< 框架指针 */
	pMainFrame->m_paneDeviceTab.m_dlgDeviceTab.m_dlgAGVList.Output(cstrFilePath);
	pMainFrame->m_paneDeviceTab.m_dlgDeviceTab.m_dlgCAList.Output(cstrFilePath);
	pMainFrame->m_paneDeviceTab.m_dlgDeviceTab.m_dlgChargerCtrlList.Output(cstrFilePath);
	pMainFrame->m_paneDeviceTab.m_dlgDeviceTab.m_dlgTVList.Output(cstrFilePath);
	pMainFrame->m_paneDeviceTab.m_dlgDeviceTab.m_dlgCallList.Output(cstrFilePath);
	pMainFrame->m_paneDeviceTab.m_dlgDeviceTab.m_dlgWatchList.Output(cstrFilePath);
	pMainFrame->m_panePathTab.m_dlgPathTab.m_dlgTrafficList.Output(cstrFilePath);
	pMainFrame->m_panePathTab.m_dlgPathTab.m_dlgRestQueueList.Output(cstrFilePath);

	return true;
}

bool CMapCreaterView::InputAll(CString cstrFilePath)
{
	Input(cstrFilePath);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	/*!< 框架指针 */
	pMainFrame->m_paneDeviceTab.m_dlgDeviceTab.m_dlgAGVList.Input(cstrFilePath);
	pMainFrame->m_paneDeviceTab.m_dlgDeviceTab.m_dlgCAList.Input(cstrFilePath);
	pMainFrame->m_paneDeviceTab.m_dlgDeviceTab.m_dlgChargerCtrlList.Input(cstrFilePath);
	pMainFrame->m_paneDeviceTab.m_dlgDeviceTab.m_dlgTVList.Input(cstrFilePath);
	pMainFrame->m_paneDeviceTab.m_dlgDeviceTab.m_dlgCallList.Input(cstrFilePath);
	pMainFrame->m_paneDeviceTab.m_dlgDeviceTab.m_dlgWatchList.Input(cstrFilePath);
	pMainFrame->m_panePathTab.m_dlgPathTab.m_dlgTrafficList.Input(cstrFilePath);
	pMainFrame->m_panePathTab.m_dlgPathTab.m_dlgRestQueueList.Input(cstrFilePath);

	return true;
}

void CMapCreaterView::DeleteMark(unsigned int unMark)
{
	Cancel();

	OnRButtonUp(0, 0);

	if (m_mapMarks.find(unMark) == m_mapMarks.end())
	{
		return;
	}

	lock_guard<mutex> lock(m_mutexLock);

	delete m_mapMarks[unMark];
	m_mapMarks.erase(m_mapMarks.find(unMark));

	return;
}

void CMapCreaterView::LocateMark(unsigned int unMark)
{
	Cancel();

	OnRButtonUp(0,0);

	if (m_mapMarks.find(unMark) == m_mapMarks.end())
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);

	CPoint ptWin = CGraph::VecToWin(m_mapMarks[unMark]->GetCenterPoint());

	m_mapMarks[unMark]->Select(ptWin);
	m_mapMarks[unMark]->Confirm();

	CPoint ptDis = CPoint(rect.right / 2, rect.bottom / 2) - ptWin;
	CGraph::SetOffset(ptDis + CGraph::GetOffset());

	return;
}

void CMapCreaterView::DeleteAllMark()
{
	Cancel();

	OnRButtonUp(0, 0);

	lock_guard<mutex> lock(m_mutexLock);

	for (map<unsigned int, CGraphicMark*>::iterator it = m_mapMarks.begin(); it != m_mapMarks.end();)
	{
		delete it->second;
		it = m_mapMarks.erase(it);
	}

	return;
}

void CMapCreaterView::DeleteStation(unsigned int unStation)
{
	Cancel();

	OnRButtonUp(0, 0);

	if (m_mapStations.find(unStation) == m_mapStations.end())
	{
		return;
	}

	lock_guard<mutex> lock(m_mutexLock);

	delete m_mapStations[unStation];
	m_mapStations.erase(m_mapStations.find(unStation));

	return;
}

void CMapCreaterView::LocateStation(unsigned int unStation)
{
	Cancel();

	OnRButtonUp(0, 0);

	if (m_mapStations.find(unStation) == m_mapStations.end())
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);

	CPoint ptWin = CGraph::VecToWin(m_mapStations[unStation]->GetCenterPoint());

	m_mapStations[unStation]->Select(ptWin);
	m_mapStations[unStation]->Confirm();

	CPoint ptDis = CPoint(rect.right / 2, rect.bottom / 2) - ptWin;
	CGraph::SetOffset(ptDis + CGraph::GetOffset());

	return;
}

void CMapCreaterView::DeleteAllStation()
{
	Cancel();

	OnRButtonUp(0, 0);

	lock_guard<mutex> lock(m_mutexLock);

	for (map<unsigned int, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end();)
	{
		delete it->second;
		it = m_mapStations.erase(it);
	}

	return;
}

void CMapCreaterView::UpdateStation(unsigned int unStation, CString cstrName, unsigned int unMark)
{
	Cancel();

	OnRButtonUp(0, 0);

	if (m_mapStations.find(unStation) == m_mapStations.end())
	{
		return;
	}

	m_mapStations[unStation]->SetName(cstrName);
	m_mapStations[unStation]->SetMark(unMark);

	return;
}

void CMapCreaterView::DeleteRest(unsigned int unRest)
{
	Cancel();

	OnRButtonUp(0, 0);

	if (m_mapRests.find(unRest) == m_mapRests.end())
	{
		return;
	}

	lock_guard<mutex> lock(m_mutexLock);

	delete m_mapRests[unRest];
	m_mapRests.erase(m_mapRests.find(unRest));

	return;
}

void CMapCreaterView::LocateRest(unsigned int unRest)
{
	Cancel();

	OnRButtonUp(0, 0);

	if (m_mapRests.find(unRest) == m_mapRests.end())
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);

	CPoint ptWin = CGraph::VecToWin(m_mapRests[unRest]->GetCenterPoint());

	m_mapRests[unRest]->Select(ptWin);
	m_mapRests[unRest]->Confirm();

	CPoint ptDis = CPoint(rect.right / 2, rect.bottom / 2) - ptWin;
	CGraph::SetOffset(ptDis + CGraph::GetOffset());

	return;
}

void CMapCreaterView::DeleteAllRest()
{
	Cancel();

	OnRButtonUp(0, 0);

	lock_guard<mutex> lock(m_mutexLock);

	for (map<unsigned int, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end();)
	{
		delete it->second;
		it = m_mapRests.erase(it);
	}

	return;
}

void CMapCreaterView::UpdateRest(unsigned int unRest, unsigned int unMark)
{
	Cancel();

	OnRButtonUp(0, 0);

	if (m_mapRests.find(unRest) == m_mapRests.end())
	{
		return;
	}

	m_mapRests[unRest]->SetMark(unMark);

	return;
}

void CMapCreaterView::DeleteCharger(unsigned int unCharger)
{
	Cancel();

	OnRButtonUp(0, 0);

	if (m_mapChargers.find(unCharger) == m_mapChargers.end())
	{
		return;
	}

	lock_guard<mutex> lock(m_mutexLock);

	delete m_mapChargers[unCharger];
	m_mapChargers.erase(m_mapChargers.find(unCharger));

	return;
}

void CMapCreaterView::LocateCharger(unsigned int unCharger)
{
	Cancel();

	OnRButtonUp(0, 0);

	if (m_mapChargers.find(unCharger) == m_mapChargers.end())
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);

	CPoint ptWin = CGraph::VecToWin(m_mapChargers[unCharger]->GetCenterPoint());

	m_mapChargers[unCharger]->Select(ptWin);
	m_mapChargers[unCharger]->Confirm();

	CPoint ptDis = CPoint(rect.right / 2, rect.bottom / 2) - ptWin;
	CGraph::SetOffset(ptDis + CGraph::GetOffset());

	return;
}

void CMapCreaterView::DeleteAllCharger()
{
	Cancel();

	OnRButtonUp(0, 0);

	lock_guard<mutex> lock(m_mutexLock);

	for (map<unsigned int, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end();)
	{
		delete it->second;
		it = m_mapChargers.erase(it);
	}

	return;
}

void CMapCreaterView::UpdateCharger(unsigned int unCharger, unsigned int unCtrl, unsigned int unMark)
{
	Cancel();

	OnRButtonUp(0, 0);

	if (m_mapChargers.find(unCharger) == m_mapChargers.end())
	{
		return;
	}

	m_mapChargers[unCharger]->SetCtrl(unCtrl);
	m_mapChargers[unCharger]->SetMark(unMark);

	return;
}

void CMapCreaterView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcWnd;	/*!< 客户区尺寸 */
	GetClientRect(rcWnd);

	CGraph::SetClientRect(rcWnd);

	return;
}


BOOL CMapCreaterView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pBackground)
	{
		if (zDelta > 0)
		{
			m_pBackground->Scale();
		}
		else
		{
			m_pBackground->Scale(true);
		}
		
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CMapCreaterView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;
	GetClientRect(rcClient);

	InvalidateRect(rcClient);
	UpdateWindow();

	CView::OnTimer(nIDEvent);
}


void CMapCreaterView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	SetTimer(0, 1000 / 30, NULL);
	return;
}


BOOL CMapCreaterView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return true;

	//return CView::OnEraseBkgnd(pDC);
}


void CMapCreaterView::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (m_pBackground)
	{
		m_pBackground->Reset();
	}

	CView::OnMButtonDblClk(nFlags, point);

	return;
}


void CMapCreaterView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pBackground)
	{
		m_pBackground->Select(point);
	}

	CView::OnMButtonDown(nFlags, point);

	return;
}


void CMapCreaterView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pBackground)
	{
		m_pBackground->Confirm();
	}

	CView::OnMButtonUp(nFlags, point);

	return;
}


void CMapCreaterView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnMouseMove(nFlags, point);

	CPoint ptVector = CGraph::WinToVec(point);

	HCURSOR hCur = nullptr;

	if (m_pMark)
	{
		m_pMark->SetCenterPoint(ptVector);

		hCur = LoadCursor(NULL, IDC_HAND);
	}

	if (m_pStation)
	{
		m_pStation->SetCenterPoint(ptVector);

		hCur = LoadCursor(NULL, IDC_HAND);
	}

	if (m_pCharger)
	{
		m_pCharger->SetCenterPoint(ptVector);

		hCur = LoadCursor(NULL, IDC_HAND);
	}

	if (m_pRest)
	{
		m_pRest->SetCenterPoint(ptVector);

		hCur = LoadCursor(NULL, IDC_HAND);
	}

	if (m_pTrack)
	{
		m_pTrack->SetEndPoint(ptVector);

		hCur = LoadCursor(NULL, IDC_HAND);
	}

	if (m_pBackground)
	{
		HCURSOR hRecv = nullptr;

		hRecv = m_pBackground->Drag(point);

		if (hRecv)
		{
			hCur = hRecv;
		}
	}

	for (list<CGraphicTrack*>::iterator it = m_listTracks.begin(); it != m_listTracks.end(); it++)
	{
		HCURSOR hRecv = nullptr;

		hRecv = (*it)->Drag(point);

		if (hRecv)
		{
			hCur = hRecv;
		}
	}

	for (map<unsigned int, CGraphicMark*>::iterator it = m_mapMarks.begin(); it != m_mapMarks.end(); it++)
	{
		HCURSOR hRecv = nullptr;

		hRecv = it->second->Drag(point);

		if (hRecv)
		{
			hCur = hRecv;
		}
	}

	for (map<unsigned int, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end(); it++)
	{
		HCURSOR hRecv = nullptr;

		hRecv = it->second->Drag(point);

		if (hRecv)
		{
			hCur = hRecv;
		}
	}

	for (map<unsigned int, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); it++)
	{
		HCURSOR hRecv = nullptr;

		hRecv = it->second->Drag(point);

		if (hRecv)
		{
			hCur = hRecv;
		}
	}

	for (map<unsigned int, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); it++)
	{
		HCURSOR hRecv = nullptr;

		hRecv = it->second->Drag(point);

		if (hRecv)
		{
			hCur = hRecv;
		}
	}

	if (hCur)
	{
		hCur = SetCursor(hCur);
	}
	else
	{
		hCur = LoadCursor(NULL, IDC_ARROW);
		hCur = SetCursor(hCur);
	}

	return;
}


void CMapCreaterView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnLButtonUp(nFlags, point);

	lock_guard<mutex> lock(m_mutexLock);

	CPoint ptVector = CGraph::WinToVec(point);
	ptVector = CGraph::Correct(ptVector);

	if (m_pMark)
	{
		m_pMark->SetCenterPoint(ptVector);

		m_mapMarks[m_pMark->GetNo()] = m_pMark;
		AddMark(m_mapMarks[m_pMark->GetNo()]);
		m_pMark = nullptr;

		return;
	}

	if (m_pStation)
	{
		m_pStation->SetCenterPoint(ptVector);

		m_mapStations[m_pStation->GetNo()] = m_pStation;
		AddStation(m_mapStations[m_pStation->GetNo()]);
		m_pStation = nullptr;

		return;
	}

	if (m_pCharger)
	{
		m_pCharger->SetCenterPoint(ptVector);

		m_mapChargers[m_pCharger->GetNo()] = m_pCharger;
		AddCharger(m_mapChargers[m_pCharger->GetNo()]);
		m_pCharger = nullptr;

		return;
	}

	if (m_pRest)
	{
		m_pRest->SetCenterPoint(ptVector);

		m_mapRests[m_pRest->GetNo()] = m_pRest;
		AddRest(m_mapRests[m_pRest->GetNo()]);
		m_pRest = nullptr;

		return;
	}

	if (m_pTrack)
	{
		m_pTrack->SetEndPoint(ptVector);

		m_listTracks.push_back(m_pTrack);
		m_pTrack = nullptr;

		return;
	}

	for (list<CGraphicTrack*>::iterator it = m_listTracks.begin(); it != m_listTracks.end(); it++)
	{
		(*it)->Confirm();
	}

	for (map<unsigned int, CGraphicMark*>::iterator it = m_mapMarks.begin(); it != m_mapMarks.end(); it++)
	{
		if (it->second->IsSelected())
		{
			AddMark(it->second);
		}

		it->second->Confirm();
	}

	for (map<unsigned int, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end(); it++)
	{
		if (it->second->IsSelected())
		{
			AddStation(it->second);
		}

		it->second->Confirm();
	}

	for (map<unsigned int, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); it++)
	{
		if (it->second->IsSelected())
		{
			AddCharger(it->second);
		}

		it->second->Confirm();
	}

	for (map<unsigned int, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); it++)
	{
		if (it->second->IsSelected())
		{
			AddRest(it->second);
		}

		it->second->Confirm();
	}

	return;
}


void CMapCreaterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnLButtonDown(nFlags, point);

	CPoint ptVector = CGraph::WinToVec(point);
	ptVector = CGraph::Correct(ptVector);

	if (m_pMark)
	{
		return;
	}

	if (m_pStation)
	{
		return;
	}

	if (m_pCharger)
	{
		return;
	}

	if (m_pRest)
	{
		return;
	}

	if (m_pTrack)
	{
		m_pTrack->SetStartPoint(ptVector);

		return;
	}

	for (list<CGraphicTrack*>::iterator it = m_listTracks.begin(); it != m_listTracks.end(); it++)
	{
		if ((*it)->IsInside(point))
		{
			if (m_bCtrlKey && (*it)->IsSelected())
			{
				(*it)->Cancel();

				continue;
			}

			(*it)->Select(point);
		}
		else
		{
			if (m_bCtrlKey && (*it)->IsSelected())
			{
				(*it)->Select(point);

				continue;
			}

			(*it)->Cancel();
		}
	}

	for (map<unsigned int, CGraphicMark*>::iterator it = m_mapMarks.begin(); it != m_mapMarks.end(); it++)
	{
		if (it->second->IsInside(point))
		{
			if (m_bCtrlKey && it->second->IsSelected())
			{
				it->second->Cancel();

				continue;
			}

			it->second->Select(point);
		}
		else
		{
			if (m_bCtrlKey && it->second->IsSelected())
			{
				it->second->Select(point);

				continue;
			}

			it->second->Cancel();
		}
	}

	for (map<unsigned int, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end(); it++)
	{
		if (it->second->IsInside(point))
		{
			if (m_bCtrlKey && it->second->IsSelected())
			{
				it->second->Cancel();

				continue;
			}

			it->second->Select(point);
		}
		else
		{
			if (m_bCtrlKey && it->second->IsSelected())
			{
				it->second->Select(point);

				continue;
			}

			it->second->Cancel();
		};
	}

	for (map<unsigned int, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); it++)
	{
		if (it->second->IsInside(point))
		{
			if (m_bCtrlKey && it->second->IsSelected())
			{
				it->second->Cancel();

				continue;
			}

			it->second->Select(point);
		}
		else
		{
			if (m_bCtrlKey && it->second->IsSelected())
			{
				it->second->Select(point);

				continue;
			}

			it->second->Cancel();
		};
	}

	for (map<unsigned int, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); it++)
	{
		if (it->second->IsInside(point))
		{
			if (m_bCtrlKey && it->second->IsSelected())
			{
				it->second->Cancel();

				continue;
			}

			it->second->Select(point);
		}
		else
		{
			if (m_bCtrlKey && it->second->IsSelected())
			{
				it->second->Select(point);

				continue;
			}

			it->second->Cancel();
		};
	}

	return;
}


void CMapCreaterView::OnZoomIn()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pBackground)
	{
		m_pBackground->Scale();
	}

	return;
}


void CMapCreaterView::OnZoomOut()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pBackground)
	{
		m_pBackground->Scale(true);
	}

	return;
}


void CMapCreaterView::OnMapReset()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pBackground)
	{
		m_pBackground->Reset();
	}

	return;
}


void CMapCreaterView::OnMapMark()
{
	// TODO: 在此添加命令处理程序代码
	m_bCtrlKey = false;

	Cancel();

	CSetMarkDlg setMarkDlg;

	if (setMarkDlg.DoModal() == IDOK)
	{
		unsigned int unNo = setMarkDlg.m_unNo;

		if (m_mapMarks.find(unNo) != m_mapMarks.end())
		{
			if (MessageBox(_T("已存在此地标卡,是否重新设置此地标卡坐标?"), _T("提示"), MB_ICONQUESTION | MB_YESNO) == IDNO)
			{
				return;
			}

			m_mapMarks[unNo]->Select(CGraph::VecToWin(m_mapMarks[unNo]->GetCenterPoint()));
		}
		else
		{
			m_pMark = new CGraphicMark(unNo);
		}
	}

	return;
}


void CMapCreaterView::OnMapStation()
{
	// TODO: 在此添加命令处理程序代码
	m_bCtrlKey = false;

	Cancel();

	CSetStationDlg setStationDlg;

	if (setStationDlg.DoModal() == IDOK)
	{
		unsigned int unNo = setStationDlg.m_unNo;

		if (m_mapStations.find(unNo) != m_mapStations.end())
		{
			if (MessageBox(_T("已存在此工位,是否重新设置此工位参数以及坐标?"), _T("提示"), MB_ICONQUESTION | MB_YESNO) == IDNO)
			{
				return;
			}
			m_mapStations[unNo]->SetMark(setStationDlg.m_unMark);
			m_mapStations[unNo]->SetName(setStationDlg.m_cstrName);
			m_mapStations[unNo]->Select(CGraph::VecToWin(m_mapStations[unNo]->GetCenterPoint()));
		}
		else
		{
			m_pStation = new CGraphicStation(unNo, setStationDlg.m_unMark, setStationDlg.m_cstrName);
		}
	}

	return;
}


void CMapCreaterView::OnMapRest()
{
	// TODO: 在此添加命令处理程序代码
	m_bCtrlKey = false;

	Cancel();

	CSetRestDlg setRestDlg;

	if (setRestDlg.DoModal() == IDOK)
	{
		unsigned int unNo = setRestDlg.m_unNo;

		if (m_mapRests.find(unNo) != m_mapRests.end())
		{
			if (MessageBox(_T("已存在此待机位,是否重新设置此待机位参数以及坐标?"), _T("提示"), MB_ICONQUESTION | MB_YESNO) == IDNO)
			{
				return;
			}
			m_mapRests[unNo]->SetMark(setRestDlg.m_unMark);
			m_mapRests[unNo]->Select(CGraph::VecToWin(m_mapRests[unNo]->GetCenterPoint()));
		}
		else
		{
			m_pRest = new CGraphicRest(unNo, setRestDlg.m_unMark);
		}
	}

	return;
}


void CMapCreaterView::OnMapCharger()
{
	// TODO: 在此添加命令处理程序代码
	m_bCtrlKey = false;

	Cancel();

	CSetChargerDlg setChargerDlg;

	if (setChargerDlg.DoModal() == IDOK)
	{
		unsigned int unNo = setChargerDlg.m_unNo;

		if (m_mapChargers.find(unNo) != m_mapChargers.end())
		{
			if (MessageBox(_T("已存在此充电器,是否重新设置此充电器参数以及坐标?"), _T("提示"), MB_ICONQUESTION | MB_YESNO) == IDNO)
			{
				return;
			}
			m_mapChargers[unNo]->SetMark(setChargerDlg.m_unMark);
			m_mapChargers[unNo]->SetCtrl(setChargerDlg.m_unCtrl);
			m_mapChargers[unNo]->Select(CGraph::VecToWin(m_mapChargers[unNo]->GetCenterPoint()));
		}
		else
		{
			m_pCharger = new CGraphicCharger(unNo, setChargerDlg.m_unMark, setChargerDlg.m_unCtrl);
		}
	}

	return;
}


void CMapCreaterView::OnTrackLine()
{
	// TODO: 在此添加命令处理程序代码
	m_bCtrlKey = false;

	Cancel();

	m_pTrack = new CGraphicTrack(false);
}


void CMapCreaterView::OnTrackArc()
{
	// TODO: 在此添加命令处理程序代码
	m_bCtrlKey = false;

	Cancel();

	m_pTrack = new CGraphicTrack(true);
}


void CMapCreaterView::OnMapDelete()
{
	// TODO: 在此添加命令处理程序代码
	lock_guard<mutex> lock(m_mutexLock);

	for (map<unsigned int, CGraphicMark*>::iterator it = m_mapMarks.begin(); it != m_mapMarks.end();)
	{
		if (it->second->IsSelected() == false)
		{
			it++;
			continue;
		}

		DeleteMark(it->second);

		delete it->second;
		it = m_mapMarks.erase(it);
	}

	for (map<unsigned int, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end();)
	{
		if (it->second->IsSelected() == false)
		{
			it++;
			continue;
		}

		DeleteStation(it->second);

		delete it->second;
		it = m_mapStations.erase(it);
	}

	for (map<unsigned int, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end();)
	{
		if (it->second->IsSelected() == false)
		{
			it++;
			continue;
		}

		DeleteCharger(it->second);

		delete it->second;
		it = m_mapChargers.erase(it);
	}

	for (map<unsigned int, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end();)
	{
		if (it->second->IsSelected() == false)
		{
			it++;
			continue;
		}

		DeleteRest(it->second);

		delete it->second;
		it = m_mapRests.erase(it);
	}

	for (list<CGraphicTrack*>::iterator it = m_listTracks.begin(); it != m_listTracks.end();)
	{
		if ((*it)->IsSelected() == false)
		{
			it++;
			continue;
		}

		delete *it;
		it = m_listTracks.erase(it);
	}

	return;
}


void CMapCreaterView::OnMapSelectall()
{
	// TODO: 在此添加命令处理程序代码
	for (map<unsigned int, CGraphicMark*>::iterator it = m_mapMarks.begin(); it != m_mapMarks.end();it++)
	{
		it->second->Select(CGraph::VecToWin(it->second->GetCenterPoint()));
		it->second->Confirm();
	}

	for (map<unsigned int, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end(); it++)
	{
		it->second->Select(CGraph::VecToWin(it->second->GetCenterPoint()));
		it->second->Confirm();
	}

	for (map<unsigned int, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); it++)
	{
		it->second->Select(CGraph::VecToWin(it->second->GetCenterPoint()));
		it->second->Confirm();
	}

	for (map<unsigned int, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); it++)
	{
		it->second->Select(CGraph::VecToWin(it->second->GetCenterPoint()));
		it->second->Confirm();
	}

	for (list<CGraphicTrack*>::iterator it = m_listTracks.begin(); it != m_listTracks.end(); it++)
	{
		(*it)->Select(CGraph::VecToWin((*it)->GetStartPoint()));
		(*it)->Confirm();
	}

	return;
}


BOOL CMapCreaterView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_hAccel)
	{
		if (::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
		{
			return(TRUE);
		}
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_CONTROL:
			m_bCtrlKey = true;
			break;
		default:
			break;
		}
	}
	else if (pMsg->message == WM_KEYUP)
	{
		switch (pMsg->wParam)
		{
		case VK_CONTROL:
			m_bCtrlKey = false;
			break;
		default:
			break;
		}
	}

	return CView::PreTranslateMessage(pMsg);
}


void CMapCreaterView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnLButtonDblClk(nFlags, point);

	for (map<unsigned int, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end(); it++)
	{
		if (it->second->IsInside(point))
		{
			CSetStationDlg setStationDlg;
			setStationDlg.m_bEdit = true;
			setStationDlg.m_unNo = it->second->GetNo();
			setStationDlg.m_unMark = it->second->GetMark();
			setStationDlg.m_cstrName = it->second->GetName();

			if (setStationDlg.DoModal() == IDOK)
			{
				it->second->SetMark(setStationDlg.m_unMark);
				it->second->SetName(setStationDlg.m_cstrName);

				AddStation(it->second);
			}

			return;
		}
	}

	for (map<unsigned int, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); it++)
	{
		if (it->second->IsInside(point))
		{
			CSetChargerDlg setChargerDlg;
			setChargerDlg.m_bEdit = true;
			setChargerDlg.m_unNo = it->second->GetNo();
			setChargerDlg.m_unMark = it->second->GetMark();
			setChargerDlg.m_unCtrl = it->second->GetCtrl();

			if (setChargerDlg.DoModal() == IDOK)
			{
				it->second->SetMark(setChargerDlg.m_unMark);
				it->second->SetCtrl(setChargerDlg.m_unCtrl);

				AddCharger(it->second);
			}

			return;
		}
	}

	for (map<unsigned int, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); it++)
	{
		if (it->second->IsInside(point))
		{
			CSetRestDlg setRestDlg;
			setRestDlg.m_bEdit = true;
			setRestDlg.m_unNo = it->second->GetNo();
			setRestDlg.m_unMark = it->second->GetMark();

			if (setRestDlg.DoModal() == IDOK)
			{
				it->second->SetMark(setRestDlg.m_unMark);

				AddRest(it->second);
			}

			return;
		}
	}

	return;
}


void CMapCreaterView::OnFileIn()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlgFile(true,_T(".map"),NULL, OFN_HIDEREADONLY,_T("地图文件(*.map)|*.map|"));

	if (dlgFile.DoModal() == IDOK)
	{
		m_cstrFilePath = dlgFile.GetPathName();

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	/*!< 框架指针 */
		pMainFrame->SetWindowText(_T("地图制作器V2.0-") + m_cstrFilePath);

		InputAll(m_cstrFilePath);
	}
}


void CMapCreaterView::OnFileOut()
{
	// TODO: 在此添加命令处理程序代码
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	CString cstrFileName = _T("");
	cstrFileName.Format(_T("地图%04d%02d%02d%02d%02d%02d"), systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
	CFileDialog dlgFile(false, _T(".map"), cstrFileName.GetBuffer(), OFN_HIDEREADONLY, _T("地图文件(*.map)|*.map|"));

	if (dlgFile.DoModal() == IDOK)
	{
		m_cstrFilePath = dlgFile.GetPathName();

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	/*!< 框架指针 */
		pMainFrame->SetWindowText(_T("地图制作器V2.0-") + m_cstrFilePath);

		OutputAll(m_cstrFilePath);
	}
}


void CMapCreaterView::OnExit()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	/*!< 框架指针 */
	pMainFrame->OnClose();
}


void CMapCreaterView::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码

	if (m_cstrFilePath.IsEmpty())
	{
		OnFileOut();
	}
	else
	{
		OutputAll(m_cstrFilePath);
	}
}


void CMapCreaterView::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnClose();
}


void CMapCreaterView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}
