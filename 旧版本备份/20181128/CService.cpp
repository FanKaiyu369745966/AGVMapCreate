#include "stdafx.h"
#include "CService.h"

#ifdef _SERVICE_H

CService::CService()
{
	m_pTcpServer = nullptr;
	m_pCA = nullptr;
	m_pArm = nullptr;
	m_pStocker = nullptr;

	m_pThreadService = nullptr;
	m_hShutdownEvent = nullptr;

	for (int i = 0; i < 5; i++)
	{
		m_strLed[i] = "";
	}

	m_unNo = 0;
	m_unRow = 0;
	m_unCol = 0;
}

CService::~CService()
{
	Close();
}

bool CService::Open()
{
	LoadConfig(L"../Data/WMSConfig.xml");

	if (m_adoDB.OnInitADOConn() == false)
	{
		TRACE(_T("���ӷ�����ʧ��.\r\n"));
		AfxMessageBox(L"���ӷ�����ʧ��.");
		return false;
	}

	if (m_pTcpServer == nullptr || m_pTcpServer->BeginListen() == false)
	{
		TRACE(_T("���������ʧ��.\r\n"));
		AfxMessageBox(L"���������ʧ��.");
		return false;
	}

	StartThread();

	OpenScreen();

	return true;
}

void CService::Close()
{
	CloseThread();

	lock_guard<mutex> lock(m_lock);

	if (m_pTcpServer)
	{
		delete m_pTcpServer;
		m_pTcpServer = nullptr;
	}

	if (m_pCA)
	{
		delete m_pCA;
		m_pCA = nullptr;
	}

	if (m_pArm)
	{
		delete m_pArm;
		m_pArm = nullptr;
	}

	for (map<unsigned int, CPipeline*>::iterator it = m_mapPipeline.begin(); it != m_mapPipeline.end();)
	{
		delete it->second;
		it = m_mapPipeline.erase(it);
	}

	if (m_pStocker)
	{
		delete m_pStocker;
		m_pStocker = nullptr;
	}

	for (list<CTcpClient*>::iterator it = m_listTcpClients.begin(); it != m_listTcpClients.end();)
	{
		delete *it;
		it = m_listTcpClients.erase(it);
	}

	CloseScreen();
}

bool CService::AutoGetOut(unsigned int unNo, unsigned int unRow, unsigned int unCol)
{
	_CARGO cargo = GetCargo(m_adoDB, unNo, unRow, unCol);

	if (cargo.IsNull())
	{
		AfxMessageBox(_T("����ʧ��!�û�λ�����ڻ���!"));
		return false;
	}

	if (GetAllWMSTask(m_adoDB).size() != 0)
	{
		AfxMessageBox(_T("����ʧ��!��ǰ����δ���!"));
		return false;
	}

	return CreateWMSTask(m_adoDB,unNo,unRow,unCol,WMSTASKTYPE_OUT);
}

bool CService::GetOut(unsigned int unNo, unsigned int unRow, unsigned int unCol)
{
	return ClearIn(m_adoDB,unNo,unRow,unCol);
}

bool CService::PutIn(unsigned int unNo, unsigned int unRow, unsigned int unCol)
{
	_CARGO cargo = GetCargo(m_adoDB, unNo, unRow, unCol);

	if (cargo.IsNull() == false)
	{
		AfxMessageBox(_T("���ʧ��!�û�λ�Ѵ��ڻ���!"));
		return false;
	}

	return RecordIn(m_adoDB,unNo,unRow,unCol,"��ש","��",1);
}

