#include "stdafx.h"
#include "CService.h"
#include "XML/Xml.h"

#ifdef _SERVICE_H

CService::CService()
{
	m_pServer = nullptr;
	m_pThreadFunc = nullptr;
	m_hShutdownEvent = nullptr;
	m_hParent = nullptr;
	m_bPermission = true;
}

CService::~CService()
{
	Destroy();
}

void CService::DownLoadConfig()
{
}

void CService::UpLoadConfig()
{

}

void CService::LoadXMLConfig()
{
}

void CService::LoadDatabaseConfig()
{
}

void CService::Connect()
{
	if (m_pServer == nullptr)
	{
		return;
	}

	SOCKET socket = m_pServer->GetClient();
	if (socket == INVALID_SOCKET)
	{
		return;
	}

	std::string strClientIP = "";
	unsigned short usClientPort = 0;

	CTCPServer::GetSocketAddress(socket, strClientIP, usClientPort);

#ifdef _MAGNETICTRACK_H

	/*!
	 * AGV���
	*/
	for (std::map<unsigned char, CTCPMNavAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		CTCPMNavAGV *pAGV = it->second;	/*!< AGVָ�� */

		std::string strIP = "";		/*!< AGV��IP */
		short sPort = 0;			/*!< AGV�󶨶˿� */
		pAGV->GetIP(strIP);
		sPort = pAGV->GetPort();

		if (strClientIP == strIP && (sPort == -1 || (unsigned short)sPort == usClientPort))
		{
			if (pAGV->Connect(socket))
			{
				/*!
				 * �����豸����״̬
				*/

				std::string strRecord = "";
				BufferString::std_string_format(strRecord, "��ϵͳSystem��������AGV�����:%d����IP:%s�����˿�:%d��", pAGV->GetNo(), strClientIP.c_str(), usClientPort);
				// �����־
				DebugPrint(strRecord.c_str());
				// ��¼��־
				Record(strRecord);
			}

			return;
		}
	}

#ifdef _SELECT_FUNC

#ifdef _CHARGERCONTROLER_H
	/*!
	 * �����������
	*/
	for (std::map<unsigned char, CChargerControler*>::iterator it = m_mapChargerCtrlers.begin(); it != m_mapChargerCtrlers.end(); ++it)
	{
		CChargerControler *pChargerCtrler = it->second;	/*!< AGVָ�� */

		std::string strIP = "";		/*!< AGV��IP */
		short sPort = 0;			/*!< AGV�󶨶˿� */
		pChargerCtrler->GetIP(strIP);
		sPort = pChargerCtrler->GetPort();

		if (strClientIP == strIP && (sPort == -1 || (unsigned short)sPort == usClientPort))
		{
			if (pChargerCtrler->Connect(socket))
			{
				/*!
				 * �����豸����״̬
				*/

				std::string strRecord = "";
				BufferString::std_string_format(strRecord, "��ϵͳSystem�������ӳ������������:%d����IP:%s�����˿�:%d��", pChargerCtrler->GetNo(), strClientIP.c_str(), usClientPort);
				// �����־
				DebugPrint(strRecord.c_str());
				// ��¼��־
				Record(strRecord);
			}

			return;
		}
	}
#endif // !_CHARGERCONTROLER_H

#endif // !_SELECT_FUNC

#endif // !_MAGNETICTRACK_H

#ifdef _SELECT_FUNC

#ifdef _CA_H
	/*!
	 * CA�������
	*/
	for (std::map<unsigned short, CCA*>::iterator it = m_mapCAs.begin(); it != m_mapCAs.end(); ++it)
	{
		CCA *pCA = it->second;		/*!< CAָ�� */

		std::string strIP = "";		/*!< AGV��IP */
		short sPort = 0;			/*!< AGV�󶨶˿� */
		pCA->GetIP(strIP);
		sPort = pCA->GetPort();

		if (strClientIP == strIP && (sPort == -1 || (unsigned short)sPort == usClientPort))
		{
			if (pCA->Connect(socket))
			{
				/*!
				 * �����豸����״̬
				*/

				std::string strRecord = "";
				BufferString::std_string_format(strRecord, "��ϵͳSystem��������CA�������������:%d����IP:%s�����˿�:%d��", pCA->GetNo(), strClientIP.c_str(), usClientPort);
				// �����־
				DebugPrint(strRecord.c_str());
				// ��¼��־
				Record(strRecord);
			}

			return;
		}
	}

#endif // _CA_H

#endif // !_SELECT_FUNC

	return;
}

void CService::Disconnect()
{
	if (m_pServer && m_pServer->GetListenStatus())
	{
		m_pServer->EndListen();

		std::string strIP = "";
		m_pServer->GetIP(strIP);

		std::string strRecord = "";
		BufferString::std_string_format(strRecord, "��ϵͳSystem���жϷ���˼�����IP:%s�����˿�:%d��", strIP.c_str(), m_pServer->GetPort());
		// �����־
		DebugPrint(strRecord.c_str());
		// ��¼��־
		Record(strRecord);
	}

#ifdef _MAGNETICTRACK_H

	/*!
	 * AGV���
	*/
	for (std::map<unsigned char, CTCPMNavAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		CTCPMNavAGV *pAGV = it->second;	/*!< AGVָ�� */

		if (pAGV->IsConnected())
		{
			pAGV->Break();

			std::string strIP = "";
			pAGV->GetIP(strIP);

			std::string strRecord = "";
			BufferString::std_string_format(strRecord, "��ϵͳSystem���ж���AGV���ӡ�IP:%s�����˿�:%d��", strIP.c_str(), pAGV->GetPort());
			// �����־
			DebugPrint(strRecord.c_str());
			// ��¼��־
			Record(strRecord);
		}
	}

#ifdef _SELECT_FUNC

#ifdef _CHARGERCONTROLER_H
	/*!
	 * �����������
	*/
	for (std::map<unsigned char, CChargerControler*>::iterator it = m_mapChargerCtrlers.begin(); it != m_mapChargerCtrlers.end(); ++it)
	{
		CChargerControler *pChargerCtrler = it->second;	/*!< AGVָ�� */

		if (pChargerCtrler->IsConnected())
		{
			pChargerCtrler->Break();

			std::string strIP = "";
			pChargerCtrler->GetIP(strIP);

			std::string strRecord = "";
			BufferString::std_string_format(strRecord, "��ϵͳSystem���ж�������������ӡ�IP:%s�����˿�:%d��", strIP.c_str(), pChargerCtrler->GetPort());
			// �����־
			DebugPrint(strRecord.c_str());
			// ��¼��־
			Record(strRecord);
		}
	}
#endif // !_CHARGERCONTROLER_H

#ifdef _CA_H
	/*!
	 * CA�������
	*/
	for (std::map<unsigned short, CCA*>::iterator it = m_mapCAs.begin(); it != m_mapCAs.end(); ++it)
	{
		CCA *pCA = it->second;		/*!< CAָ�� */

		if (pCA->IsConnected())
		{
			pCA->Break();

			std::string strIP = "";
			pCA->GetIP(strIP);

			std::string strRecord = "";
			BufferString::std_string_format(strRecord, "��ϵͳSystem���ж���CA�����������ӡ�IP:%s�����˿�:%d��", strIP.c_str(), pCA->GetPort());
			// �����־
			DebugPrint(strRecord.c_str());
			// ��¼��־
			Record(strRecord);
		}
	}

#endif // _CA_H

#ifdef _LED_H
	/*!< LED�� */
#endif // !_LED_H

#endif // !_SELECT_FUNC

#endif // !_MAGNETICTRACK_H

	return;
}

void CService::FunctionThread()
{
	while (true)
	{

#ifdef _SELECT_FUNC

#ifdef _CA_H
		ProcessCallMsg();
#endif // _CA_H

#endif // !_SELECT_FUNC

#ifdef _MAGNETICTRACK_H
		TrafficControl();

#ifdef _SELECT_FUNC

#ifdef _SELECT_AUTO_GOUP
		RestPositionControl();
#endif // !_SELECT_AUTO_GOUP

#ifdef _CHARGERCONTROLER_H
		ProcessChargerControlerData();

#endif // !_CHARGERCONTROLER_H

#endif // !_SELECT_FUNC

#endif // !_MAGNETICTRACK_H

		if (WaitForSingleObject(m_hShutdownEvent, 100) == WAIT_OBJECT_0)
		{
			break;
		}
	}

	return;
}

void CService::StartThread()
{
	EndThread();

	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(nullptr, true, false, nullptr);
	}

	m_pThreadFunc = new std::thread(&CService::FunctionThread, this);

	// ��¼��־
	Record("��ϵͳSystem��ҵ���ܡ�������.");
	// �������
	DebugPrint("AGVContorlSystem/CService/StartThread ���������߳�.");

	return;
}

