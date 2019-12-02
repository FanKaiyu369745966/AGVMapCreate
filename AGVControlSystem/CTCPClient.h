/*!
 * @file CTCPClient.h
 * @brief TCP�ͻ���ģ���ļ�
 *
 * ���ڴ���TCP�ͻ��˵�ģ���ļ�
 * @author FanKaiyu
 * @date 2018-12-24
 * @version 3.0
*/

#pragma once
#include <string>
#include <thread>
#include <mutex>
#include "Buffer.h"

#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H

/*!
 * @class CTCPClient
 * @brief TCP�ͻ���ģ���ļ�
 *
 * ���ڴ���TCP�ͻ��˵�ģ���ļ�
*/
class CTCPClient
{
public:
	/*!
	 * @brief  Ĭ�Ϲ��캯��
	 * @param wstring �ͻ���IP
	 * @param short �ͻ��˶˿�,Ĭ��Ϊ-1������˿�
	*/
	CTCPClient(std::wstring wstrIP,short sPort = -1);

	/*!
	 * @brief  Ĭ�Ϲ��캯��
	 * @param string �ͻ���IP
	 * @param short �ͻ��˶˿�,Ĭ��Ϊ-1������˿�
	*/
	CTCPClient(std::string strIP, short sPort = -1);

#ifdef _AFX
	/*!
	 * @brief  Ĭ�Ϲ��캯��
	 * @param CString �ͻ���IP
	 * @param short �ͻ��˶˿�,Ĭ��Ϊ-1�������
	*/
	CTCPClient(CString cstrIP, short sPort = -1);
#endif // !_AFX
	
	~CTCPClient();

protected:
	std::string m_strIP;				/*!< �ͻ��˰󶨵�IP��ַ */
	short m_sPort;						/*!< �ͻ��˰󶨵Ķ˿� */
	std::thread *m_pThreadRecv;			/*!< ���ݽ����߳�ָ�� */
	std::mutex m_mutexLockRecv;			/*!< ���ݽ����߳��� */
	SOCKET m_socket;					/*!< �ͻ���SOCKET������ */
	BufferString::Buffer m_bufferRecv;	/*!< ���Խ������ݵĻ����� */
#ifdef _WIN64
	unsigned long long m_ullRecvTime;	/*!< �������ݵ�ʱ��(ϵͳ����ʱ��) */
#elif _WIN32
	unsigned long m_ulRecvTime;			/*!< �������ݵ�ʱ��(ϵͳ����ʱ��) */
#endif // !_WIN64

public:
	/*!
	 * @brief �ͻ��˰�IP��ַ
	 * @param wstring �ͻ��˰�IP
	*/
	void SetIP(std::wstring wstrIP);

	/*!
	 * @brief �ͻ��˰�IP��ַ
	 * @param string �ͻ��˰�IP
	*/
	void SetIP(std::string strIP);

#ifdef _AFX
	/*!
	 * @brief �ͻ��˰�IP��ַ
	 * @param CString �ͻ��˰�IP
	*/
	void SetIP(CString cstrIP);
#endif // !_AFX

	/*!
	 * @brief ��ȡ�ͻ��˰�IP
	 * @return wstring �ͻ��˰�IP
	*/
	void GetIP(std::wstring &wstrIP) const;

	/*!
	 * @brief ��ȡ�ͻ��˰�IP
	 * @return string �ͻ��˰�IP
	*/
	void GetIP(std::string &strIP) const;

#ifdef _AFX
	/*!
	 * @brief ��ȡ�ͻ��˰�IP
	 * @return CString �ͻ��˰�IP
	*/
	void GetIP(CString &cstrIP) const;
#endif // !_AFX

	/*!
	 * @brief ���ÿͻ��˰󶨶˿�
	 * @param short �ͻ��˰󶨶˿�,Ӧ������֤�˿ڲ��ᱻ����Ӧ�ó���ռ��.
	*/
	void SetPort(short sPort);

	/*!
	 * @breif ��ȡ�ͻ��˰󶨶˿�
	 * @return short �ͻ��˰󶨶˿�
	*/
	short GetPort() const;

	/*!
	 * @brief ���ӿͻ���
	 * @param SOCKET �ͻ���socket������
	 * @return bool ���ӳɹ�����true,ʧ�ܷ���false
	*/
	bool Connect(SOCKET socket);