void CService::LoadConfig(wstring wstrPath)
{
	string strPath = _bstr_t(wstrPath.c_str);

	TiXmlDocument xmlDoc; /*!< XML�ĵ� */
	if (xmlDoc.LoadFile(strPath.c_str()) == false)
	{
		TRACE(_T("��ȡ������Ϣʧ��.\r\n"));
		return;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< ���ڵ� */
	if (pXmlRoot == NULL || pXmlRoot->NoChildren())
	{
		TRACE(_T("��ȡ������Ϣʧ��.\r\n"));
		return;
	}

	TiXmlElement *pXmlElemt = pXmlRoot->FirstChildElement(); /*!< �ӽڵ�*/

	while (pXmlElemt)
	{
		CString cstrElemtName(pXmlElemt->Value()); /*!< �ӽڵ����� */

		/*!
		 * ��ȡ�������Ϣ
		*/
		if (cstrElemtName == _T("Server"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< �ӽڵ����� */

			wstring wstrIP = L"";		/*!< IP��ַ */
			unsigned int unPort = 0;	/*!< �˿� */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< ��������*/
				CString cstrAttrValue(pAttr->Value()); /*!< ����ֵ */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("IP"))
				{
					wstrIP = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Port"))
				{
					unPort = _ttoi(cstrAttrValue);
				}
			}

			if (m_pTcpServer)
			{
				delete m_pTcpServer;
			}

			m_pTcpServer = new CTcpServer(unPort, wstrIP);
		}

		/*!
		 * ��ȡCA������Ϣ
		*/
		else if (cstrElemtName == _T("CA"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< �ӽڵ����� */

			unsigned int unNo = 0;		/*!< ��� */
			wstring wstrIP = L"";		/*!< IP��ַ */
			unsigned int unPort = 0;	/*!< �˿� */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< ��������*/
				CString cstrAttrValue(pAttr->Value()); /*!< ����ֵ */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					unNo = _ttoi(cstrAttrValue);
				}
				else if(cstrAttrName == _T("IP"))
				{
					wstrIP = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Port"))
				{
					unPort = _ttoi(cstrAttrValue);
				}
			}

			if (m_pCA)
			{
				delete m_pCA;
			}

			m_pCA = new CCA(unNo, wstrIP, unPort);
		}

		/*!
		 * ��ȡ��е����Ϣ
		*/
		else if (cstrElemtName == _T("Arm"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< �ӽڵ����� */

			unsigned int unNo = 0;		/*!< ��� */
			wstring wstrIP = L"";		/*!< IP��ַ */
			unsigned int unPort = 0;	/*!< �˿� */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< ��������*/
				CString cstrAttrValue(pAttr->Value()); /*!< ����ֵ */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					unNo = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("IP"))
				{
					wstrIP = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Port"))
				{
					unPort = _ttoi(cstrAttrValue);
				}
			}

			if (m_pArm)
			{
				delete m_pArm;
			}

			m_pArm = new CArm(unNo, wstrIP, unPort);
		}

		/*!
		 * ��ȡ��������Ϣ
		*/
		else if (cstrElemtName == _T("Pipeline"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< �ӽڵ����� */

			unsigned int unNo = 0;		/*!< ��� */
			wstring wstrIP = L"";		/*!< IP��ַ */
			unsigned int unPort = 0;	/*!< �˿� */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< ��������*/
				CString cstrAttrValue(pAttr->Value()); /*!< ����ֵ */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					unNo = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("IP"))
				{
					wstrIP = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Port"))
				{
					unPort = _ttoi(cstrAttrValue);
				}
			}

			if (m_mapPipeline.find(unNo) != m_mapPipeline.end())
			{
				delete m_mapPipeline[unNo];
			}

			m_mapPipeline[unNo] = new CPipeline(unNo, wstrIP, unPort);
		}

		/*!
		 * ��ȡ�Ѷ����Ϣ
		*/
		else if (cstrElemtName == _T("Stocker"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< �ӽڵ����� */

			unsigned int unNo = 0;		/*!< ��� */
			wstring wstrIP = L"";		/*!< IP��ַ */
			unsigned int unPort = 0;	/*!< �˿� */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< ��������*/
				CString cstrAttrValue(pAttr->Value()); /*!< ����ֵ */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					unNo = _ttoi(cstrAttrValue);
				}
				else if (cstrAttrName == _T("IP"))
				{
					wstrIP = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Port"))
				{
					unPort = _ttoi(cstrAttrValue);
				}
			}

			if (m_pStocker)
			{
				delete m_pStocker;
			}

			m_pStocker = new CStocker(unNo, wstrIP, unPort);
		}


		pXmlElemt = pXmlElemt->NextSiblingElement();
	}

	TRACE(_T("��ȡ������Ϣ���.\r\n"));
	return;
}

void CService::ClientConnect()
{
	if (m_pTcpServer == nullptr)
	{
		return;
	}

	SOCKET socket = m_pTcpServer->GetClient();

	if (socket == INVALID_SOCKET)
	{
		return;
	}

	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);

	//�����׽��ֻ�ȡ��ַ��Ϣ
	if (::getpeername(socket, (SOCKADDR*)&addrClient, &addrClientlen) != 0)
	{
		TRACE(_T("��SOCKET�׽����л�ȡIP��ַ��Ϣʧ�ܣ�\r\n"));
		return;
	}

	wstring wstrIp = L""; /*!< IP��ַ */
	wstrIp = _bstr_t(inet_ntoa(addrClient.sin_addr));

	unsigned int unPort = 0; /*!< �˿� */
	unPort = htons(addrClient.sin_port);

	CTcpClient *pTcpClient = new CTcpClient(wstrIp, unPort);
	pTcpClient->Connect(socket);

	/*!
	 * ��CA��������ƥ��
	*/
	if (m_pCA)
	{
		if (m_pCA->m_wstrIP == wstrIp && m_pCA->m_unPort == unPort)
		{
			m_pCA->Connect(pTcpClient);
		}
	}

	/*!
	 * ���е�ֽ���ƥ��
	*/
	if (m_pArm)
	{
		if (m_pArm->m_wstrIP == wstrIp &&(m_pArm->m_unPort == 0 || m_pArm->m_unPort == unPort))
		{
			m_pArm->Connect(pTcpClient);
		}
	}

	/*!
	 * �������߽���ƥ��
	*/
	for (map<unsigned int, CPipeline*>::iterator it = m_mapPipeline.begin(); it != m_mapPipeline.end();it++)
	{
		CPipeline *pPipeline = it->second;

		if (pPipeline->m_wstrIP == wstrIp && (pPipeline->m_unPort == 0 || pPipeline->m_unPort == unPort))
		{
			pPipeline->Connect(pTcpClient);
		}
	}

	/*!
	 * ��Ѷ������ƥ��
	*/
	if (m_pStocker)
	{
		if (m_pStocker->m_wstrIP == wstrIp && (m_pStocker->m_unPort == 0 || m_pStocker->m_unPort == unPort))
		{
			m_pStocker->Connect(pTcpClient);
		}
	}

	if (pTcpClient->GetLinkCount() == 0)
	{
		pTcpClient->Break();
		delete pTcpClient;
	}
	else
	{
		if (m_listTcpClients.max_size() == m_listTcpClients.size())
		{
			ClearFreeClient();
		}

		lock_guard<mutex> lock(m_lock);

		m_listTcpClients.push_back(pTcpClient);
	}

	return;
}

