/*!
 * @file CCharger.h
 * @brief ����������ļ�
 *
 * ����������Ļ����������������
 * @author FanKaiyu
 * @date 2019-01-11
 * @version 2.0
*/

#pragma once
#include <string>

#define ONEDAY 86400000L	/*!< 1��(ms) */
#define ONESECOND 1000L		/*!< 1��(ms)*/
#define ONEMINUTE 60000L	/*!< 1����(ms) */
#define ONEHOUR 3600000L	/*!< 1Сʱ(ms) */

#ifndef _CHARGER_H
#define _CHARGER_H

/*! @brief �����״̬ */
enum _CHARGERSTATUS
{
	_CHARGERSTATUS_NONE,	/*!< δʹ��:���û�������δ��ʼ���ʱΪ��״̬,���û�ʹ������뿪������������Ϊ��״̬ */
	_CHARGERSTATUS_USING,	/*!< ʹ����:��翪ʼ���û�����ʱ��Ϊ��״̬ */
	_CHARGERSTATUS_FINISH,	/*!< ʹ�����:����������Ϊ��״̬ */
};

/*!
 * @class CCharger
 * @brief ���������
 *
 * ����������Ļ����������������
*/
class CCharger
{
public:
	CCharger(unsigned char byNo, unsigned char byController = 0, std::string strName = "");
	CCharger(unsigned char byNo, unsigned char byController = 0, std::wstring wstrName = L"");
#ifdef _AFX
	CCharger(unsigned char byNo, unsigned char byController = 0, CString cstrName = _T(""));
#endif // _AFX
	~CCharger();
protected:
	unsigned char m_byNo;				/*!< �������� */
#ifdef _WIN64
	unsigned long long m_ullChargeTime;	/*!< ��翪ʼʱ��,ͨ�������Կ��Լ����ͨ��ʱ�� */
#elif _WIN32
	unsigned long m_ulChargeTime;		/*!< ��翪ʼʱ��,ͨ�������Կ��Լ����ͨ��ʱ�� */
#endif // !_WIN64
	unsigned long m_ulMinDuration;		/*!< ��̳�����ʱ��(��λ:ms) */
	void *m_pUser;						/*!< ʹ�ô˳�������û�ָ��,ͨ��ΪAGVָ��,�������ж��û��Ƿ���ռ�ô˳���� */
	unsigned char m_byStatus;			/*!< �����״̬ */
	unsigned char m_byController;		/*!< ���Ʊ�������Ŀ�������� */
	unsigned char m_byPower;			/*!< ��ǰ��ص��� */
	std::string m_strName;				/*!< ���� */

public:
	/*!
	 * @brief ���ñ��
	 * @param unsigned char �³�������
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetNo(unsigned char byNo);

	/*!
	 * @brief ��ȡ���
	 * @return unsigned char ��������
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief ��������
	 * @param string �³��λ����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetName(std::string strName);

	/*!
	 * @brief ��������
	 * @param wstring �³��λ����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetName(std::wstring wstrName);

#ifdef _AFX
	/*!
	 * @brief ��������
	 * @param CString �³��λ����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetName(CString cstrName);
#endif // !_AFX

	/*!
	 * @brief ��ȡ���λ����
	 * @param string& ��λ����
	*/
	void GetName(std::string& strName);

	/*!
	 * @brief ��ȡ���λ����
	 * @param wstring& ��λ����
	*/
	void GetName(std::wstring& wstrName);

#ifdef _AFX
	/*!
	 * @brief ��ȡ���λ����
	 * @param CString& ��λ����
	*/
	void GetName(CString& cstrName);
#endif // !_AFX

	/*!
	 * @brief ��ʼ���
	 * @param unsigned long ��̳�����ʱ��(��λ:ms)
	 * @param void* �û�֮�У���д�˲������ڿ�ʼ����ͬʱ�û������˳����
	*/
	void StartCharging(unsigned long ulMinDuration = 0, void* pUser = nullptr);

	/*!
	 * @brief �������
	 * @param void* �û�ָ��,��д�˲������ڽ�������ͬʱ����û��Գ������ռ��.
	*/
	void EndCharging(void *pUser = nullptr);

	/*!
	 * @brief ���ó����
	*/
	void Reset();

	/*!
	 * @brief ����
	 * @param void *pUser �û�ָ��
	 * @return bool �����ɹ�����true,ʧ�ܷ���false
	*/
	bool Lock(void *pUser);

	/*!
	 * @brief ����
	 * @param void *pUser �û�ָ��
	*/
	void Unlock(void *pUser);

#ifdef _WIN64
	/*!
	 * @brief ��ȡ��ʼ���ʱ��
	 * @return unsigned long long ��翪ʼʱ��(��λ:ms)
	*/
	unsigned long long GetStartTime();
#elif _WIN32
	/*!
	 * @brief ��ȡ��ʼ���ʱ��
	 * @return unsigned long ��翪ʼʱ��(��λ:ms)
	*/
	unsigned long GetStartTime();
#endif // !_WIN64

#ifdef _WIN64
	/*!
	 * @brief ��ȡ������ʱ��
	 * @return unsigned long long ������ʱ��(��λ:ms)
	*/
	unsigned long long GetDurativeTime();
#elif _WIN32
	/*!
	 * @brief ��ȡ������ʱ��
	 * @return unsigned long ������ʱ��(��λ:ms)
	*/
	unsigned long GetDurativeTime();
#endif // !_WIN64

