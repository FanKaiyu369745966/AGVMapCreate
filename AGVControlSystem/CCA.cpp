#include "stdafx.h"
#include "CCA.h"

#ifdef _CA_H

std::list<std::string> CCA::g_listRecv;	
std::mutex CCA::g_lock;					

CCA::CCA(unsigned short usNo, std::string strIP, short sPort, std::string strText):CTCPClient(strIP, sPort)
{
	m_usNo = usNo;
	m_strText = strText;

	m_unLoginCheck = 0;
	m_unDisTime = 0;
	m_pThreadHeart = nullptr;
	m_hShutdownEvent = nullptr;
	m_bHeart = false;
}

CCA::CCA(unsigned short usNo, std::wstring wstrIP, short sPort, std::wstring wstrText) : CTCPClient(wstrIP, sPort)
{
	m_usNo = usNo;
	BufferString::std_wstring_to_string(m_strText, wstrText);

	m_unLoginCheck = 0;
	m_unDisTime = 0;
	m_pThreadHeart = nullptr;
	m_hShutdownEvent = nullptr;
	m_bHeart = false;
}

#ifdef _AFX
CCA::CCA(unsigned short usNo, CString cstrIP, short sPort, CString cstrText) : CTCPClient(cstrIP, sPort)
{
	m_usNo = usNo;
	BufferString::CString_to_std_string(m_strText, cstrText);

	m_unLoginCheck = 0;
	m_unDisTime = 0;
	m_pThreadHeart = nullptr;
	m_hShutdownEvent = nullptr;
	m_bHeart = false;
}
#endif // !_AFX

CCA::~CCA()
{
	Break();

	if (m_hShutdownEvent)
	{
		CloseHandle(m_hShutdownEvent);
		m_hShutdownEvent = nullptr;
	}
}

bool CCA::SetNo(unsigned short usNo)
{
	if (usNo > CA_MAX_CLIENT_ADDR)
	{
		return false;
	}

	if (m_usNo == usNo)
	{
		return false;
	}

	m_usNo = usNo;

	return true;
}

unsigned short CCA::GetNo() const
{
	return m_usNo;
}

void CCA::GetNo(std::string & strNo)
{
	BufferString::std_string_format(strNo, "%d", m_usNo);

	return;
}

void CCA::GetNo(std::wstring & wstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_wstring(wstrNo, strNo);

	return;
}

bool CCA::SetText(std::string strText)
{
	if (m_strText == strText)
	{
		return false;
	}

	m_strText = strText;

	return true;
}

bool CCA::SetText(std::wstring wstrText)
{
	std::string strText = "";
	BufferString::std_wstring_to_string(strText, wstrText);

	return SetText(strText);
}

void CCA::GetText(std::string & strText)
{
	strText = m_strText;

	return;
}

void CCA::GetText(std::wstring & wstrText)
{
	BufferString::std_string_to_wstring(wstrText, m_strText);

	return;
}

#ifdef _AFX
void CCA::GetNo(CString & cstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_CString(cstrNo, strNo);

	return;
}

bool CCA::SetText(CString cstrText)
{
	std::string strText = "";
	BufferString::CString_to_std_string(strText, cstrText);

	return SetText(strText);
}

void CCA::GetText(CString & cstrText)
{
	BufferString::std_string_to_CString(cstrText, m_strText);

	return;
}
#endif //!_AFX

bool CCA::Connect(SOCKET socket)
{
	CTCPClient::Connect(socket);

	StartThread();

	AskLogon();

	return true;
}

void CCA::Break()
{
	// ��ֹ���ݴ����߳�
	CloseThread();

	return;
}

bool CCA::IsConnected()
{
	if (m_pThreadHeart)
	{
		if (m_pThreadHeart->joinable())
		{
			return true;
		}
	}
	return CTCPClient::IsConnected();
}

std::string CCA::GetRecvMsg()
{
	std::string strText = "";

	if (g_listRecv.size() == 0)
	{
		return strText;
	}

	unsigned int unCall = 0;

	// ����
	g_lock.lock();

	try
	{
		strText = g_listRecv.front();
		g_listRecv.pop_front();
	}
	catch (std::exception& e)
	{
		DebugPrint("%s.", e.what());
	}

	// ����
	g_lock.unlock();

	return strText;
}

