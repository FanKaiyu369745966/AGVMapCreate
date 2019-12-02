/*!
 * @file AGVPath.h
 * @brief AGV·���ṹ���ļ�
 *
 * AGV·����ṹ������
 * @author FanKaiyu
 * @date 2019-01-06
 * @version 1.0
*/
#pragma once
#include "Buffer.h"

#ifndef _AGVPATH_H
#define _AGVPATH_H

/*! @brief RFID������ */
enum _RFIDFUNCTION
{
	_RFIDFUNC_LEFT = 0x1F,				/*!< ���������ƶ� */
	_RFIDFUNC_RIGHT = 0x2F,				/*!< ���Ҳ�����ƶ� */
	_RFIDFUNC_MIDDLE = 0x3F,			/*!< ���м�����ƶ� */
	_RFIDFUNC_STATION = 0x4F,			/*!< ��λ����RFID��Ϊ��λ���ڿ� */
	_RFIDFUNC_REST = 0x5F,				/*!< ����λ����RFID��Ϊ����λ���ڿ� */
	_RFIDFUNC_CHARGER = 0x6F,			/*!< ���λ����RFID��Ϊ���λ���ڿ� */
	_RFIDFUNC_TRAFFIC = 0x7F,			/*!< ��ͨ����λ����RFID��Ϊ��ͨ����λ���ڿ� */
};

#define _RFIDFUNC_OBS_CLOSE 0x01		/*!< AGV�ǽӴ�ʽ���Ϲ� */
#define _RFIDFUNC_MUSIC_CLOSE 0x02		/*!< AGV���ֹ� */

/*! @brief ��λ/���λ���AGVǰ��ĳ��� */
enum _RFIDDIRECTION
{
	_RFIDFUNC_DIRECTION_NONE = 0,	/*!< �� 0000*/
	_RFIDFUNC_DIRECTION_FRONT,		/*!< ǰ 0001*/
	_RFIDFUNC_DIRECTION_BACK,		/*!< �� 0010*/
	_RFIDFUNC_DIRECTION_LEFT,		/*!< �� 0011*/
	_RFIDFUNC_DIRECTION_RIGHT,		/*!< �� 0100*/
};

