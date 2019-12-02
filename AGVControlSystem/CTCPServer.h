/*!
 * @file CTCPServer.h
 * @brief TCP�����ģ���ļ�
 *
 * ���ڴ���TCP����˵�ģ���ļ�
 * @author FanKaiyu
 * @date 2018-12-24
 * @version 3.0
*/
#pragma once
#include <string>
#include <thread>
#include <list>
#include <mutex>

#ifndef _TCPSERVER_H
#define _TCPSERVER_H

/*!
 * @class CTCPServer
 * @brief TCP�����ģ���ļ�
 *
 * ���ڴ���TCP����˵�ģ���ļ�
*/
class CTCPServer
{
public:
	/*!
	 * @brief  Ĭ�Ϲ��캯��
	 * @param unsigned short ����˶˿�
	 * @param wstring �����IP,Ĭ��Ϊ��
	*/
	CTCPServer(unsigned short usPort, std::wstring wstrIP = L"");

	/*!
	 * @brief  Ĭ�Ϲ��캯��
	 * @param unsigned short ����˶˿�
	 * @param string �����IP,Ĭ��Ϊ��
	*/
	CTCPServer(unsigned short usPort,std::string strIP = "");

#ifdef _AFX
	/*!
	 * @brief  Ĭ�Ϲ��캯��
	 * @param unsigned short ����˶˿�
	 * @param CString �����IP,Ĭ��Ϊ��
	*/
	CTCPServer(unsigned short usPort, CString cstrIP = _T(""));
#endif // !_AFX

	~CTCPServer();

protected:
	std::string m_strIP;			/*!< �󶨵ķ����IP,Ϊ��ʱĬ��Ϊ��ǰ����IP��ַ */
	unsigned short m_usPort;		/*!< �󶨵ķ���˶˿� */
	std::thread* m_pThreadListen;	/*!< ����˼����߳�ָ�� */
	std::list<SOCKET> m_listClient;	/*!< �ͻ��˽����б� */
	std::mutex m_mutexLock;			/*!< �̻߳����� */
	SOCKET m_socket;				/*!< �����SOCKET������ */

public:
	/*!
	 * @brief ����˰�IP��ַ
	 * @param wstring �����IP
	*/
	void SetIP(std::wstring wstrIP);

	/*!
	 * @brief ����˰�IP��ַ
	 * @param string �����IP
	*/
	void SetIP(std::string strIP);

#ifdef _AFX
	/*!
	 * @brief ����˰�IP��ַ
	 * @param CString �����IP
	*/
	void SetIP(CString cstrIP);
#endif // !_AFX
	
	/*!
	 * @brief ��ȡ����˰�IP
	 * @return wstring �����IP
	*/
	void GetIP(std::wstring &wstrIP) const;

	/*!
	 * @brief ��ȡ����˰�IP
	 * @return string �����IP
	*/
	void GetIP(std::string &strIP) const;

#ifdef _AFX
	/*!
	 * @brief ��ȡ����˰�IP
	 * @return CString �����IP
	*/
	void GetIP(CString &cstrIP) const;
#endif // !_AFX

	/*!
	 * @brief ���÷���˶˿�
	 * @param unsigned short ����˶˿�,Ӧ������֤�˿ڲ��ᱻ����Ӧ�ó���ռ��.
	*/
	void SetPort(unsigned short unPort);

	/*!
	 * @breif ��ȡ����˶˿�
	 * @return unsigned short ����˶˿�
	*/
	unsigned short GetPort() const;

	/*!
	 * @brief ��ʼ����
	 * @return bool ������ʼ����true,ʧ�ܷ���false
	*/
	bool StartListen();

	/*!
	 * @brief ��������
	*/
	void EndListen();

	/*!
	 * @brief ��ȡ����˼���״̬
	 * @return bool �����з���true,δ��������false
	*/
	bool GetListenStatus() const;

	/*!
	 * @brief ��ȡ����ͻ���SOCKET������
	 * @return SOCKET �����SOCKET������,�޿ͻ��˽��뷵��INVALID_SOCKET
	*/
	SOCKET GetClient();

	/*!
	 * @brief ��ȡSocket�������е�IP��ַ,�˿���Ϣ
	 * @param SOCKET socket������
	 * @param string IP��ַ
	 * @param unsigned short �˿�
	 * @return bool ��ȡ�ɹ�����true,ʧ�ܷ���false
	*/
	static bool GetSocketAddress(SOCKET& socket,std::string& strIP,unsigned short& usPort);

	/*!
	 * @brief ��ȡSocket�������е�IP��ַ,�˿���Ϣ
	 * @param SOCKET socket������
	 * @param wstring IP��ַ
	 * @param unsigned short �˿�
	 * @return bool ��ȡ�ɹ�����true,ʧ�ܷ���false
	*/
	static bool GetSocketAddress(SOCKET& socket, std::wstring& wstrIP, unsigned short& usPort);

#ifdef _AFX
	/*!
	 * @brief ��ȡSocket�������е�IP��ַ,�˿���Ϣ
	 * @param SOCKET socket������
	 * @param CString IP��ַ
	 * @param unsigned short �˿�
	 * @return bool ��ȡ�ɹ�����true,ʧ�ܷ���false
	*/
	static bool GetSocketAddress(SOCKET& socket,CString& cstrIP, unsigned short& usPort);
#endif //_AFX

protected:
	/*!
	 * @brief ���������߳�
	*/
	void OpenListenThread();

	/*!
	 * @brief �رռ����߳�
	*/
	void CloseListenThread();

	/*!
	 * @brief �����߳�
	*/
	void ListenThread();

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
	 * @brief ��ȡ����˼���״̬
	 * @param string& ����˼���״̬
	*/
	void IsListened(std::string& strListen);

	/*!
	 * @brief ��ȡ����˼���״̬
	 * @param wstring& ����˼���״̬
	*/
	void IsListened(std::wstring& wstrListen);

#ifdef _AFX
	/*!
	 * @brief ��ȡ�˿�
	 * @param CString& �˿�
	*/
	void GetPort(CString& cstrPort);

	/*!
	 * @brief ��ȡ����˼���״̬
	 * @param CString& ����˼���״̬
	 * @param COLORREF& ������ʾ�û�����ɫ
	*/
	void IsListened(CString& cstrListen, COLORREF& color);
#endif // !_AFX
};

#endif // !_TCPSERVER_H