void CService::EndThread()
{
	if (m_pThreadFunc)
	{
		if (m_pThreadFunc->joinable())
		{
			SetEvent(m_hShutdownEvent);

			m_pThreadFunc->join();
		}

		delete m_pThreadFunc;
		m_pThreadFunc = nullptr;

		// ��¼��־
		Record("��ϵͳSystem��ҵ���ܡ��رա�.");
		// �������
		DebugPrint("AGVContorlSystem/CService/StartThread �رչ����߳�.");
	}

	return;
}

void CService::DestroyData()
{
	// �������
	DebugPrint("AGVContorlSystem/CService/StartThread ��������.");

	if (m_pServer)
	{
		delete m_pServer;

		m_pServer = nullptr;
	}

#ifdef _SELECT_FUNC

#ifdef _CHARGERCONTROLER_H
	for (std::map<unsigned char, CChargerControler*>::iterator it = m_mapChargerCtrlers.begin(); it != m_mapChargerCtrlers.end(); )
	{
		delete it->second;

		it = m_mapChargerCtrlers.erase(it);
	}
#endif // !_CHARGERCONTROLER_H

#ifdef _CA_H
	for (std::map<unsigned short, CCA*>::iterator it = m_mapCAs.begin(); it != m_mapCAs.end(); )
	{
		delete it->second;

		it = m_mapCAs.erase(it);
	}

	for (std::map<unsigned char, CAWatch*>::iterator it = m_mapCAWatchs.begin(); it != m_mapCAWatchs.end(); )
	{
		delete it->second;

		it = m_mapCAWatchs.erase(it);
	}
#endif // !_CA_H

#ifdef _LED_H
	/*!< LED�� */
#endif // !_LED_H

#endif // !_SELECT_FUNC

#ifdef _MAGNETICTRACK_H
	for (std::map<unsigned char, CTCPMNavAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); )
	{
		delete it->second;

		it = m_mapAGVs.erase(it);
	}

	for (std::map<unsigned short, CRFID*>::iterator it = m_mapRFIDs.begin(); it != m_mapRFIDs.end(); )
	{
		delete it->second;

		it = m_mapRFIDs.erase(it);
	}

	for (std::map<unsigned char, CMNavStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end(); )
	{
		delete it->second;

		it = m_mapStations.erase(it);
	}

	for (std::map<unsigned char, CMNavRestPosition*>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); )
	{
		delete it->second;

		it = m_mapRests.erase(it);
	}

	for (std::map<unsigned short, CMTTrafficCtrlArea*>::iterator it = m_mapTraffic.begin(); it != m_mapTraffic.end(); )
	{
		delete it->second;

		it = m_mapTraffic.erase(it);
	}

#ifdef _SELECT_FUNC
	for (std::map<unsigned char, CMNavCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); )
	{
		delete it->second;

		it = m_mapChargers.erase(it);
	}

#ifdef _CA_H
	for (std::map<unsigned short, CACall*>::iterator it = m_mapCACalls.begin(); it != m_mapCACalls.end(); )
	{
		delete it->second;

		it = m_mapCACalls.erase(it);
	}
#endif // _CA_H

#endif // !_SELECT_FUNC

#endif 

	if (m_hShutdownEvent)
	{
		CloseHandle(m_hShutdownEvent);

		m_hShutdownEvent = nullptr;
	}

	return;
}

bool CService::Open()
{
	std::string strRecord = "";

	LoadXMLConfig();

	if (m_adoDB.OnInitADOConn())
	{
		BufferString::std_string_format(strRecord, "��ϵͳSystem���������ݿ⡾���Ӵ�:%s�����û���:%s��������:%s���ɹ�.",
			(char*)m_adoDB.m_bstrConnect,(char*)m_adoDB.m_bstrUser,(char*)m_adoDB.m_bstrPassword);

		// ��¼��־
		Record(strRecord);
		// �������
		DebugPrint(strRecord.c_str());
	}
	else
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("�������ݿ�ʧ��!"), _T("����ʧ��"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "��ϵͳSystem���������ݿ⡾���Ӵ�:%s�����û���:%s��������:%s��ʧ��.",
			(char*)m_adoDB.m_bstrConnect, (char*)m_adoDB.m_bstrUser, (char*)m_adoDB.m_bstrPassword);

		// ��¼��־
		Record(strRecord);
		// �������
		DebugPrint(strRecord.c_str());

		return false;
	}

	LoadDatabaseConfig();

	if (m_bPermission == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("ȱ��ʹ��Ȩ��!"), _T("����ʧ��"), MB_ICONERROR);
		}
#endif // _AFX

		// ��¼��־
		Record(strRecord);
		// �������
		DebugPrint(strRecord.c_str());

		return false;
	}

	if (m_pServer && m_pServer->GetListenStatus() == false)
	{
		std::string strIP = "";
		m_pServer->GetIP(strIP);

		if (m_pServer->StartListen())
		{
			BufferString::std_string_format(strRecord, "��ϵͳSystem������ˡ�IP:%s:%d����������.", strIP.c_str(), m_pServer->GetPort());

			// ��¼��־
			Record(strRecord);
			// �������
			DebugPrint(strRecord.c_str());
		}
		else
		{
#ifdef _AFX
			CString cstrMsg = _T("");
			CString cstrIP = _T("");
			m_pServer->GetIP(cstrIP);
			cstrMsg.Format(_T("�����IP:%s:%d����ʧ��!"), cstrIP, m_pServer->GetPort());
			if (m_hParent)
			{
				MessageBox(m_hParent, cstrMsg, _T("����ʧ��"), MB_ICONERROR);
			}
#endif // _AFX

			BufferString::std_string_format(strRecord, "��ϵͳSystem������ˡ�IP:%s:%d������ʧ��.", strIP.c_str(), m_pServer->GetPort());

			// ��¼��־
			Record(strRecord);
			// �������
			DebugPrint(strRecord.c_str());

			return false;
		}
	}

#ifdef _AFX
	m_TaskManager.OpenManager(m_adoDB, m_hParent);
#else
	m_TaskManager.OpenManager(m_adoDB);
#endif // _AFX

	StartThread();

	return true;
}

#ifdef _AFX
bool CService::Open(const HWND hParent)
{
	if (hParent == nullptr)
	{
		DebugPrint("��Ч�ĸ����ھ��");
		return false;
	}

	m_hParent = hParent;

	return Open();
}
#endif //_AFX

void CService::Close()
{
	EndThread();
	Disconnect();
}

void CService::Destroy()
{
	Close();
	DestroyData();
}

bool CService::GetAdmin(std::string strPassword)
{
	std::string strSql = "";
	BufferString::std_string_format(strSql,
		"select * from [%s]",
		"USER_MANAGER_TABLE"
	);

	_RecordsetPtr pRecordset = m_adoDB.GetRecordSet(_bstr_t(strSql.c_str())); /*!< ����� */

	if (pRecordset == nullptr || pRecordset->adoEOF)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ԱȨ�޻�ȡʧ��!��ȡ���ݿ�ʧ��."), _T("����ԱȨ�޻�ȡ"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	std::string strAesAdminPassword = ""; /*!< AES���ܺ�Ĺ���Ա���� */

	_variant_t var;	/*!< ֵ */

	// ��ȡAES���ܺ�Ĺ���Ա����
	var = pRecordset->GetCollect("admin_password");
	// ֵ��Ϊ��
	if (var.vt != VT_NULL)
	{
		strAesAdminPassword = (char*)_bstr_t(var);
	}

	if (strAesAdminPassword == "")
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ԱȨ�޻�ȡʧ��!δ���ù���Ա����."), _T("����ԱȨ�޻�ȡ"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	std::string strAdminPassword = m_aes.DecryptionAES(strAesAdminPassword);	/*!< ���Ĺ���Ա���� */

	if (strAdminPassword == strPassword)
	{
		return true;
	}

#ifdef _AFX
	if (m_hParent)
	{
		MessageBox(m_hParent, _T("����ԱȨ�޻�ȡʧ��!���벻��ȷ."), _T("����ԱȨ�޻�ȡ"), MB_ICONERROR);
	}
#endif // _AFX

	return false;
}

bool CService::GetAdmin(std::wstring wstrPassword)
{
	std::string strPassword = "";
	BufferString::std_wstring_to_string(strPassword, wstrPassword);

	return GetAdmin(strPassword);
}

#ifdef _AFX
bool CService::GetAdmin(CString cstrPassword)
{
	std::string strPassword = "";
	BufferString::CString_to_std_string(strPassword, cstrPassword);

	return GetAdmin(strPassword);
}
#endif //_AFX

