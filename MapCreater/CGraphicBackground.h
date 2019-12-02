/*!
 * @file CGraphicBackground.h
 * @brief CGraphicBackground��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-10-15
 * @version 2.0
*/

#pragma once
#include "CGraph.h"
#include <mutex>

using namespace std;

#ifndef _CGRAPHICBACKGROUND_H
#define _CGRAPHICBACKGROUND_H

/*!
 * @class CGraph
 * @brief ����ͼ��
 *
 * �̳л���ͼ��,���Ʊ���ͼ���������Լ���ק�����ŵȲ���
*/
class CGraphicBackground :
	public CGraph
{
public:
	CGraphicBackground(Color colorLine = Color::Black,float fWidth = 2.0f,CString cstrPath = _T("../Image/background.jpg"));
	~CGraphicBackground();

protected:
	Image *m_pImage;		/*!< ����ͼ */
	Pen *m_pPen;			/*!< ���� */
	Color m_colorLine;		/*!< ��������ɫ */
	float m_fWidth;			/*!< �����߿�� */
	CString m_cstrPath;		/*!< ����ͼ·�� */
	bool m_bMove;			/*!< �ƶ���ʶ */
	CPoint m_ptPrepoint;	/*!< ��ʼ���� */
	CPoint m_ptLastpoint;	/*!< ��ֹ���� */
	CPoint m_ptLocal;		/*!< ƫ������¼ */

protected:
	mutex m_mutexLock;

public:
	/*!
	 * @brief ���ø�����
	 * @param colorLine ��������ɫ
	 * @param fWidth �����߿��
	*/
	void SetLine(Color colorLine, float fWidth);

	/*!
	 * @brief ��ȡ������
	 * @param colorLine ���ظ�������ɫ
	 * @param fWidth ���ظ����߿��
	*/
	void GetLine(Color &colorLine, float &fWidth);

	/*!
	 * @brief ���ñ���ͼ
	 * @param cstrPath ����ͼ·��
	*/
	void SetImage(CString cstrPath);

	/*!
	 * @brief ��ȡ����ͼ
	 * @return CString ����ͼ·��
	*/
	CString GetImage();

	/*!
	 * @brief ����
	 *
	 * Ĭ�ϷŴ�
	 * @param bool ��С��ʶ
	*/
	void Scale(bool bShrink = false);

	/*!
	 * @brief ��λ
	*/
	void Reset();

	/*!
	 * @brief ��ק
	 * @param ptWinPoint ��������
	 * @return HCURSOR ���ָ����״
	*/
	HCURSOR Drag(CPoint ptWinPoint);

	/*!
	 * @brief ѡ��
	 *
	 * �����������ͼ,�����˺���
	 * @param ptWinPoint ��������
	*/
	void Select(CPoint ptWinPoint);

	/*!
	 * @brief ȡ����ק
	*/
	void Cancel();

	/*!
	 * @brief �����ק
	*/
	void Confirm();

public:
	/*!
	 * @brief ���Ʊ���ͼ
	 * @param pDC ��ͼ�����
	*/
	void DrawBackground(CDC *pDC);

	/*!
	 * @brief ���Ƹ�����
	 * @param pDC ��ͼ�����
	*/
	void DrawLine(CDC *pDC);

	/*!
	 * @brief ��������
	 * @param pDC ��ͼ�����
	*/
	void DrawPoint(CDC *pDC);
};

#endif // !_CGRAPHICBACKGROUND_H