bool CCA::SendToWatch(unsigned char byWatch, std::string strText)
{
	if (byWatch > 200)
	{
		DebugPrint("AGVControlSystem/CCA.cpp/SendToWatch CA:%d ������Ϣ���ֱ�ʧ��!��Ч���ֱ���:%d.", m_usNo, byWatch);
		return false;
	}

	if (strText.length() > 40)
	{
		DebugPrint("AGVControlSystem/CCA.cpp/SendToWatch CA:%d ������Ϣ���ֱ�ʧ��!��������Ϣ����:%s(%d�ֽ�).", m_usNo, strText.c_str(),strlen(strText.c_str()));
		return false;
	}

	char lpszStr[48];

	// ��ʼ��
	memset(lpszStr, ' ', 48);

	memcpy_s(lpszStr, 48, strText.c_str(), strText.length());

	lpszStr[47] = 0;

	/*!
	 * USB���ݰ��ϳ�
	*/
	BufferString::Buffer bfUsb; /*!< USB ���� */

	bfUsb.m_unSize = 56 + 4;

	bfUsb.m_pszBuffer = new unsigned char[bfUsb.m_unSize];

	// ��ʼ�����ݰ�
	memset(bfUsb.m_pszBuffer, 0, bfUsb.m_unSize);

	bfUsb.m_pszBuffer[0] = 0x01;				/*!< ��ǰ����(TCP���ݰ�) */
	bfUsb.m_pszBuffer[1] = 0x00;				/*!< ��ǰ����(TCP���ݰ�) */
	bfUsb.m_pszBuffer[2] = 0x01;				/*!< ��ǰ����(TCP���ݰ�) */
	bfUsb.m_pszBuffer[3] = 0x00;				/*!< ��ǰ����(TCP���ݰ�) */
	bfUsb.m_pszBuffer[4] = 0x55;				/*!< ֡ͷ */
	bfUsb.m_pszBuffer[5] = 0xAA;				/*!< ֡ͷ */
	bfUsb.m_pszBuffer[6] = 0x01;				/*!< ��ַ */
	bfUsb.m_pszBuffer[7] = CA_USBCMD_SEND;		/*!< ������ */
	bfUsb.m_pszBuffer[8] = 50;					/*!< ���ݳ��� */
	bfUsb.m_pszBuffer[9] = 0x00;				/*!< ��Ϣ����ַ */
	bfUsb.m_pszBuffer[10] = byWatch;			/*!< ��Ϣ����ַ */
	memcpy_s(&bfUsb.m_pszBuffer[11], 48, lpszStr, 48); /*!< �ı����� */
	unsigned int unCheck = CreateCheck(bfUsb.m_pszBuffer + 6, bfUsb.m_unSize - 6); /*!< У�� */
	bfUsb.m_pszBuffer[bfUsb.m_unSize - 1] = unCheck & 0xFF;

	/*!
	 * �ϳ�TCP����
	*/
	BufferString::Buffer package = CreatePackage(CA_TCPCMD_DATACOMM, m_usNo, bfUsb.m_pszBuffer, bfUsb.m_unSize); /*!< TCP���ݰ� */

	SendData((char*)package.m_pszBuffer, package.m_unSize);

	return true;
}

bool CCA::SendToWatch(unsigned char byWatch, std::wstring wstrText)
{
	std::string strText = "";
	BufferString::std_wstring_to_string(strText, wstrText);
	return SendToWatch(byWatch, strText);
}

#ifdef _AFX
bool CCA::SendToWatch(unsigned char byWatch, CString cstrText)
{
	std::string strText = "";
	BufferString::CString_to_std_string(strText, cstrText);
	return SendToWatch(byWatch, strText);
}
#endif //!_AFX

