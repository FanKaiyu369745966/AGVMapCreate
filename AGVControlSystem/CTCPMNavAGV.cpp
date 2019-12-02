#include "stdafx.h"
#include "CTCPMNavAGV.h"

#ifdef _AGV_H

CTCPMNavAGV::CTCPMNavAGV(unsigned char byNo, unsigned char byType, unsigned char byDriver, float fMaxSpeed, std::string strIP, short sPort, unsigned char byDevType)
	: CAGV(byNo,byType, byDriver,fMaxSpeed),CTCPClient(strIP, sPort),CProcessData(byDevType)
{
	m_pThreadSend = nullptr;
	m_hShutdownEvent = nullptr;
}

CTCPMNavAGV::CTCPMNavAGV(unsigned char byNo, unsigned char byType, unsigned char byDriver, float fMaxSpeed, std::wstring wstrIP, short sPort, unsigned char byDevType)
	: CAGV(byNo, byType, byDriver, fMaxSpeed), CTCPClient(wstrIP, sPort), CProcessData(byDevType)
{
	m_pThreadSend = nullptr;
	m_hShutdownEvent = nullptr;
}

#ifdef _AFX
CTCPMNavAGV::CTCPMNavAGV(unsigned char byNo, unsigned char byType, unsigned char byDriver, float fMaxSpeed, CString cstrIP, short sPort, unsigned char byDevType)
	: CAGV(byNo, byType, byDriver, fMaxSpeed), CTCPClient(cstrIP, sPort), CProcessData(byDevType)
{
	m_pThreadSend = nullptr;
	m_hShutdownEvent = nullptr;
}
#endif // !_AFX

CTCPMNavAGV::~CTCPMNavAGV()
{
	Break();

	if (m_hShutdownEvent)
	{
		CloseHandle(m_hShutdownEvent);
		m_hShutdownEvent = nullptr;
	}
}

void CTCPMNavAGV::SendPackage(BufferString::Buffer bfPackage)
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	for (std::list<BufferString::Buffer>::iterator it = m_listSend.begin(); it != m_listSend.end(); it++)
	{
		if (*it == bfPackage)
		{
			return;
		}
	}

	if (m_listSend.max_size() == m_listSend.size())
	{
		m_listSend.pop_front();
	}

	m_listSend.push_back(bfPackage);

	return;
}

void CTCPMNavAGV::SendPackage(byte * byPackage, unsigned int unPackageLen)
{
	return SendPackage(BufferString::Buffer(byPackage, unPackageLen));
}

void CTCPMNavAGV::StartSendThread()
{
	if (m_pThreadSend && m_pThreadSend->joinable())
	{
		EndSendThread();
	}

	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(nullptr, true, false, nullptr);
	}

	m_mutexLock.lock();

	m_listSend.clear();

	m_mutexLock.unlock();

	m_pThreadSend = new std::thread(&CTCPMNavAGV::SendThread,this);

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/StartSendThread AGV:%d IP:%s:%d �������������ݷ����߳�.",m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);
	TRACE(_T("[New]���ݷ����̵߳�ַ:%08X.\r\n"), m_pThreadSend);

	return;
}