void CService::ClearFreeClient()
{
	lock_guard<mutex> lock(m_lock);

	for (list<CTcpClient*>::iterator it = m_listTcpClients.begin(); it != m_listTcpClients.end();)
	{
		CTcpClient* pTcpClient = *it;

		if (pTcpClient->GetLinkCount() != 0)
		{
			it++;
			continue;
		}

		if (pTcpClient->IsConnected())
		{
			pTcpClient->Break();
		}

		delete *it;

		it = m_listTcpClients.erase(it);
	}

	return;
}

void CService::StartThread()
{
	if (m_pThreadService != nullptr)
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

	m_pThreadService = new thread(&CService::ServiceThread, this);

	return;
}

void CService::CloseThread()
{
	if (m_pThreadService)
	{
		if (m_pThreadService->joinable())
		{
			SetEvent(m_hShutdownEvent);

			m_pThreadService->join();
		}

		delete m_pThreadService;
		m_pThreadService = nullptr;
	}

	return;
}

void CService::ServiceThread()
{
	while (true)
	{
		ClientConnect();

		ProcessTask();

		GetCallMsg();

		if (WaitForSingleObject(m_hShutdownEvent, 1000) == WAIT_OBJECT_0)
		{
			break;
		}
	}

	return;
}

void CService::ProcessTask()
{
	list<_WMSTASK> listWMS;

	listWMS = GetAllWMSTask(m_adoDB);

	for (list<_WMSTASK>::iterator it = listWMS.begin(); it != listWMS.end(); it++)
	{
		_WMSTASK& task = *it;

		if (task.m_strType == WMSTASKTYPE_IN)
		{
			ProcessTaskIn(task);
		}
		else if (task.m_strType == WMSTASKTYPE_OUT)
		{
			ProcessTaskOut(task);
		}

		/*!
		 * ÿ��ֻ����1������
		 * ��1��������ɺ�,�ٴ�����һ������
		*/
		if (task.m_unStatus != TASKIN_FINISH || task.m_unStatus != TASKOUT_FINISH)
		{
			break;
		}
	}

	return;
}