/*!
 * @brief AGV·�����������ܿ�ṹ��
 * �����������˵��:
 * 1�ֽ� �� 8λ��� �� (��)0000  (��)0000
 * ���и�4λ Ϊ ��λ/���λ���AGVǰ��ĳ���
 * ȡֵ���_RFIDDIRECTION
 * ��4λ Ϊ AGV���Ͽ���״̬�Լ�AGV���ֿ���״̬
 * ȡֵ_RFIDFUNC_OBS_CLOSE �� _RFIDFUNC_MUSIC_CLOSE
 * Ҳ����ʹ��_RFIDFUNC_OBS_CLOSE|_RFIDFUNC_MUSIC_CLOSE ��ʾͬʱ�رձ����Լ�����
 *
 * ����������ĺϳɷ�ʽΪ:
 * ((��λ/���λ���AGVǰ��ĳ���) << 4) | (AGV���Ͽ���״̬�Լ�AGV���ֿ���״̬)
*/
typedef struct _AGVPathPointOtherFunction
{
protected:
	unsigned char m_byDirection:4;
	unsigned char m_byMusic : 1;
	unsigned char m_byObs : 1;
public:
	_AGVPathPointOtherFunction()
	{
		m_byDirection = _RFIDFUNC_DIRECTION_NONE;
		m_byMusic = 0;
		m_byObs = 0;
	}

	_AGVPathPointOtherFunction(bool bMusic, bool bObs, unsigned char byDirection)
	{
		m_byDirection = _RFIDFUNC_DIRECTION_NONE;
		m_byMusic = bMusic & 0x01;
		m_byObs = bObs & 0x01;
	}

	_AGVPathPointOtherFunction(unsigned char byObsAndMusic, unsigned char byDirection)
	{
		m_byDirection = _RFIDFUNC_DIRECTION_NONE;
		SetObsAndMusic(byObsAndMusic);
	}

	_AGVPathPointOtherFunction(unsigned char byFlag)
	{
		SetObsAndMusicAndDirection(byFlag);
	}

	_AGVPathPointOtherFunction(const _AGVPathPointOtherFunction& func)
	{
		m_byDirection = func.m_byDirection;
		m_byMusic = func.m_byMusic;
		m_byObs = func.m_byObs;
	}

	void operator=(const _AGVPathPointOtherFunction& func)
	{
		m_byDirection = func.m_byDirection;
		m_byMusic = func.m_byMusic;
		m_byObs = func.m_byObs;
	}
public:
	/*!
	 * @brief ���ñ���״̬
	 * @param bool �±��Ͽ���״̬
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetObs(bool bObs)
	{
		if (m_byObs == bObs)
		{
			return false;
		}

		m_byObs = bObs & 0x01;

		return;
	}

	/*!
	 * @brief ��ȡ����״̬
	 * @return bool ���Ͽ���״̬
	*/
	bool GetObs() const
	{
		return m_byObs;
	}

	/*!
	 * @brief ��������״̬
	 * @param bool �����ֿ���״̬
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetMusic(bool bMusic)
	{
		if (m_byMusic == bMusic)
		{
			return false;
		}

		m_byMusic = bMusic & 0x01;

		return;
	}

	/*!
	 * @brief ��ȡ����״̬
	 * @return bool ���ֿ���״̬
	*/
	bool GetMusic() const
	{
		return m_byMusic;
	}

	/*!
	 * @brief �������������״̬
	 * @param unsigned char ���������״̬��
	*/
	void SetObsAndMusic(unsigned char byFlag)
	{
		m_byObs = byFlag & 0x1;
		m_byMusic = (byFlag >> 1) & 0x1;

		return;
	}

	/*!
	 * @brief ���÷���
	 * @param unsigned char ������
	*/
	void SetDirection(unsigned char byFlag)
	{
		m_byDirection = byFlag;

		return;
	}

	/*!
	 * @brief ��ȡ����
	 * @return unsigned char ������
	*/
	unsigned char GetDirection() const
	{
		return m_byDirection;
	}

	/*!
	 * @brief ��ȡ����
	 * @param string& ����˵��
	*/
	void GetDirection(std::string& strDirection)
	{
		switch (m_byDirection)
		{
		case _RFIDFUNC_DIRECTION_NONE:
			strDirection = "�޳���";
			break;
		case _RFIDFUNC_DIRECTION_FRONT:
			strDirection = "ǰ��";
			break;
		case _RFIDFUNC_DIRECTION_BACK:
			strDirection = "����";
			break;
		case _RFIDFUNC_DIRECTION_LEFT:
			strDirection = "����";
			break;
		case _RFIDFUNC_DIRECTION_RIGHT:
			strDirection = "����";
			break;
		}

		return;
	}

	/*!
	 * @brief ��ȡ����
	 * @param wstring& ����˵��
	*/
	void GetDirection(std::wstring& wstrDirection)
	{
		std::string strDirection = "";
		GetDirection(strDirection);

		std_string_to_wstring(wstrDirection, strDirection);

		return;
	}

#ifdef _AFX
	/*!
	 * @brief ��ȡ����
	 * @param CString& ����˵��
	*/
	void GetDirection(CString &cstrDirection)
	{
		std::string strDirection = "";
		GetDirection(strDirection)

			std_string_to_CStrig(cstrDirection, strDirection);

		return;
	}
#endif // !_AFX

	/*!
	 * @brief �������֡������Լ�����
	 * @param unsigned char ���֡������Լ����������
	*/
	void SetObsAndMusicAndDirection(unsigned char byFlag)
	{
		SetObsAndMusic(byFlag & 0xF);
		SetDirection((byFlag >> 4) & 0xF);

		return;
	}

	/*!
	 * @brief ��ȡ���֡������Լ����������
	 * @return unsigned char ���֡������Լ����������
	*/
	unsigned char GetFlag() const
	{
		return (m_byDirection << 4) | ((m_byMusic << 1) | m_byObs);
	}

} _OtherFunc;

