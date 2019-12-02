/*!
 * @file CGraphicMark.h
 * @brief CGraphicMark��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-10-22
 * @version 2.0
*/

#pragma once
#include "CGraph.h"

#ifndef _CGRAPHICMARK_H
#define _CGRAPHICMARK_H

/*!
 * @class CGraphicMark
 * @brief �ر꿨��
 *
 * �̳л���ͼ��,���Ƶر꿨ͼ���Լ���ק�Ȳ���
*/
class CGraphicMark :
	public CGraph
{
public:
	CGraphicMark(unsigned short usNo,CPoint ptCenter = CPoint(0,0));
	~CGraphicMark();

protected:
	unsigned short m_usNo;	/*!< ��� */

protected:
	static unsigned int g_unWidth;	/*!< �� */
	static unsigned int g_unHeight;	/*!< �� */

protected:
	CPoint m_ptCenter;		/*!< �������� */
	CPoint m_ptPrepoint;	/*!< ��ʼ���� */
	CPoint m_ptLastpoint;	/*!< ��ֹ���� */
	CPoint m_ptLocal;		/*!< ��¼���� */
	bool m_bMove;			/*!< �ƶ���ʶ */
	bool m_bSelect;			/*!< ѡ�б�ʶ */

public:
	/*!
	 * @brief ��ȡ���
	 * @return unsigned short ���
	*/
	unsigned short GetNo() const;

	/*!
	 * @brief ���������Ƿ���������
	 * @param ptWinpoint ��������
	 * @return �������ڷ���true,���ڷ���false
	*/
	bool IsInside(CPoint ptWinpoint);

	/*!
	 * @brief ��ȡѡ��״̬
	 * @return bool ѡ�з���true,δѡ�з���false
	*/
	bool IsSelected();

	/*!
	 * @brief ѡ��
	 * @param ptWinpoint ��������
	*/
	void Select(CPoint ptWinpoint);

	/*!
	 * @brief ��ק
	 * @param  ptWinpoint ��������
	 * @return HCURSOR ���ָ����״
	*/
	HCURSOR Drag(CPoint ptWinPoint);

	/*!
	 * @brief ȷ��
	*/
	void Confirm();

	/*!
	 * @brief ȡ��
	*/
	void Cancel();

	/*!
	 * @brief ��ȡ��������
	 * @return CPoint ��������
	*/
	CPoint GetCenterPoint() const;

	/*!
	 * @brief ������������
	 * @param ptCenter ��������
	*/
	void SetCenterPoint(CPoint ptCenter);

	/*!
	 * @brief ����
	 * @param pDC ���ھ��
	*/
	void Draw(CDC *pDC);
};

#endif //!_CGRAPHICMARK_H