void CService::ProcessTaskOut(_WMSTASK & task)
{
	/*!
	 * ��������,��ʼ��������
	 * �������׶�1
	*/
	if (task.m_unStatus == TASKOUT_NONE)
	{
		UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL1_OUT);

		CStringA cstraText = "";
		cstraText.Format("���⿪ʼ:%d��,%d��.\r\n", task.m_unRow, task.m_unCol);
		SendToLED(cstraText.GetBuffer());
	}
	/*!
	 * �׶�1.1 ����
	 * �˽׶δ������ȡ������������ƽ̨
	 * ���ƻ�е��AGV�ƶ���ƽ̨
	 * ����Ǳ��ʽAGV����ȴ�λ
	 * ���Ʊ���ʽAGV����������1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL1_OUT)
	{
		_STOCKERTASK stockerTask = m_pStocker->GetTask();	/*!< �Ѷ������ */

		if (stockerTask.m_unRow != task.m_unRow || stockerTask.m_unCol != task.m_unCol || stockerTask.m_unAction != STOCKERACT_REMOVAL)
		{
			m_pStocker->Removal(task.m_unRow, task.m_unCol);

			CStringA cstraText = "";
			cstraText.Format("�Ѷ����ʼȡ��:%d��,%d��.\r\n", task.m_unRow, task.m_unCol);
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// �������
			if (stockerTask.m_unStatus == 0x02 && ClearIn(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol))
			{
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL1_MOVETOTABLE);

				CStringA cstraText = "";
				cstraText.Format("�Ѷ��ȡ�����:%d��,%d��.\r\n", task.m_unRow, task.m_unCol);
				SendToLED(cstraText.GetBuffer());
			}
		}

		_AGVTASK taskAGV;
		
		taskAGV = GetAGVTask(m_adoDB,AGVTYPE_A,1,AGVTASKACT_MOVE);

		if (taskAGV.IsNull())
		{
			CreateAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ��е��AGV�ƶ���ƽ̨.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

		if (taskAGV.IsNull())
		{
			CreateAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ����ȴ�λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * ����ʽAGV�ƶ���������1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		if (taskAGV.IsNull())
		{
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ����ʽAGV�ƶ���������1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * �׶�1.2 ��е��AGV�ƶ���ƽ̨
	 * �˽׶��жϻ�е��AGV�Ƿ�λ
	 * ����Ǳ��ʽAGV����ȴ�λ
	 * ���Ʊ���ʽAGV����������1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL1_MOVETOTABLE)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_MOVE);

		if (taskAGV.IsNull())
		{
			CreateAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ��е��AGV�ƶ���ƽ̨.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL1_MOVETOTABLE);

				CStringA cstraText = "��е��AGV����ƽ̨.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * Ǳ��ʽAGV�ƶ����ȴ�λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ����ȴ�λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * ����ʽAGV�ƶ���������1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// �����ڱ���ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ����ʽAGV�ƶ���������1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * �׶�1.3 ��е��AGV��ƽ̨��ȡ�߻���
	 * �˽׶��жϻ�е��AGV�����Ƿ����
	 * ����Ǳ��ʽAGV����ȴ�λ
	 * ���Ʊ���ʽAGV����������1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL1_LOAD)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_LOAD);

		if (taskAGV.IsNull())
		{
			CreateAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_LOAD);

			CStringA cstraText = "֪ͨ��е��AGV����.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL1_MOVETOUNLOAD);

				CStringA cstraText = "��е��AGV�������.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * Ǳ��ʽAGV�ƶ����ȴ�λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ����ȴ�λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * ����ʽAGV�ƶ���������1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// �����ڱ���ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ����ʽAGV�ƶ���������1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * �׶�1.3 ��е��AGV�ƶ���ж��λ
	 * �˽׶��жϻ�е��AGV�Ƿ񵽴�ж��λ
	 * ����Ǳ��ʽAGV����ȴ�λ
	 * ���Ʊ���ʽAGV����������1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL1_MOVETOUNLOAD)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		if (taskAGV.IsNull())
		{
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ��е��AGV�ƶ�������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL1_UNLOAD);

				CStringA cstraText = "��е��AGV��������λ.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * Ǳ��ʽAGV�ƶ����ȴ�λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ����ȴ�λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * ����ʽAGV�ƶ���������1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// �����ڱ���ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ����ʽAGV�ƶ���������1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * �׶�1.4 ��е��AGV���������ж��λ�ϳ���
	 * �˽׶��жϻ�е��AGV�Ƿ��������
	 * ����Ǳ��ʽAGV����ȴ�λ
	 * ���Ʊ���ʽAGV����������1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL1_UNLOAD)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_UNLOAD);

		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_UNLOAD);

			CStringA cstraText = "֪ͨ��е��AGV����.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL2_MOVETOWAIT);

				CStringA cstraText = "��е��AGV�����������.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * Ǳ��ʽAGV�ƶ����ȴ�λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ����ȴ�λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * ����ʽAGV�ƶ���������1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// �����ڱ���ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ����ʽAGV�ƶ���������1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * �׶�2.1 Ǳ��ʽAGV�ƶ����ȴ�λ,�ȴ���е��AGV�������
	 * �˽׶��ж�Ǳ��ʽAGV�Ƿ񵽴�ȴ�λ
	 * ���Ʊ���ʽAGV����������1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL2_MOVETOWAIT)
	{
		_AGVTASK taskAGV;

		/*!
		 * Ǳ��ʽAGV�ƶ����ȴ�λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 3, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ����ȴ�λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// Ǳ��ʽAGV�ƶ��������
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL2_EXCHANGE);

				CStringA cstraText = "Ǳ��ʽAGV����ȴ�λ.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * ����ʽAGV�ƶ���������1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// �����ڱ���ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ����ʽAGV�ƶ���������1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * �׶�2.3 Ǳ��ʽAGV����
	 * �˽׶��ж�Ǳ��ʽAGV�����ϳ��Ƿ����
	 * ���Ʊ���ʽAGV����������1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL2_EXCHANGE)
	{
		_AGVTASK taskAGV;

		/*!
		 * ����ʽAGV�ƶ���������1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// �����ڱ���ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ����ʽAGV�ƶ���������1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * 1��Ǳ��ʽAGV�ƶ���ж��λ
		 * 2��Ǳ��ʽAGVж��
		 * 3��Ǳ��ʽAGV�ƶ�������λ
		 * 4��Ǳ��ʽAGV����
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ�������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// Ǳ��ʽAGV�ƶ�δ���
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "Ǳ��ʽAGV��������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_UNLOAD);

		// ������Ǳ��ʽAGV����
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_UNLOAD);

			CStringA cstraText = "֪ͨǱ��ʽAGV����.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// Ǳ��ʽAGV����δ���
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "Ǳ��ʽAGV�������.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ�������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// Ǳ��ʽAGV�ƶ�δ���
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "Ǳ��ʽAGV��������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_LOAD);

		// ������Ǳ��ʽAGV����
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_LOAD);

			CStringA cstraText = "֪ͨǱ��ʽAGV����.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// Ǳ��ʽAGV�����������
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL2_MOVETOARM);

				CStringA cstraText = "Ǳ��ʽAGV����λ���.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * �׶�2.4 Ǳ��ʽAGV�ƶ����̶���е��
	 * �˽׶��ж�Ǳ��ʽAGV�Ƿ񵽴�̶���е��
	 * ���Ʊ���ʽAGV����������1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL2_MOVETOARM)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ����̶���е��.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// Ǳ��ʽAGV�ƶ��������
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL2_LOAD);

				CStringA cstraText = "Ǳ��ʽAGV����̶���е��.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * ����ʽAGV�ƶ���������1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// �����ڱ���ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ����ʽAGV�ƶ���������1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * �׶�2.5 �̶���е��������������1
	 * �˽׶��жϹ̶���е���Ƿ���������Լ�������1�Ƿ�����
	 * ���Ʊ���ʽAGV����������1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL2_LOAD)
	{
		unsigned int unAction = 0;	/*!< ��е�ֵ�ǰ���� */
		unsigned int unStatus = 0;	/*!< ��е�ֵ�ǰ״̬ */
		m_pArm->GetActionStatus(unAction, unStatus);

		// δ��ʼ����
		if (unAction != ARMACT_LOAD || unStatus == 0)
		{
			m_pArm->Load();

			CStringA cstraText = "�̶���е�ֿ�ʼ����.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		// �������
		else if (unStatus == 2)
		{
			CStringA cstraText = "�̶���е���������.\r\n";
			SendToLED(cstraText.GetBuffer());

			if (m_mapPipeline.find(1) != m_mapPipeline.end())
			{
				// ������1����
				if (m_mapPipeline[1]->GetCargoStatus())
				{
					// ��������״̬
					UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL3_MOVETOPIPELINE1);

					cstraText = "������1����.\r\n";
					SendToLED(cstraText.GetBuffer());
				}
			}
		}

		_AGVTASK taskAGV;

		/*!
		 * ����ʽAGV�ƶ���������1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// �����ڱ���ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ����ʽAGV�ƶ���������1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * �׶�3.1 ����ʽAGV�ƶ���������1
	 * �˽׶��жϱ���ʽAGV�Ƿ񵽴�������1
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL3_MOVETOPIPELINE1)
	{
		_AGVTASK taskAGV;

		/*!
		 * ����ʽAGV�ƶ���������1
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// �����ڱ���ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ����ʽAGV�ƶ���������1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// ����ʽAGV�ƶ��������
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_LEVEL3_LOADFROMPIPELINE1);

				CStringA cstraText = "����ʽAGV����������1.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * �׶�3.2 ����ʽAGV��������1�ϵĻ���ȡ��
	 * �˽׶��жϱ���ʽAGV�Ƿ��������
	*/
	else if (task.m_unStatus == TASKOUT_LEVEL3_LOADFROMPIPELINE1)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_LOAD);

		// �����ڱ���ʽAGV��������
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_LOAD);

			CStringA cstraText = "֪ͨ����ʽAGV����.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// ����ʽAGV�����������
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "����ʽAGV�������.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		if (m_mapPipeline.find(1) != m_mapPipeline.end())
		{
			// ������1����
			if (m_mapPipeline[1]->GetCargoStatus() == false)
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKOUT_FINISH);

				CStringA cstraText = "������1����.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * �������
	 * �����������
	*/
	else if (task.m_unStatus == TASKOUT_FINISH)
	{
		if (GetWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, WMSTASKTYPE_IN).IsNull())
		{
			// ���ȫ��AGV����
			if (ClearAGVTask(m_adoDB))
			{
				// �����������
				CreateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, WMSTASKTYPE_IN);
			}
		}
		else
		{
			// �����������
			ClearWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, WMSTASKTYPE_OUT);
		}

		CStringA cstraText = "";
		cstraText.Format("�������:%d��,%d��.\r\n", task.m_unRow, task.m_unCol);
		SendToLED(cstraText.GetBuffer());
	}

	return;
}

