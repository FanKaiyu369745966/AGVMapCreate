#include "stdafx.h"
#include "CTaskManager.h"

#ifdef _TASK_H

CTaskManager::CTaskManager()
{
	m_pTheadUpdate = nullptr;
}

CTaskManager::~CTaskManager()
{
	Clear();
}

bool CTaskManager::OpenManager(const ADOConn ado)
{
	m_ado = ado;

	if (m_ado.OnInitADOConn() == false)
	{
		return false;
	}

	StartUpdateThread();

	return true;
}

bool CTaskManager::OpenManager(const ADOConn ado, HWND hParent)
{
	if (hParent == nullptr)
	{
		DebugPrint("��Ч�ĸ����ھ��");
		return false;
	}

	m_hParent = hParent;

	return OpenManager(ado);
}

void CTaskManager::CloseManager()
{
	EndUpdateThread();

	return;
}

void CTaskManager::StartUpdateThread()
{
	EndUpdateThread();

	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(nullptr, true, false, nullptr);
	}

	m_pTheadUpdate = new std::thread(&CTaskManager::UpdateThread, this);

	// ��¼��־
	Record("��ϵͳSystem�������������������.");
	// �������
	DebugPrint("AGVContorlSystem/CTaskManager/StartUpdateThread ������������߳�.");

	return;
}

void CTaskManager::EndUpdateThread()
{
	if (m_pTheadUpdate)
	{
		if (m_pTheadUpdate->joinable())
		{
			SetEvent(m_hShutdownEvent);

			m_pTheadUpdate->join();
		}

		delete m_pTheadUpdate;
		m_pTheadUpdate = nullptr;

		// ��¼��־
		Record("��ϵͳSystem��������������رա�.");
		// �������
		DebugPrint("AGVContorlSystem/CTaskManager/StartUpdateThread �ر���������߳�.");
	}

	return;
}

void CTaskManager::UpdateThread()
{
	ADOConn ado = m_ado;		/*!< ADO���� */
	_RecordsetPtr pRecordset = nullptr; /*!< ����� */
	std::string strSql = "";	/*!< SQL ��� */

	BufferString::std_string_format(strSql, "select * from [%s] order by [%s]", \
		Task::g_taskTable.m_strTable.c_str(),Task::g_taskTable.m_strPublish.c_str());

	while (true)
	{
		// ��ȡ�����
		pRecordset =  ado.GetRecordSet(_bstr_t(strSql.c_str()));

		// �޽��
		if (pRecordset == nullptr || pRecordset->adoEOF)
		{
			continue;
		}

		while (!pRecordset->adoEOF)
		{
			AGVTask agvTask;

			_variant_t var;	/*!< ֵ */

			// ��ȡ�����
			var = pRecordset->GetCollect(Task::g_taskTable.m_strNo.c_str());
			// ֵ��Ϊ��
			if (var.vt != VT_NULL)
			{
				// ��Ч�������
				if (agvTask.SetNo((unsigned short)var) == false)
				{
					continue;
				}
			}

			// ��ȡ�׶κ�
			var = pRecordset->GetCollect(Task::g_taskTable.m_strLevel.c_str());
			// ֵ��Ϊ��
			if (var.vt != VT_NULL)
			{
				// ��Ч�Ľ׶κ�
				if (agvTask.SetLevel((unsigned char)var) == false)
				{
					continue;
				}
			}

			// ��ȡ��������
			var = pRecordset->GetCollect(Task::g_taskTable.m_strTarget.c_str());
			// ֵ��Ϊ��
			if (var.vt != VT_NULL)
			{
				agvTask.SetTarget((unsigned short)var);
			}

			// ��ȡ��ʼ����
			var = pRecordset->GetCollect(Task::g_taskTable.m_strBegin.c_str());
			// ֵ��Ϊ��
			if (var.vt != VT_NULL)
			{
				agvTask.SetBegin((unsigned short)var);
			}

			// ��ȡ������
			var = pRecordset->GetCollect(Task::g_taskTable.m_strAction.c_str());
			// ֵ��Ϊ��
			if (var.vt != VT_NULL)
			{
				agvTask.SetAction((char*)_bstr_t(var));
			}

			// ��ȡ����״̬
			var = pRecordset->GetCollect(Task::g_taskTable.m_strStatus.c_str());
			// ֵ��Ϊ��
			if (var.vt != VT_NULL)
			{
				agvTask.SetStatus((unsigned char)var);
			}

			// ��ȡ�쳣��Ϣ
			var = pRecordset->GetCollect(Task::g_taskTable.m_strError.c_str());
			// ֵ��Ϊ��
			if (var.vt != VT_NULL)
			{
				agvTask.SetError((char*)_bstr_t(var));
			}

			if (UpdateTask(agvTask) == false)
			{
				continue;
			}

			AGVTask* pTask = GetTask(agvTask.GetNo(), agvTask.GetLevel());

			if (pTask == nullptr)
			{
				continue;
			}

#ifdef _AFX
			if (m_hParent)
			{
				::PostMessage(m_hParent, WM_UPDATE_TASK, (WPARAM)pTask, 0);
			}
#endif // _AFX
		}

		if (WaitForSingleObject(m_hShutdownEvent, 100) == WAIT_OBJECT_0)
		{
			break;
		}
	}

	return;
}

