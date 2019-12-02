/*!
 * @file CGraphicCharger.h
 * @brief CGraphicCharger��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-10-22
 * @version 2.0
*/

#pragma once
#include "CGraph.h"
#include <mutex>

using namespace std;

#ifndef _CGRAPHICCHARGER_H
#define _CGRAPHICCHARGER_H

/*!
 * @class CGraphicCharger
 * @brief �����ͼ��
 *
 * �̳л���ͼ��,���Ƴ����ͼ���Լ���ק�Ȳ���
*/
class CGraphicCharger :
	public CGraph
{
public:
	CGraphicCharger(unsigned char byNo,unsigned short usMark,unsigned char byCtrl,CString cstrName = _T(""));
	~CGraphicCharger();

protected:
	unsigned char m_byNo;		/*!< ��� */
	unsigned short m_usMark;	/*!< �ر꿨 */
	unsigned char m_byCtrl;		/*!< ������ */
	CString m_cstrName;			/*!< ���� */
#ifdef _WIN64
	unsigned long long m_ullChargeTime;	/*!< ��翪ʼʱ�� */
#elif _WIN32
	unsigned long m_ulChargeTime;		/*!< ��翪ʼʱ�� */
#endif

protected:
	bool m_bPower;			/*!< ͨ��״̬ */

protected:
	static unsigned int g_unWidth;	/*!< �� */
	static unsigned int g_unHeight;	/*!< �� */
	static CString g_cstrPath;		/*!< ����ͼ·�� */
	static CString g_cstrPower;		/*!< ����ʶ·�� */

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
	 * @brief ��������
	 * @param CString ����
	*/
	void SetName(CString cstrName);

	/*!
	 * @brief ��ȡ����
	 * @return CString ����
	*/
	CString GetName() const;

#ifdef _WIN64
	/*!
	 * @brief ���ó��״̬
	 * @param bPower trueΪ���,falseΪ�ϵ�
	*/
	void SetPower(bool bPower,unsigned long long ullChargeTime);
#elif _WIN32
	/*!
	 * @brief ���ó��״̬
	 * @param bPower trueΪ���,falseΪ�ϵ�
	*/
	void SetPower(bool bPower, unsigned long ulChargeTime);
#endif

	/*!
	 * @brief ���ÿ�����
	 * @param unsigned char ���������
	*/
	void SetCtrl(unsigned char byCtrl);

	/*!
	 * @brief ��ȡ������
	 * @return unsigned char ���������
	*/
	unsigned char GetCtrl() const;

	/*!
	 * @brief ���õر꿨
	 * @param unsigned short �ر꿨
	*/
	void SetMark(unsigned short usMark);

	/*!
	 * @brief ��ȡ�ر꿨
	 * @return unsigned short �ر꿨
	*/
	unsigned short GetMark() const;

	/*!
	 * @brief ���ñ���ͼ
	 * @param cstrPath ����ͼ·��
	 * @param cstrPower	����ʶ·��
	*/
	static void SetImage(CString cstrPath,CString cstrPower);

	/*!
	 * @brief ��ȡ����ͼ
	 * @param cstrPath ���ر���ͼ·��
	 * @param cstrPower	���س���ʶ·��
	*/
	static void GetImage(CString &cstrPath, CString &cstrPower);

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

#endif //!_CGRAPHICCHARGER_H