/*!
 * @file CGraphicAGV.h
 * @brief CGraphicAGV��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-10-15
 * @version 2.0
*/

#pragma once
#include "CGraph.h"

#ifndef _CGRAPHICAGV_H
#define _CGRAPHICAGV_H

enum _GraphicAGVActionStatus
{
	_GRAPHICAGVACTSTA_NONE,
	_GRAPHICAGVACTSTA_LOAD,
	_GRAPHICAGVACTSTA_UNLOAD,
};

/*!
 * @class CGraphicAGV
 * @brief AGVͼ��
 *
 * �̳л���ͼ��,����AGVͼ���Լ�����AGV״̬����AGVͼ�εȲ���
*/
class CGraphicAGV :
	public CGraph
{
public:
	CGraphicAGV(unsigned char byNo,CString cstrType,CString cstrMove);
	~CGraphicAGV();

protected:
	unsigned char m_byNo;		/*!< ��� */
	CString m_cstrType;			/*!< ���� Pǣ��ʽ SǱ��ʽ T����ʽ F�泵ʽ A��е��ʽ L����ʽ */
	CString m_cstrMove;			/*!< ���˶����� S���� D˫�� Fȫ�� */

protected:
	bool m_bRun;				/*!< �ƶ���ʶ */
	bool m_bUnVol;				/*!< ���������ʶ */
	bool m_bError;				/*!< �쳣��ʶ */
	bool m_bObs;				/*!< ���ϱ�ʶ */
	bool m_bCargo;				/*!< �ػ�״̬ */
	unsigned char m_byAction;	/*!< ����״̬ 0-�޶���/��� 1-������ 2-������ */

protected:
	bool m_bShow;				/*!< ��ʾ��ʶ�� */
	CPoint m_ptCenter;			/*!< �������� ʸ������ */

protected:
	static unsigned int g_unWidth;	/*!< �� */
	static unsigned int g_unHeight;	/*!< �� */
	static CString g_cstrAGV;		/*!< AGV��ͼ */
	static CString g_cstrRun;		/*!< �ƶ���ͼ */
	static CString g_cstrUnVol;		/*!< ����������ͼ */
	static CString g_cstrError;		/*!< �쳣��ͼ */
	static CString g_cstrObs;		/*!< ������ͼ */
	static CString g_cstrLoad;		/*!< ������ͼ */
	static CString g_cstrUnload;	/*!< ������ͼ */
	static CString g_cstrCargo;		/*!< ������ͼ */

public:
	/*!
	 * @brief ��������
	 * @param cstrType ����
	*/
	void SetType(CString cstrType);

	/*!
	 * @brief ���ÿ��˶�����
	 * @param cstrMove ���˶�����
	*/
	void SetMove(CString cstrMove);

	/*!
	 * @brief ��ȡ���
	 * @return unsigned int ���
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief ��ȡ����
	 * return CString ���� Pǣ��ʽ SǱ��ʽ T����ʽ F�泵ʽ A��е��ʽ L����ʽ
	*/
	CString GetType() const;

	/*!
	 * @brief ��ȡ���˶�����
	 * @return CString ���˶����� S���� D˫�� Fȫ��
	*/
	CString GetMove() const;

	/*!
	 * @brief ������ͼ
	 * @param CString AGV��ͼ·��
	 * @param CString �ƶ���ͼ·��
	 * @param CString ����������ͼ·��
	 * @param CString �쳣��ͼ·��
	 * @param CString ������ͼ·��
	 * @param CString ������ͼ·��
	 * @param CString ������ͼ·��
	 * @param CString �ػ���ͼ·��
	*/
	static void SetImage(CString cstrAGV, CString cstrRun, CString cstrUnVol, CString cstrError, CString cstrObs, CString cstrLoad, CString cstrUnload, CString cstrCargo);

	/*!
	 * @brief ����AGV��ͼ
	 * @param CString AGV��ͼ·��
	*/
	static void SetAGVImage(CString cstrAGV);

	/*!
	 * @brief �����ƶ���ͼ
	 * @param CString �ƶ���ͼ·��
	*/
	static void SetRunImage(CString cstrRun);

	/*!
	 * @brief ���õ���������ͼ
	 * @param CString ����������ͼ·��
	*/
	static void SetUnVolImage(CString cstrUnVol);

	/*!
	 * @brief �����쳣��ͼ
	 * @param CString �쳣��ͼ·��
	*/
	static void SetErrorImage(CString cstrError);

	/*!
	 * @brief ���ñ�����ͼ
	 * @param CString ������ͼ·��
	*/
	static void SetObsImage(CString cstrObs);

	/*!
	 * @brief ����������ͼ
	 * @param CString ������ͼ·��
	*/
	static void SetLoadImage(CString cstrLoad);

	/*!
	 * @brief ����������ͼ
	 * @param CString ������ͼ·��
	*/
	static void SetUnloadImage(CString cstrUnload);

	/*!
	 * @brief �����ػ���ͼ
	 * @param CString �ػ���ͼ·��
	*/
	static void SetCargoImage(CString cstrCargo);

	/*!
	 * @brief ��ȡ��ͼ
	 * @param CString& ����AGV��ͼ·��
	 * @param CString& �����ƶ���ͼ·��
	 * @param CString& ���ص���������ͼ·��
	 * @param CString& �����쳣��ͼ·��
	 * @param CString& ���ر�����ͼ·��
	 * @param CString& ������ͼ·��
	 * @param CString& ������ͼ·��
	 * @param CString& �ػ���ͼ·��
	*/
	static void GetImage(CString &cstrAGV, CString &cstrRun, CString &cstrUnVol, CString &cstrError, CString &cstrObs, CString& cstrLoad, CString& cstrUnload, CString& cstrCargo);

	/*!
	 * @brief ��ȡAGV��ͼ·��
	 * @return CString AGV��ͼ·��
	*/
	static CString GetAGVImage();

	/*!
	 * @brief ��ȡ�ƶ���ͼ·��
	 * @return CString �ƶ���ͼ·��
	*/
	static CString GetRunImage();

	/*!
	 * @brief ��ȡ����������ͼ·��
	 * @return CString ����������ͼ·��
	*/
	static CString GetUnVolImage();

	/*!
	 * @brief ��ȡ�쳣��ͼ·��
	 * @return CString �쳣��ͼ·��
	*/
	static CString GetErrorImage();

	/*!
	 * @brief ��ȡ������ͼ·��
	 * @return CString ������ͼ·��
	*/
	static CString GetObsImage();

	/*!
	 * @brief ��ȡ������ͼ·��
	 * @return CString ������ͼ·��
	*/
	static CString GetLoadImage();

	/*!
	 * @brief ��ȡ������ͼ·��
	 * @return CString ������ͼ·��
	*/
	static CString GetUnloadImage();

	/*!
	 * @brief ��ȡ�ػ���ͼ·��
	 * @return CString �ػ���ͼ·��
	*/
	static CString GetCargoImage();
	
	/*!
	 * @brief ����״̬
	 * @param bool �ƶ���ʶ
	 * @param bool ���������ʶ
	 * @param bool �쳣��ʶ
	 * @param bool ���ϱ�ʶ
	 * @param bool �ػ�״̬
	 * @param unsigned char ����״̬
	*/
	void SetStatus(bool bRun, bool bUnVol, bool bError, bool bObs, bool bCargo, unsigned char byAction);

	/*!
	 * @brief �����ƶ�״̬
	 * @param bool �ƶ���ʶ 
	*/
	void SetRunStatus(bool bRun);

	/*!
	 * @brief ���õ�������״̬
	 * @param bool ���������ʶ
	*/
	void SetUnVolStatus(bool bUnVol);

	/*!
	 * @brief �����쳣״̬
	 * @param bool �쳣��ʶ
	*/
	void SetErrorStatus(bool bError);

	/*!
	 * @brief ���ñ���״̬
	 * @param bool ���ϱ�ʶ
	*/
	void SetObsStatus(bool bObs);

	/*!
	 * @brief �����ػ�״̬
	 * @param bool �ػ���ʶ
	*/
	void SetCargoStatus(bool bCargo);

	/*!
	 * @brief ���ö���״̬
	 * @param unsigned char ����״̬
	*/
	void SetActionStatus(unsigned char byAction);

	/*!
	 * @brief �Ƿ���ʾ
	 * @param bShow trueΪ��ʾ falseΪ����
	*/
	void IsShow(bool bShow);

	/*!
	 * @brief ������������
	 * @param ptCenter ��������(ʸ������)
	*/
	void SetCenterPoint(CPoint ptCenter);

	/*!
	 * @brief ��ȡ��������
	 * @return CPoint ��������(ʸ������)
	*/
	CPoint GetCenterPoint() const;

	/*!
	 * @brief ��ͼ
	 * @param pDC ���ھ��
	*/
	void Draw(CDC *pDC);
};

#endif //!_CGRAPHICAGV_H