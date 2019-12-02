#include "stdafx.h"
#include "CTCPClient.h"
#include <Ws2tcpip.h>

#ifdef _TCPCLIENT_H

CTCPClient::CTCPClient(std::wstring wstrIP, short sPort)
{
	BufferString::std_wstring_to_string(m_strIP, wstrIP);

	m_sPort = sPort;
	m_pThreadRecv = nullptr;
	m_socket = INVALID_SOCKET;

#ifdef _WIN64
	m_ullRecvTime = 0;
#elif _WIN32
	m_ulRecvTime = 0;
#endif //!_WIN64
}

CTCPClient::CTCPClient(std::string strIP, short sPort)
{
	m_strIP = strIP;
	m_sPort = sPort;
	m_pThreadRecv = nullptr;
	m_socket = INVALID_SOCKET;

#ifdef _WIN64
	m_ullRecvTime = 0;
#elif _WIN32
	m_ulRecvTime = 0;
#endif //!_WIN64
}

#ifdef _AFX
CTCPClient::CTCPClient(CString cstrIP, short sPort)
{
	BufferString::CString_to_std_string(m_strIP, cstrIP);

	m_sPort = sPort;
	m_pThreadRecv = nullptr;
	m_socket = INVALID_SOCKET;

#ifdef _WIN64
	m_ullRecvTime = 0;
#elif _WIN32
	m_ulRecvTime = 0;
#endif //!_WIN64
}
#endif // !_AFX

CTCPClient::~CTCPClient()
{
	Disconnect();
}

void CTCPClient::SetIP(std::wstring wstrIP)
{
	std::string strIP = "";
	BufferString::std_wstring_to_string(strIP, wstrIP);

	SetIP(strIP);

	return;
}

void CTCPClient::SetIP(std::string strIP)
{
	m_strIP = strIP;

	if (IsConnected())
	{
		// �ж�����
		Disconnect();
	}

	return;
}

#ifdef _AFX
void CTCPClient::SetIP(CString cstrIP)
{
	std::string strIP = "";
	BufferString::CString_to_std_string(strIP, cstrIP);

	SetIP(strIP);

	return;
}
#endif // !_AFX