bool CTaskManager::PublishTask(AGVTask task)
{
	unsigned short usNo = task.GetNo();

	if (task.GetNo() == 0)
	{
		return false;
	}

	if (m_mapTaskList.find(usNo) == m_mapTaskList.end())
	{
		m_mapTaskList[usNo] = new AGVTaskArr(usNo);
	}

	return m_mapTaskList[usNo]->AddTask(task);
}

bool CTaskManager::Add(AGVTask newTask)
{
	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		// insert into ����(�����,�׶κ�,��������,��ʼ����,������,����״̬,ִ����,�쳣��Ϣ,����ʱ��) values(%d,%d,%d,%d,%s,%d,%d,%s,getdate())
		"insert into [%s]([%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]) values(%d,%d,%d,%d,'%s',%d,%d,'%s',getdate());",
		// ����								  ������ֶ�						 �׶κ��ֶ�
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strNo.c_str(), Task::g_taskTable.m_strLevel.c_str(),
		//���������ֶ�							��ʼ�����ֶ�						�������ֶ�
		Task::g_taskTable.m_strTarget.c_str(), Task::g_taskTable.m_strBegin.c_str(), Task::g_taskTable.m_strAction.c_str(),
		//����״̬�ֶ�							ִ�����ֶ�								�쳣��Ϣ�ֶ�
		Task::g_taskTable.m_strStatus.c_str(), Task::g_taskTable.m_strExecuter.c_str(), Task::g_taskTable.m_strError.c_str(),
		//����ʱ��
		Task::g_taskTable.m_strPublish,
		//�����		�׶κ�			��������			��ʼ����		������				����״̬
		newTask.GetNo(), newTask.GetLevel(), newTask.GetTarget(), newTask.GetBegin(), newTask.GetAction().c_str(), newTask.GetStatus(),
		//ִ����			�쳣��Ϣ
		newTask.GetExecuter(), newTask.GetError().c_str()
	);

	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		// insert into ����(�����,�׶κ�,��������,��ʼ����,������,����״̬,ִ����,�쳣��Ϣ,����ʱ��) values(%d,%d,%d,%d,%s,%d,%d,%s,getdate())
		"%s insert into [%s]([%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]) values(%d,%d,%d,%d,'%s',%d,%d,'%s',getdate());",
		//				����								  ������ֶ�						 �׶κ��ֶ�
		strSql.c_str(),Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strNo.c_str(), Task::g_taskTable.m_strLevel.c_str(),
		//���������ֶ�							��ʼ�����ֶ�						�������ֶ�
		Task::g_taskTable.m_strTarget.c_str(), Task::g_taskTable.m_strBegin.c_str(), Task::g_taskTable.m_strAction.c_str(),
		//����״̬�ֶ�							ִ�����ֶ�								�쳣��Ϣ�ֶ�
		Task::g_taskTable.m_strStatus.c_str(), Task::g_taskTable.m_strExecuter.c_str(), Task::g_taskTable.m_strError.c_str(),
		//����ʱ��
		Task::g_taskTable.m_strPublish,
		//�����		�׶κ�					��������			��ʼ����				������				����״̬
		newTask.GetNo(), newTask.GetLevel() + 1, newTask.GetTarget(), newTask.GetTarget(), _TASKACTION_A_END, Task::_TASKSTA_NONE,
		//ִ����				�쳣��Ϣ
		newTask.GetExecuter(), _TASKERROR_A_NONE
	);

	return true;
}

