/*!
 * @file Task.h
 * @brief Taskͷ�ļ�
 * @author FanKaiyu
 * @date 2018-11-26
 * @version 1.0
*/

#pragma once
#include "stdafx.h"
#include <string>
#include <list>
#include <map>
#include "ADOConn.h"

using namespace std;

/*!
 * ��λ˵��:
 * ��λ1��ƽ̨
 * ��λ2����е��AGV����/���ϵ�
 * ��λ3��Ǳ��ʽAGV�ȴ���
 * ��λ4��Ǳ��ʽAGVж�ϳ���
 * ��λ5��Ǳ��ʽAGV���ϳ���
 * ��λ6���̶���е��
 * ��λ7��������1
 * ��λ8��������2
*/

/*!
 * ����λ˵��:
 * ����λ1������ʽAGV����λ
 * ����λ2��Ǳ��ʽAGV����λ
*/

/*!
 * WMS������ṹ
 * ���� WMS_TASK_TABLE
 * �ֶ� WMSTask_Type ���� varchar(50) ˵�����������ͣ��ַ������͡�ֵΪ����⡱/�����⡱
 * �ֶ� WMSTask_No ���� int ˵�������ܱ��
 * �ֶ� WMSTask_Row ���� int ˵���������к�
 * �ֶ� WMSTask_Col ���� int ˵���������к�
 * �ֶ� WMSTask_Status ���� int ˵��������ִ�����̽׶Σ����TASKIN/TASKOUT�ṹ��
 * �ֶ� WMSTask_Publish ���� datetime ˵�������񷢲�ʱ��
 * �ֶ� WMSTask_Finish ���� datetiem ˵����������ɵ�ʱ��
 *
 * ��������н��ܹ�����1������
*/

/*!
 * AGV������ṹ
 * ���� Task
 * �ֶ� AgvNo ���� int ˵����ִ��AGV���
 * �ֶ� AgvType ���� nvarchar(50) ˵����ִ��AGV���͡�ֵΪ��P��ǣ��ʽ����S��Ǳ��ʽ����T������ʽ����A����е��ʽ����L������ʽ����F���泵ʽ
 * �ֶ� Station ���� int ˵��������λ��ţ�ִ�з�������ʱ���ֶδ������λ���
 * �ֶ� Action ���� nvarchar(50) ˵������������ֵΪ���ƶ����������ϡ��������ϡ�����ͨ�С��������ء�
 * �ֶ� Status ���� nvarchar(20) ˵��������״̬��ֵΪ��δִ�С�����ִ�С�����ִ����ɡ�
 * �ֶ� Erro ���� nvarchar(20) ˵���������쳣��Ϣ��Ĭ��ֵ:"����"
 * �ֶ� IssueTime ���� datetime ˵�������񷢲�ʱ��
 * �ֶ� FinishTime ���� datetime ˵�����������ʱ��
*/

#define AGVTASKSTA_NONE "δִ��"
#define AGVTASKSTA_EXE "ִ��"
#define AGVTASKSTA_FINISH "���"
#define AGVTASKACT_MOVE "�ƶ�"
#define AGVTASKACT_LOAD "����"
#define AGVTASKACT_UNLOAD "����"
#define AGVTASKACT_PASS "ͨ��"
#define AGVTASKACT_RETURN "����"
#define AGVTYPE_P "P"
#define AGVTYPE_S "S"
#define AGVTYPE_T "T"
#define AGVTYPE_A "A"
#define AGVTYPE_F "F"
#define AGVTYPE_L "L"
#define WMSTASKTYPE_IN "���"
#define WMSTASKTYPE_OUT "����"