void CTCPClient::GetIP(std::wstring & wstrIP) const
{
	if (m_strIP != "")
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

void CTCPClient::GetIP(std::string & strIP) const
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
void CTCPClient::GetIP(CString & cstrIP) const
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

void CTCPClient::SetPort(short sPort)
{
	m_sPort = sPort;

	if (IsConnected())
	{
		// �ж�����
		Disconnect();
	}

	return;
}

short CTCPClient::GetPort() const
{
	return m_sPort;
}

bool CTCPClient::Connect(SOCKET socket)
{
	if (socket == INVALID_SOCKET)
	{
		DebugPrint("AGVControlSystem/CTCPClient.cpp/Connect ��Ч��SOCKET�׽��ֲ���!");
		return false;
	}

	if (m_socket != socket)
	{
		Disconnect();
	}

	m_socket = socket;

	if (OpenDataRecvThread() == false)
	{
		CloseDataRecvThread();

		shutdown(m_socket, SD_BOTH);
		m_socket = INVALID_SOCKET;

		return false;
	}

	return true;
}

bool CTCPClient::Connect(std::string strIP, unsigned short usPort)
{
	std::string strClientIP = "0.0.0.0";
	GetIP(strClientIP);

	// ��ʼ���׽���
	WORD sockVersion = MAKEWORD(2, 2);

	WSADATA datawsa;

	// WSAStartup�첽�׽�����������
	if (WSAStartup(sockVersion, &datawsa) != 0)
	{
		DebugPrint("AGVControlSystem/CTCPClient.cpp/Connect Error:IP:%s:%d ��ʼ���׽���ʧ��.", strClientIP.c_str(), (unsigned short)m_sPort);

		return false;
	}

	// �����׽���
	SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /*!< �ͻ���SOCKET������*/

	if (sClient == INVALID_SOCKET)
	{
		DebugPrint("AGVControlSystem/CTCPClient.cpp/Connect Error:IP:%s:%d �����׽���ʧ��.", strClientIP.c_str(), (unsigned short)m_sPort);

		return false;
	}

	SOCKADDR_IN addrLocal; /*!< ���ص�ַ*/
	addrLocal.sin_family = AF_INET;

	// �󶨶˿�
	if (m_sPort != -1)
	{
		addrLocal.sin_port = htons(m_sPort);
	}
	else
	{
		// ��portָ��Ϊ0,�����bindʱ��ϵͳ��Ϊ��ָ��һ�����õĶ˿ں�
		addrLocal.sin_port = 0;
	}

	// ��IP
	if (m_strIP == "")
	{
		addrLocal.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		// addrLocal.sin_addr.s_addr = inet_addr(m_strIP.c_str());

		inet_pton(AF_INET, m_strIP.c_str(), &addrLocal.sin_addr.s_addr);
	}

	//���׽��� 
	int retVal; /*!< ����ֵ*/
	retVal = ::bind(sClient, (LPSOCKADDR)&addrLocal, sizeof(SOCKADDR_IN));

	if (SOCKET_ERROR == retVal)
	{
		//�ر��׽���
		sClient = INVALID_SOCKET;

		//�ͷ��׽�����Դ;
		WSACleanup();

		DebugPrint("AGVControlSystem/CTCPClient.cpp/Connect Error:IP:%s:%d ���׽���ʧ��.", strClientIP.c_str(), (unsigned short)m_sPort);

		return false;
	}

	// ��ʼ��sockaddr_in�ṹ��
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(usPort);
	// serAddr.sin_addr.S_un.S_addr = inet_addr(strIP.c_str());

	inet_pton(AF_INET, strIP.c_str(), &serAddr.sin_addr.s_addr);

	// ���ӷ�����
	if (connect(sClient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		shutdown(sClient, SD_BOTH);

		DebugPrint("AGVControlSystem/CTCPClient.cpp/Connect IP:%s:%d ���ӷ����IP:%s:%dʧ��.", strClientIP.c_str(), (unsigned short)m_sPort,strIP.c_str(), usPort);

		return false;
	}

	return Connect(sClient);
}

bool CTCPClient::Connect(std::wstring wstrIP, unsigned short usPort)
{
	std::string strIP = "";

	BufferString::std_wstring_to_string(strIP, wstrIP);

	return Connect(strIP, usPort);
}

#ifdef _AFX
bool CTCPClient::Connect(CString cstrIP, unsigned short usPort)
{
	std::string strIP = "";

	BufferString::CString_to_std_string(strIP, cstrIP);

	SetIP(m_strIP);

	return Connect(strIP, usPort);
}
#endif // !_AFX

void CTCPClient::Disconnect()
{
	CloseDataRecvThread();

	return;
}

bool CTCPClient::IsConnected() const
{
	if (m_socket != INVALID_SOCKET)
	{
		return true;
	}

	return false;
}

bool CTCPClient::SendData(char * pData, unsigned int unLen)
{
	std::string strIP = "";
	GetIP(strIP);

	int nIndex = 0; /*!< �ɹ����͵��ֽ���*/

	nIndex = send(m_socket, pData, unLen, 0);

	std::string strDataRecv = "";
	for (unsigned int i = 0; i < unLen; i++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", pData[i]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("AGVControlSystem/CTCPClient.cpp/SendData IP:%s:%d ��������:%s.�ַ�����%s.", strIP.c_str(), (unsigned short)m_sPort, strDataRecv.c_str(), pData);

	if (nIndex == SOCKET_ERROR)
	{
		DebugPrint("----AGVControlSystem/CTCPClient.cpp/SendData Error:IP:%s:%d ��������ʧ��.", strIP.c_str(), (unsigned short)m_sPort);

		CloseDataRecvThread();

		return false;
	}

	return true;
}

void CTCPClient::InsertData(char * pData, unsigned int unLen)
{
	std::lock_guard<std::mutex> m_lockGuard(m_mutexLockRecv);

	m_bufferRecv = BufferString::Buffer((unsigned char*)pData, unLen) + m_bufferRecv;

	std::string strIP = "";
	GetIP(strIP);

	std::string strDataRecv = "";
	for (unsigned int i = 0; i < unLen; i++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", pData[i]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("AGVControlSystem/CTCPClient.cpp/InsertData IP:%s:%d ��������:%s.�ַ�����%s.", strIP.c_str(), (unsigned short)m_sPort, strDataRecv.c_str(), pData);

	strDataRecv = "";
	for (unsigned int i = 0; i < m_bufferRecv.m_unSize; i++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", m_bufferRecv.m_pszBuffer[i]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("----AGVControlSystem/CTCPClient.cpp/AddData IP:%s:%d ��ǰ����������:%s.�ַ�����%s.", strIP.c_str(), (unsigned short)m_sPort, strDataRecv.c_str(),m_bufferRecv.m_pszBuffer);

	return;
}

void CTCPClient::AddData(char * pData, unsigned int unLen)
{
	std::lock_guard<std::mutex> m_lockGuard(m_mutexLockRecv);

	m_bufferRecv += BufferString::Buffer((unsigned char*)pData, unLen);

	std::string strIP = "";
	GetIP(strIP);

	std::string strDataRecv = "";
	for (unsigned int i = 0; i < unLen; i++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", pData[i]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("AGVControlSystem/CTCPClient.cpp/AddData IP:%s:%d �������:%s.�ַ�����%s.", strIP.c_str(), (unsigned short)m_sPort, strDataRecv.c_str(), pData);

	strDataRecv = "";
	for (unsigned int i = 0; i < m_bufferRecv.m_unSize; i++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", m_bufferRecv.m_pszBuffer[i]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("----AGVControlSystem/CTCPClient.cpp/AddData IP:%s:%d ��ǰ����������:%s.�ַ�����%s.", strIP.c_str(), (unsigned short)m_sPort, strDataRecv.c_str(), m_bufferRecv.m_pszBuffer);

	return;
}

BufferString::Buffer CTCPClient::GetBuffer()
{
	std::lock_guard<std::mutex> m_lockGuard(m_mutexLockRecv);

	BufferString::Buffer recv = m_bufferRecv;
	m_bufferRecv.Clear();

	std::string strIP = "";
	GetIP(strIP);

	std::string strDataRecv = "";
	for (unsigned int i = 0; i < recv.m_unSize; i++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", recv.m_pszBuffer[i]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("AGVControlSystem/CTCPClient.cpp/GetBuffer IP:%s:%d ��ȡ����:%s.�ַ�����%s.", strIP.c_str(), (unsigned short)m_sPort, strDataRecv.c_str(), recv.m_pszBuffer);

	return recv;
}

#ifdef _WIN64
void CTCPClient::GetRecvTime(unsigned long long& ullRecvTime)
{
	ullRecvTime = m_ullRecvTime;

	return;
}
#elif _WIN32
void CTCPClient::GetRecvTime(unsigned long& ulRecvTime)
{
	ulRecvTime = m_ulRecvTime;

	return;
}
#endif // !_WIN64

bool CTCPClient::OpenDataRecvThread()
{
	std::string strIP = "";
	GetIP(strIP);

	if (m_pThreadRecv != nullptr)
	{
		CloseDataRecvThread();
	}

	m_pThreadRecv = new std::thread(&CTCPClient::DataRecvThread, this);

	DebugPrint("AGVControlSystem/CTCPClient.cpp/OpenDataRecvThread IP:%s:%d �������������ݽ����߳�.", strIP.c_str(), (unsigned short)m_sPort);
	TRACE(_T("[New]���ݽ����̵߳�ַ:%08X.\r\n"), m_pThreadRecv);

	if (m_pThreadRecv)
	{
		return true;
	}

	return false;
}

void CTCPClient::CloseDataRecvThread()
{
	std::string strIP = "";
	GetIP(strIP);

	if (m_pThreadRecv != nullptr)
	{
		if (m_pThreadRecv->joinable())
		{
			if (m_socket != INVALID_SOCKET)
			{
				// �ر�SOCKET�׽���
				shutdown(m_socket, SD_BOTH);
				m_socket = INVALID_SOCKET;

				DebugPrint("AGVControlSystem/CTCPClient.cpp/CloseDataRecvThread IP:%s:%d �ر��׽���.", strIP.c_str(), (unsigned short)m_sPort);
			}

			DebugPrint("----AGVControlSystem/CTCPClient.cpp/CloseDataRecvThread IP:%s:%d �ȴ����ݽ����߳̽���.", strIP.c_str(), (unsigned short)m_sPort);
			m_pThreadRecv->join();
		}

		DebugPrint("--------AGVControlSystem/CTCPClient.cpp/CloseDataRecvThread IP:%s:%d �������ݽ����߳�.", strIP.c_str(), (unsigned short)m_sPort);
		TRACE(_T("[Delete]���ݽ����̵߳�ַ:%08X.\r\n"), m_pThreadRecv);

		delete m_pThreadRecv;
		m_pThreadRecv = nullptr;
	}

	return;
}

void CTCPClient::DataRecvThread()
{
	char achDataBuffer[1500];		/*!< ���ݽ�����*/
	int nBufferLen = 0;				/*!< ���������ݳ���*/

	fd_set fds;

	std::string strIP = "";
	GetIP(strIP);

	DebugPrint("--------AGVControlSystem/CTCPClient.cpp/DataRecvThread IP:%s:%d ���ݽ����߳̿�ʼ.--------", strIP.c_str(), (unsigned short)m_sPort);

	while (m_socket != INVALID_SOCKET)
	{
		nBufferLen = 0;

		// ��ʼ��������
		memset(achDataBuffer, 0, 1500);

		FD_ZERO(&fds);
		FD_SET(m_socket, &fds);

		switch (select((int)m_socket + 1, &fds, NULL, NULL, NULL))
		{
		case  SOCKET_ERROR:
			if (errno == WSAEINTR)
			{
				continue;
			}

			shutdown(m_socket, SD_BOTH);
			m_socket = INVALID_SOCKET;

			DebugPrint("AGVControlSystem/CTCPClient.cpp/DataRecvThread Error:IP:%s:%d �ͻ��������ж�.", strIP.c_str(), (unsigned short)m_sPort);

			return;
		case 0:
			continue;
		default:
			if (FD_ISSET(m_socket, &fds))
			{
				DebugPrint("AGVControlSystem/CTCPClient.cpp/DataRecvThread IP:%s:%d �ȴ���������.", strIP.c_str(), (unsigned short)m_sPort);

				// ��������
				nBufferLen = recv(m_socket, achDataBuffer, 1500, 0);

				if (SOCKET_ERROR == nBufferLen)
				{
					DebugPrint("----AGVControlSystem/CTCPClient.cpp/DataRecvThread Error:IP:%s:%d ���������쳣.", strIP.c_str(), (unsigned short)m_sPort);

					shutdown(m_socket, SD_BOTH);
					m_socket = INVALID_SOCKET;

					return;
				} // if (SOCKET_ERROR == nBufferLen)

				if (nBufferLen > 0)
				{
					std::string strDataRecv = "";
					for (int i = 0; i < nBufferLen; i++)
					{
						std::string strDataHex = "";
						BufferString::std_string_format(strDataHex, "%02X ", achDataBuffer[i]);
						strDataRecv += strDataRecv;
					}

					DebugPrint("--------AGVControlSystem/CTCPClient.cpp/DataRecvThread IP:%s:%d ��������:%s.�ַ�����%s.", strIP.c_str(), (unsigned short)m_sPort, strDataRecv.c_str(), achDataBuffer);

#ifdef _WIN64
					m_ullRecvTime = GetTickCount64();
#elif _WIN32
					m_ulRecvTime = GetTickCount();
#endif //!_WIN64

					AddData(achDataBuffer, nBufferLen);
				} // if (nBufferLen > 0)
			} // if (FD_ISSET(m_socket, &fds))
			break;
		}

		Sleep(1);
	}

	DebugPrint("--------AGVControlSystem/CTCPClient.cpp/DataRecvThread IP:%s:%d �˳����ݽ����߳�.--------", strIP.c_str(), (unsigned short)m_sPort);

#ifdef _WIN64
	m_ullRecvTime = 0;
#elif _WIN32
	m_ulRecvTime = 0;
#endif //!_WIN64

	return;
}

bool CTCPClient::GetLocalIP(char * lpszIP) const
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
	DebugPrint("AGVControlSystem/CTCPClient.cpp/GetLocalIP hostname:%s\n", hostname);

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
	if (m_sPort > -1)
	{
		sprintf_s(lpszPort, 6, "%d", m_sPort);
		getaddrinfo(hostname, lpszPort, &hints, &res);
	}
	else
	{
		getaddrinfo(hostname, NULL, &hints, &res);
	}

	if (ret == -1) 
	{
		perror("getaddrinfo");
		return false;
	}

	for (cur = res; cur != NULL; cur = cur->ai_next)
	{
		addr = (struct sockaddr_in *)cur->ai_addr;
		
		sprintf_s(lpszIP, IP4_ADDRESS_STRING_BUFFER_LENGTH,"%d.%d.%d.%d",
			(*addr).sin_addr.S_un.S_un_b.s_b1,
			(*addr).sin_addr.S_un.S_un_b.s_b2,
			(*addr).sin_addr.S_un.S_un_b.s_b3,
			(*addr).sin_addr.S_un.S_un_b.s_b4);

		DebugPrint("AGVControlSystem/CTCPClient.cpp/GetLocalIP IP:%s\n", lpszIP);
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
	// strcpy_s(lpszIP, IP4_ADDRESS_STRING_BUFFER_LENGTH, inet_ntoa(*(in_addr*)*host->h_addr_list));

	//inet_ntop(AF_INET, (in_addr*)*host->h_addr_list, lpszIP, IP4_ADDRESS_STRING_BUFFER_LENGTH);

	return true;
}

void CTCPClient::GetPort(std::string & strPort)
{
	if (m_sPort != -1)
	{
		BufferString::std_string_format(strPort, "%d", (unsigned short)m_sPort);
	}
	else
	{
		strPort = "���";
	}

	return;
}

void CTCPClient::GetPort(std::wstring & wstrPort)
{
	std::string strPort = "";
	GetPort(strPort);

	BufferString::std_string_to_wstring(wstrPort, strPort);

	return;
}

void CTCPClient::IsConnected(std::string & strConnect)
{
	if (IsConnected())
	{
		strConnect = "������";
	}
	else
	{
		strConnect = "δ����";
	}

	return;
}

void CTCPClient::IsConnected(std::wstring & wstrConnect)
{
	std::string strConnect = "";
	IsConnected(strConnect);

	BufferString::std_string_to_wstring(wstrConnect, strConnect);

	return;
}

#ifdef _AFX
void CTCPClient::GetPort(CString & cstrPort)
{
	std::string strPort = "";
	GetPort(strPort);

	BufferString::std_string_to_CString(cstrPort, strPort);

	return;
}

void CTCPClient::IsConnected(CString & cstrConnect, COLORREF & color)
{
	std::string strConnect = "";
	IsConnected(strConnect);

	BufferString::std_string_to_CString(cstrConnect, strConnect);

	if (IsConnected())
	{
		color = COLOR_GREEN;
	}
	else
	{
		color = COLOR_GRAY;
	}

	return;
}
#endif // !_AFX

#endif //!_TCPCLIENT_H