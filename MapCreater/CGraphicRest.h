/*!
 * @file CGraphicRest.h
 * @brief CGraphicRest��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-10-22
 * @version 2.0
*/

#pragma once
#include "CGraph.h"

#ifndef _CGRAPHICREST_H
#define _CGRAPHICREST_H

class CGraphicRest :
	public CGraph
{
public:
	CGraphicRest(unsigned char byNo,unsigned short usMark,CString cstrName = _T(""));
	~CGraphicRest();

protected:
	unsigned char m_byNo;			/*!< ��� */
	unsigned short m_usMark;		/*!< �ر꿨 */
	CString m_cstrName;				/*!< ���� */ 

protected:
	static unsigned int g_unWidth;	/*!< �� */
	static unsigned int g_unHeight;	/*!< �� */
	static CString g_cstrPath;		/*!< ����ͼ·�� */

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
	 * @return unsigned char ���
	*/
	unsigned char GetNo() const;
	
	/*!
	 * @brief ���õر꿨
	 * @param usMark �ر꿨
	*/
	void SetMark(unsigned short usMark);

	/*!
	 * @brief ��ȡ�ر꿨
	 * @return unsigned short �ر꿨
	*/
	unsigned short GetMark() const;

	/*!
	 * @brief ��������
	 * @param CString ����
	*/
	void SetName(CString cstrName);

	/*!
	 * @brief ��ȡ����
	 * @return CString ����
	*/
	CString GetName() const;

	/*!
	 * @brief ���ñ���ͼ
	 * @param cstrPath ����ͼ·��
	*/
	static void SetImage(CString cstrPath);

	/*!
	 * @brief ��ȡ����ͼ
	 * @param cstrPath ���ر���ͼ·��
	 * @param cstrPower	���س���ʶ·��
	*/
	static CString GetImage();

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
	CPoint GetCenterPoint();

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

#endif // !_CGRAPHICREST_H