/*! @brief ���׶��������� */
enum TASKIN
{
	TASKIN_NONE,							/*!< δִ�� */
	/*!
	 * �׶�1 �������߿�ʼ
	 * ����->������2->ͨ��->������1->��е��->Ǳ��
	 */
	TASKIN_LEVEL1_MOVETOPIPELINE2,			/*!< �ƶ���������2,��ʱ���ͱ���ʽAGV���� */
	TASKIN_LEVEL1_UNLOADTOPIPELINE2,		/*!< ������������2,��ʱ���ͱ���ʽAGV���� */
	TASKIN_LEVEL1_MOVETOPIPELINE1,			/*!< �ƶ���������1,��ʱ���ͱ���ʽAGV�Լ�Ǳ��ʽAGV����*/
	TASKIN_LEVEL1_PASSTOPIPELINE1,			/*!< ��ȡͨ����������1,��ʱ���ͱ���ʽAGV���� */
	TASKIN_LEVEL1_UNLOADFROMPIPELINE1,		/*!< ��������1������Ǳ��ʽAGV,��ʱ���ƻ�е�� */
	/*!
	 * �׶�2 ��Ǳ��ʽAGV��ʼ
	 * Ǳ��->ж��->����->����
	*/
	TASKIN_LEVEL2_MOVETOUNLOAD,				/*!< �ƶ���ж����,��ʱ����Ǳ��ʽ�Լ���е��ʽAGV���� */
	/*!
	 * �׶�3 �ӻ�е��ʽAGV��ʼ
	 * ��е��->����->�ƶ���ƽ̨->����
	*/
	TASKIN_LEVEL3_MOVETOLOAD,				/*!< �ƶ���ж����,��ʱ���ͻ�е��ʽAGV����,Ǳ��ʽж�����ƶ��������㡢װ������������*/
	TASKIN_LEVEL3_LOAD,						/*!< ����������е��AGV */
	TASKIN_LEVEL3_MOVETOTABLE,				/*!< ��е��AGV�ƶ���ƽ̨ */
	TASKIN_LEVEL3_UNLOAD,					/*!< ������ƽ̨ */
	/*!
	 * �׶�4 ��ƽ̨��ʼ
	 * ƽ̨->���
	*/
	TASKIN_LEVEL4_IN,						/*!< �����ƽ̨ */
	TASKIN_FINISH,							/*!< ������� */
};

/*! @brief ����׶��������� */
enum TASKOUT
{
	TASKOUT_NONE,							/*!< δִ�� */
	/*!
	 * �׶�1 ��ƽ̨��ʼ
	 * ����->ƽ̨->��е��AGV->ж��
	*/
	TASKOUT_LEVEL1_OUT,						/*!< �ӻ��ܳ�����ƽ̨,��ʱ���ͻ�е�֡�Ǳ��ʽ������ʽAGV���� */
	TASKOUT_LEVEL1_MOVETOTABLE,				/*! <��е��AGV�ƶ���ƽ̨ */
	TASKOUT_LEVEL1_LOAD,					/*!< ��ƽ̨��������е��AGV */
	TASKOUT_LEVEL1_MOVETOUNLOAD,			/*!< ��е��AGV�ƶ���ж�� */
	TASKOUT_LEVEL1_UNLOAD,					/*!< ������ж�����ϳ� */
	/*!
	 * �׶�2 ��Ǳ��ʽ��ʼ
	 * �ȴ�->ж��->����->��е��
	*/
	TASKOUT_LEVEL2_MOVETOWAIT,				/*!< �ƶ����ȴ�λ */
	TASKOUT_LEVEL2_EXCHANGE,				/*!< ���� */
	TASKOUT_LEVEL2_MOVETOARM,				/*!< �ƶ�����е�� */
	TASKOUT_LEVEL2_LOAD,					/*!< ��е��������������1 */
	/*!
	 * �׶�3 �������߿�ʼ
	*/
	TASKOUT_LEVEL3_MOVETOPIPELINE1,			/*!< �ƶ���������1 */
	TASKOUT_LEVEL3_LOADFROMPIPELINE1,		/*!< ��������1���� */
	TASKOUT_FINISH,					
};