void CTCPMNavAGV::EndSendThread()
{
	if (m_pThreadSend)
	{
		if (m_pThreadSend->joinable())
		{
			SetEvent(m_hShutdownEvent);

			DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/StartSendThread AGV:%d IP:%s:%d �ȴ����ݷ����߳̽���.", m_byNo,m_strIP.c_str(), (unsigned short)m_sPort);
			m_pThreadSend->join();
		}

		DebugPrint("----AGVControlSystem/CTCPMNavAGV.cpp/StartSendThread AGV:%d IP:%s:%d �������ݷ����߳�.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);
		TRACE(_T("[Delete]���ݽ����̵߳�ַ:%08X.\r\n"), m_pThreadSend);

		delete m_pThreadSend;
		m_pThreadSend = nullptr;
	}

	return;
}

void CTCPMNavAGV::SendThread()
{
	DebugPrint("--------AGVControlSystem/CTCPMNavAGV.cpp/SendThread AGV:%d IP:%s:%d ���ݷ����߳̿�ʼ.--------", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);
#ifdef _WIN64
	unsigned long long ullHeartbeat = 0;	/*!< ��������ʱ�� */
#elif _WIN32
	unsigned long ulHeartbeat = 0;			/*!< ��������ʱ�� */
#endif // !_WIN64

	while (true)
	{
		if (IsConnected() == false)
		{
			break;
		}

#ifdef _WIN64
		if (ullHeartbeat == 0 || GetTickCount64() - ullHeartbeat >= 1000)
		{
			Heartbeat();
		}
#elif _WIN32
		if (ulHeartbeat == 0 || GetTickCount() - ulHeartbeat >= 1000)
		{
			Heartbeat();
		}
#endif // !_WIN64

		m_mutexLock.lock();

		std::list<BufferString::Buffer> listSend = m_listSend;
		m_listSend.clear();

		m_mutexLock.unlock();

		unsigned int unLift = 1000;	/*!< ʣ��ʱ�� */

		for (std::list<BufferString::Buffer>::iterator it = listSend.begin(); it != listSend.end(); ++it)
		{
			if (SendData((char*)(*it).m_pszBuffer, (*it).m_unSize) == false)
			{
				SetError(_AGVERR_COMM);
				break;
			}
			else
			{
				if (GetError() == _AGVERR_COMM)
				{
					SetError(_AGVERR_NONE);
				}
			}

			if (WaitForSingleObject(m_hShutdownEvent, 100) == WAIT_OBJECT_0)
			{
				CTCPClient::Disconnect();

				goto Exit;
			}

			unLift -= 100;
		}

		if (unLift > 0)
		{
			if (WaitForSingleObject(m_hShutdownEvent, unLift) == WAIT_OBJECT_0)
			{
				CTCPClient::Disconnect();

				break;
			}
		}
	}

Exit:
	DebugPrint("--------AGVControlSystem/CTCPMNavAGV.cpp/SendThread AGV:%d IP:%s:%d �˳����ݷ����߳�.--------", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);

	return;
}

void CTCPMNavAGV::Heartbeat()
{
	byte data[13];
	memset(data, 0, 13);

	data[0] = 0x1f;
	std::list<BufferString::Buffer> bfPackage = CreatePackge(data, 13, m_byNo);

	for (std::list<BufferString::Buffer>::iterator it = bfPackage.begin(); it != bfPackage.end(); it++)
	{
		SendPackage(*it);
	}

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/Heartbeat AGV:%d IP:%s:%d ֪ͨAGV����.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);

	return;
}

void CTCPMNavAGV::MoveTo(unsigned short usRFID)
{
	Action(m_usCurRFID, usRFID, _AGVACT_MOVE);

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/MoveTo AGV:%d IP:%s:%d ֪ͨAGV��%d�ƶ���%d.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort, m_usCurRFID, usRFID);

	return;
}

void CTCPMNavAGV::TrafficPass()
{
	Action(m_usCurRFID, m_usCurRFID, _AGVACT_TRAFFIC,_AGVACTSTA_FINISH);

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/TrafficPass AGV:%d IP:%s:%d ����AGVͨ��%d.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort, m_usCurRFID);

	return;
}

void CTCPMNavAGV::TrafficStop(unsigned short usRFID)
{
	Action(m_usCurRFID, usRFID, _AGVACT_TRAFFIC);

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/TrafficStop AGV:%d IP:%s:%d ֪ͨAGV��%d��ͨ����ֹͣ.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort, usRFID);

	return;
}

void CTCPMNavAGV::Load()
{
	Action(m_usCurRFID, m_usCurRFID, _AGVACT_LOAD);

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/Load AGV:%d IP:%s:%d ֪ͨAGV��%d����.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort, m_usCurRFID);

	return;
}

void CTCPMNavAGV::Unload()
{
	Action(m_usCurRFID, m_usCurRFID, _AGVACT_UNLOAD);

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/Unload AGV:%d IP:%s:%d ֪ͨAGV��%d����.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort, m_usCurRFID);

	return;
}

void CTCPMNavAGV::Scream()
{
	ManualSet(m_usCurRFID, _AGVSTA_SCREAM);

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/Restart AGV:%d IP:%s:%d ֪ͨAGV��ͣ.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);

	return;
}

void CTCPMNavAGV::Restart()
{
	ManualSet(m_usCurRFID, _AGVSTA_WAIT);

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/Restart AGV:%d IP:%s:%d ֪ͨAGV��λ.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);

	return;
}

void CTCPMNavAGV::Reset()
{
	ManualSet(0, 0);

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/Reset AGV:%d IP:%s:%d ֪ͨAGV����״̬.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);

	return;
}