void CCA::CreateLogonMessage(unsigned int & unLogin, unsigned int & unCheck)
{
	unLogin = 0;

	/*!
	 * ����ע����
	*/
	// ��ʼ�������������
	srand((unsigned)time(NULL));

	for (int i = 0; i < 8; i++)
	{
		unLogin += (unsigned int)((rand() % 0x0F) * pow(16.0, i));

		Sleep(1);
	}

	DebugPrint("AGVControlSystem/CCA.cpp/CreateLogonMessage CA:%d ����ע����:%08X.", m_usNo, unLogin);

	/*!
	 * ����ע��У����
	*/
	unCheck = unLogin ^ 0x3421bcda;

	DebugPrint("----AGVControlSystem/CCA.cpp/CreateLogonMessage CA:%d �׶�һ:%08X.", m_usNo, unCheck);

	unCheck = ((unCheck & 0xFF000000) / 0x100) + ((unCheck & 0xFF0000) * 0x100) + ((unCheck & 0xFF00) / 0x100) + ((unCheck & 0xFF) * 0x100);

	DebugPrint("--------AGVControlSystem/CCA.cpp/CreateLogonMessage CA:%d �׶ζ�:%08X.", m_usNo, unCheck);

	unCheck ^= 0x4321abcd;

	DebugPrint("------------AGVControlSystem/CCA.cpp/CreateLogonMessage CA:%d ����ע��У����:%08X.", m_usNo, unCheck);

	return;
}

void CCA::AskLogon()
{
	unsigned int unLogin = 0;
	CreateLogonMessage(unLogin, m_unLoginCheck);
	unsigned char aucLogin[4];
	memcpy_s(aucLogin, 4, &unLogin, 4);
	BufferString::Buffer package = CreatePackage(0x01, 0x0000, aucLogin, 4);

	SendData((char*)package.m_pszBuffer, package.m_unSize);

	return;
}

unsigned short CCA::CreateCheck(unsigned char * pData, unsigned int unLen)
{
	unsigned int unSum = 0;

	for (unsigned int i = 0; i < unLen; i++)
	{
		unSum += pData[i];
	}

	unSum = ~unSum;

	return (unsigned short)(unSum & 0xFFFF);
}

BufferString::Buffer CCA::CreatePackage(unsigned char byCmd, unsigned short usTarget, unsigned char *pData, unsigned int unLen)
{
	unsigned int unPackageLen = 11 + unLen;
	unsigned char *pPackage = new unsigned char[unPackageLen];/*!< ���ݰ�ָ�� */
	unsigned short usSrcAddr = CA_SERVER_ADDR;	/*!< Դ��ַ */
	// ��ʼ�����ݰ�
	memset(pPackage, 0, unPackageLen);

	pPackage[0] = 0x1b;									/*!< ����ͷ */
	pPackage[unPackageLen - 1] = 0x1b;					/*!< ����β */

	unsigned short usDataLen = unPackageLen - 4;		/*!< ���ݳ��� */
	memcpy_s(pPackage + 1, 2, &usDataLen, 2);
	memcpy_s(pPackage + 3, 2, &usTarget, 2);			/*!< Ŀ�ĵ�ַ */
	memcpy_s(pPackage + 5, 2, &usSrcAddr, 2);			/*!< Դ��ַ */
	pPackage[7] = byCmd;								/*!< ������ */
	memcpy_s(pPackage + 8, unLen, pData, unLen);		/*!< ���ݰ� */
	unsigned int unCheck = CreateCheck(pPackage + 1, unPackageLen - 4);	/*!< У�� */
	pPackage[unPackageLen - 3] = unCheck & 0xFF;	
	pPackage[unPackageLen - 2] = (unCheck >> 8) & 0xFF;

	BufferString::Buffer package(pPackage, unPackageLen);
	delete pPackage;

	return Translate(package);
}

void CCA::HeartBeat(unsigned int unDisTime)
{
	unsigned int unTextLen = 14;
	char *lpszText = new char[unTextLen];
	sprintf_s(lpszText, unTextLen, "CA-Heart=%03dS", unDisTime);

	BufferString::Buffer package = CreatePackage(0x02, m_usNo, (unsigned char*)lpszText, unTextLen);

	SendData((char*)package.m_pszBuffer, package.m_unSize);

	delete lpszText;

	return;
}

