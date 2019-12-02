#include "stdafx.h"
#include "CTCPServer.h"
#include "Buffer.h"
#include "DebugPrint.h"
#include <WS2tcpip.h>

#ifdef _TCPSERVER_H

CTCPServer::CTCPServer(unsigned short usPort, std::wstring wstrIP)
{
	BufferString::std_wstring_to_string(m_strIP, wstrIP);
	m_usPort = usPort;
	m_pThreadListen = nullptr;
	m_socket = INVALID_SOCKET;
}


CTCPServer::CTCPServer(unsigned short usPort, std::string strIP)
{
	m_strIP = strIP;
	m_usPort = usPort;
	m_pThreadListen = nullptr;
	m_socket = INVALID_SOCKET;
}


#ifdef _AFX
CTCPServer::CTCPServer(unsigned short usPort, CString cstrIP)
{
	BufferString::CString_to_std_string(m_strIP, cstrIP);
	m_usPort = usPort;
	m_pThreadListen = nullptr;
	m_socket = INVALID_SOCKET;
}
#endif // !_AFX


CTCPServer::~CTCPServer()
{
	EndListen();
}


void CTCPServer::SetIP(std::wstring wstrIP)
{
	std::string strIP = "";
	BufferString::std_wstring_to_string(strIP, wstrIP);

	SetIP(strIP);

	return;
}


void CTCPServer::SetIP(std::string strIP)
{
	m_strIP = strIP;

	if (GetListenStatus())
	{
		// ��������
		EndListen();
		StartListen();
	}

	return;
}


#ifdef _AFX
void CTCPServer::SetIP(CString cstrIP)
{
	std::string strIP = "";
	BufferString::CString_to_std_string(strIP, cstrIP);

	SetIP(strIP);

	return;
}
#endif // !_AFX


void CTCPServer::GetIP(std::wstring &wstrIP) const
{
	if(m_strIP != "")
	{
		BufferString::std_string_to_wstring(wstrIP, m_strIP);
	}
	else
	{
		std::string strIP = "";
		GetIP(strIP);

		BufferString::std_string_to_wstring(wstrIP, strIP);
	}

	return;
}


void CTCPServer::GetIP(std::string &strIP) const
{
	if (m_strIP != "")
	{
		strIP = m_strIP;
	}
	else
	{
		char *lpszLocalIP = new char[IP4_ADDRESS_STRING_BUFFER_LENGTH];
		if (GetLocalIP(lpszLocalIP))
		{
			strIP = lpszLocalIP;
		}
		else
		{
			strIP = "0.0.0.0";
		}

		delete[] lpszLocalIP;
	}

	return;
}


#ifdef _AFX
void CTCPServer::GetIP(CString &cstrIP) const
{
	if (m_strIP != "")
	{
		BufferString::std_string_to_CString(cstrIP, m_strIP);
	}
	else
	{
		char *lpszLocalIP = new char[IP4_ADDRESS_STRING_BUFFER_LENGTH];
		if (GetLocalIP(lpszLocalIP))
		{
			BufferString::std_string_to_CString(cstrIP, lpszLocalIP);
		}
		else
		{
			cstrIP = _T("0.0.0.0");
		}

		delete[] lpszLocalIP;
	}
	return;
}
#endif // !_AFX


void CTCPServer::SetPort(unsigned short usPort)
{
	m_usPort = usPort;

	if (GetListenStatus())
	{
		// ��������
		EndListen();
		StartListen();
	}

	return;
}


unsigned short CTCPServer::GetPort() const
{
	return m_usPort;
}


bool CTCPServer::StartListen()
{
	EndListen();

	std::string strIP = "";
	GetIP(strIP);

	WSADATA wsd;

	SOCKADDR_IN addrServ; /*!< ��������ַ*/

	int retVal; /*!< ����ֵ*/

	// ��ʼ���׽��ֶ�̬��  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		DebugPrint("AGVControlSystem/CTCPServer.cpp/StartListen Error:�����:%s:%d ��ʼ���׽���ʧ��.", strIP.c_str(),m_usPort);

		return false;
	}

	//�����׽���  
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == m_socket)
	{
		//�ͷ��׽�����Դ
		WSACleanup();

		DebugPrint("AGVControlSystem/CTCPServer.cpp/StartListen Error:�����:%s:%d �����׽���ʧ��.", strIP.c_str(), m_usPort);

		return false;
	}

	//�������׽��ֵ�ַ   
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(m_usPort);
	if (m_strIP == "")
	{
		addrServ.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		// addrServ.sin_addr.s_addr = inet_addr(m_strIP.c_str());

		inet_pton(AF_INET, m_strIP.c_str(), &addrServ.sin_addr.s_addr);
	}

	//���׽���  
	retVal = ::bind(m_socket, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));

	if (SOCKET_ERROR == retVal)
	{
		//�ر��׽���
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;

		//�ͷ��׽�����Դ;
		WSACleanup();

		DebugPrint("AGVControlSystem/CTCPServer.cpp/StartListen Error:�����:%s:%d ���׽���ʧ��.", strIP.c_str(), m_usPort);

		return false;
	}

	OpenListenThread();

	return true;
}