unsigned short CTaskManager::GetCurMaxNo()
{
	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		// select max(������ֶ�) as MaxValue form ����
		"select max([%s]) as MaxValue from [%s]",
		//������ֶ�						����
		Task::g_taskTable.m_strNo.c_str(), Task::g_taskTable.m_strTable.c_str()
	);

	_RecordsetPtr pRecordset = m_ado.GetRecordSet(_bstr_t(strSql.c_str())); /*!< ����� */

	if (pRecordset == nullptr || pRecordset->adoEOF)
	{
		return 0;
	}

	unsigned short usMaxNo = 0;	/*!< �������� */

	_variant_t var;	/*!< ֵ */

	// ��ȡ��������
	var = pRecordset->GetCollect(Task::g_taskTable.m_strNo.c_str());
	// ֵ��Ϊ��
	if (var.vt != VT_NULL)
	{
		usMaxNo = (unsigned short)var;
	}

	if (usMaxNo == UINT16_MAX)
	{
		usMaxNo = 0;
	}

	return usMaxNo;
}

AGVTask* CTaskManager::GetTask(unsigned short usNo, unsigned char byLevel)
{
	AGVTaskArr* pTaskArr = GetTaskArray(usNo);

	if (pTaskArr == nullptr)
	{
		return nullptr;
	}

	if (byLevel == 0)
	{
		return nullptr;
	}

	std::map<unsigned char,AGVTask*>& mapTask = pTaskArr->GetTaskList();

	if (mapTask.find(byLevel) == mapTask.end())
	{
		return nullptr;
	}

	return mapTask[byLevel];
}

AGVTask CTaskManager::Get(unsigned short usNo, unsigned char byLevel)
{
	AGVTask *pTask = GetTask(usNo, byLevel);

	return *pTask;
}

AGVTaskArr CTaskManager::GetArray(unsigned short usNo)
{
	if (usNo == 0)
	{
		return AGVTaskArr();
	}

	if (m_mapTaskList.find(usNo) == m_mapTaskList.end())
	{
		return AGVTaskArr();
	}

	return *m_mapTaskList[usNo];
}

std::map<unsigned short, AGVTaskArr> CTaskManager::GetList()
{
	// TODO: �ڴ˴����� return ���
	std::map<unsigned short, AGVTaskArr> list;

	for (std::map<unsigned short, AGVTaskArr*>::iterator it = m_mapTaskList.begin(); it != m_mapTaskList.end(); ++it)
	{
		list[it->first] = *(it->second);
	}

	return list;
}