bool CService::SetAdmin(std::string strOldPassword, std::string strNewPassword)
{
	if(strNewPassword == "")
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ԱȨ������ʧ��!���벻��Ϊ��."), _T("����ԱȨ������"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	std::string strSql = "";
	BufferString::std_string_format(strSql,
		"select * from [%s]",
		"USER_MANAGER_TABLE"
	);

	_RecordsetPtr pRecordset = m_adoDB.GetRecordSet(_bstr_t(strSql.c_str())); /*!< ����� */

	if (pRecordset == nullptr || pRecordset->adoEOF)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ԱȨ������ʧ��!��ȡ���ݿ�ʧ��."), _T("����ԱȨ������"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}
	std::string strAesAdminPassword = ""; /*!< AES���ܺ�Ĺ���Ա���� */

	_variant_t var;	/*!< ֵ */

	// ��ȡAES���ܺ�Ĺ���Ա����
	var = pRecordset->GetCollect("admin_password");

	// ֵ��Ϊ��
	if (var.vt != VT_NULL)
	{
		strAesAdminPassword = (char*)_bstr_t(var);
	}

	if (strAesAdminPassword != "")
	{
		std::string strAdminPassword = m_aes.DecryptionAES(strAesAdminPassword);	/*!< ���Ĺ���Ա���� */

		if (strOldPassword != strAdminPassword)
		{
#ifdef _AFX
			if (m_hParent)
			{
				MessageBox(m_hParent, _T("����ԱȨ������ʧ��!���벻��ȷ."), _T("����ԱȨ�޻�ȡ"), MB_ICONERROR);
			}
#endif // _AFX

			return false;
		}
	}

	std::string strAesNewPassword = m_aes.EncryptionAES(strNewPassword);

	BufferString::std_string_format(strSql,
		"update [%s] set [%s]=%s where [%s]=%s",
		"AGV_SYSTEM_CONFIG", "Value" , strAesNewPassword.c_str(),"Name", "Admin"
	);

	// ִ��SQL���
	if (m_adoDB.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// �������
		DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

		BufferString::std_string_format(strSql,
			"insert into[%s]([%s],[%s]) values('%s','%s')",
			"AGV_SYSTEM_CONFIG", "Name","Value", "Admin", strAesNewPassword.c_str()
		);

		// ִ��SQL���
		if (m_adoDB.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
		{
			// �������
			DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

#ifdef _AFX
			if (m_hParent)
			{
				MessageBox(m_hParent, _T("����ԱȨ������ʧ��!���ݿ�д��ʧ��."), _T("����ԱȨ�޻�ȡ"), MB_ICONERROR);
			}
#endif // _AFX
			return false;
		}
	}

	return true;
}

bool CService::SetAdmin(std::wstring wstrOldPassword, std::wstring wstrNewPassword)
{
	std::string strOldPassword = "", strNewPassword = "";
	BufferString::std_wstring_to_string(strOldPassword, wstrOldPassword);
	BufferString::std_wstring_to_string(strNewPassword, wstrNewPassword);

	return SetAdmin(strOldPassword, strNewPassword);
}

#ifdef _AFX
bool CService::SetAdmin(CString cstrOldPassword, CString cstrNewPassword)
{
	std::string strOldPassword = "", strNewPassword = "";
	BufferString::CString_to_std_string(strOldPassword, cstrOldPassword);
	BufferString::CString_to_std_string(strNewPassword, cstrNewPassword);

	return SetAdmin(strOldPassword, strNewPassword);
}
#endif //_AFX

#ifdef _SELECT_FUNC

#ifdef _CA_H
void CService::ProcessCallMsg()
{
	std::string strMsg = "";	/*!< ���������ص���Ϣ */
	strMsg = CCA::GetRecvMsg();

	if (strMsg == "")
	{
		return;
	}

	/*!
	 * ���ݹ���Ժ��������ص���Ϣ���н���
	 * ���������Ӧ������
	 * Զ�̺�����������ɽ�Ϊ��һ������
	 * ���ǿ���ͨ��ʹ�ö����������ϵķ�ʽ��ɽ�Ϊ���ӵ�����
	 * ���������ϵ�
	 * Ϊ��ֹ��û�к��з���,��κ�������������ظ��·����������.
	 * ÿ�����������ܺ���1������ֻ�е��������ʱ���ܽ�����һ�εĺ���
	 * �������в�������ͬ��ͬ���ݵ�����
	*/
	unsigned int unCallNo = 0;	/*!< ��������� */
	std::string strText = "";	/*!< ��������Ϣ�ı� */

	/*!
	 * ������Ϣ
	*/

	// ��������
	PublishCallTask(unCallNo, strText);
	
	return;
}

void CService::SendMsgToWatch(std::string strMsg, unsigned char byStation)
{
	for (std::map<unsigned char, CAWatch*>::iterator it = m_mapCAWatchs.begin(); it != m_mapCAWatchs.end(); ++it)
	{
		CAWatch* pWatch = it->second;

		if (pWatch->m_byStation != byStation && pWatch->m_byStation != 0)
		{
			continue;
		}

		if (pWatch->m_usCA == 0)
		{
			std::map<unsigned short, CCA*> m_mapCAs;

			for (std::map<unsigned short, CCA*>::iterator itCA = m_mapCAs.begin(); itCA != m_mapCAs.end(); ++itCA)
			{
				CCA* pCA = itCA->second;

				pCA->SendToWatch(pWatch->m_byNo, strMsg);
			}
		}
		else
		{
			if (m_mapCAs.find(pWatch->m_usCA) == m_mapCAs.end())
			{
				continue;
			}

			m_mapCAs[pWatch->m_usCA]->SendToWatch(pWatch->m_byNo, strMsg);
		}
	}

	return;
}

void CService::SendMsgToWatch(std::wstring wstrMsg, unsigned char byStation)
{
	std::string strMsg = "";
	BufferString::std_wstring_to_string(strMsg, wstrMsg);

	SendMsgToWatch(strMsg, byStation);

	return;
}

#ifdef _AFX
void CService::SendMsgToWatch(CString cstrMsg, unsigned char byStation)
{
	std::string strMsg = "";
	BufferString::CString_to_std_string(strMsg, cstrMsg);

	SendMsgToWatch(strMsg, byStation);

	return;
}
#endif // _AFX

#endif // !_CA_H

#ifdef _CHARGERCONTROLER_H
void CService::ProcessChargerControlerData()
{

}

#endif // !_CHARGERCONTROLER_H

#endif // !_SELECT_FUNC

#ifdef _MAGNETICTRACK_H
void CService::TrafficControl()
{
	for (std::map<unsigned short, CMTTrafficCtrlArea*>::iterator it = m_mapTraffic.begin(); it != m_mapTraffic.end(); ++it)
	{
		CMTTrafficCtrlArea* pTrafficArea = it->second;

		unsigned short usCtrlRFID = pTrafficArea->GetCtrlRFID();	/*!< ����RFID����� */

		assert(usCtrlRFID != 0);

		if (m_mapRFIDs.find(usCtrlRFID) == m_mapRFIDs.end())
		{
			// ���Ƶ���Ч
			continue;
		}

		CRFID* pCtrlRFID = m_mapRFIDs[usCtrlRFID];	/*!< ����RFID���ṹ�� */

		CTCPMNavAGV *pAGV = nullptr;	/*!< ���Ƶ�AGVָ�� */

		// AGV�����ڴ���״̬�����ֶ�ģʽ���޵�
		if (pAGV->GetStatus() != _AGVSTA_TRAFFIC || pAGV->GetMode() == false || pAGV->GetPower() == 0)
		{
			continue;
		}

#ifdef _WIN64
		unsigned long long ullUseTime = 0;	/*!< RFIDʹ��ʱ�� */
		pAGV = (CTCPMNavAGV *)pCtrlRFID->GetUser(ullUseTime);
#elif _WIN32
		unsigned long ulUseTime = 0;	/*!< RFIDʹ��ʱ�� */
		pAGV = (CTCPMNavAGV *)pCtrlRFID->GetUser(ulUseTime);
#endif //!_WIN64

		if (pAGV == nullptr)
		{
			// ��AGV�ڹ��Ƶ�
			continue;
		}

		bool bNext = true;	/*!< ��һ�׶α�ʶ */

		/*!
		 * ����RFID�����
		*/
		std::set<unsigned short> setStop = pTrafficArea->GetStopRFIDList();	/*!< ����RFID������� */

		for (std::set<unsigned short>::iterator itStop = setStop.begin(); itStop != setStop.end(); ++itStop)
		{
			unsigned short usStopRFID = *itStop;	/*!< ����RFID����� */

			if (usStopRFID == 0)
			{
				continue;
			}

			if (m_mapRFIDs.find(usStopRFID) == m_mapRFIDs.end())
			{
				// ���е���Ч
				continue;
			}

			CRFID* pStopRFID = m_mapRFIDs[usStopRFID];	/*!< ����RFID���ṹ�� */
			
			if (pStopRFID->GetUser())
			{
				// ����AGV�ڽ���RFID����
				bNext = false;
				break;
			}
		}

		if (bNext == false)
		{
			continue;
		}

		/*!
		 * �ԱȽ�ͨ���Ƶ���
		*/
		std::set<unsigned short> setCompare = pTrafficArea->GetCompareAreaList();	/*!< �ԱȽ�ͨ���Ƶ��� */

		for (std::set<unsigned short>::iterator itCompare = setCompare.begin(); itCompare != setCompare.end(); ++itCompare)
		{
			unsigned short usArea = *itCompare;	/*!< ��ͨ���Ƶ��� */

			if (m_mapTraffic.find(usArea) == m_mapTraffic.end())
			{
				// �Աȵ���Ч
				continue;
			}

			CMTTrafficCtrlArea *pCompareArea = m_mapTraffic[usArea];	/*!< �ԱȽ�ͨ���Ƶ�ṹ�� */

			/*!
			 * ����RFID�����
			*/
			std::set<unsigned short> setCompareStop = pCompareArea->GetStopRFIDList();	/*!< ����RFID������� */

			for (std::set<unsigned short>::iterator itStop = setCompareStop.begin(); itStop != setCompareStop.end(); ++itStop)
			{
				unsigned short usStopRFID = *itStop;	/*!< ����RFID����� */

				if (usStopRFID == 0)
				{
					// ���е���Ч
					continue;
				}

				if (m_mapRFIDs.find(usStopRFID) == m_mapRFIDs.end())
				{
					// ���е���Ч
					continue;
				}

				CRFID* pStopRFID = m_mapRFIDs[usStopRFID];	/*!< ����RFID���ṹ�� */

				if (pStopRFID->GetUser())
				{
					// ����AGV�ڽ���RFID����
					bNext = false;
					break;
				}
			}

			if (bNext == false)
			{
				break;
			}

			unsigned short usCompareRFID = pCompareArea->GetCtrlRFID();	/*!< �ԱȽ�ͨ���Ƶ����RFID����� */

			if(usCompareRFID == 0)
			{
				// �Աȵ�Ĺ��Ƶ���Ч
				continue;
			}

			if (m_mapRFIDs.find(usCompareRFID) == m_mapRFIDs.end())
			{
				// �Աȵ�Ĺ��Ƶ���Ч
				continue;
			}

			CRFID *pCompareCtrlRFID = m_mapRFIDs[usCompareRFID];	/*!< ����RFID���ṹ�� */

#ifdef _WIN64
			unsigned long long ullCompareUseTime = 0;		/*!< �Ա�RFIDʹ��ʱ�� */
			if (pCompareCtrlRFID->GetUser(ullCompareUseTime))
			{
				// �Աȵ�Ĺ��Ƶ����AGV
				if (ullCompareUseTime < ullUseTime)
				{
					// �Աȵ�ʹ��ʱ�����ڹ��Ƶ�ʹ��ʱ��
					bNext = false;
					break;
				}
			}
			else
			{
				// �Աȵ�Ĺ��Ƶ㲻����AGV
				continue;
			}
#elif _WIN32
			unsigned long ulCompareUseTime = 0;		/*!< �Ա�RFIDʹ��ʱ�� */
			if (pCompareCtrlRFID->GetUser(ulCompareUseTime))
			{
				// �Աȵ�Ĺ��Ƶ����AGV
				if (ulCompareUseTime < ulUseTime)
				{
					// �Աȵ�ʹ��ʱ�����ڹ��Ƶ�ʹ��ʱ��
					bNext = false;
					break;
				}
			}
			else
			{
				// �Աȵ�Ĺ��Ƶ㲻����AGV
				continue;
			}
#endif // !_WIN64
		}

		if (bNext == false)
		{
			continue;
		}

		/*!
		 * ֪ͨAGV����ͨ��
		*/
		pAGV->TrafficPass();

	}

	return;
}

void CService::UpdateAGVStatus(unsigned char byNo,unsigned char byPower, char chSpeed, unsigned char byStatus,
	char chError, unsigned short usAction, unsigned char byActStatus, unsigned char byCargo,
	unsigned short usRFID, unsigned short usNextRFID)
{
	if (m_mapAGVs.find(byNo) == m_mapAGVs.end())
	{
		return;
	}

	CTCPMNavAGV* pAGV = nullptr;	/*!< AGVָ�� */
	pAGV = m_mapAGVs[byNo];
		
	bool bNew = false;		/*!< ���±�ʶ */
		
	if (pAGV->SetPower(byPower)
		|| pAGV->SetSpeed(chSpeed) 
		|| pAGV->SetStatus(byStatus)
		|| pAGV->SetError(chError) 
		|| pAGV->SetAction(usAction, byActStatus) 
		|| pAGV->SetCargo(byCargo) 
		|| pAGV->SetNextRFID(usNextRFID)
		)
	{
		bNew = true;
	}

	if (pAGV->SetCurRFID(usRFID))
	{
		std::string strSql = ""; /*!< SQL��� */

		// �ϳ�SQL���
		BufferString::std_string_format(strSql,
			"update [%s] set [%s]=%d where [%s]=%d",
			"AGV_RUNTIME_TABLE", "agv_cur_rfid", pAGV->GetCurRFID(), "agv_no", pAGV->GetNo()
		);

		// ִ��SQL���
		if (m_adoDB.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
		{
			// �������
			DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

			// �ϳ�SQL���
			BufferString::std_string_format(strSql,
				"insert into [%s]([%s],[%s]) values(%d,%d)",
				"AGV_RUNTIME_TABLE", "agv_no", "agv_cur_rfid", pAGV->GetNo(), pAGV->GetCurRFID()
			);

			m_adoDB.ExecuteSQL(_bstr_t(strSql.c_str()));
		}

		bNew = true;
	}

#ifdef _AFX
	if (bNew && m_hParent)
	{
		::PostMessage(m_hParent, WM_SERVICE_UPDATE, ID_DATA_AGV, (LPARAM)pAGV);
	}
#endif //_AFX

	CRFID* pCurRDIF = nullptr;	/*!< AGV��ǰRFID��ָ�� */
	CRFID* pOldRDIF = nullptr;	/*!< AGV��һRFID��ָ�� */

	if (m_mapRFIDs.find(pAGV->GetCurRFID()) == m_mapRFIDs.end())
	{
		pCurRDIF = m_mapRFIDs[pAGV->GetCurRFID()];
	}

	// ����RFID��
	if (pCurRDIF)
	{
		pCurRDIF->Lock(pAGV);
	}

	if (m_mapRFIDs.find(pAGV->GetOldRFID()) == m_mapRFIDs.end())
	{
		pOldRDIF = m_mapRFIDs[pAGV->GetOldRFID()];
	}

	// ����RFID��
	if (pOldRDIF)
	{
		pOldRDIF->Unlock(pAGV);
	}

	return;
}

void CService::UpdateAGVError(unsigned char byNo, char chError)
{
	if (m_mapAGVs.find(byNo) == m_mapAGVs.end())
	{
		return;
	}

	CTCPMNavAGV* pAGV = nullptr;	/*!< AGVָ�� */
	pAGV = m_mapAGVs[byNo];

	if (pAGV->SetError(chError) == false)
	{
		return;
	}

#ifdef _AFX
	if (m_hParent)
	{
		::PostMessage(m_hParent, WM_SERVICE_UPDATE, ID_DATA_AGV, (LPARAM)pAGV);
	}
#endif //_AFX

	return;
}

void CService::UpdateAGVRFID(unsigned char byNo, unsigned short usRFID)
{
	if (m_mapAGVs.find(byNo) == m_mapAGVs.end())
	{
		return;
	}

	CTCPMNavAGV* pAGV = nullptr;	/*!< AGVָ�� */
	pAGV = m_mapAGVs[byNo];

	if (pAGV->SetCurRFID(usRFID))
	{
#ifdef _AFX
		if (m_hParent)
		{
			::PostMessage(m_hParent, WM_SERVICE_UPDATE, ID_DATA_AGV, (LPARAM)pAGV);
		}
#endif //_AFX
	}

	CRFID* pCurRDIF = nullptr;	/*!< AGV��ǰRFID��ָ�� */
	CRFID* pOldRDIF = nullptr;	/*!< AGV��һRFID��ָ�� */

	if (m_mapRFIDs.find(pAGV->GetCurRFID()) == m_mapRFIDs.end())
	{
		pCurRDIF = m_mapRFIDs[pAGV->GetCurRFID()];
	}

	// ����RFID��
	if (pCurRDIF)
	{
		pCurRDIF->Lock(pAGV);
	}

	if (m_mapRFIDs.find(pAGV->GetOldRFID()) == m_mapRFIDs.end())
	{
		pOldRDIF = m_mapRFIDs[pAGV->GetOldRFID()];
	}

	// ����RFID��
	if (pOldRDIF)
	{
		pOldRDIF->Unlock(pAGV);
	}

	return;
}

#ifdef _TASK_H
void CService::ProcessTask()
{
	std::map<unsigned short,AGVTaskArr> taskList = m_TaskManager.GetList();

	if (taskList.size() == 0)
	{
		return;
	}

	for (std::map<unsigned short, AGVTaskArr>::iterator itArr = taskList.begin(); itArr != taskList.end(); ++itArr)
	{
		AGVTaskArr& arr = itArr->second;	/*!< ������ */

		std::map<unsigned char, AGVTask*> list = arr.GetTaskList();

		for (std::map<unsigned char, AGVTask*>::iterator it = list.begin(); it != list.end(); ++it)
		{
			AGVTask* pTask = it->second;	/*!< ����ָ�� */

			if (pTask->GetStatus() == Task::_TASKSTA_NONE)
			{
			}
			else if (pTask->GetStatus() == Task::_TASKSTA_EXECUTE)
			{
			}
			else if (pTask->GetStatus() == Task::_TASKSTA_FINISH)
			{
				break;
			}
			else if (pTask->GetStatus() == Task::_TASKSTA_PAUSE)
			{
				break;
			}
			else if (pTask->GetStatus() == Task::_TASKSTA_CANCEL)
			{
				continue;
			}
		}
	}

	return;
}
#endif _TASK_H

#ifdef _SELECT_AUTO_GOUP
void CService::RestPositionControl()
{
	for (std::map<unsigned char, CMNavRestPosition*>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); ++it)
	{
		CMNavRestPosition* pRest = it->second;	/*!< ����λ */

		unsigned short usRestRFID = pRest->GetLocation();	/*!< ����λRFID����� */

		assert(usRestRFID != 0);

		if (m_mapRFIDs.find(usRestRFID) == m_mapRFIDs.end())
		{
			// ��Ч�Ĵ���λRFID��
			continue;
		}

		CRFID *pRestRFID = m_mapRFIDs[usRestRFID];	/*!< ����λRFID�� */

		CTCPMNavAGV *pAGV = (CTCPMNavAGV *)pRestRFID->GetUser();	/*!< ����λAGVָ�� */
		if (pAGV == nullptr)
		{
			// ��AGV�ڴ���λ
			continue;
		}

		// AGV�����ڴ���״̬�����ֶ�ģʽ���޵�
		if (pAGV->GetStatus() != _AGVSTA_WAIT || pAGV->GetMode() == false || pAGV->GetPower() == 0)
		{
			continue;
		}

		bool bNext = true;	/*!< ��һ�׶α�ʶ */

		/*!
		 * ����RFID�����
		*/
		std::set<unsigned short> setStop = pRest->GetStopRFIDList();	/*!< ����RFID���б� */

		for (std::set<unsigned short>::iterator itStop = setStop.begin(); itStop != setStop.end(); ++itStop)
		{
			unsigned short usStopRFID = *itStop;	/*!< ����RFID����� */

			if (usStopRFID == 0)
			{
				// ���е���Ч
				continue;
			}

			if (m_mapRFIDs.find(usStopRFID) == m_mapRFIDs.end())
			{
				// ���е���Ч
				continue;
			}

			CRFID *pStopRFID = m_mapRFIDs[usStopRFID];	/*!< ����RFID���ṹ�� */

			if (pStopRFID->GetUser())
			{
				// ����AGV�ڽ���RFID����
				bNext = false;
				break;
			}
		}

		if (bNext == false)
		{
			continue;
		}

		if (pRest->GetNext() == 0)
		{
			// ��Ч�Ĵ���λ���
			continue;
		}

		if (m_mapRests.find(pRest->GetNext()) == m_mapRests.end())
		{
			// ��Ч�Ĵ���λ
			continue;
		}

		// CMNavRestPosition restNext(m_mapRests[rest.GetNext()]); /*!< ��һ����λ */
		CMNavRestPosition *pRestNext = m_mapRests.find(pRest->GetNext())->second; /*!< ��һ����λ */

		unsigned short usNextRestRFID = pRestNext->GetLocation();	/*!< ��һ����λRFID����� */

		if (usNextRestRFID == 0)
		{
			// ��Ч����һ����λRFID��
			continue;
		}

		if (m_mapRFIDs.find(usNextRestRFID) == m_mapRFIDs.end())
		{
			// ��Ч����һ����λRFID��
			continue;
		}

		CRFID *pNextRestRFID = m_mapRFIDs[usNextRestRFID];	/*!< ��һ����λRFID�� */

		if (pNextRestRFID->GetUser())
		{
			// ����AGV����һ����λ
			continue;
		}

		/*!
		 * ����RFID�����
		*/
		std::set<unsigned short> setNextStop = pRestNext->GetStopRFIDList();	/*!< ����RFID���б� */

		for (std::set<unsigned short>::iterator itStop = setNextStop.begin(); itStop != setNextStop.end(); ++itStop)
		{
			unsigned short usStopRFID = *itStop;	/*!< ����RFID����� */

			if (usStopRFID == 0)
			{
				// ���е���Ч
				continue;
			}

			if (m_mapRFIDs.find(usStopRFID) == m_mapRFIDs.end())
			{
				// ���е���Ч
				continue;
			}

			CRFID *pStopRFID = m_mapRFIDs[usStopRFID];	/*!< ����RFID���ṹ�� */

			if (pStopRFID->GetUser())
			{
				// ����AGV�ڽ���RFID����
				bNext = false;
				break;
			}
		}

		if (bNext == false)
		{
			continue;
		}

		/*!
		 * ֪ͨAGV�ƶ�����һ����λ
		*/
		pAGV->MoveTo(usNextRestRFID);
	}

	return;
}

#ifdef _CHARGER_H
void CService::ChargerControl()
{
}
#endif // _CHARGER_H

#endif // !_SELECT_AUTO_GOUP

#ifdef _CA_H
void CService::PublishCallTask(unsigned int unNo, std::string strMsg)
{
}

void CService::PublishCallTask(unsigned int unNo, std::wstring strMsg)
{
}

#ifdef _AFX
void CService::PublishCallTask(unsigned int unNo, CString strMsg)
{
}
#endif // _AFX
#endif // _CA_H

bool CService::AGVScream(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "���û�User��";
	std::string strCtrl = "����ͣ��";
	std::string strAGV = "AGV";

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d",byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	//std::string strAction = "";				/*!< ���� */
	//std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	//pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< ���� */
	unsigned char byActStatus = 0;			/*!< ����״̬ */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// ����
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �������Զ�ģʽ
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:��ģʽ:%s��.",strMode.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �Ѵ��ڼ�ͣ״̬
	if (pAGV->GetStatus() == _AGVSTA_SCREAM)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV�Ѵ��ڡ���ͣ��״̬."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV�Ѵ���" + strStatus + "״̬.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// ����ִ�ж���
	if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			if (MessageBox(m_hParent, _T("��ͣ��ʹAGV����ִ�еĶ����ж�,�Ƿ�ͣ?"), _T("��������"), MB_ICONWARNING | MB_YESNO) == IDNO)
			{
				return false;
			}
		}
#endif // _AFX
	}

	pAGV->Scream();

	strRecord = strUser + "����" + strAGV + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::AGVRestart(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "���û�User��";
	std::string strCtrl = "����λ��";
	std::string strAGV = "AGV";

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	//std::string strAction = "";				/*!< ���� */
	//std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	//pAGV->GetAction(strAction, strActStatus);

	//unsigned short usAction = 0;			/*!< ���� */
	//unsigned char byActStatus = 0;			/*!< ����״̬ */
	//pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// ����
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �������Զ�ģʽ
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ��!AGV���ܽ���ϵͳ�Ŀ���:��ģʽ:%s��.", strMode.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �Ѵ��ڴ���״̬
	if (pAGV->GetStatus() == _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV�Ѵ��ڴ���״̬."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV�Ѵ���" + strStatus + "״̬.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	pAGV->Restart();

	strRecord = strUser + "����" + strAGV + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::AGVMove(unsigned char byAGVNo, unsigned short usRFIDNo)
{
	std::string strRecord = "";
	std::string strUser = "���û�User��";
	std::string strCtrl = "���ƶ���";
	std::string strAGV = "AGV";

	BufferString::std_string_format(strCtrl, "���ƶ�����������:%d��", usRFIDNo);

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// ��Ч��RFID�����
	if (usRFIDNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч���ƶ�����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч���ƶ�����:%d", usRFIDNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����RFID��������Ϣ
	if (m_mapRFIDs.find(usRFIDNo) == m_mapRFIDs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч���ƶ�����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч���ƶ�����:%d", usRFIDNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	std::string strAction = "";				/*!< ���� */
	std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< ���� */
	unsigned char byActStatus = 0;			/*!< ����״̬ */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// ����
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �������Զ�ģʽ
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:��ģʽ:%s��.",strMode.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �����ڴ���״̬
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:������״̬:%s��.",strStatus.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ѵ����ƶ��յ�
	if (pAGV->GetCurRFID() == usRFIDNo)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV�Ѵ����ƶ�������."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV�Ѵ����ƶ�������.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// ����ִ�ж���
	if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����ִ�ж���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����ִ�ж���:������:" + strAction + "��������״̬:" + strActStatus + "��";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	pAGV->MoveTo(usRFIDNo);

	strRecord = strUser + "����" + strAGV + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::AGVStopAction(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "���û�User��";
	std::string strCtrl = "������ֹͣ��";
	std::string strAGV = "AGV";

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	std::string strAction = "";				/*!< ���� */
	std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< ���� */
	unsigned char byActStatus = 0;			/*!< ����״̬ */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// ����
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strAction + "ʧ��!AGV����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �������Զ�ģʽ
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:��ģʽ:%s��.",strMode.c_str());
		strRecord = strUser + "����" + strAGV + strAction + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �����ڴ���״̬
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:������״̬:%s��.",strStatus.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	if (usAction == _AGVACT_NONE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGVδִ�ж���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGVδִ�ж���.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	if (usAction == _AGVACT_TRAFFIC)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV�޷�ֹͣ�ö���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV�޷�ֹͣ�ö���:" + strAction;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	if (byActStatus == _AGVACTSTA_FINISH || byActStatus == _AGVACTSTA_NONE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV��������ɻ�δִ��."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV��������ɻ�δִ��.������:" + strAction + "��������״̬:" + strActStatus + "��";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	pAGV->StopAction();

	strRecord = strUser + "����" + strAGV  + strCtrl + "������:" + strAction + "��������״̬:" + strActStatus + "��";
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::AGVLoad(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "���û�User��";
	std::string strCtrl = "�����ϡ�";
	std::string strAGV = "AGV";

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	std::string strAction = "";				/*!< ���� */
	std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< ���� */
	unsigned char byActStatus = 0;			/*!< ����״̬ */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// ����
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �������Զ�ģʽ
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:��ģʽ:%s��.",strMode.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �����ڴ���״̬
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:������״̬:%s��.",strStatus.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// ������
	if (pAGV->GetCargo() == 0xFF)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV������."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV������.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// ��������
	if (usAction == _AGVACT_LOAD)
	{
		// δ�������
		if (byActStatus != _AGVACTSTA_FINISH)
		{
#ifdef _AFX
			if (m_hParent)
			{
				MessageBox(m_hParent, _T("����ʧ��!AGV����ִ�����϶���."), _T("�û�����"), MB_ICONERROR);
			}
#endif // _AFX

			strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����ִ��" + strAction;
			// ���DEBUG
			DebugPrint(strRecord.c_str());
			// ��¼�û�����
			Record(strRecord);
			return false;
		}
		else
		{
			if (MessageBox(m_hParent, _T("��ִ�й����϶���,�Ƿ��ٴ�ִ��?"), _T("��������"), MB_ICONWARNING | MB_YESNO) == IDNO)
			{
				return false;
			}
		}
	}
	// �ж���������ִ��
	else if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����ִ����������."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����ִ����������.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	pAGV->Load();

	strRecord = strUser + "����" + strAGV + strCtrl + "������:" + strAction + "��������״̬:" + strActStatus + "��";
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::AGVUnload(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "���û�User��";
	std::string strCtrl = "�����ϡ�";
	std::string strAGV = "AGV";

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	std::string strAction = "";				/*!< ���� */
	std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< ���� */
	unsigned char byActStatus = 0;			/*!< ����״̬ */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// ����
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �������Զ�ģʽ
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:��ģʽ:%s��.",strMode.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �����ڴ���״̬
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:������״̬:%s��.",strStatus.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// ����
	if (pAGV->GetCargo() == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// ����
	if (usAction == _AGVACT_UNLOAD)
	{
		// δ�������
		if (byActStatus != _AGVACTSTA_FINISH)
		{
#ifdef _AFX
			if (m_hParent)
			{
				MessageBox(m_hParent, _T("����ʧ��!AGV����ִ�����϶���."), _T("�û�����"), MB_ICONERROR);
			}
#endif // _AFX

			strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����ִ��" + strAction;
			// ���DEBUG
			DebugPrint(strRecord.c_str());
			// ��¼�û�����
			Record(strRecord);
			return false;
		}
		else
		{
			if (MessageBox(m_hParent, _T("��ִ�й����϶���,�Ƿ��ٴ�ִ��?"), _T("��������"), MB_ICONWARNING | MB_YESNO) == IDNO)
			{
				return false;
			}
		}
	}
	// �ж�����δִ�����
	else if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����ִ����������."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����ִ����������.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	pAGV->Unload();

	strRecord = strUser + "����" + strAGV + strCtrl + "������:" + strAction + "��������״̬:" + strActStatus + "��";
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::AGVAction(unsigned char byAGVNo, unsigned short usActionCode)
{
	std::string strRecord = "";
	std::string strUser = "���û�User��";
	std::string strCtrl = "������������";
	std::string strAGV = "AGV";

	BufferString::std_string_format(strCtrl, "��ִ�ж���:%d��", usActionCode);

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	std::string strAction = "";				/*!< ���� */
	std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< ���� */
	unsigned char byActStatus = 0;			/*!< ����״̬ */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// ����
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �������Զ�ģʽ
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:��ģʽ:%s��.",strMode.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �����ڴ���״̬
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:������״̬:%s��.",strStatus.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// ����ִ�д˶���
	if (usAction == usActionCode)
	{
		// ����δ���
		if (byActStatus != _AGVACTSTA_FINISH)
		{
#ifdef _AFX
			if (m_hParent)
			{
				MessageBox(m_hParent, _T("����ʧ��!AGV����ִ�д˶���."), _T("�û�����"), MB_ICONERROR);
			}
#endif // _AFX

			strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����ִ��" + strAction;
			// ���DEBUG
			DebugPrint(strRecord.c_str());
			// ��¼�û�����
			Record(strRecord);
			return false;
		}
		else
		{
			if (MessageBox(m_hParent, _T("��ִ�й��˶���,�Ƿ��ٴ�ִ��?"), _T("��������"), MB_ICONWARNING | MB_YESNO) == IDNO)
			{
				return false;
			}
		}
	}
	// �ж�����δִ�����
	else if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����ִ����������."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����ִ����������.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	pAGV->Action(usAction);

	strRecord = strUser + "����" + strAGV + strCtrl + "������:" + strAction + "��������״̬:" + strActStatus + "��";
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::AGVRemove(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "���û�User��";
	std::string strCtrl = "�������ƶ���";
	std::string strAGV = "AGV";

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	std::string strAction = "";				/*!< ���� */
	std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< ���� */
	unsigned char byActStatus = 0;			/*!< ����״̬ */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// ����
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �������Զ�ģʽ
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:��ģʽ:%s��.",strMode.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �����ڴ���״̬
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:������״̬:%s��.",strStatus.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// ����ִ�ж���
	if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����ִ�ж���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����ִ�ж���:������:" + strAction + "��������״̬:" + strActStatus + "��";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	unsigned short usEndRFID = pAGV->GetEndRFID();	/*!< �յ�RFID����� */
	unsigned short usCurRFID = pAGV->GetCurRFID();	/*!< ��ǰRFID����� */

	// �յ������뵱ǰ�������Ч
	if (usEndRFID == 0 && usCurRFID == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��������Ч���ƶ�����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!��������Ч���ƶ�����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	unsigned short usMoveRFID = 0;	/*!< �ƶ�RFID����� */

	if (usEndRFID != 0)
	{
		usMoveRFID = usEndRFID;
	}
	else
	{
		usMoveRFID = usCurRFID;
	}

	BufferString::std_string_format(strAction, "�������ƶ�����������:%d��", usMoveRFID);

	pAGV->Remove(usMoveRFID);

	strRecord = strUser + "����" + strAGV + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::AGVReset(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "������ԱAdmin��";
	std::string strCtrl = "�����á�";
	std::string strAGV = "AGV";

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	//std::string strAction = "";				/*!< ���� */
	//std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	//pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< ���� */
	unsigned char byActStatus = 0;			/*!< ����״̬ */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// ����
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �������Զ�ģʽ
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:��ģʽ:%s��.",strMode.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �����ڴ���״̬
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:������״̬:%s��.",strStatus.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// ����ִ�ж���
	if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			if (MessageBox(m_hParent, _T("���û�ʹAGV����ִ�еĶ����ж�,�Ƿ�����?"), _T("��������"), MB_ICONWARNING | MB_YESNO) == IDNO)
			{
				return false;
			}
		}
#endif // _AFX
	}

	pAGV->Reset();

	strRecord = strUser + "����" + strAGV + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return false;
}

bool CService::AGVSet(unsigned char byAGVNo, unsigned short usRFIDNo, unsigned char byStatus)
{
	std::string strRecord = "";
	std::string strUser = "������ԱAdmin��";
	std::string strCtrl = "�����õ�ǰ������״̬��";
	std::string strAGV = "AGV";

	BufferString::std_string_format(strCtrl, "�����õ�ǰ������״̬��������:%d����״̬:%d��", usRFIDNo, byStatus);

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	//std::string strAction = "";				/*!< ���� */
	//std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	//pAGV->GetAction(strAction, strActStatus);

	//unsigned short usAction = 0;			/*!< ���� */
	//unsigned char byActStatus = 0;			/*!< ����״̬ */
	//pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// ����
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �������Զ�ģʽ
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:��ģʽ:%s��.",strMode.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	pAGV->ManualSet(usRFIDNo, byStatus);

	strRecord = strUser + "����" + strAGV + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::AGVTrafficPass(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "������ԱAdmin��";
	std::string strCtrl = "����ͨ����ͨ�С�";
	std::string strAGV = "AGV";

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	std::string strAction = "";				/*!< ���� */
	std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< ���� */
	unsigned char byActStatus = 0;			/*!< ����״̬ */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	BufferString::std_string_format(strCtrl, "����ͨ����ͨ�С�������:%d��", pAGV->GetCurRFID());

	// ����
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �������Զ�ģʽ

	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV���ܽ���ϵͳ�Ŀ���:��ģʽ:%s��.",strMode.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �����ڽ�ͨ����״̬
	if (pAGV->GetStatus() != _AGVSTA_TRAFFIC)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV�����ڽ�ͨ����״̬."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord,"ʧ��!AGV�����ڽ�ͨ����״̬:������״̬:%s��.",strStatus.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// ����ִ�ж���
	if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����ִ�ж���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����ִ�ж���:������:" + strAction + "��������״̬:" + strActStatus + "��";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	pAGV->TrafficPass();

	strRecord = strUser + "����" + strAGV + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::AGVHandleMode(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "������ԱAdmin��";
	std::string strCtrl = "���л��ֶ�ģʽ��";
	std::string strAGV = "AGV";

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	//std::string strAction = "";				/*!< ���� */
	//std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	//pAGV->GetAction(strAction, strActStatus);

	//unsigned short usAction = 0;			/*!< ���� */
	//unsigned char byActStatus = 0;			/*!< ����״̬ */
	//pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV�Ѵ����ֶ�ģʽ."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV�Ѵ���" + strMode;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

#ifdef _AFX
	AGVTask* pTask = (AGVTask*)pAGV->GetTask();	/*!< AGV���� */
	// ������������ִ��
	if (pTask && pTask->GetStatus() == Task::_TASKSTA_EXECUTE)
	{
		if (m_hParent)
		{
			if (MessageBox(m_hParent, _T("�л�Ϊ�ֶ�ģʽ�����ж�AGV����ִ�е�����,�Ƿ��л�?"), _T("�û�����"), MB_ICONWARNING | MB_YESNO) == IDNO)
			{
				return false;
			}
		}
	}
#endif // _AFX

	pAGV->SetMode(false);

	strRecord = strUser + "����" + strAGV + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::AGVAutoMode(unsigned char byAGVNo)
{
	std::string strRecord = "";
	std::string strUser = "������ԱAdmin��";
	std::string strCtrl = "���л��Զ�ģʽ��";
	std::string strAGV = "AGV";

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	//std::string strAction = "";				/*!< ���� */
	//std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	//pAGV->GetAction(strAction, strActStatus);

	//unsigned short usAction = 0;			/*!< ���� */
	//unsigned char byActStatus = 0;			/*!< ����״̬ */
	//pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	if (pAGV->GetMode() == true)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV�Ѵ����Զ�ģʽ."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV�Ѵ���" + strMode;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	pAGV->SetMode(true);

	strRecord = strUser + "����" + strAGV + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::AGVShutdown(unsigned char byAGVNo)
{
	std::string strRecord = "";	
	std::string strUser = "������ԱAdmin��";
	std::string strCtrl = "���ػ���";
	std::string strAGV = "AGV";

	// ��Ч��AGV���
	if (byAGVNo == 0)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �ñ�Ų�����AGV������Ϣ
	if (m_mapAGVs.find(byAGVNo) == m_mapAGVs.end())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!��Ч��AGV���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ�ܣ���Ч��AGV���:%d", byAGVNo);
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	CTCPMNavAGV* pAGV = m_mapAGVs[byAGVNo];	/*!< AGVָ�� */

	std::string strIP = "";					/*!< IP��ַ */
	std::string strDrive = "";				/*!< ���˶����� */
	std::string strType = "";				/*!< ���� */
	std::string strMode = "";				/*!< ģʽ */
	std::string strStatus = "";				/*!< ״̬ */
	std::string strAction = "";				/*!< ���� */
	std::string strActStatus = "";			/*!< ����״̬ */

	pAGV->GetIP(strIP);
	pAGV->GetDriver(strDrive);
	pAGV->GetType(strType);
	pAGV->GetMode(strMode);
	pAGV->GetStatus(strStatus);
	pAGV->GetAction(strAction, strActStatus);

	unsigned short usAction = 0;			/*!< ���� */
	unsigned char byActStatus = 0;			/*!< ����״̬ */
	pAGV->GetAction(usAction, byActStatus);

	BufferString::std_string_format(strAGV,
		"AGV�����:%d����IP:%s:%d��������:%s��������:%d����״̬:%s��",
		pAGV->GetNo(), strIP.c_str(), pAGV->GetPort(), (strDrive + strType).c_str(), pAGV->GetCurRFID(), strStatus.c_str());

	// ����
	if (pAGV->IsConnected() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �������Զ�ģʽ
	if (pAGV->GetMode() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ��!AGV���ܽ���ϵͳ�Ŀ���:��ģʽ:%s��.", strMode.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// �����ڴ���״̬
	if (pAGV->GetStatus() != _AGVSTA_WAIT)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV���ܽ���ϵͳ�Ŀ���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		BufferString::std_string_format(strRecord, "ʧ��!AGV���ܽ���ϵͳ�Ŀ���:������״̬:%s��.", strStatus.c_str());
		strRecord = strUser + "����" + strAGV + strCtrl + strRecord;
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// ����ִ�ж���
	if (usAction != _AGVACT_NONE && byActStatus == _AGVACTSTA_EXE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!AGV����ִ�ж���."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strAGV + strCtrl + "ʧ��!AGV����ִ�ж���:������:" + strAction + "��������״̬:" + strActStatus + "��";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	pAGV->Shutdown();

	strRecord = strUser + "����" + strAGV + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

#ifdef _TASK_H
AGVTask CService::GetTask(unsigned short usNo, unsigned char byLevel)
{
	return m_TaskManager.Get(usNo, byLevel);
}

bool CService::EditTask(unsigned short usNo, unsigned char byLevel, AGVTask newTask)
{
	std::string strRecord = "";					/*!< ��־�ı� */
	std::string strUser = "������ԱAdmin��";	/*!< �����û� */
	std::string strCtrl = "���޸ġ�";			/*!< ���� */
	std::string strTask = "����";				/*!< ���� */

	BufferString::std_string_format(strTask, "���������:%d�����׶κ�:%d��", usNo, byLevel);

	AGVTask task = m_TaskManager.Get(usNo, byLevel);	/*!< ����ָ�� */

	// δ���ҵ�����
	if (task.IsNull())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!δ���ҵ�����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "����" + strTask + strCtrl + "ʧ��!δ���ҵ�����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	std::string strStatus = "";	/*!< ����״̬ */

	task.GetStatus(strStatus);

	BufferString::std_string_format(strTask, "���������:%d�����׶κ�:%d������������:%d������ʼ����:%d����������:%s��������״̬:%s��",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), strStatus.c_str());

	// ���񲻷���
	if (task.GetNo() != newTask.GetNo() || task.GetLevel() != newTask.GetLevel())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!�޸�������ԭ���񲻷�."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "����" + strTask + strCtrl + "ʧ��!�޸�������ԭ���񲻷�.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	std::string strNewAction = "";		/*!, �������� */
	std::string strNewError = "";		/*!< ���쳣��Ϣ */
	newTask.GetAction(strNewAction);
	newTask.GetError(strNewError);

	// �������ݲ���
	if (task.GetAction() == strNewAction && newTask.GetTarget() == task.GetTarget() && newTask.GetBegin() == task.GetBegin())
	{
#ifdef _AFX
			if (m_hParent)
			{
				MessageBox(m_hParent, _T("����ʧ��!�޸�����δ�޸�."), _T("�û�����"), MB_ICONERROR);
			}
#endif // _AFX
		strRecord = strUser + "����" + strTask + strCtrl + "ʧ��!�޸�����δ�޸�.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

#ifdef _AFX
	if (m_hParent)
	{
		if (MessageBox(m_hParent, _T("�޸ĺ�����ִ���޸ĺ������,�Ƿ��޸�?"), _T("�û�����"), MB_ICONWARNING | MB_YESNO) != IDYES)
		{
			return false;
		}
	}
#endif // _AFX

	// *pTask = newTask;

	if (m_TaskManager.Edit(newTask) == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!���ݿ�д��ʧ��."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	strRecord = strUser + "����" + strTask + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::CancelTask(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";					/*!< ��־�ı� */
	std::string strUser = "������ԱAdmin��";	/*!< �����û� */
	std::string strCtrl = "��ȡ����";			/*!< ���� */
	std::string strTask = "����";				/*!< ���� */

	BufferString::std_string_format(strTask, "���������:%d�����׶κ�:%d��", usNo, byLevel);

	AGVTask task = m_TaskManager.Get(usNo, byLevel);	/*!< ����ָ�� */

	// δ���ҵ�����
	if (task.IsNull())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!δ���ҵ�����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "����" + strTask + strCtrl + "ʧ��!δ���ҵ�����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	std::string strStatus = "";	/*!< ����״̬ */

	task.GetStatus(strStatus);

	BufferString::std_string_format(strTask, "���������:%d�����׶κ�:%d������������:%d������ʼ����:%d����������:%s��������״̬:%s��",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), strStatus.c_str());

	if (task.GetStatus() == Task::_TASKSTA_CANCEL || task.GetStatus() == Task::_TASKSTA_FINISH)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!�޷�ȡ����״̬������."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "����" + strTask + strCtrl + "ʧ��!�޷�ȡ����״̬������.��״̬:" + strStatus + "��";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	// ִ��SQL���
	if (m_TaskManager.Cancel(usNo, byLevel) == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!���ݿ�д��ʧ��."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	strRecord = strUser + "����" + strTask + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::FinishTask(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";					/*!< ��־�ı� */
	std::string strUser = "������ԱAdmin��";	/*!< �����û� */
	std::string strCtrl = "��ǿ����ɡ�";		/*!< ���� */
	std::string strTask = "����";				/*!< ���� */

	BufferString::std_string_format(strTask, "���������:%d�����׶κ�:%d��", usNo, byLevel);

	AGVTask task = m_TaskManager.Get(usNo, byLevel);	/*!< ����ָ�� */

	// δ���ҵ�����
	if (task.IsNull())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!δ���ҵ�����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "����" + strTask + strCtrl + "ʧ��!δ���ҵ�����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	std::string strStatus = "";	/*!< ����״̬ */

	task.GetStatus(strStatus);

	BufferString::std_string_format(strTask, "���������:%d�����׶κ�:%d������������:%d������ʼ����:%d����������:%s��������״̬:%s��",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), strStatus.c_str());

#ifdef _AFX
	if (m_hParent)
	{
		if (MessageBox(m_hParent, _T("ǿ�����������ڷ���,�Ƿ�ǿ�����?"), _T("�û�����"), MB_ICONWARNING | MB_YESNO) != IDYES)
		{
			return false;
		}
	}
#endif // _AFX

	// ִ��SQL���
	if (m_TaskManager.Finish(usNo, byLevel) == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!���ݿ�д��ʧ��."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
	}

	strRecord = strUser + "����" + strTask + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::PauseTask(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";					/*!< ��־�ı� */
	std::string strUser = "������ԱAdmin��";	/*!< �����û� */
	std::string strCtrl = "����ͣ��";			/*!< ���� */
	std::string strTask = "����";				/*!< ���� */

	BufferString::std_string_format(strTask, "���������:%d�����׶κ�:%d��", usNo, byLevel);

	AGVTask task = m_TaskManager.Get(usNo, byLevel);	/*!< ����ָ�� */

	// δ���ҵ�����
	if (task.IsNull())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!δ���ҵ�����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "����" + strTask + strCtrl + "ʧ��!δ���ҵ�����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);

		return false;
	}

	std::string strStatus = "";	/*!< ����״̬ */

	task.GetStatus(strStatus);

	BufferString::std_string_format(strTask, "���������:%d�����׶κ�:%d������������:%d������ʼ����:%d����������:%s��������״̬:%s��",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), strStatus.c_str());

	/*!
	 * ��������Ƿ������ͣ
	*/
	// ����ɻ���ȡ��������
	if (task.GetStatus() == Task::_TASKSTA_FINISH || task.GetStatus() == Task::_TASKSTA_CANCEL || task.GetStatus() == Task::_TASKSTA_PAUSE)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!�޷���ͣ��״̬������."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "����" + strTask + strCtrl + "ʧ��!�޷���ͣ��״̬������.��״̬:" + strStatus + "��";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);

		return false;
	}
	
	// ִ��SQL���
	if (m_TaskManager.Pause(usNo, byLevel) == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!���ݿ�д��ʧ��."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	strRecord = strUser + "����" + strTask + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::ResetTask(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";					/*!< ��־�ı� */
	std::string strUser = "������ԱAdmin��";	/*!< �����û� */
	std::string strCtrl = "�����á�";			/*!< ���� */
	std::string strTask = "����";				/*!< ���� */

	BufferString::std_string_format(strTask, "���������:%d�����׶κ�:%d��", usNo, byLevel);

	AGVTask task = m_TaskManager.Get(usNo, byLevel);	/*!< ����ָ�� */

	// δ���ҵ�����
	if (task.IsNull())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!δ���ҵ�����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX
		strRecord = strUser + "����" + strTask + strCtrl + "ʧ��!δ���ҵ�����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);
		return false;
	}

	std::string strStatus = "";	/*!< ����״̬ */

	task.GetStatus(strStatus);

	BufferString::std_string_format(strTask, "���������:%d�����׶κ�:%d������������:%d������ʼ����:%d����������:%s��������״̬:%s��",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), strStatus.c_str());

	// ִ��SQL���
	if (m_TaskManager.Reset(usNo, byLevel) == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!���ݿ�д��ʧ��."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	strRecord = strUser + "����" + strTask + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

void CService::CancelAllTasks()
{
	std::string strRecord = "";					/*!< ��־�ı� */
	std::string strUser = "������ԱAdmin��";	/*!< �����û� */
	std::string strCtrl = "��ȡ����";			/*!< ���� */
	std::string strTask = "ȫ������";			/*!< ���� */

	// ִ��SQL���
	if (m_TaskManager.CancelAll() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!���ݿ�д��ʧ��."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		return;
	}

	strRecord = strUser + "����" + strTask + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return;
}

void CService::PauseAllTasks()
{
	std::string strRecord = "";					/*!< ��־�ı� */
	std::string strUser = "������ԱAdmin��";	/*!< �����û� */
	std::string strCtrl = "����ͣ��";			/*!< ���� */
	std::string strTask = "ȫ������";			/*!< ���� */

	// ִ��SQL���
	if (m_TaskManager.PauseAll() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!���ݿ�д��ʧ��."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		return;
	}

	strRecord = strUser + "����" + strTask + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return;
}

void CService::ResetAllTasks()
{
	std::string strRecord = "";					/*!< ��־�ı� */
	std::string strUser = "������ԱAdmin��";	/*!< �����û� */
	std::string strCtrl = "�����á�";			/*!< ���� */
	std::string strTask = "ȫ������";			/*!< ���� */

	// ִ��SQL���
	if (m_TaskManager.ResetAll() == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!���ݿ�д��ʧ��."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		return;
	}

	strRecord = strUser + "����" + strTask + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return;
}

bool CService::InsertTask(unsigned short usNo, unsigned char byLevel, AGVTask newTask)
{
	std::string strRecord = "";					/*!< ��־�ı� */
	std::string strUser = "������ԱAdmin��";	/*!< �����û� */
	std::string strCtrl = "�����롿";			/*!< ���� */
	std::string strTask = "����";				/*!< ���� */

	BufferString::std_string_format(strTask, "���������:%d�����׶κ�:%d������������:%d������ʼ����:%d����������:%s��",
		newTask.GetNo(), newTask.GetLevel(), newTask.GetTarget(), newTask.GetBegin(), newTask.GetAction().c_str());

	if (newTask.GetNo() != usNo)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!�޷���������Ų���ͬ������."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strTask + strCtrl + "ʧ��!�޷���������Ų���ͬ������.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);

		return false;
	}

	AGVTask task = m_TaskManager.Get(usNo, byLevel);	/*!< ����ָ�� */

	// δ���ҵ�����
	if (task.IsNull())
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!δ���ҵ�����."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		strRecord = strUser + "����" + strTask + strCtrl + "ʧ��!δ���ҵ�����.";
		// ���DEBUG
		DebugPrint(strRecord.c_str());
		// ��¼�û�����
		Record(strRecord);

		return false;
	}

	if (task.GetStatus() == Task::_TASKSTA_FINISH)
	{
#ifdef _AFX
		if (m_hParent)
		{
			if (MessageBox(m_hParent, _T("����������󣬵�ǰ���������.�Ƿ����?"), _T("�û�����"), MB_ICONWARNING | MB_YESNO) != IDYES)
			{
				return false;
			}
		}
#endif //_AFX
	}

	if (m_TaskManager.Insert(usNo, byLevel, newTask) == false)
	{
#ifdef _AFX
		if (m_hParent)
		{
			MessageBox(m_hParent, _T("����ʧ��!���ݿ�д��ʧ��."), _T("�û�����"), MB_ICONERROR);
		}
#endif // _AFX

		return false;
	}

	strRecord = strUser + "����" + strTask + strCtrl;
	// ���DEBUG
	DebugPrint(strRecord.c_str());
	// ��¼�û�����
	Record(strRecord);

	return true;
}

bool CService::AddTask(AGVTask newTask)
{
	return false;
}
#endif // _TASK_H
#endif // !_MAGNETICTRACK_H
#endif // !_SERVICE