void CTCPServer::EndListen()
{
	std::string strIP = "";
	GetIP(strIP);

	CloseListenThread();

	std::lock_guard<std::mutex> lock(m_mutexLock);

	for (std::list<SOCKET>::iterator it = m_listClient.begin(); it != m_listClient.end();)
	{
		std::string strClientIP = "";
		unsigned short usClientPort = 0;

		GetSocketAddress(*it, strClientIP, usClientPort);

		DebugPrint("AGVControlSystem/CTCPServer.cpp/EndListen �����:%s:%d �رտͻ�������.SOCKET:%d.IP:%s:%d.", strIP.c_str(), m_usPort,*it, strClientIP.c_str(), usClientPort);

		shutdown(*it, SD_BOTH);
		closesocket(*it);
		it = m_listClient.erase(it);
	}

	return;
}


bool CTCPServer::GetListenStatus() const
{
	if (m_socket == INVALID_SOCKET)
	{
		return false;
	}

	return true;
}


SOCKET CTCPServer::GetClient()
{
	std::string strIP = "";
	GetIP(strIP);

	SOCKET socket = INVALID_SOCKET;

	if (m_listClient.size() == 0)
	{
		return socket;
	}

	std::lock_guard<std::mutex> lock(m_mutexLock);

	socket = m_listClient.front();

	m_listClient.pop_front();

	std::string strClientIP = "";
	unsigned short usClientPort = 0;

	GetSocketAddress(socket, strClientIP, usClientPort);

	DebugPrint("AGVControlSystem/CTCPServer.cpp/GetClient �����:%s:%d ��ȡ�ͻ�������.SOCKET:%d.IP:%s:%d.", strIP.c_str(), m_usPort, socket, strClientIP.c_str(), usClientPort);

	return socket;
}

bool CTCPServer::GetSocketAddress(SOCKET & socket, std::string & strIP, unsigned short & usPort)
{
	std::string strClientIP = "";
	unsigned int unClientPort = 0;

	if (socket == INVALID_SOCKET)
	{
		DebugPrint("AGVControlSystem/CTCPServer.cpp/GetSocketAddress ��Ч��SOCKET�׽���!");
		return false;
	}

	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);

	//�����׽��ֻ�ȡ��ַ��Ϣ
	if (::getpeername(socket, (SOCKADDR*)&addrClient, &addrClientlen) != 0)
	{
		DebugPrint("AGVControlSystem/CTCPServer.cpp/GetSocketAddress ��SOCKET:%d�׽����л�ȡIP��ַ��Ϣʧ��!", socket);
		return false;
	}

	// strIP = inet_ntoa(addrClient.sin_addr);
	char lpszIP[IP4_ADDRESS_STRING_BUFFER_LENGTH];
	inet_ntop(AF_INET, (void*)&addrClient.sin_addr, lpszIP, IP4_ADDRESS_STRING_BUFFER_LENGTH);
	strIP = lpszIP;

	usPort = htons(addrClient.sin_port);

	return true;
}

bool CTCPServer::GetSocketAddress(SOCKET & socket, std::wstring & wstrIP, unsigned short & usPort)
{
	std::string strIP = "";

	if (GetSocketAddress(socket, strIP, usPort) == false)
	{
		return false;
	}

	BufferString::std_string_to_wstring(wstrIP, strIP);

	return true;
}

#ifdef _AFX
bool CTCPServer::GetSocketAddress(SOCKET & socket, CString & cstrIP, unsigned short & usPort)
{
	std::string strIP = "";

	if (GetSocketAddress(socket, strIP, usPort) == false)
	{
		return false;
	}

	BufferString::std_string_to_CString(cstrIP, strIP);

	return true;
}
#endif //_AFX