BufferString::Buffer CCA::Translate(BufferString::Buffer &bfData)
{
	BufferString::Buffer bfTrans; /*!< ת�������� */
	bfTrans.m_unSize = bfData.m_unSize * 2;

	bfTrans.m_pszBuffer = new unsigned char[bfTrans.m_unSize];

	// ��ʼ��
	memset(bfTrans.m_pszBuffer, 0, bfTrans.m_unSize);

	// ֡ͷ
	bfTrans.m_pszBuffer[0] = bfData.m_pszBuffer[0];

	int nT = 1;

	for (unsigned int nO = 1; nO < bfData.m_unSize - 1; nO++)
	{
		unsigned char ucData = bfData.m_pszBuffer[nO];

		if (ucData == 0x1a || ucData == 0x1b)
		{
			DebugPrint("AGVControlSystem/CCA.cpp/Analysis ת��.");
			bfTrans.m_pszBuffer[nT] = 0x1a;

			nT++;

			switch (ucData)
			{
			case 0x1a:
				bfTrans.m_pszBuffer[nT] = 0x01;
				break;
			case 0x1b:
				bfTrans.m_pszBuffer[nT] = 0x02;
				break;
			}
		}
		else
		{
			bfTrans.m_pszBuffer[nT] = ucData;
		}

		nT++;
	}

	// ֡β
	bfTrans.m_pszBuffer[nT] = bfData.m_pszBuffer[bfData.m_unSize - 1];

	nT++;

	return BufferString::Buffer(bfTrans.m_pszBuffer, nT);
}

BufferString::Buffer CCA::Analysis(BufferString::Buffer &bfData)
{
	BufferString::Buffer bfOrigin; /*!< ԭ���� */

	bfOrigin.m_unSize = bfData.m_unSize;

	bfOrigin.m_pszBuffer = new unsigned char[bfData.m_unSize];

	// ��ʼ��
	memset(bfOrigin.m_pszBuffer, 0, bfOrigin.m_unSize);

	bfOrigin.m_pszBuffer[0] = bfData.m_pszBuffer[0];

	int nO = 1;

	for (unsigned int nR = 1; nR < bfData.m_unSize - 1; nR++, nO++)
	{
		unsigned char ucData = bfData.m_pszBuffer[nR];

		if (ucData == 0x1a)
		{
			DebugPrint("AGVControlSystem/CCA.cpp/Analysis ��ԭ.");
			unsigned char ucNext = bfData.m_pszBuffer[nR + 1];

			switch (ucNext)
			{
			case 0x01:
				bfOrigin.m_pszBuffer[nO] = 0x1a;
				nR++;
				break;
			case 0x02:
				bfOrigin.m_pszBuffer[nO] = 0x1b;
				nR++;
				break;
			default:
				bfOrigin.m_pszBuffer[nO] = bfData.m_pszBuffer[nR];
			}
		}
		else
		{
			bfOrigin.m_pszBuffer[nO] = bfData.m_pszBuffer[nR];
		}
	}

	bfOrigin.m_pszBuffer[nO] = bfData.m_pszBuffer[bfData.m_unSize - 1];

	return bfOrigin;
}

void CCA::ProcessData(BufferString::Buffer & buffer)
{
	unsigned char *pData = buffer.m_pszBuffer;	/*!< ָ��ʣ�����ݵ�ָ�� */
	unsigned int unDataLen = buffer.m_unSize;	/*!< ʣ�����ݵ��ֽ��� */

	while (true)
	{
		/*!
		 * ���ұ���ͷ 0x1b
		 *
		 * �Ҳ�������ͷʱ������ȫ������
		*/
		unsigned char *pHead = nullptr;				/*!< ָ����ͷ��ָ�� */
		pHead = (unsigned char*)memchr(pData, 0x1b, unDataLen);

		// �ޱ���ͷ�����
		if (pHead == nullptr)
		{
			pData = nullptr;
			break;
		}

		// ���ڱ���ͷ�����

		/*!
		 * ���ұ���β
		 *
		 * �Ҳ�������βʱ��������ͷ�Ժ������
		*/
		unsigned char *pTail = nullptr; /*!< ָ����β��ָ�� */
		pTail = (unsigned char*)memchr(pHead + 1, 0x1b, buffer.m_unSize - (pHead - buffer.m_pszBuffer));

		// �ޱ���β�����
		if (pTail == nullptr)
		{
			pData = pHead;
			unDataLen = buffer.m_unSize - (pData - buffer.m_pszBuffer);
			break;
		}

		// ���ڱ���β�����

		/*!
		 * �����ݳ��Ƚ��жԱ�
		 *
		 * ���ݳ��Ȳ���ʱ����������β����
		*/
		unsigned int unLen = pTail - pHead - 3;	/*!< ���ݳ��� */
		unsigned int unPackageLen = 0; /*!< ���ĳ��� */
		memcpy_s(&unPackageLen, 2, pHead + 1, 2);

		// ���ݳ��Ȳ��������
		if (unLen != unPackageLen)
		{
			pData = pTail;
			unDataLen = buffer.m_unSize - (pData - buffer.m_pszBuffer);
			continue;
		}

		// ���ݳ�����������

		/*!
		 * ��У��ֵ���жԱ�
		 *
		 * У��ֵ����ʱ������ͷβ�Լ��м����������
		*/
		unsigned short usCheck = CreateCheck(pHead + 1, unPackageLen); /*!< ����У��ֵ */
		unsigned short usPackageCheck = 0; /*!< ����У��ֵ */
		memcpy_s(&usPackageCheck, 2, pTail - 2, 2);

		// У��ֵ���ʱ
		if (usCheck == usPackageCheck)
		{
			ProcessPackage(pHead, pTail - pHead);
		}

		// ��ʣ������
		if (buffer.m_pszBuffer - pTail == buffer.m_unSize)
		{
			pData = nullptr;
			break;
		}
		// ����δ���������
		else
		{
			pData = pTail + 1;
			unDataLen = buffer.m_unSize - (pData - buffer.m_pszBuffer);
		}
	}

	// ��ʣ�����ݵ����
	if (pData == nullptr)
	{
		buffer.Clear();
	}
	else
	{
		buffer = BufferString::Buffer(pData, unDataLen);
	}

	return;
}