/*!< @brief AGV����ṹ�� */
typedef struct _AGVTaskStruct
{
	int m_unNo;			/*!< ִ��AGV��� */
	string m_strType;	/*!< ִ��AGV���� */
	int m_unStation;	/*!< ����λ��ţ�ִ�з�������ʱ���ֶδ������λ��� */
	string m_strAction; /*!< ������ */
	string m_strStatus; /*!< ����״̬ */
	string m_strError;	/*!< �����쳣��Ϣ */
	string m_strIssueTime; /*!< ���񷢲�ʱ�� */
	string m_strFinishTime; /*!< �������ʱ�� */

	_AGVTaskStruct()
	{
		m_unNo = 0;		
		m_strType = "";
		m_unStation = 0;
		m_strAction = "";
		m_strStatus = "";
		m_strError = "";
		m_strIssueTime = "";
		m_strFinishTime = "";
	}

	_AGVTaskStruct(const _AGVTaskStruct& agvTask)
	{
		m_unNo = agvTask.m_unNo;
		m_strType = agvTask.m_strType;
		m_unStation = agvTask.m_unStation;
		m_strAction = agvTask.m_strAction;
		m_strStatus = agvTask.m_strStatus;
		m_strError = agvTask.m_strError;
		m_strIssueTime = agvTask.m_strIssueTime;
		m_strFinishTime = agvTask.m_strFinishTime;
	}

	void operator= (const _AGVTaskStruct& agvTask)
	{
		m_unNo = agvTask.m_unNo;
		m_strType = agvTask.m_strType;
		m_unStation = agvTask.m_unStation;
		m_strAction = agvTask.m_strAction;
		m_strStatus = agvTask.m_strStatus;
		m_strError = agvTask.m_strError;
		m_strIssueTime = agvTask.m_strIssueTime;
		m_strFinishTime = agvTask.m_strFinishTime;
	}

	bool IsNull()
	{
		if (m_strType == "" || m_unStation == 0 || m_strAction == "" || m_strStatus == "" || m_strIssueTime == "")
		{
			return true;
		}

		return false;
	}

} _AGVTASK;

/*!< @brief WMS����ṹ�� */
typedef struct _WMSTaskStruct
{
	unsigned int m_unNo;		/*!< ���ܱ�� */
	unsigned int m_unRow;		/*!< ������ */
	unsigned int m_unCol;		/*!< ������ */
	string m_strType;			/*!< �������� */
	unsigned int m_unStatus;	/*!< ����״̬ */
	string m_strPublish;		/*!< ���񷢲�ʱ�� */

	_WMSTaskStruct()
	{
		m_unNo = 0;
		m_unRow = 0;
		m_unCol = 0;
		m_strType = "";
		m_unStatus = 0;
		m_strPublish = "";
	}

	_WMSTaskStruct(const _WMSTaskStruct& WMSTask)
	{
		m_unNo = WMSTask.m_unNo;
		m_unRow = WMSTask.m_unRow;
		m_unCol = WMSTask.m_unCol;
		m_strType = WMSTask.m_strType;
		m_unStatus = WMSTask.m_unStatus;
		m_strPublish = WMSTask.m_strPublish;
	}

	void operator= (const _WMSTaskStruct& WMSTask)
	{
		m_unNo = WMSTask.m_unNo;
		m_unRow = WMSTask.m_unRow;
		m_unCol = WMSTask.m_unCol;
		m_strType = WMSTask.m_strType;
		m_unStatus = WMSTask.m_unStatus;
		m_strPublish = WMSTask.m_strPublish;
	}

	bool IsNull()
	{
		if (m_unNo == 0 || m_unRow == 0 || m_unCol == 0 || m_strType == "" || m_strPublish=="")
		{
			return true;
		}

		return false;
	}

} _WMSTASK;