void CTCPMNavAGV::Remove(unsigned short usRFID)
{
	Action(m_usCurRFID, usRFID, _AGVACT_REMOVE);

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/Remove AGV:%d IP:%s:%d ֪ͨAGV��%d�����ƶ���%d.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort, m_usCurRFID, usRFID);

	return;
}

void CTCPMNavAGV::StopAction()
{
	Action(m_usCurRFID, m_usCurRFID, _AGVACT_NONE,_AGVACTSTA_NONE);

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/StopAction AGV:%d IP:%s:%d ֪ͨAGVֹͣ����.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);

	return;
}

void CTCPMNavAGV::Action(unsigned short usAction)
{
	unsigned short usCurRFID = GetCurRFID();	/*!< ��ǰRFID�������Ϣ */

	Action(usCurRFID, usCurRFID, usAction);

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/Action AGV:%d IP:%s:%d ֪ͨAGV��%dִ��%02X����.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort, usCurRFID, usAction & 0xFF);

	return;
}

void CTCPMNavAGV::Action(unsigned short usCurRFID, unsigned short usRFID, unsigned short usAction, unsigned char byActionStatus)
{
	byte data[13];
	memset(data, 0, 13);

	data[0] = 0x2f;
	data[1] = (usCurRFID >> 8) & 0xFF;
	data[2] = usCurRFID & 0xFF;
	data[3] = (usRFID >> 8) & 0xFF;
	data[4] = usRFID & 0xFF;
	data[9] = (usAction >> 8) & 0xFF;
	data[10] = usAction & 0xFF;
	data[11] = byActionStatus;

	std::list<BufferString::Buffer> bfPackage = CreatePackge(data, 13, m_byNo);

	for (std::list<BufferString::Buffer>::iterator it = bfPackage.begin(); it != bfPackage.end(); ++it)
	{
		SendPackage(*it);
	}

	return;
}

void CTCPMNavAGV::Shutdown()
{
	unsigned short usCurRFID = GetCurRFID();	/*!< ��ǰRFID�������Ϣ */

	ManualSet(usCurRFID,0xFE);

	DebugPrint("AGVControlSystem/CTCPMNavAGV.cpp/Restart AGV:%d IP:%s:%d ֪ͨAGV�ػ�.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);

	return;
}

void CTCPMNavAGV::ManualSet(unsigned short usRFID, unsigned char byStatus)
{
	byte data[13];
	memset(data, 0, 13);

	data[0] = 0x3f;
	data[1] = (usRFID >> 8) & 0xFF;
	data[2] = usRFID & 0xFF;
	data[5] = byStatus;

	std::list<BufferString::Buffer> bfPackage = CreatePackge(data, 13, m_byNo);

	for (std::list<BufferString::Buffer>::iterator it = bfPackage.begin(); it != bfPackage.end(); ++it)
	{
		SendPackage(*it);
	}

	return;
}

bool CTCPMNavAGV::Connect(SOCKET socket)
{
	if (CTCPClient::Connect(socket))
	{
		StartSendThread();

		return true;
	}

	return false;
}

bool CTCPMNavAGV::Connect(std::string strIP, unsigned short usPort)
{
	if (CTCPClient::Connect(strIP, usPort))
	{
		StartSendThread();

		return true;
	}

	return false;
}

bool CTCPMNavAGV::Connect(std::wstring wstrIP, unsigned short usPort)
{
	if (CTCPClient::Connect(wstrIP, usPort))
	{
		StartSendThread();

		return true;
	}

	return false;
}

#ifdef _AFX
bool CTCPMNavAGV::Connect(CString cstrIP, unsigned short usPort)
{
	if (CTCPClient::Connect(cstrIP, usPort))
	{
		StartSendThread();

		return true;
	}

	return false;
}
#endif // !_AFX

void CTCPMNavAGV::Break()
{
	EndSendThread();

	return;
}

bool CTCPMNavAGV::IsConnected() const
{
	if (m_pThreadSend && m_pThreadSend->joinable())
	{
		return true;
	}

	return CTCPClient::IsConnected();
}

void CTCPMNavAGV::IsConnected(std::string & strConnect)
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

void CTCPMNavAGV::IsConnected(std::wstring & wstrConnect)
{
	std::string strConnect = "";
	IsConnected(strConnect);

	BufferString::std_string_to_wstring(wstrConnect, strConnect);

	return;
}

#ifdef _AFX
void CTCPMNavAGV::IsConnected(CString & cstrConnect, COLORREF & color)
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
#endif // !_AGV_H