void CService::ProcessTaskIn(_WMSTASK & task)
{
	/*!
	 * ��������,��ʼ��������
	 * �������׶�1
	*/
	if (task.m_unStatus == TASKIN_NONE)
	{
		// ��������״̬
		UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL1_MOVETOPIPELINE2);

		CStringA cstraText = "";
		cstraText.Format("��⿪ʼ:%d��,%d��.\r\n", task.m_unRow, task.m_unCol);
		SendToLED(cstraText.GetBuffer());
	}
	/*!
	 * �׶�1.1 ����ʽAGV�ƶ���������2
	 * �˽׶μ�ⱳ��ʽAGV�Ƿ񵽴�������2
	 * ����Ǳ��ʽAGV�ƶ����̶���е��
	 * ���ƻ�е��AGV�ƶ���ж��λ
	*/
	else if (task.m_unStatus == TASKIN_LEVEL1_MOVETOPIPELINE2)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 8, AGVTASKACT_MOVE);

		// �����ڱ���ʽAGV�ƶ�����
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 8, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ����ʽAGV�ƶ���������2.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// ����ʽAGV�ƶ����
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL1_UNLOADTOPIPELINE2);

				CStringA cstraText = "����ʽAGV����������2.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * Ǳ��ʽAGV�ƶ����̶���е��
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV����̶���е��.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * ��е��AGV�ƶ���ж��λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// �����ڻ�е��AGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ��е��AGV����̶�����λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * �׶�1.2 ����ʽAGV������������2
	 * �˽׶μ�ⱳ��ʽAGV�Ƿ��������
	 * ����Ǳ��ʽAGV�ƶ����̶���е��
	 * ���ƻ�е��AGV�ƶ���ж��λ
	*/
	else if (task.m_unStatus == TASKIN_LEVEL1_UNLOADTOPIPELINE2)
	{
		_AGVTASK taskAGV;

		/*!
		 * Ǳ��ʽAGV�ƶ����̶���е��
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV����̶���е��.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * ��е��AGV�ƶ���ж��λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// �����ڻ�е��AGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ��е��AGV����̶�����λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * ����ʽAGV����
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 8, AGVTASKACT_UNLOAD);

		// �����ڱ���ʽAGV��������
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 8, AGVTASKACT_UNLOAD);

			CStringA cstraText = "֪ͨ����ʽAGV����.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// ����ʽAGV�������
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "����ʽAGV�������.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		if (m_mapPipeline.find(2) != m_mapPipeline.end())
		{
			// ������2����
			if (m_mapPipeline[2]->GetCargoStatus())
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL1_MOVETOPIPELINE1);

				CStringA cstraText = "������2����.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * �׶�1.3 ����ʽAGV�ƶ���������1
	 * �˽׶μ�ⱳ��ʽAGV����������1
	 * ����Ǳ��ʽAGV�ƶ����̶���е��
	 * ���ƻ�е��AGV�ƶ���ж��λ
	*/
	else if (task.m_unStatus == TASKIN_LEVEL1_MOVETOPIPELINE1)
	{
		_AGVTASK taskAGV;

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

		// �����ڱ���ʽAGV�ƶ�����
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ����ʽAGV�ƶ���������1.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// ����ʽAGV�ƶ����
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL1_PASSTOPIPELINE1);

				CStringA cstraText = "����ʽAGV����������1.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * Ǳ��ʽAGV�ƶ����̶���е��
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ����̶���е��.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * ��е��AGV�ƶ���ж��λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// �����ڻ�е��AGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ��е��AGV�ƶ�������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * �׶�1.4 ʹ������2�Ļ���ͨ������ʽAGV����������1
	 * �˽׶μ��������2�Ƿ���أ�������1�Ƿ�����
	 * ����Ǳ��ʽAGV�ƶ����̶���е��
	 * ���ƻ�е��AGV�ƶ���ж��λ
	*/
	else if (task.m_unStatus == TASKIN_LEVEL1_PASSTOPIPELINE1)
	{
		_AGVTASK taskAGV;

		/*!
		 * Ǳ��ʽAGV�ƶ����̶���е��
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ����̶���е��.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * ��е��AGV�ƶ���ж��λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// �����ڻ�е��AGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ��е��AGV�ƶ�������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * ����ʽAGVͨ��
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_PASS);

		// �����ڱ���ʽAGVͨ������
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_T, 7, AGVTASKACT_PASS);

			CStringA cstraText = "֪ͨ����ʽAGV�Ž�������1��2.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// ����ʽAGVͨ�����
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "����ʽAGV�Ž�������1��2���.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		if (m_mapPipeline.find(2) != m_mapPipeline.end())
		{
			// ������2����
			if (m_mapPipeline[2]->GetCargoStatus())
			{
				return;
			}

			CStringA cstraText = "������2����.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		if (m_mapPipeline.find(1) != m_mapPipeline.end())
		{
			// ������1����
			if (m_mapPipeline[1]->GetCargoStatus())
			{
				CStringA cstraText = "������2����.\r\n";
				SendToLED(cstraText.GetBuffer());

				taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 1, AGVTASKACT_RETURN);

				// �����ڱ���ʽAGV��������
				if (taskAGV.IsNull())
				{
					// ��������
					CreateAGVTask(m_adoDB, AGVTYPE_T, 1, AGVTASKACT_RETURN);

					CStringA cstraText = "֪ͨ����ʽAGV���ش���λ.\r\n";
					SendToLED(cstraText.GetBuffer());
				}
				else
				{
					// ��������״̬
					UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL1_UNLOADFROMPIPELINE1);
				}
			}
		}
	}
	/*!
	 * �׶�1.5 �̶���е�ֽ������������1����Ǳ��ʽAGV�ϳ���
	 * �˽׶μ���е���Ƿ�������ɣ�������1�Ƿ����,Ǳ��ʽAGV�Ƿ񵽴�̶���е��
	 * ���ƻ�е��AGV�ƶ���ж��λ
	*/
	else if (task.m_unStatus == TASKIN_LEVEL1_UNLOADFROMPIPELINE1)
	{
		_AGVTASK taskAGV;

		/*!
		 * Ǳ��ʽAGV�ƶ����̶���е��
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 6, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ����̶���е��.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// Ǳ��ʽAGV�ƶ����
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "Ǳ��ʽAGV����̶���е��.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * ��е��AGV�ƶ���ж��λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// �����ڻ�е��AGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ��е��AGV�ƶ�������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		unsigned int unAction = 0;	/*!< ��е�ֵ�ǰ���� */
		unsigned int unStatus = 0;	/*!< ��е�ֵ�ǰ״̬ */
		m_pArm->GetActionStatus(unAction, unStatus);

		// δ��ʼ����
		if (unAction != ARMACT_UNLOAD || unStatus == 0)
		{
			m_pArm->Unload();

			CStringA cstraText = "֪ͨ�̶���е������.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		// �������
		else if (unStatus == 2)
		{
			CStringA cstraText = "�̶���е���������.\r\n";
			SendToLED(cstraText.GetBuffer());

			if (m_mapPipeline.find(1) != m_mapPipeline.end())
			{
				// ������1����
				if (m_mapPipeline[1]->GetCargoStatus() == false)
				{
					// ��������״̬
					UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL2_MOVETOUNLOAD);

					CStringA cstraText = "������1����.\r\n";
					SendToLED(cstraText.GetBuffer());
				}
			}
		}
	}
	/*!
	 * �׶�2.1 Ǳ��ʽAGV�ƶ���ж����
	 * �˽׶μ��Ǳ��ʽAGV�Ƿ񵽴�ж����
	 * ���ƻ�е��AGV�ƶ���ж��λ
	*/
	else if (task.m_unStatus == TASKIN_LEVEL2_MOVETOUNLOAD)
	{
		_AGVTASK taskAGV;

		/*!
		 * Ǳ��ʽAGV�ƶ����ȴ�λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ�������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// Ǳ��ʽAGV�ƶ����
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL3_LOAD);

				CStringA cstraText = "Ǳ��ʽAGV��������λ.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}

		/*!
		 * ��е��AGV�ƶ���ж��λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// �����ڻ�е��AGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ��е��AGV�ƶ�������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}
	/*!
	 * �׶�3.1 ��е��AGV�ƶ���ж����
	 * �˽׶μ���е��AGV�Ƿ񵽴�ж����
	 * ����Ǳ��ʽAGV����
	*/
	else if (task.m_unStatus == TASKIN_LEVEL3_MOVETOLOAD)
	{
		_AGVTASK taskAGV;

		/*!
		 * ��е��AGV�ƶ���ж��λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

		// �����ڻ�е��AGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ��е��AGV�ƶ�������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// ��е��AGV�ƶ����
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL3_LOAD);

				CStringA cstraText = "��е��AGV��������λ.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * �׶�3.1 ��е��AGV���ϳ��ϵĻ���ȡ��
	 * �˽׶μ���е��AGV�Ƿ��������
	 * ����Ǳ��ʽAGV����
	*/
	else if (task.m_unStatus == TASKIN_LEVEL3_LOAD)
	{
		_AGVTASK taskAGV;

		/*!
		 * ��е��AGV����
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_LOAD);

		// �����ڻ�е��AGV����
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_A, 2, AGVTASKACT_LOAD);

			CStringA cstraText = "֪ͨ��е��AGV����.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// ��е��AGV�������
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL3_MOVETOTABLE);

				CStringA cstraText = "��е��AGV�������.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * �׶�3.2 ��е��AGV�ƶ���ƽ̨
	 * �˽׶μ���е��AGV�Ƿ񵽴�ƽ̨
	 * ����Ǳ��ʽAGV����
	*/
	else if (task.m_unStatus == TASKIN_LEVEL3_MOVETOTABLE)
	{
		_AGVTASK taskAGV;

		/*!
		 * ��е��AGV�ƶ���ƽ̨
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_MOVE);

		// �����ڻ�е��AGV�ƶ���ƽ̨
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨ��е��AGV�ƶ�������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// ��е��AGV�ƶ����
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL3_UNLOAD);

				CStringA cstraText = "��е��AGV��������λ.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * �׶�3.3 ��е��AGV���������ƽ̨��
	 * �˽׶μ���е��AGV�Ƿ��������
	 * ����Ǳ��ʽAGV����
	*/
	else if (task.m_unStatus == TASKIN_LEVEL3_UNLOAD)
	{
		_AGVTASK taskAGV;

		/*!
		 * ��е��AGV����
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_UNLOAD);

		// �����ڻ�е��AGV����
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_A, 1, AGVTASKACT_UNLOAD);

			CStringA cstraText = "֪ͨ��е��AGV����.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// ��е��AGV�������
			if (taskAGV.m_strStatus == AGVTASKSTA_FINISH)
			{
				// ��������״̬
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_LEVEL4_IN);

				CStringA cstraText = "��е��AGV�������.\r\n";
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	/*!
	 * �׶�4.1 �Ѷ����ƽ̨�ϵĻ����������
	 * �˽׶μ��Ѷ���Ƿ�������
	 * ����Ǳ��ʽAGV����
	*/
	else if (task.m_unStatus == TASKIN_LEVEL4_IN)
	{
		_STOCKERTASK stockerTask = m_pStocker->GetTask();	/*!< �Ѷ������ */

		if (stockerTask.m_unRow != task.m_unRow || stockerTask.m_unCol != task.m_unCol || stockerTask.m_unAction != STOCKERACT_WAREHOUSING)
		{
			m_pStocker->Warehousing(task.m_unRow, task.m_unCol);

			CStringA cstraText = "";
			cstraText.Format("�Ѷ����ʼ���:%d��,%d��.\r\n", task.m_unRow, task.m_unCol);
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			// ������
			if (stockerTask.m_unStatus == 0x02 && RecordIn(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol,"��ש","��",1))
			{
				UpdateWMSTask(m_adoDB, task.m_unNo, task.m_unRow, task.m_unCol, task.m_strType, TASKIN_FINISH);

				CStringA cstraText = "";
				cstraText.Format("�Ѷ��������:%d��,%d��.\r\n", task.m_unRow, task.m_unCol);
				SendToLED(cstraText.GetBuffer());
			}
		}
	}
	else if (task.m_unStatus == TASKIN_FINISH)
	{
		_AGVTASK taskAGV;

		/*!
		 * ����ʽAGV���ش���λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_T, 1, AGVTASKACT_RETURN);

		// ���ڱ���ʽAGV����
		if (taskAGV.IsNull() == false)
		{
			// ����ʽAGV����δ���
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "����ʽAGV�������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		/*!
		 * Ǳ��ʽAGV���ش���λ
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 2, AGVTASKACT_RETURN);

		// ����Ǳ��ʽAGV����
		if (taskAGV.IsNull() == false)
		{
			// Ǳ��ʽAGV����δ���
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "Ǳ��ʽAGV�������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		if (ClearAGVTask(m_adoDB))
		{
			ClearAllWMSTask(m_adoDB);
		}

		CStringA cstraText = "";
		cstraText.Format("������:%d��,%d��.\r\n", task.m_unRow, task.m_unCol);
		SendToLED(cstraText.GetBuffer());
	}

	_AGVTASK taskAGV;

	taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_MOVE);

	// �ƶ���ж��λ���񲻴��ڻ�����δ���
	if (taskAGV.IsNull() || taskAGV.m_strStatus != AGVTASKSTA_FINISH)
	{
		return;
	}
	else
	{
		/*!
		 * Ǳ��ʽAGV��������
		*/
		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_UNLOAD);

		// ������Ǳ��ʽAGV����
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 4, AGVTASKACT_UNLOAD);

			CStringA cstraText = "֪ͨǱ��ʽAGV����.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "Ǳ��ʽAGV�������.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_MOVE);

		// ������Ǳ��ʽAGV�ƶ�
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_MOVE);

			CStringA cstraText = "֪ͨǱ��ʽAGV�ƶ�������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "Ǳ��ʽAGV��������λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_LOAD);

		// ������Ǳ��ʽAGV����
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 5, AGVTASKACT_LOAD);

			CStringA cstraText = "֪ͨǱ��ʽAGV����.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
		else
		{
			if (taskAGV.m_strStatus != AGVTASKSTA_FINISH)
			{
				return;
			}

			CStringA cstraText = "Ǳ��ʽAGV�����������.\r\n";
			SendToLED(cstraText.GetBuffer());
		}

		taskAGV = GetAGVTask(m_adoDB, AGVTYPE_S, 2, AGVTASKACT_RETURN);

		// ������Ǳ��ʽAGV����
		if (taskAGV.IsNull())
		{
			// ��������
			CreateAGVTask(m_adoDB, AGVTYPE_S, 2, AGVTASKACT_RETURN);


			CStringA cstraText = "֪ͨǱ��ʽAGV���ش���λ.\r\n";
			SendToLED(cstraText.GetBuffer());
		}
	}

	return;
}