bool CTaskManager::Publish(AGVTask task)
{
	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		// insert into ����(�����,�׶κ�,��������,��ʼ����,������,����״̬,ִ����,�쳣��Ϣ,����ʱ��) values(%d,%d,%d,%d,%s,%d,%d,%s,getdate())
		"insert into [%s]([%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]) values(%d,%d,%d,%d,'%s',%d,%d,'%s',getdate());",
		// ����								  ������ֶ�						 �׶κ��ֶ�
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strNo.c_str(), Task::g_taskTable.m_strLevel.c_str(),
		//���������ֶ�							��ʼ�����ֶ�						�������ֶ�
		Task::g_taskTable.m_strTarget.c_str(), Task::g_taskTable.m_strBegin.c_str(), Task::g_taskTable.m_strAction.c_str(),
		//����״̬�ֶ�							ִ�����ֶ�								�쳣��Ϣ�ֶ�
		Task::g_taskTable.m_strStatus.c_str(), Task::g_taskTable.m_strExecuter.c_str(), Task::g_taskTable.m_strError.c_str(),
		//����ʱ��
		Task::g_taskTable.m_strPublish,
		//�����		�׶κ�			��������			��ʼ����		������				����״̬
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), task.GetStatus(),
		//ִ����			�쳣��Ϣ
		task.GetExecuter(),task.GetError().c_str()
		);

	// ִ��SQL���
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// �������
		DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());
		return false;
	}

	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"��ϵͳSystem���������������:%d�����׶κ�:%d������������:%d������ʼ����:%d����������:%s��������״̬:%d����ִ����:%s��",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), task.GetStatus(), task.GetExecuter()
	);

	// ��¼��־
	Record(strRecord);
	// �������
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Update(AGVTask task)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"��ϵͳSystem���������������:%d�����׶κ�:%d������������:%d������ʼ����:%d����������:%s��������״̬:%d����ִ����:%s�����쳣��Ϣ:%s��",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), task.GetStatus(), task.GetExecuter(),task.GetError().c_str()
	);

	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		// update ���� set ����״̬=%d,ִ����=%d,�쳣��Ϣ=%s
		"update [%s] set [%s]=%d,[%s]=%d,[%s]='%s'",
		// ����									����״̬�ֶ�							����״̬
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), task.GetStatus(),
		//ִ�����ֶ�								ִ����				�쳣��Ϣ�ֶ�
		Task::g_taskTable.m_strExecuter.c_str(), task.GetExecuter(), Task::g_taskTable.m_strError.c_str(),
		// �쳣��Ϣ
		task.GetError().c_str()
	);

	// �������
	if (task.GetStatus() == Task::_TASKSTA_FINISH)
	{
		// �ϳ�SQL���
		BufferString::std_string_format(strSql,
			"%s,[%s]=getdate()",
			//					���ʱ���ֶ�
			strSql.c_str(), Task::g_taskTable.m_strFinish.c_str()
		);
	}

	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		"%s where [%s]=%d and [%s]=%d;",
		//				������ֶ�							�����			�׶κ��ֶ�
		strSql.c_str(), Task::g_taskTable.m_strNo.c_str(), task.GetNo(), Task::g_taskTable.m_strLevel.c_str(),
		//�׶κ�
		task.GetLevel()
	);

	// ִ��SQL���
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// �������
		DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

		if (Publish(task) == false)
		{
			strRecord += "ʧ��!";

			// ��¼��־
			Record(strRecord);
			// �������
			DebugPrint(strRecord.c_str());

			return false;
		}
		else
		{
			return true;
		}
	}

	// ��¼��־
	Record(strRecord);
	// �������
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Delete(unsigned short usNo)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"��ϵͳSystem��ɾ�����������:%d��",
		usNo
	);

	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		// insert into ��ʷ���� select * form ���� where ����.�����=%d;delete from ���� where �����=%ds
		"insert into [%s] select * from [%s] where [%s].[%s]=%d;delete from [%s] where [%s]=%d;",
		// ��ʷ����									����								����
		Task::g_taskTable.m_strHisTable.c_str(), Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strTable.c_str(),
		// ������ֶ�						�����	����								������ֶ�							�����
		Task::g_taskTable.m_strNo.c_str(), usNo, Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strNo.c_str(), usNo
	);

	// ִ��SQL���
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// �������
		DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

		strRecord += "ʧ��!";

		// ��¼��־
		Record(strRecord);
		// �������
		DebugPrint(strRecord.c_str());

		return false;
	}

	DeleteTask(usNo);

	// ��¼��־
	Record(strRecord);
	// �������
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Edit(AGVTask task)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"��ϵͳSystem���༭���������:%d�����׶κ�:%d������������:%d������ʼ����:%d����������:%s��������״̬:%d����ִ����:%s�����쳣��Ϣ:%s��",
		task.GetNo(), task.GetLevel(), task.GetTarget(), task.GetBegin(), task.GetAction().c_str(), task.GetStatus(), task.GetExecuter(), task.GetError().c_str()
	);

	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		// update ���� set ��������=%d,��ʼ����=%d,������=%s,����״̬=%d,ִ����=%d,�쳣��Ϣ=%s
		"update [%s] set [%s]=%d,[%s]=%d,[%s]='%s',[%s]=%d,[%s]=%d,[%s]='%s'",
		// ����									���������ֶ�							��������
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strTarget.c_str(), task.GetTarget(),
		// ��ʼ�����ֶ�							��ʼ����		�������ֶ�						������
		Task::g_taskTable.m_strBegin.c_str(), task.GetBegin(), Task::g_taskTable.m_strAction.c_str(),task.GetAction().c_str(),
		// ����״̬�ֶ�							����״̬		ִ�����ֶ�									ִ����
		Task::g_taskTable.m_strStatus.c_str(), task.GetStatus(),Task::g_taskTable.m_strExecuter.c_str(), task.GetExecuter(),
		// �쳣��Ϣ�ֶ�							�쳣��Ϣ
		Task::g_taskTable.m_strError.c_str(), task.GetError().c_str()
	);

	// �������
	if (task.GetStatus() == Task::_TASKSTA_FINISH)
	{
		// �ϳ�SQL���
		BufferString::std_string_format(strSql,
			"%s,[%s]=getdate()",
			//					���ʱ���ֶ�
			strSql.c_str(), Task::g_taskTable.m_strFinish.c_str()
		);
	}

	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		"%s where [%s]=%d and [%s]=%d;",
		//				������ֶ�							�����			�׶κ��ֶ�
		strSql.c_str(), Task::g_taskTable.m_strNo.c_str(), task.GetNo(), Task::g_taskTable.m_strLevel.c_str(),
		//�׶κ�
		task.GetLevel()
	);

	// ִ��SQL���
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// �������
		DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

		strRecord += "ʧ��!";

		// ��¼��־
		Record(strRecord);
		// �������
		DebugPrint(strRecord.c_str());
		
		return false;
	}

	// ��¼��־
	Record(strRecord);
	// �������
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Cancel(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"��ϵͳSystem��ȡ�����������:%d�����׶κ�:%d��",
		usNo, byLevel
	);

	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		//update ���� set ״̬�ֶ�=ȡ�� where ������ֶ�=%d and �׶κ��ֶ�=%d and ״̬�ֶ� not int(��ɣ�ȡ��)
		"update [%s] set [%s]=%d where [%s]=%d and [%s]=%d and [%s] not in(%d,%d);",
		// ����									״̬�ֶ�							ȡ��״̬
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_CANCEL,
		//������ֶ�						�����	�׶κ��ֶ�							�׶κ�
		Task::g_taskTable.m_strNo.c_str(), usNo, Task::g_taskTable.m_strLevel.c_str(), byLevel,
		//״̬�ֶ�								���״̬				ȡ��״̬
		Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_FINISH, Task::_TASKSTA_CANCEL
	);

	// ִ��SQL���
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// �������
		DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

		strRecord += "ʧ��!";

		// ��¼��־
		Record(strRecord);
		// �������
		DebugPrint(strRecord.c_str());

		return false;
	}

	// ��¼��־
	Record(strRecord);
	// �������
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Pause(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"��ϵͳSystem����ͣ���������:%d�����׶κ�:%d��",
		usNo, byLevel
	);

	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		//update ���� set ״̬�ֶ�=��ͣ where ������ֶ�=%d and �׶κ��ֶ�=%d and ״̬�ֶ� not int(��ɣ�ȡ��)
		"update [%s] set [%s]=%d where [%s]=%d and [%s]=%d and [%s] not in(%d,%d,%d);",
		// ����									״̬�ֶ�							��ͣ״̬
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_PAUSE,
		//������ֶ�						�����	�׶κ��ֶ�							�׶κ�
		Task::g_taskTable.m_strNo.c_str(), usNo, Task::g_taskTable.m_strLevel.c_str(), byLevel,
		//״̬�ֶ�								���״̬				ȡ��״̬			��ͣ״̬
		Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_FINISH, Task::_TASKSTA_CANCEL, Task::_TASKSTA_PAUSE
	);

	// ִ��SQL���
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// �������
		DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

		strRecord += "ʧ��!";

		// ��¼��־
		Record(strRecord);
		// �������
		DebugPrint(strRecord.c_str());

		return false;
	}

	// ��¼��־
	Record(strRecord);
	// �������
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Finish(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"��ϵͳSystem��ǿ��������������:%d�����׶κ�:%d��",
		usNo, byLevel
	);

	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		//update ���� set ״̬�ֶ�=��� where ������ֶ�=%d and �׶κ��ֶ�=%d and ״̬�ֶ� not int(���)
		"update [%s] set [%s]=%d where [%s]=%d and [%s]=%d and [%s] not in(%d);",
		// ����									״̬�ֶ�							���״̬
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_FINISH,
		//������ֶ�						�����	�׶κ��ֶ�							�׶κ�
		Task::g_taskTable.m_strNo.c_str(), usNo, Task::g_taskTable.m_strLevel.c_str(), byLevel,
		//״̬�ֶ�								���״̬
		Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_FINISH
	);

	// ִ��SQL���
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// �������
		DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

		strRecord += "ʧ��!";

		// ��¼��־
		Record(strRecord);
		// �������
		DebugPrint(strRecord.c_str());

		return false;
	}

	// ��¼��־
	Record(strRecord);
	// �������
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Reset(unsigned short usNo, unsigned char byLevel)
{
	std::string strRecord = "";
	BufferString::std_string_format(strRecord,
		"��ϵͳSystem���������������:%d�����׶κ�:%d��",
		usNo, byLevel
	);

	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		//update ���� set ״̬�ֶ�=δִ�� where ������ֶ�=%d and �׶κ��ֶ�>=%d
		"update [%s] set [%s]=%d where [%s]=%d and [%s]>=%d;",
		// ����									״̬�ֶ�							����״̬
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_NONE,
		//������ֶ�						�����	�׶κ��ֶ�							�׶κ�
		Task::g_taskTable.m_strNo.c_str(), usNo, Task::g_taskTable.m_strLevel.c_str(), byLevel
	);

	// ִ��SQL���
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// �������
		DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

		strRecord += "ʧ��!";

		// ��¼��־
		Record(strRecord);
		// �������
		DebugPrint(strRecord.c_str());

		return false;
	}

	// ��¼��־
	Record(strRecord);
	// �������
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::CancelAll()
{
	std::string strRecord = "��ϵͳSystem��ȡ��ȫ������";

	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		//update ���� set ״̬�ֶ�=ȡ�� where ״̬�ֶ� not in (ȡ��,���)
		"update [%s] set [%s]=%d where [%s] not in(%d,%d);",
		// ����									״̬�ֶ�							����״̬
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_CANCEL,
		//״̬�ֶ�								ȡ��					���
		Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_CANCEL,Task::_TASKSTA_FINISH
	);

	// ִ��SQL���
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// �������
		DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

		strRecord += "ʧ��!";

		// ��¼��־
		Record(strRecord);
		// �������
		DebugPrint(strRecord.c_str());

		return false;
	}

	// ��¼��־
	Record(strRecord);
	// �������
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::PauseAll()
{
	std::string strRecord = "��ϵͳSystem����ͣȫ������";

	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		//update ���� set ״̬�ֶ�=��ͣ where ״̬�ֶ� not in (ȡ��,���,��ͣ)
		"update [%s] set [%s]=%d where [%s] not in(%d,%d,%d);",
		// ����									״̬�ֶ�							��ͣ״̬
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_PAUSE,
		//״̬�ֶ�								ȡ��					���				��ͣ
		Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_CANCEL, Task::_TASKSTA_FINISH,Task::_TASKSTA_PAUSE
	);

	// ִ��SQL���
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// �������
		DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

		strRecord += "ʧ��!";

		// ��¼��־
		Record(strRecord);
		// �������
		DebugPrint(strRecord.c_str());

		return false;
	}

	// ��¼��־
	Record(strRecord);
	// �������
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::ResetAll()
{
	std::string strRecord = "��ϵͳSystem������ȫ������";

	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		//update ���� set ״̬�ֶ�=����
		"update [%s] set [%s]=%d;",
		// ����									״̬�ֶ�							����״̬
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_NONE
	);

	// ִ��SQL���
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// �������
		DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

		strRecord += "ʧ��!";

		// ��¼��־
		Record(strRecord);
		// �������
		DebugPrint(strRecord.c_str());

		return false;
	}

	// ��¼��־
	Record(strRecord);
	// �������
	DebugPrint(strRecord.c_str());

	return true;
}