/*!
 * @brief ����AGV����
 * @param ADOConn& ���ݿ����Ӵ�
 * @param string ִ�������AGV����
 * @param int ����λ��ţ�ִ�з�������ʱ���ֶδ������λ���
 * @param string ִ�е�������
 * @return bool �����ɹ�����true,ʧ�ܷ���false
*/
static bool CreateAGVTask(ADOConn& ado,string strType, int Station, string strAction)
{
	CString cstrSql = _T("");
	cstrSql.Format(_T("insert into Task([AgvNo],[AgvType],[Station],[Action],[Status]) values(0,'%s',%d,'%s','%s')"), strType.c_str(), Station, strAction.c_str(), AGVTASKSTA_NONE);

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief ��ȡAGV����
 * @param ADOConn& ���ݿ����Ӵ�
 * @param string ִ�������AGV����
 * @param int ����λ��ţ�ִ�з�������ʱ���ֶδ������λ���
 * @param string ִ�е�������
 * @return _AGVSTASK AGV����ṹ��
*/
static _AGVTASK GetAGVTask(ADOConn& ado, string strType, int Station, string strAction)
{
	_AGVTASK agvTask;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from Task where [AgvType]='%s' and [Station]=%d and [Action]='%s'"), strType, Station, strAction);

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	if (pRecordset == nullptr || pRecordset->adoEOF)
	{
		return agvTask;
	}

	_variant_t var;	/*!< �ֶ�ֵ */

	var = pRecordset->GetCollect(_T("AgvNo"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_unNo = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("AgvType"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_strType = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("Station"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_unStation = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("Action"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_strAction = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("Status"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_strStatus = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("Error"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_strError = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("IssueTime"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_strIssueTime = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("FinishTime"));
	if (var.vt != VT_NULL)
	{
		agvTask.m_strFinishTime = _bstr_t(var);
	}

	return agvTask;
}

/*!
 * @brief ��ȡȫ��AGV����
 * @param ADOConn& ���ݿ����Ӵ�
 * @return _AGVSTASK AGV����ṹ���б�
*/
static list<_AGVTASK> GetAllAGVTask(ADOConn& ado)
{
	list<_AGVTASK> listAGV;

	CString cstrSql = _T("select * from Task order by [IssueTime]");
	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_AGVTASK agvTask;

		_variant_t var;	/*!< �ֶ�ֵ */

		var = pRecordset->GetCollect(_T("AgvNo"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("AgvType"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("Station"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_unStation = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("Action"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_strAction = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("Status"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_strStatus = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("Error"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_strError = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("IssueTime"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_strIssueTime = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("FinishTime"));
		if (var.vt != VT_NULL)
		{
			agvTask.m_strFinishTime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listAGV.push_back(agvTask);
	}

	return listAGV;
}

/*!
 * @brief ���AGV����
 * @param ADOConn& ���ݿ����Ӵ�
 * @return bool ����ɹ�����true,ʧ�ܷ���false
*/
static bool ClearAGVTask(ADOConn& ado)
{
	CString cstrSql = _T("delete Task");

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief ����WMS����
 * @param ADOConn& ���ݿ����Ӵ�
 * @param unsigend int ���ܱ��
 * @param unsigend int ������
 * @param unsigned int ������
 * @param string ��������
 * @return bool �����ɹ�����true,ʧ�ܷ���false
*/
static bool CreateWMSTask(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol, string strType)
{
	CString cstrSql = _T("");
	if (strType == WMSTASKTYPE_IN)
	{
		cstrSql.Format(_T("insert into WMS_TASK_TABLE([WMSTask_No],[WMSTask_Row],[WMSTask_Col],[WMSTask_Type],[WMSTask_Status]) values(%d,%d,%d,'%s',%d)"), unNo, unRow, unCol, strType.c_str(),TASKIN_NONE);
	}
	else if (strType == WMSTASKTYPE_OUT)
	{
		cstrSql.Format(_T("insert into WMS_TASK_TABLE([WMSTask_No],[WMSTask_Row],[WMSTask_Col],[WMSTask_Type],[WMSTask_Status]) values(%d,%d,%d,'%s',%d)"), unNo, unRow, unCol, strType.c_str(), TASKOUT_NONE);
	}

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief ����WMS����
 * @param ADOConn& ���ݿ����Ӵ�
 * @param unsigend int ���ܱ��
 * @param unsigend int ������
 * @param unsigned int ������
 * @param string ��������
 * @param unsigned int ����״̬
 * @return bool �����ɹ�����true,ʧ�ܷ���false
*/
static bool UpdateWMSTask(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol, string strType,unsigned int unStatus)
{
	CString cstrSql = _T("");

	if (unStatus == TASKOUT_FINISH || unStatus == TASKIN_FINISH)
	{
		cstrSql.Format(_T("update WMS_TASK_TABLE set [WMSTask_Status]=%d,[WMSTask_Finish]=getdate() where [WMSTask_No]=%d and [WMSTask_Row]=%d and [WMSTask_Col]=%d and [WMSTask_Type]='%s'"), unStatus, unNo, unRow, unCol, strType.c_str());
	}
	else
	{
		cstrSql.Format(_T("update WMS_TASK_TABLE set [WMSTask_Status]=%d where [WMSTask_No]=%d and [WMSTask_Row]=%d and [WMSTask_Col]=%d and [WMSTask_Type]='%s'"), unStatus, unNo, unRow, unCol, strType.c_str());
	}

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief ���ȫ��WMS����
 * @param ADOConn& ���ݿ����Ӵ�
 * @return bool �����ɹ�����true,ʧ�ܷ���false
*/
static bool ClearAllWMSTask(ADOConn& ado)
{
	CString cstrSql = _T("delete WMS_TASK_TABLE");

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief ���WMS����
 * @param ADOConn& ���ݿ����Ӵ�
 * @param unsigend int ���ܱ��
 * @param unsigend int ������
 * @param unsigned int ������
 * @param string ��������
 * @return bool �����ɹ�����true,ʧ�ܷ���false
*/
static bool ClearWMSTask(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol, string strType)
{
	CString cstrSql = _T("");
	cstrSql.Format(_T("delete WMS_TASK_TABLE where [WMSTask_No]=%d and [WMSTask_Row]=%d and [WMSTask_Col]=%d and [WMSTask_Type]='%s'"), unNo, unRow, unCol, strType.c_str());

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief ��ȡȫ��WMS����
 * @param ADOConn& ���ݿ����Ӵ�
 * @return list<_WMSTASK> WMS����ṹ���б�
*/
static list<_WMSTASK> GetAllWMSTask(ADOConn& ado)
{
	list<_WMSTASK> listWMS;

	CString cstrSql = _T("select * from WMS_TASK_TABLE order by [WMSTask_Publish]");
	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_WMSTASK task;

		_variant_t var;	/*!< �ֶ�ֵ */

		var = pRecordset->GetCollect(_T("WMSTask_No"));
		if (var.vt != VT_NULL)
		{
			task.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSTask_Row"));
		if (var.vt != VT_NULL)
		{
			task.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSTask_Col"));
		if (var.vt != VT_NULL)
		{
			task.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSTask_Type"));
		if (var.vt != VT_NULL)
		{
			task.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSTask_Status"));
		if (var.vt != VT_NULL)
		{
			task.m_unStatus = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSTask_Publish"));
		if (var.vt != VT_NULL)
		{
			task.m_strPublish = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listWMS.push_back(task);
	}

	return listWMS;
}

/*!
 * @brief ��ȡWMS����
 * @param ADOConn& ���ݿ����Ӵ�
 * @param unsigend int ���ܱ��
 * @param unsigend int ������
 * @param unsigned int ������
 * @param string ��������
 * @return _WMSTASK WMS����ṹ��
*/
static _WMSTASK GetWMSTask(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol, string strType)
{
	_WMSTASK task;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_TASK_TABLE where [WMSTask_No]=%d and [WMSTask_Row]=%d and [WMSTask_Col]=%d and [WMSTask_Type]='%s'"),unNo, unRow, unCol, strType.c_str());

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	if (pRecordset == nullptr || pRecordset->adoEOF)
	{
		return task;
	}

	_variant_t var;	/*!< �ֶ�ֵ */

	var = pRecordset->GetCollect(_T("WMSTask_No"));
	if (var.vt != VT_NULL)
	{
		task.m_unNo = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSTask_Row"));
	if (var.vt != VT_NULL)
	{
		task.m_unRow = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSTask_Col"));
	if (var.vt != VT_NULL)
	{
		task.m_unCol = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSTask_Type"));
	if (var.vt != VT_NULL)
	{
		task.m_strType = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("WMSTask_Status"));
	if (var.vt != VT_NULL)
	{
		task.m_unStatus = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSTask_Publish"));
	if (var.vt != VT_NULL)
	{
		task.m_strPublish = _bstr_t(var);
	}

	return task;
}