void CCA::ProcessPackage(unsigned char * pData, unsigned int unLen)
{
	BufferString::Buffer package = Analysis(BufferString::Buffer(pData, unLen));

	unsigned short usPackageLen = 0; /*!< ���ĳ��� */
	memcpy_s(&usPackageLen, 2, package.m_pszBuffer + 1, 2);

	unsigned short usTraget = 0; /*!< Ŀ�ĵ�ַ */
	memcpy_s(&usTraget, 2, package.m_pszBuffer + 3, 2);

	unsigned short usFromCA = 0; /*!< Դ��ַ = ������ */
	memcpy_s(&usFromCA, 2, package.m_pszBuffer + 5, 2);

	unsigned char byTCPCmd = *(package.m_pszBuffer + 7); /*!< ������ */

	// ע����Ϣ�ظ�
	if (byTCPCmd == CA_TCPCMD_LOGON)
	{
		unsigned int unLogin = 0; /*!< ע���� */
		memcpy_s(&unLogin, 4, package.m_pszBuffer + 8, 4);

		DebugPrint("AGVControlSystem/CCA.cpp/ProcessPackage CA:%d ע����Ϣ�ظ�.ע����:%08X.", m_usNo, unLogin);

		// ע����Ϣ��֤ͨ��
		m_bHeart = true;

		DebugPrint("----AGVControlSystem/CCA.cpp/ProcessPackage CA:%d ��ʼ��������.",m_usNo);
	}
	// �����ظ�
	else if (byTCPCmd == CA_TCPCMD_HEARTBEAT)
	{
		DebugPrint("AGVControlSystem/CCA.cpp/ProcessPackage CA:%d �����ظ�.", m_usNo);
	}
	// �ӻ��ظ�
	else if (byTCPCmd == CA_TCPCMD_DATACOMM)
	{
		unsigned short usTotal = 0; /*!< �ܰ��� */
		memmove_s(&usTotal, 2, package.m_pszBuffer + 8, 2);

		unsigned short usCur = 0; /*!< ��ǰ���� */
		memmove_s(&usCur, 2, package.m_pszBuffer + 10, 2);

		unsigned short usDataLen = usPackageLen - 11;			/*!< ���ĳ���(USB����) */
		unsigned char *pData = package.m_pszBuffer + 12;		/*!< ָ��δ���������ָ�� */

		/*!
		 * ���ұ���ͷ(USB����)
		*/
		unsigned char* pHead = nullptr; /*!< ָ����ͷ��ָ�� */
		pHead = (unsigned char*)memchr(pData, 0x55, usDataLen);

		// δ�ҵ�����ͷ�����
		if (pHead == nullptr)
		{
			return;
		}

		// ���ڱ���ͷ�����

		// ����ͷ����ȫƥ��
		if (*(pHead + 1) != 0xAA)
		{
			return;
		}

		unsigned char byAddr = *(pHead + 2); /*!< ��ַ */
		unsigned char byUsbCmd = *(pHead + 3); /*!< ������ */
		unsigned char byLen = *(pHead + 4); /*!< ���ݳ���(USB����) */
		unsigned char* pUsbData = pHead + 5; /*!< ������ */
		unsigned char byPackageCheck = *(pHead + 5 + byLen); /*!< ����У��ֵ */
		unsigned char byCheck = CreateCheck(pHead + 2, byLen + 3) & 0xFF; /*!< ����У��ֵ */

		// У��ֵ������
		if (byCheck != byPackageCheck)
		{
			return;
		}

		// ���������ϴ�
		if (byAddr == 0x02 && byUsbCmd == CA_USBCMD_UPLOAD_SEND)
		{
			unsigned char byStrLen = (*(pHead + 5) - '0') * 10 + (*(pHead + 6) - '0'); /*!< �ı����� */

			if (byStrLen == 0 || byStrLen != byLen - 2)
			{
				return;
			}

			char *c = new char[byStrLen + 1];
			memset(c, 0, byStrLen + 1);
			memcpy(c, pHead + 7, byStrLen);

			std::string strText(c);

			// ����
			g_lock.lock();

			g_listRecv.push_back(strText);

			// ����
			g_lock.unlock();

			delete c;

			DebugPrint("AGVControlSystem/CCA.cpp/ProcessPackage CA:%d ������Ϣ:%s.\r\n", m_usNo, strText.c_str());
		}
	}

	return;
}