bool CTaskManager::Insert(unsigned short usNo, unsigned char byLevel, AGVTask newTask)
{
	std::string strRecord = "";					/*!< ��־�ı� */

	BufferString::std_string_format(strRecord, "��ϵͳSystem���������������:%d�����׶κ�:%d������������:%d������ʼ����:%d����������:%s��",
		newTask.GetNo(), newTask.GetLevel(), newTask.GetTarget(), newTask.GetBegin(), newTask.GetAction().c_str());

	std::string strSql = ""; /*!< SQL��� */
	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		//update ���� set �׶κ�=�׶κ�+1 where �����=%d and �׶κ�>=%d;
		"update [%s] set [%s]=[%s]+1 where [%s]=%d and [%s]>=%d;",
		// ����									�׶κ��ֶ�							�׶κ�		������ֶ�
		Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strLevel.c_str(), byLevel, Task::g_taskTable.m_strNo.c_str(),
		//�����	�׶κ��ֶ�						�׶κ�	
		usNo, Task::g_taskTable.m_strLevel.c_str(), byLevel
	);

	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		//update ���� set ״̬=δִ�� where �����=%d and �׶κ�>=%d and ״̬ not in (ȡ��,��ͣ)
		"%s update[%s] set [%s]=%d where [%s]=%d and [%s]>=%d and [%s]not in(%d, %d);",
		//				����									״̬�ֶ�								״̬:δִ��
		strSql.c_str(), Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strStatus.c_str(), Task::_TASKSTA_NONE,
		//������ֶ�						�����	�׶κ��ֶ�							�׶κ�	״̬�ֶ�
		Task::g_taskTable.m_strNo.c_str(), usNo, Task::g_taskTable.m_strLevel.c_str(), byLevel, Task::g_taskTable.m_strStatus.c_str(),
		//״̬:ȡ��				״̬:��ͣ
		Task::_TASKSTA_CANCEL,Task::_TASKSTA_PAUSE
	);

	// �ϳ�SQL���
	BufferString::std_string_format(strSql,
		// insert into ����(�����,�׶κ�,��������,��ʼ����,������,����״̬,ִ����,�쳣��Ϣ,����ʱ��) values(%d,%d,%d,%d,%s,%d,%d,%s,getdate())
		"%s insert into [%s]([%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]) values(%d,%d,%d,%d,'%s',%d,%d,'%s',getdate());",
		//				����								  ������ֶ�						 �׶κ��ֶ�
		strSql.c_str(), Task::g_taskTable.m_strTable.c_str(), Task::g_taskTable.m_strNo.c_str(), Task::g_taskTable.m_strLevel.c_str(),
		//���������ֶ�							��ʼ�����ֶ�						�������ֶ�
		Task::g_taskTable.m_strTarget.c_str(), Task::g_taskTable.m_strBegin.c_str(), Task::g_taskTable.m_strAction.c_str(),
		//����״̬�ֶ�							ִ�����ֶ�								�쳣��Ϣ�ֶ�
		Task::g_taskTable.m_strStatus.c_str(), Task::g_taskTable.m_strExecuter.c_str(), Task::g_taskTable.m_strError.c_str(),
		//����ʱ��
		Task::g_taskTable.m_strPublish,
		//�����		�׶κ�			��������			��ʼ����		������				����״̬
		newTask.GetNo(), newTask.GetLevel(), newTask.GetTarget(), newTask.GetBegin(), newTask.GetAction().c_str(), newTask.GetStatus(),
		//ִ����			�쳣��Ϣ
		newTask.GetExecuter(), newTask.GetError().c_str()
	);

	// ִ��SQL���
	if (m_ado.ExecuteSQL(_bstr_t(strSql.c_str())) == false)
	{
		// �������
		DebugPrint("ִ��SQL���ʧ��!���:%s", strSql.c_str());

		strRecord += "ʧ��!";

		// ��¼��־
		Record(strRecord);
		// �������
		DebugPrint(strRecord.c_str());

		return false;
	}

	// ��¼��־
	Record(strRecord);
	// �������
	DebugPrint(strRecord.c_str());

	return true;
}