void CService::OpenScreen()
{
	if (!m_led.OpenScreen())
	{
		AfxMessageBox(_T("����ʧ��"));
	}

	return;
}

void CService::CloseScreen()
{
	if (!m_led.CloseScreen())
	{
		AfxMessageBox(_T("����ʧ��"));
	}

	return;
}

void CService::SendToLED(string strText)
{
	// �������ͬ���ı�
	for (int i = 0; i < 10; i++)
	{
		if (strText == m_strLed[i])
		{
			return;
		}
	}

	for (int i = 0; i < 9; i++)
	{
		m_strLed[i + 1] = m_strLed[i];
	}

	m_strLed[0] = strText;

	if (!m_led.Connect())
	{
		//AfxMessageBox(_T("��������ʧ��"));
		return;
	}

	string strOutput = "";

	for (int i = 0; i < 10; i++)
	{
		strOutput += m_strLed[i];
	}

	USES_CONVERSION;
	if (!m_led.SendText((char*)strOutput.c_str()))
	{
		//AfxMessageBox(_T("�����ı�ʧ��"));
	}

	if (!m_led.Disconnect())
	{
		//AfxMessageBox(_T("�Ͽ�����ʧ��"));
	}

	return;
}

void CService::GetCallMsg()
{
	string strMsg = CCA::GetRecvMsg();

	if (strMsg == "")
	{
		return;
	}

	if (strstr(strMsg.c_str(), "����"))
	{
		GetOut(m_unNo,m_unRow,m_unCol);
	}

	return;
}

#endif // !_SERVICE_H