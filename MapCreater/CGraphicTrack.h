#pragma once
#include "CGraph.h"

#ifndef _CGRAPHICTRACK_H
#define _CGRAPHICTRACK_H

class CGraphicTrack :
	public CGraph
{
public:
	CGraphicTrack(bool bArc);
	~CGraphicTrack();

protected:
	CPoint m_ptStart;	/*!< ������� */
	CPoint m_ptEnd;		/*!< �յ����� */
	bool m_bArc;		/*!< ���߱�ʶ */

protected:
	static float g_fWidth;	/*!< �� */

protected:
	CPoint m_ptPrepoint;	/*!< ��ʼ���� */
	CPoint m_ptLastpoint;	/*!< ��ֹ���� */
	CPoint m_ptLocal;		/*!< ��¼������� */
	CPoint m_ptDistance;	/*!< ��� */
	bool m_bMove;			/*!< �ƶ���ʶ */
	bool m_bSelect;			/*!< ѡ�б�ʶ */
	bool m_bShow;			/*!< ��ʾ��ʶ */

public:
	/*!
	 * @brief �����������
	 * @param ptStart �������(ʸ������)
	*/
	void SetStartPoint(CPoint ptStart);

	/*!
	 * @brief �����յ�����
	 * @param ptEnd �յ�����(ʸ������)
	*/
	void SetEndPoint(CPoint ptEnd);

	/*!
	 * @brief ���ù������
	 * @param bArc ����Ϊtrue,ֱ��Ϊfalse
	*/
	void SetType(bool bArc);

	/*!
	 * @brief ��ȡ�������(ʸ������)
	 * @return CPoint �������
	*/
	CPoint GetStartPoint();

	/*!
	 * @brief ��ȡ�յ�����(ʸ������)
	 * @return CPoint �յ�����
	*/
	CPoint GetEndPoint();

	/*!
	 * @brief ��ȡ�������
	 * @return bool trueΪ����,falseΪֱ��
	*/
	bool GetType();

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
	 * @brief ����
	 * @param pDC ���ھ��
	*/
	void Draw(CDC *pDC);

	/*!
	 * @brief ���ù�����
	 * @param fWidth ���
	*/
	static void SetWidth(float fWidth);

	/*!
	 * @brief ��ȡ������
	 * @return float ���
	*/
	static float GetWidth();

	/*!
	 * @brief ����ת�Ƕ�
	 * @param dbRadian ����ֵ
	 * @return double �Ƕ�ֵ
	*/
	static double RadianToAngle(double dbRadian);

	/*!
	 * @brief �Ƕ�ת����
	 * @param dbAngle �Ƕ�ֵ
	 * @return double ����ֵ
	*/
	static double AngleToRadian(double dbAngle);
};

#endif //!_CGRAPHICTRACK_H