	/*!
	 * @brief ���ӷ����
	 * @param string �����IP��ַ
	 * @param unsigned short ����˶˿� 
	 * @return bool ���ӳɹ�����true,ʧ�ܷ���false
	*/
	bool Connect(std::string strIP, unsigned short usPort);

	/*!
	 * @brief ���ӷ����
	 * @param wstring �����IP��ַ
	 * @param unsigned short ����˶˿�
	 * @return bool ���ӳɹ�����true,ʧ�ܷ���false
	*/
	bool Connect(std::wstring wstrIP, unsigned short usPort);

#ifdef _AFX
	/*!
	 * @brief ���ӷ����
	 * @param CString �����IP��ַ
	 * @param unsigned short ����˶˿�
	 * @return bool ���ӳɹ�����true,ʧ�ܷ���false
	*/
	bool Connect(CString cstrIP, unsigned short usPort);
#endif // !_AFX

	/*!
	 * @brief �ж�����
	*/
	void Disconnect();

	/*!
	 * @brief ��ȡ�ͻ�������״̬
	 * @return bool �����ӷ���true,δ���ӷ���false
	*/
	bool IsConnected() const;

	/*!
	 * @brief ��������
	 * @param char* ���ݿ��׵�ַ
	 * @param unsigned int ���ݿⳤ��
	 * @return bool ���ͳɹ�����true,ʧ�ܷ���false
	*/
	bool SendData(char *pData, unsigned int unLen);

	/*!
	 * @brief ��������
	 * 
	 * �����ݿ�������ݻ�����ͷ��
	 * @param char* ���ݿ��׵�ַ
	 * @param unsigned int ���ݿ鳤��
	*/
	void InsertData(char *pData, unsigned int unLen);

	/*!
	 * @brief �������
	 *
	 * �����ݿ�������ݻ�����β��
	 * @param char* ���ݿ��׵�ַ
	 * @param unsigned int ���ݿⳤ��
	*/
	void AddData(char *pData, unsigned int unLen);

	/*!
	 * @brief ��ȡ����
	 *
	 * �����ݴ����ݻ��������������������ݻ�����
	 * @return Buffer ���ݿ�
	*/
	BufferString::Buffer GetBuffer();

#ifdef _WIN64
	/*!
	 * @brief ��ȡ���ݽ��յ�ʱ��(ϵͳ����ʱ��)
	 * @param unsigned long long ���ݽ��յ�ʱ��(ϵͳ����ʱ��)
	*/
	void GetRecvTime(unsigned long long& ullRecvTime);
#elif _WIN32
	/*!
	 * @brief ��ȡ���ݽ��յ�ʱ��(ϵͳ����ʱ��)
	 * @param unsigned long ���ݽ��յ�ʱ��(ϵͳ����ʱ��)
	*/
	void GetRecvTime(unsigned long& ulRecvTime);
#endif // !_WIN64

protected:
	/*!
	 * @brief �������ݽ����߳�
	 * @return bool �����ɹ�����true,ʧ�ܷ���false
	*/
	bool OpenDataRecvThread();

	/*!
	 * @brief �ر����ݽ����߳�
	*/
	void CloseDataRecvThread();

	/*!
	 * @brief ���ݽ����߳�
	*/
	void DataRecvThread();

	/*!
	 * @brief ��ȡ����IP��ַ
	 * @param char* ����IP��ַ�ַ���
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool GetLocalIP(char* lpszIP) const;

public:
	/*!
	 * @brief ��ȡ�˿�
	 * @param string& �˿�
	*/
	void GetPort(std::string& strPort);

	/*!
	 * @brief ��ȡ�˿�
	 * @param string& �˿�
	*/
	void GetPort(std::wstring& wstrPort);

	/*! 
	 * @brief ��ȡ�ͻ�������״̬
	 * @param string& �ͻ�������״̬
	*/
	void IsConnected(std::string& strConnect);

	/*!
	 * @brief ��ȡ�ͻ�������״̬
	 * @param wstring& �ͻ�������״̬
	*/
	void IsConnected(std::wstring& wstrConnect);

#ifdef _AFX
	/*!
	 * @brief ��ȡ�˿�
	 * @param CString& �˿�
	*/
	void GetPort(CString& cstrPort);

	/*!
	 * @brief ��ȡ�ͻ�������״̬
	 * @param CString& �ͻ�������״̬
	 * @param COLORREF& ������ʾ�û�����ɫ
	*/
	void IsConnected(CString& cstrConnect, COLORREF& color);
#endif // !_AFX
};

#endif //!_TCPCLIENT_H