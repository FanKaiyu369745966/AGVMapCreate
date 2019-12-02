/*!
 * @file CGraph.h
 * @brief CGraph��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-10-16
 * @version 2.0
*/
#pragma once
#include <gdiplus.h>

using namespace Gdiplus;

#ifndef _CGRAPH_H
#define _CGRAPH_H

#define MAX_SCALE 10.0f
#define MIN_SCALE 0.4f
#define DEFALUT_SCALE 10.0f

/*!
 * @class CGraph
 * @brief ����ͼ��
 *
 * ��ͼ�Ļ�������,����������ͼ����̳�
 * 10�������10cm
 * ��ͼ���ߴ�Ϊ500*500m
 * ��50000 * 50000
*/
class CGraph
{
public:
	CGraph();
	~CGraph();

protected:
	GdiplusStartupInput m_gdiplusInput;		/*!< GDI+��ʼ�������� */
	GdiplusStartupOutput m_gdiplusOutput;	/*!< GDI+��ʼ�������� */
	ULONG_PTR m_ulGdiplusToken;

protected:
	static CPoint g_ptOffset;			/*!< ƫ���� */
	static float g_fScale;				/*!< ���ű��� */
	static unsigned int g_unDistance;	/*!< ������ */
	static CSize g_sizeVector;			/*!< ʸ��ͼ�ߴ� */
	static CRect g_rcClient;			/*!< �ͻ����ߴ� */
	static bool g_bFlash;				/*!< ��˸��ʶ */
#ifdef _WIN64
	static unsigned long long g_ullFlashTime;	/*!< ��˸��ʼʱ�� */
#elif _WIN32
	static unsigned long g_ulFlashTime;			/*!< ��˸��ʼʱ�� */
#endif 

public:
	/*!
	 * @brief ���ÿͻ����ߴ�
	 * @param rcClient �ͻ����ߴ�
	*/
	static void SetClientRect(CRect rcClient);

	/*!
	 * @brief ��ȡƫ����
	 * @return CPoint ƫ����
	*/
	static CPoint GetOffset();

	/*!
	 * @brief ����ƫ����
	 * @param ptOffset ƫ����
	*/
	static void SetOffset(CPoint ptOffset);

	/*!
	 * @brief ��ȡ���ű���
	 * @return float fScale
	*/
	static float GetScale();

	/*!
	 * @brief �������ű���
	 * @param fScale ���ű���
	*/
	static void SetScale(float fScale);

	/*!
	 * @brief ��ʸ������ת��Ϊ��������
	 * @param ptVector ʸ������
	 * @return CPoint ��������
	*/
	static CPoint VecToWin(CPoint ptVector);

	/*!
	 * @brief ����������ת��δʸ������
	 * @param ptWindow ��������
	 * @return CPoint ʸ������
	*/
	static CPoint WinToVec(CPoint ptWindow);

	/*!
	 * @brief ����ʸ������
	 * @param ptVector ʸ������
	 * @return CPoint �������ʸ������
	*/
	static CPoint Correct(CPoint ptVector);

	/*!
	 * @brief ��λ
	 *
	 * ����ͼ�����ƶ���ָ���������
	 * @param CPoint ��λ����
	*/
	static void Locate(CPoint ptLocation);
};

#endif // !_CGRAPH_H