void CTCPServer::OpenListenThread()
{
	std::string strIP = "";
	GetIP(strIP);

	if (m_pThreadListen)
	{
		CloseListenThread();
	}

	m_pThreadListen = new std::thread(&CTCPServer::ListenThread, this);

	DebugPrint("AGVControlSystem/CTCPServer.cpp/CloseListenThread �����:%s:%d ���������������߳�.", strIP.c_str(), m_usPort);
	TRACE(_T("[New]�����̵߳�ַ:%08X.\r\n"), m_pThreadListen);

	return;
}


void CTCPServer::CloseListenThread()
{
	std::string strIP = "";
	GetIP(strIP);

	if (m_pThreadListen != nullptr)
	{
		if (m_pThreadListen->joinable())
		{
			if (m_socket != INVALID_SOCKET)
			{
				// �ر�SOCKET�׽���
				shutdown(m_socket, SD_BOTH);
				closesocket(m_socket);
				m_socket = INVALID_SOCKET;

				DebugPrint("AGVControlSystem/CTCPServer.cpp/CloseListenThread �����:%s:%d �رշ�����׽���.", strIP.c_str(), m_usPort);
			}

			DebugPrint("----AGVControlSystem/CTCPServer.cpp/CloseListenThread �����:%s:%d �ȴ������߳̽���.", strIP.c_str(), m_usPort);

			m_pThreadListen->join();
		}

		DebugPrint("--------AGVControlSystem/CTCPServer.cpp/CloseListenThread �����:%s:%d ���ټ����߳�.", strIP.c_str(), m_usPort);
		TRACE(_T("[Delete]�����̵߳�ַ:%08X.\r\n"), m_pThreadListen);

		delete m_pThreadListen;
		m_pThreadListen = nullptr;
	}

	return;
}


void CTCPServer::ListenThread()
{
	std::string strIP = "";
	GetIP(strIP);

	// �߳�����
	DebugPrint("--------AGVControlSystem/CTCPServer.cpp/CloseListenThread �����:%s:%d ������ʼ.--------", strIP.c_str(), m_usPort);

	while (true)
	{
		//��ʼ����   
		int retVal = listen(m_socket, 1); /*!< ����ֵ */

		if (SOCKET_ERROR == retVal)
		{
			// �����쳣

			//�ر��׽���
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;

			//�ͷ��׽�����Դ;
			WSACleanup();

			DebugPrint("AGVControlSystem/CTCPServer.cpp/ListenThread Error:�����:%s:%d �����쳣,�˳������߳�.", strIP.c_str(), m_usPort);

			return;
		}

		//���ܿͻ�������  
		sockaddr_in addrClient; /*!< �ͻ��˵�ַ*/

		int addrClientlen = sizeof(addrClient); /*!< �ͻ��˳���*/

		SOCKET sClient = accept(m_socket, (sockaddr FAR*)&addrClient, &addrClientlen); /*!< �ͻ���SOCKET������*/

		if (INVALID_SOCKET == sClient)
		{
			DebugPrint("----AGVControlSystem/CTCPServer.cpp/ListenThread Error:�����:%s:%d ���տͻ�������ʧ��,�˳������߳�.",strIP.c_str(), m_usPort);

			// �ͻ��˽����쳣
			std::list<SOCKET>::iterator it;

			for (it = m_listClient.begin(); it != m_listClient.end(); ++it)
			{
				std::string strClientIP = "";
				unsigned short usClientPort = 0;

				GetSocketAddress(*it, strClientIP, usClientPort);

				DebugPrint("--------AGVControlSystem/CTCPServer.cpp/ListenThread Error:�����:%s:%d �رտͻ�������.SOCKET:%d.IP:%s:%d.", strIP.c_str(), m_usPort, *it, strClientIP, usClientPort);

				shutdown(*it, SD_BOTH);
				closesocket(*it);
			}

			//�ر��׽���
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;

			//�ͷ��׽�����Դ;
			WSACleanup();

			return;
		}
		else
		{
			// ����
			m_mutexLock.lock();

			if (m_listClient.size() == m_listClient.max_size())
			{
				DebugPrint("----AGVControlSystem/CTCPServer.cpp/ListenThread �����:%s:%d �ͻ��˽����б�����.", strIP.c_str(), m_usPort);

				shutdown(m_listClient.front(), SD_BOTH);
				closesocket(m_listClient.front());
				m_listClient.pop_front();
			}

			std::string strClientIP = "";
			unsigned short usClientPort = 0;

			GetSocketAddress(sClient, strClientIP, usClientPort);

			DebugPrint("--------AGVControlSystem/CTCPServer.cpp/ListenThread �����:%s:%d ���տͻ�������.SOCKET:%d.IP:%s:%d.", strIP.c_str(), m_usPort,sClient, strClientIP, usClientPort);

			// �����б�
			m_listClient.push_back(sClient);

			// ����
			m_mutexLock.unlock();
		}

		Sleep(1);
	}

	DebugPrint("--------AGVControlSystem/CTCPServer.cpp/ListenThread �����:%s:%d ����˼����߳��˳�.--------", strIP.c_str(), m_usPort);

	return;
}