AGVTaskArr* CTaskManager::GetTaskArray(unsigned short usNo)
{
	if (usNo == 0)
	{
		return nullptr;
	}

	if (m_mapTaskList.find(usNo) == m_mapTaskList.end())
	{
		return nullptr;
	}

	return m_mapTaskList[usNo];
}

std::map<unsigned short, AGVTaskArr*>& CTaskManager::GetTaskList()
{
	// TODO: �ڴ˴����� return ���
	return m_mapTaskList;
}

bool CTaskManager::UpdateTask(AGVTask task)
{
	unsigned short usNo = task.GetNo();

	if (task.GetNo() == 0)
	{
		return false;
	}

	if (m_mapTaskList.find(usNo) == m_mapTaskList.end())
	{
		return PublishTask(task);
	}
	else
	{
		AGVTaskArr* pArr = GetTaskArray(usNo);

		return pArr->UpdateTask(task);
	}

	return false;
}

void CTaskManager::DeleteTask(unsigned short usNo)
{
	if (m_mapTaskList.find(usNo) != m_mapTaskList.end())
	{
		delete m_mapTaskList[usNo];
		m_mapTaskList.erase(m_mapTaskList.find(usNo));
	}

	return;
}

void CTaskManager::Clear()
{
	for (std::map<unsigned short, AGVTaskArr*>::iterator it = m_mapTaskList.begin(); it != m_mapTaskList.end();)
	{
		delete it->second;
		it = m_mapTaskList.erase(it);
	}
}

#endif // !_TASK_H

void Task::SetTaskTable(const TaskTable task)
{
	g_taskTable = task;

	return;
}