/*!
 * @brief AGV·����ṹ��
 *
 * ��Ҫ��
 * RFID�����(2�ֽ�)
 * ������(1�ֽ�)�����_RFIDFUNCTION
 * ���ٶ�(1�ֽ�)�� ��ֵΪǰ��,��ֵΪ����,0Ϊֹͣ
 * ��������(1�ֽ�)�����а�����λ/���λ���AGVǰ��ĳ���AGV���Ͽ���״̬�Լ�AGV���ֿ���״̬
 * ���
*/
typedef struct _AGVPathPointStruct
{
protected:
	unsigned short m_usRFID;		/*!< RFID����� */
	unsigned char m_byFunc;			/*!< �������� */
	char m_chSpeed;					/*!< ���ٶ� */
public:
	_OtherFunc m_otherFunc;			/*!< �������� */

public:
	_AGVPathPointStruct(unsigned short usRFID,unsigned char byFunc,char chSpeed,bool bMusic = true,bool bObs = true,unsigned char byDirection = _RFIDFUNC_DIRECTION_NONE)
	{
		m_usRFID = usRFID;
		m_byFunc = byFunc;
		m_chSpeed = chSpeed;
		m_otherFunc.SetObs(bObs);
		m_otherFunc.SetMusic(bMusic);
		m_otherFunc.SetDirection(byDirection);
	}

	_AGVPathPointStruct(unsigned short usRFID, unsigned char byFunc, char chSpeed ,unsigned char byObsAndMusic = 0, unsigned char byDirection = _RFIDFUNC_DIRECTION_NONE)
	{
		m_usRFID = usRFID;
		m_byFunc = byFunc;
		m_chSpeed = chSpeed;
		m_otherFunc.SetObsAndMusic(byObsAndMusic);
		m_otherFunc.SetDirection(byDirection);
	}

	_AGVPathPointStruct(unsigned short usRFID, unsigned char byFunc, char chSpeed, unsigned char byFlag = 0)
	{
		m_usRFID = usRFID;
		m_byFunc = byFunc;
		m_chSpeed = chSpeed;
		m_otherFunc.SetObsAndMusicAndDirection(byFlag);
	}

	_AGVPathPointStruct(const _AGVPathPointStruct& path)
	{
		m_usRFID = path.m_usRFID;
		m_byFunc = path.m_byFunc;
		m_chSpeed = path.m_chSpeed;
		m_otherFunc = path.m_otherFunc;
	}

	void operator=(const _AGVPathPointStruct& path)
	{
		m_usRFID = path.m_usRFID;
		m_byFunc = path.m_byFunc;
		m_chSpeed = path.m_chSpeed;
		m_otherFunc = path.m_otherFunc;
	}

	/*!
	 * @brief ����RFID�����
	 * @param unsigned short ��RFID�����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetRFID(unsigned short usRFID)
	{
		if (m_usRFID == usRFID)
		{
			return false;
		}

		m_usRFID = usRFID;

		return true;
	}

	/*!
	 * @brief ��ȡRFID�����
	 * @return unsigned short RFID�����
	*/
	unsigned short GetRFID() const
	{
		return m_usRFID;
	}

	/*!
	 * @brief ���ù���
	 * @param unsigned char �¹���
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetFunction(unsigned char byFync)
	{
		if (m_byFunc == byFync)
		{
			return false;
		}

		m_byFunc = byFync;

		return true;
	}

	/*!
	 * @brief ��ȡ����
	 * @return unsigned char ������
	*/
	unsigned char GetFunction()
	{
		return m_byFunc;
	}

	/*!
	 * @brief ��ȡ����
	 * @param string& ����˵��
	*/
	void GetFunction(std::string &strFunc)
	{
		switch (m_byFunc)
		{
		case _RFIDFUNC_LEFT:
			strFunc = "���������ƶ�";
			break;
		case _RFIDFUNC_RIGHT:
			strFunc = "���Ҳ�����ƶ�";
			break;
		case _RFIDFUNC_MIDDLE:
			strFunc = "���м�����ƶ�";
			break;
		case _RFIDFUNC_STATION:
			strFunc = "��λ";
			break;
		case _RFIDFUNC_REST:
			strFunc = "����λ";
			break;
		case _RFIDFUNC_CHARGER:
			strFunc = "���λ";
			break;
		case _RFIDFUNC_TRAFFIC:
			strFunc = "��ͨ����";
			break;
		}

		return;
	}

	/*!
	 * @brief ��ȡ����
	 * @param wstring& ����˵��
	*/
	void GetFunction(std::wstring &wstrFunc)
	{
		std::string strFunc = "";
		GetFunction(strFunc);

		std_string_to_wstring(wstrFunc, strFunc);

		return;
	}

#ifdef _AFX
	/*!
	 * @brief ��ȡ����
	 * @param CString& ����˵��
	*/
	void GetFunction(CString &cstrFunc)
	{
		std::string strFunc = "";
		GetFunction(strFunc);

		std_string_to_CStrig(cstrFunc, strFunc);

		return;
	}
#endif // !_AFX

	/*!
	 * @brief �����ٶ�
	 * @param char ���ٶ�
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetSpeed(char chSpeed)
	{
		if (m_chSpeed == chSpeed)
		{
			return false;
		}

		if (chSpeed > 100 | chSpeed < -100)
		{
			return false;
		}

		m_chSpeed = chSpeed;

		return true;
	}

	/*!
	 * @brief ��ȡ�ٶ�
	 * @return char �ٶ�
	*/
	char GetSpeed() const
	{
		return m_chSpeed;
	}

}_AGVPatchPoint;

#endif // !_AGVPATH_H