bool CTCPServer::GetLocalIP(char* lpszIP) const
{
	//1.��ʼ��wsa  
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		return false;
	}

	//2.��ȡ������  
	char hostname[256];
	ret = gethostname(hostname, sizeof(hostname));
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	DebugPrint("AGVControlSystem/CTCPServer.cpp/GetLocalIP hostname:%s\n", hostname);

	//3.��ȡ����ip  
	struct addrinfo hints;
	struct addrinfo *res, *cur;
	struct sockaddr_in *addr;
	// char lpszIP[IP4_ADDRESS_STRING_BUFFER_LENGTH];

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;		/* Allow IPv4 */
	hints.ai_flags = AI_PASSIVE;	/* For wildcard IP address */
	hints.ai_protocol = 0;			/* Any protocol */
	hints.ai_socktype = SOCK_STREAM;

	char lpszPort[6];
	sprintf_s(lpszPort, 6, "%d", m_usPort);
	getaddrinfo(hostname, lpszPort, &hints, &res);

	if (ret == -1)
	{
		perror("getaddrinfo");
		return false;
	}

	for (cur = res; cur != NULL; cur = cur->ai_next)
	{
		addr = (struct sockaddr_in *)cur->ai_addr;

		sprintf_s(lpszIP, IP4_ADDRESS_STRING_BUFFER_LENGTH, "%d.%d.%d.%d",
			(*addr).sin_addr.S_un.S_un_b.s_b1,
			(*addr).sin_addr.S_un.S_un_b.s_b2,
			(*addr).sin_addr.S_un.S_un_b.s_b3,
			(*addr).sin_addr.S_un.S_un_b.s_b4);

		DebugPrint("AGVControlSystem/CTCPServer.cpp/GetLocalIP IP:%s\n", lpszIP);
	}

	freeaddrinfo(res);

	WSACleanup();

	/*
	HOSTENT* host = gethostbyname(hostname);
	if (host == NULL)
	{
		return false;
	}
	*/

	//4.ת��Ϊchar*����������  
	// strcpy_s(lpszIP,IP4_ADDRESS_STRING_BUFFER_LENGTH, inet_ntoa(*(in_addr*)*host->h_addr_list));

	//inet_ntop(AF_INET, (in_addr*)*host->h_addr_list, lpszIP, IP4_ADDRESS_STRING_BUFFER_LENGTH);

	return true;
}

void CTCPServer::GetPort(std::string & strPort)
{
	BufferString::std_string_format(strPort, "%d", m_usPort);

	return;
}

void CTCPServer::GetPort(std::wstring & wstrPort)
{
	std::string strPort = "";
	GetPort(strPort);

	BufferString::std_string_to_wstring(wstrPort, strPort);

	return;
}

void CTCPServer::IsListened(std::string & strListen)
{
	if (GetListenStatus())
	{
		strListen = "�Ѽ���";
	}
	else
	{
		strListen = "δ����";
	}

	return;
}

void CTCPServer::IsListened(std::wstring & wstrListen)
{
	std::string strListen = "";
	IsListened(strListen);

	BufferString::std_string_to_wstring(wstrListen, strListen);

	return;
}

void CTCPServer::GetPort(CString & cstrPort)
{
	std::string strPort = "";
	GetPort(strPort);

	BufferString::std_string_to_CString(cstrPort, strPort);

	return;
}

void CTCPServer::IsListened(CString & cstrListen, COLORREF & color)
{
	std::string strListen = "";
	IsListened(strListen);

	BufferString::std_string_to_CString(cstrListen, strListen);

	if (GetListenStatus())
	{
		color = COLOR_GREEN;
	}
	else
	{
		color = COLOR_GRAY;
	}

	return;
}

#endif // !_TCPSERVER_H