	/*!
	 * @brief ��̳�����ʱ��
	 * @return unsigned long ��̳�����ʱ��(��λ:ms)
	*/
	unsigned long GetMinDurativeTime();

	/*!
	 * @brief ��ȡ״̬
	 * @return unsigned char �����״̬
	*/
	unsigned char GetStatus() const;

	/*!
	 * @brief ��ȡ�û�
	 * @return void* �û�ָ��
	*/
	void* GetUser() const;

	/*!
	 * @brief ���ÿ�����
	 * @param unsigned char �¿��������
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetController(unsigned char byController);

	/*!
	 * @brief ��ȡ������
	 * @return unsigned char ���������
	*/
	unsigned char GetController() const;

	/*!
	 * @brief ���õ�ǰ��ص���(%)
	 * @param unsigned char ��ǰ����µ���
	 * @return ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetPower(unsigned char byPower);

	/*!
	 * @brief ��ȡ��ǰ��ص���(%)
	 * @return unsigned char ��ǰ��ص���
	*/
	unsigned char GetPower() const;
public:
	/*!
	 * @brief ��ȡ���
	 * @param string& ���
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief ��ȡ���
	 * @param wstring& ���
	*/
	void GetNo(std::wstring& wstrNo);

	/*!
	 * @brief ��ȡ������ʱ��
	 * @param string& ������ʱ��
	*/
	void GetDurativeTime(std::string& strTime);

	/*!
	 * @brief ��ȡ������ʱ��
	 * @param wstring& ������ʱ��
	*/
	void GetDurativeTime(std::wstring& wstrTime);

	/*!
	 * @brief ��̳�����ʱ��
	 * @param string& ��̳�����ʱ��
	*/
	void GetMinDurativeTime(std::string& strTime);

	/*!
	 * @brief ��̳�����ʱ��
	 * @param wstring& ��̳�����ʱ��
	*/
	void GetMinDurativeTime(std::wstring& wstrTime);

	/*!
	 * @brief ��ȡ�����״̬
	 * @param string& �����״̬
	*/
	void GetStatus(std::string& strStatus);

	/*!
	 * @brief ��ȡ�����״̬
	 * @param wstring& �����״̬
	*/
	void GetStatus(std::wstring& wstrStatus);

	/*!
	 * @brief ��ȡ������
	 * @param string& ���������
	*/
	void GetController(std::string& strController);

	/*!
	 * @brief ��ȡ������
	 * @param wstring& ���������
	*/
	void GetController(std::wstring& wstrController);

	/*!
	 * @brief ��ȡ��ǰ��ص���(%)
	 * @return string ��ǰ��ص���
	*/
	void GetPower(std::string& strPower);

	/*!
	 * @brief ��ȡ��ǰ��ص���(%)
	 * @return wstring ��ǰ��ص���
	*/
	void GetPower(std::wstring& wstrPower);

	/*!
	 * @brief ��ȡ״̬��
	 * @param string ״̬
	 * @return unsigned char ״̬��
	*/
	static unsigned char GetStatusCode(std::string strStatus);

	/*!
	 * @brief ��ȡ״̬��
	 * @param wstring ״̬
	 * @return unsigned char ״̬��
	*/
	static unsigned char GetStatusCode(std::wstring wstrStatus);

	/*!
	 * @brief ��ȡ״̬
	 * @param unsigned char ״̬��
	 * @param string& ״̬
	*/
	static void GetStatus(unsigned char byStatus, std::string& strStatus);

	/*!
	 * @brief ��ȡ״̬
	 * @param unsigned char ״̬��
	 * @param wstring& ״̬
	*/
	static void GetStatus(unsigned char byStatus, std::wstring& wstrStatus);

#ifdef _AFX
	/*!
	 * @brief ��ȡ���
	 * @param CString& ���
	*/
	void GetNo(CString& cstrNo);

	/*!
	 * @brief ��ȡ������ʱ��
	 * @param CString& ������ʱ��
	*/
	void GetDurativeTime(CString& cstrTime);

	/*!
	 * @brief ��̳�����ʱ��
	 * @param CString& ��̳�����ʱ��
	*/
	void GetMinDurativeTime(CString& cstrTime);

	/*!
	 * @brief ��ȡ�����״̬
	 * @param CString& �����״̬
	 * @param COLORREF& ������ʾ�û�����ɫ
	*/
	void GetStatus(CString& cstrStatus,COLORREF& color);

	/*!
	 * @brief ��ȡ������
	 * @param CString& ���������
	*/
	void GetController(CString& cstrController);

	/*!
	 * @brief ��ȡ��ǰ��ص���(%)
	 * @return CString ��ǰ��ص���
	*/
	void GetPower(CString& cstrPower);

	/*!
	 * @brief ��ȡ״̬��
	 * @param CString ״̬
	 * @return unsigned char ״̬��
	*/
	static unsigned char GetStatusCode(CString cstrStatus);

	/*!
	 * @brief ��ȡ״̬
	 * @param unsigned char ״̬��
	 * @param CString& ״̬
	*/
	static void GetStatus(unsigned char byStatus, CString& cstrStatus);
#endif // !_AFX
};

#endif // !_CHARGER_H