void CCA::StartThread()
{
	if (m_pThreadHeart != nullptr)
	{
		CloseThread();
	}

	// ��ʼ���̹߳ر��¼�
	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(nullptr, true, false, nullptr);
	}

	m_pThreadHeart = new std::thread(&CCA::ProcessDataThread, this);

	return;
}

void CCA::CloseThread()
{
	if (m_pThreadHeart)
	{
		if (m_pThreadHeart->joinable())
		{
			SetEvent(m_hShutdownEvent);

			m_pThreadHeart->join();
		}

		delete m_pThreadHeart;
		m_pThreadHeart = nullptr;
	}

	return;
}

void CCA::ProcessDataThread()
{
	DebugPrint("--------AGVControlSystem/CCA.cpp/ProcessDataThread CA:%d ���ݴ����߳̿�ʼ.--------", m_usNo);

#ifdef _WIN64
	unsigned long long ullHeartTime = 0;
#elif _WIN32
	unsigned long ulHeartTime = 0;
#endif

	while (true)
	{
		if (IsConnected() == false)
		{
			break;
		}

		BufferString::Buffer data = GetBuffer();
		if (data.m_unSize != 0)
		{
			ProcessData(data);
		}

		if (data.m_unSize != 0)
		{
			InsertData((char*)data.m_pszBuffer, data.m_unSize);
		}
	
#ifdef _WIN64

		if (m_bHeart && (ullHeartTime == 0 || GetTickCount64() - ullHeartTime >= m_unDisTime * 1000))
		{
			HeartBeat(m_unDisTime);
			ullHeartTime = GetTickCount64();
		}

		// ����2��δ���յ���������
		unsigned long long ullRecvTime = 0;
		GetRecvTime(ullRecvTime);

		if (ullRecvTime != 0 && GetTickCount64() - ullRecvTime >= 3 * m_unDisTime * 1000)
		{
			CTCPClient::Disconnect();

			break;
		}
#elif _WIN32

		if (m_bHeart && (ulHeartTime == 0 || GetTickCount() - ulHeartTime >= m_unDisTime * 1000))
		{
			HeartBeat(m_unDisTime);
			ulHeartTime = GetTickCount();
		}

		// ����2��δ���յ���������
		unsigned long ulRecvTime = 0;
		GetRecvTime(ulRecvTime);

		if (ulRecvTime != 0 && GetTickCount() - ulRecvTime >= 3 * m_unDisTime * 1000)
		{
			CTCPClient::Disconnect();

			break;
		}
#endif

		if (WaitForSingleObject(m_hShutdownEvent, 1000) == WAIT_OBJECT_0)
		{
			CTCPClient::Disconnect();

			break;
		}
	}

	m_bHeart = false;

	DebugPrint("--------AGVControlSystem/CCA.cpp/ProcessDataThread CA:%d ���ݴ����߳̽���.--------", m_usNo);

	return;
}

#endif // !_CA_H