/*!
 * @file CTaskManager.h
 * @brief ��������������ļ�
 *
 * ������������������ļ�
 * @author FanKaiyu
 * @date 2019-02-18
 * @version 1.0
*/

#pragma once
#include <string>
#include <thread>
#include <mutex>
#include "ADO/ADOConn.h"

#ifndef _TASK_H
#define _TASK_H

#define WM_UPDATE_TASK WM_USER+3

/*!
 * ������
*/
#ifdef _UNICODE
#define _TASKACTION_W_EMPTYMOVE		L"�����ƶ�"
#define _TASKACTION_W_FULLMOVE		L"�����ƶ�"
#define _TASKACTION_W_MOVE			L"�ƶ�"
#define _TASKACTION_W_LOAD			L"����"
#define _TASKACTION_W_UNLOAD		L"����"
#define _TASKACTION_W_END			L"����"
#endif 

#define _TASKACTION_A_EMPTYMOVE		"�����ƶ�"
#define _TASKACTION_A_FULLMOVE		"�����ƶ�"
#define _TASKACTION_A_MOVE			"�ƶ�"
#define _TASKACTION_A_LOAD			"����"
#define _TASKACTION_A_UNLOAD		"����"
#define _TASKACTION_A_END			"����"

#ifdef _AFX
#define _TASKACTION_EMPTYMOVE		_T("�����ƶ�")
#define _TASKACTION_FULLMOVE		_T("�����ƶ�")
#define _TASKACTION_MOVE			_T("�ƶ�")
#define _TASKACTION_LOAD			_T("����")
#define _TASKACTION_UNLOAD			_T("����")
#define _TASKACTION_END				_T("����")
#endif 

/*!
 * �����쳣
*/
#ifdef _AFX
#define _TASKERROR_NONE				_T("����")
#define _TASKERROR_TYPE				_T("������Ч:������AGV�޷�ִ�д�������")
#define _TASKERROR_TARGET			_T("������Ч:������Ч������ֵ,�޷�ִ������")
#define _TASKERROR_ACTION			_T("������Ч:��Ч��������")
#define _TASKERROR_ACTOVERTIME		_T("�������:������ִ�г�ʱ")
#define _TASKERROR_OUTLINE			_T("�������:ִ�������AGV����")
#define _TASKERROR_FULL				_T("�������:AGV������,�޷�����")
#define _TASKERROR_EMPTY			_T("�������:AGV�ѿ���,�޷�����")
#endif

#ifdef _UNICODE
#define _TASKERROR_W_NONE			L"����"
#define _TASKERROR_W_TYPE			L"������Ч:������AGV�޷�ִ�д�������"
#define _TASKERROR_W_TARGET			L"������Ч:������Ч������ֵ,�޷�ִ������"
#define _TASKERROR_W_ACTION			L"������Ч:��Ч��������"
#define _TASKERROR_W_ACTOVERTIME	L"�������:������ִ�г�ʱ"
#define _TASKERROR_W_OUTLINE		L"�������:ִ�������AGV����"
#define _TASKERROR_W_FULL			L"�������:AGV������,�޷�����"
#define _TASKERROR_W_EMPTY			L"�������:AGV�ѿ���,�޷�����"
#endif

#define _TASKERROR_A_NONE			"����"
#define _TASKERROR_A_TYPE			"������Ч:������AGV�޷�ִ�д�������"
#define _TASKERROR_A_TARGET			"������Ч:������Ч������ֵ,�޷�ִ������"
#define _TASKERROR_A_ACTION			"������Ч:��Ч��������"
#define _TASKERROR_A_ACTOVERTIME	"�������:������ִ�г�ʱ"
#define _TASKERROR_A_OUTLINE		"�������:ִ�������AGV����"
#define _TASKERROR_A_FULL			"�������:AGV������,�޷�����"
#define _TASKERROR_A_EMPTY			"�������:AGV�ѿ���,�޷�����"


namespace Task
{
	/*!
	 * ����״̬
	*/
	enum _TASKSTATUS
	{
		_TASKSTA_NONE,		/*!< δִ�� */
		_TASKSTA_EXECUTE,	/*!< ִ�� */
		_TASKSTA_FINISH,	/*!< ���:��ɵ����񲻿��Իָ� */
		_TASKSTA_PAUSE,		/*!< ��ͣ:��ͣ��������Իָ� */
		_TASKSTA_CANCEL,	/*!< ȡ��:������ǰ����׶�������һ����׶� */
	};

	typedef struct _TaskTable_Struct
	{
		std::string m_strTable;			/*!< ������� */
		std::string m_strHisTable;		/*!< ������ʷ���� */
		std::string m_strNo;			/*!< �ֶ�:����� */
		std::string m_strLevel;			/*!< �ֶ�:�׶κ� */
		std::string m_strTarget;		/*!< �ֶ�:�������� */
		std::string m_strBegin;			/*!< �ֶ�:��ʼ���� */
		std::string m_strAction;		/*!< �ֶ�:������ */
		std::string m_strStatus;		/*!< �ֶ�:����״̬ */
		std::string m_strExecuter;		/*!< �ֶ�:ִ���� */
		std::string m_strError;			/*!< �ֶ�:�쳣��Ϣ */
		std::string m_strPublish;		/*!< �ֶ�:����ʱ�� */
		std::string m_strFinish;		/*!< �ֶ�:���ʱ�� */

		_TaskTable_Struct()
		{
			m_strTable = "TASK_RUNTIME_TABLE";
			m_strHisTable = "TASK_HISTORY_TABLE";
			m_strNo = "task_no";
			m_strLevel = "task_level";
			m_strTarget = "task_target";
			m_strBegin = "task_begin";
			m_strAction = "task_action";
			m_strStatus = "task_status";
			m_strExecuter = "task_executer";
			m_strError = "task_error";
			m_strPublish = "task_publish_time";
			m_strFinish = "task_finish_time";
		}

		_TaskTable_Struct(const _TaskTable_Struct& table)
		{
			m_strTable = table.m_strTable;
			m_strHisTable = table.m_strHisTable;
			m_strNo = table.m_strNo;
			m_strLevel = table.m_strLevel;
			m_strTarget = table.m_strTarget;
			m_strBegin = table.m_strBegin;
			m_strAction = table.m_strAction;
			m_strStatus = table.m_strStatus;
			m_strExecuter = table.m_strExecuter;
			m_strError = table.m_strError;
			m_strPublish = table.m_strPublish;
			m_strFinish = table.m_strFinish;
		}

		void operator=(const _TaskTable_Struct& table)
		{
			m_strTable = table.m_strTable;
			m_strHisTable = table.m_strHisTable;
			m_strNo = table.m_strNo;
			m_strLevel = table.m_strLevel;
			m_strTarget = table.m_strTarget;
			m_strBegin = table.m_strBegin;
			m_strAction = table.m_strAction;
			m_strStatus = table.m_strStatus;
			m_strError = table.m_strError;
			m_strExecuter = table.m_strExecuter;
			m_strPublish = table.m_strPublish;
			m_strFinish = table.m_strFinish;
		}

		/*!
		 * @brief ����ִ�����ֶ���
		 * @param string �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetExecuter(std::string strExecuter)
		{
			if (m_strExecuter == strExecuter)
			{
				return false;
			}

			m_strExecuter = strExecuter;

			return true;
		}

		/*!
		 * @brief ����ִ�����ֶ���
		 * @param wstring �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetExecuter(std::wstring wstrExecuter)
		{
			std::string strExecuter = "";
			BufferString::std_wstring_to_string(strExecuter, wstrExecuter);

			return SetExecuter(strExecuter);
		}

		/*!
		 * @brief ����ִ�����ֶ���
		 * @param CString �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetExecuter(CString cstrExecuter)
		{
			std::string strExecuter = "";

			BufferString::CString_to_std_string(strExecuter, cstrExecuter);

			return SetExecuter(strExecuter);
		}

		/*!
		 * @brief ��ȡִ�����ֶ���
		 * @param string& �ֶ���
		*/
		std::string GetExecuter() const
		{
			return m_strExecuter;
		}

		/*!
		 * @brief ��ȡִ�����ֶ���
		 * @param string& �ֶ���
		*/
		void GetExecuter(std::string& strExecuter)
		{
			strExecuter = m_strExecuter;

			return;
		}

		/*!
		 * @brief ��ȡִ�����ֶ���
		 * @param wstring& �ֶ���
		*/
		void GetExecuter(std::wstring& wstrExecuter)
		{
			BufferString::std_string_to_wstring(wstrExecuter, m_strExecuter);

			return;
		}

		/*!
		 * @brief ��ȡִ�����ֶ���
		 * @param CString& �ֶ���
		*/
		void GetExecuter(CString& cstrExecuter)
		{
			BufferString::std_string_to_CString(cstrExecuter, m_strExecuter);

			return;
		}

		/*!
		 * @brief �������ʱ���ֶ���
		 * @param string �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetFinish(std::string strFinish)
		{
			if (m_strFinish == strFinish)
			{
				return false;
			}

			m_strFinish = strFinish;

			return true;
		}

		/*!
		 * @brief �������ʱ���ֶ���
		 * @param wstring �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetFinish(std::wstring wstrFinish)
		{
			std::string strFinish = "";
			BufferString::std_wstring_to_string(strFinish, wstrFinish);

			return SetFinish(strFinish);
		}

		/*!
		 * @brief ��ȡ���ʱ���ֶ���
		 * @param string& �ֶ���
		*/
		std::string GetFinish() const
		{
			return m_strFinish;
		}

		/*!
		 * @brief ��ȡ���ʱ���ֶ���
		 * @param string& �ֶ���
		*/
		void GetFinish(std::string& strFinish)
		{
			strFinish = m_strFinish;

			return;
		}

		/*!
		 * @brief ��ȡ���ʱ���ֶ���
		 * @param wstring& �ֶ���
		*/
		void GetFinish(std::wstring& wstrFinish)
		{
			BufferString::std_string_to_wstring(wstrFinish, m_strFinish);

			return;
		}
		
		/*!
		 * @brief ���÷���ʱ���ֶ���
		 * @param string �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetPublish(std::string strPublish)
		{
			if (m_strPublish == strPublish)
			{
				return false;
			}

			m_strPublish = strPublish;

			return true;
		}

		/*!
		 * @brief ���÷���ʱ���ֶ���
		 * @param wstring �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetPublish(std::wstring wstrPublish)
		{
			std::string strPublish = "";
			BufferString::std_wstring_to_string(strPublish, wstrPublish);

			return SetPublish(strPublish);
		}

		/*!
		 * @brief ��ȡ����ʱ�����ֶ���
		 * @return string �ֶ���
		*/
		std::string GetPublish() const
		{
			return m_strPublish;
		}

		/*!
		 * @brief ��ȡ����ʱ���ֶ���
		 * @param string& �ֶ���
		*/
		void GetPublish(std::string& strPublish)
		{
			strPublish = m_strPublish;

			return;
		}

		/*!
		 * @brief ��ȡ����ʱ���ֶ���
		 * @param wstring& �ֶ���
		*/
		void GetPublish(std::wstring& wstrPublish)
		{
			BufferString::std_string_to_wstring(wstrPublish, m_strPublish);

			return;
		}

		/*!
		 * @brief �����쳣��Ϣ�ֶ���
		 * @param string �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetError(std::string strError)
		{
			if (m_strError == strError)
			{
				return false;
			}

			m_strError = strError;

			return true;
		}

		/*!
		 * @brief �����쳣��Ϣ�ֶ���
		 * @param wstring �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetError(std::wstring wstrError)
		{
			std::string strError = "";
			BufferString::std_wstring_to_string(strError, wstrError);

			return SetError(strError);
		}

		/*!
		 * @brief ��ȡ�쳣��Ϣ���ֶ���
		 * @return string �ֶ���
		*/
		std::string GetError() const
		{
			return m_strError;
		}

		/*!
		 * @brief ��ȡ�쳣��Ϣ�ֶ���
		 * @param string& �ֶ���
		*/
		void GetError(std::string& strError)
		{
			strError = m_strError;

			return;
		}

		/*!
		 * @brief ��ȡ�쳣��Ϣ�ֶ���
		 * @param wstring& �ֶ���
		*/
		void GetError(std::wstring& wstrError)
		{
			BufferString::std_string_to_wstring(wstrError, m_strError);

			return;
		}

		/*!
		 * @brief ��������״̬�ֶ���
		 * @param string �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetStatus(std::string strStatus)
		{
			if (m_strStatus == strStatus)
			{
				return false;
			}

			m_strStatus = strStatus;

			return true;
		}

		/*!
		 * @brief ��������״̬�ֶ���
		 * @param wstring �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetStatus(std::wstring wstrStatus)
		{
			std::string strStatus = "";
			BufferString::std_wstring_to_string(strStatus, wstrStatus);

			return SetStatus(strStatus);
		}

		/*!
		 * @brief ��ȡ����״̬���ֶ���
		 * @return string �ֶ���
		*/
		std::string GetStatus() const
		{
			return m_strStatus;
		}

		/*!
		 * @brief ��ȡ����״̬�ֶ���
		 * @param string& �ֶ���
		*/
		void GetStatus(std::string strStatus)
		{
			strStatus = m_strStatus;

			return;
		}

		/*!
		 * @brief ��ȡ����״̬�ֶ���
		 * @param wstring& �ֶ���
		*/
		void GetStatus(std::wstring wstrStatus)
		{
			BufferString::std_string_to_wstring(wstrStatus, m_strStatus);

			return;
		}

		/*!
		 * @brief �����������ֶ���
		 * @param string �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetAction(std::string strAction)
		{
			if (m_strAction == strAction)
			{
				return false;
			}

			m_strAction = strAction;

			return true;
		}

		/*!
		 * @brief �����������ֶ���
		 * @param wstring �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetAction(std::wstring wstrAction)
		{
			std::string strAction = "";
			BufferString::std_wstring_to_string(strAction, wstrAction);

			return SetAction(strAction);
		}

		/*!
		 * @brief ��ȡ�������ֶ���
		 * @return string �ֶ���
		*/
		std::string GetAction() const
		{
			return m_strAction;
		}

		/*!
		 * @brief ��ȡ�������ֶ���
		 * @param string& �ֶ���
		*/
		void GetAction(std::string& strAction)
		{
			strAction = m_strAction;

			return;
		}

		/*!
		 * @brief ��ȡ�������ֶ���
		 * @param wstring& �ֶ���
		*/
		void GetAction(std::wstring& wstrAction)
		{
			BufferString::std_string_to_wstring(wstrAction, m_strAction);

			return;
		}

		/*!
		 * @brief ������ʼ�����ֶ���
		 * @param string �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetBegin(std::string strBegin)
		{
			if (m_strBegin == strBegin)
			{
				return false;
			}

			m_strBegin = strBegin;

			return true;
		}

		/*!
		 * @brief ������ʼ�����ֶ���
		 * @param wstring �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetBegin(std::wstring wstrBegin)
		{
			std::string strBegin = "";
			BufferString::std_wstring_to_string(strBegin, wstrBegin);

			return SetBegin(strBegin);
		}

		/*!
		 * @brief ��ȡ��ʼ�����ֶ���
		 * @return string �ֶ���
		*/
		std::string GetBegin() const
		{
			return m_strBegin;
		}

		/*!
		 * @brief ��ȡ��ʼ�����ֶ���
		 * @param string& �ֶ���
		*/
		void GetBegin(std::string& strBegin)
		{
			strBegin = m_strBegin;

			return;
		}

		/*!
		 * @brief ��ȡ��ʼ�����ֶ���
		 * @param wstring& �ֶ���
		*/
		void GetBegin(std::wstring& wstrBegin)
		{
			BufferString::std_string_to_wstring(wstrBegin, m_strBegin);

			return;
		}

		/*!
		 * @brief �������������ֶ���
		 * @param string �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetTarget(std::string strTarget)
		{
			if (m_strTarget == strTarget)
			{
				return false;
			}

			m_strTarget = strTarget;

			return true;
		}

		/*!
		 * @brief �������������ֶ���
		 * @param wstring �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetTarget(std::wstring wstrTarget)
		{
			std::string strTarget = "";
			BufferString::std_wstring_to_string(strTarget, wstrTarget);

			return SetTarget(strTarget);
		}

		/*!
		 * @brief ��ȡ���������ֶ���
		 * @return string �ֶ���
		*/
		std::string GetTarget() const
		{
			return m_strTarget;
		}

		/*!
		 * @brief ��ȡ���������ֶ���
		 * @param string& �ֶ���
		*/
		void GetTarget(std::string& strTarget)
		{
			strTarget = m_strTarget;

			return;
		}

		/*!
		 * @brief ��ȡ���������ֶ���
		 * @param wstring& �ֶ���
		*/
		void GetTarget(std::wstring& wstrTarget)
		{
			BufferString::std_string_to_wstring(wstrTarget, m_strTarget);

			return;
		}

		/*!
		 * @brief ���ý׶κ��ֶ���
		 * @param string �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetLevel(std::string strLevel)
		{
			if (m_strLevel == strLevel)
			{
				return false;
			}

			m_strLevel = strLevel;

			return true;
		}

		/*!
		 * @brief ���ý׶κ��ֶ���
		 * @param wstring �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetLevel(std::wstring wstrLevel)
		{
			std::string strLevel = "";
			BufferString::std_wstring_to_string(strLevel, wstrLevel);

			return SetLevel(strLevel);
		}

		/*!
		 * @brief ��ȡ�׶κ��ֶ���
		 * @return string �ֶ���
		*/
		std::string GetLevel() const
		{
			return m_strLevel;
		}

		/*!
		 * @brief ��ȡ�׶κ��ֶ���
		 * @param string& �ֶ���
		*/
		void GetLevel(std::string& strLevel)
		{
			strLevel = m_strLevel;

			return;
		}

		/*!
		 * @brief ��ȡ�׶κ��ֶ���
		 * @param wstring& �ֶ���
		*/
		void GetLevel(std::wstring& wstrLevel)
		{
			BufferString::std_string_to_wstring(wstrLevel, m_strLevel);

			return;
		}

		/*!
		 * @brief ����������ֶ���
		 * @param string �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetNo(std::string strNo)
		{
			if (m_strNo == strNo)
			{
				return false;
			}

			m_strNo = strNo;
			
			return true;
		}

		/*!
		 * @brief ����������ֶ���
		 * @param wstring �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetNo(std::wstring wstrNo)
		{
			std::string strNo = "";
			BufferString::std_wstring_to_string(strNo, wstrNo);

			return SetNo(strNo);
		}

		/*!
		 * @brief ��ȡ������ֶ���
		 * @return string �ֶ���
		*/
		std::string GetNo() const
		{
			return m_strNo;
		}

		/*!
		 * @brief ��ȡ������ֶ���
		 * @param string& �ֶ���
		*/
		void GetNo(std::string strNo)
		{
			m_strNo = strNo;

			return;
		}

		/*!
		 * @brief ��ȡ������ֶ���
		 * @param wstring& �ֶ���
		*/
		void GetNo(std::wstring& wstrNo)
		{
			BufferString::std_string_to_wstring(wstrNo, m_strNo);

			return;
		}

		/*!
		 * @brief �����������
		 * @param string ����
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetTable(std::string strTable)
		{
			if (m_strTable == strTable)
			{
				return false;
			}

			m_strTable = strTable;

			return true;
		}

		/*!
		 * @brief �����������
		 * @param wstring ����
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetTable(std::wstring wstrTable)
		{
			std::string strTable = "";

			BufferString::std_wstring_to_string(strTable, wstrTable);

			return SetTable(strTable);
		}

		/*!
		 * @brief ��ȡ�������
		 * @param string& ����
		*/
		void GetTable(std::string& strTable)
		{
			strTable = m_strTable;

			return;
		}

		/*!
		 * @brief ��ȡ�������
		 * @return string ����
		*/
		std::string GetTable() const
		{
			return m_strTable;
		}

		/*!
		 * @brief ��ȡ�������
		 * @param wstring& ����
		*/
		void GetTable(std::wstring& wstrTable)
		{
			BufferString::std_string_to_wstring(wstrTable, m_strTable);

			return;
		}

		/*!
		 * @brief ����������ʷ����
		 * @param string ����
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetHisTable(std::string strHisTable)
		{
			if (m_strHisTable == strHisTable)
			{
				return false;
			}

			m_strHisTable = strHisTable;

			return true;
		}

		/*!
		 * @brief ����������ʷ����
		 * @param wstring ����
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetHisTable(std::wstring wstrHisTable)
		{
			std::string strHisTable = "";

			BufferString::std_wstring_to_string(strHisTable, wstrHisTable);

			return SetHisTable(strHisTable);
		}

		/*!
		 * @brief ��ȡ������ʷ����
		 * @param string& ����
		*/
		void GetHisTable(std::string& strHisTable)
		{
			strHisTable = m_strHisTable;

			return;
		}

		/*!
		 * @brief ��ȡ������ʷ����
		 * @return string ����
		*/
		std::string GetHisTable() const
		{
			return m_strHisTable;
		}

		/*!
		 * @brief ��ȡ������ʷ����
		 * @param wstring& ����
		*/
		void GetHisTable(std::wstring& wstrHisTable)
		{
			BufferString::std_string_to_wstring(wstrHisTable, m_strHisTable);

			return;
		}

#ifdef _AFX
		/*!
		 * @brief �����������
		 * @param CString ����
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetTable(CString cstrTable)
		{
			std::string strTable = "";

			BufferString::CString_to_std_string(strTable, cstrTable);

			return SetTable(strTable);
		}

		/*!
		 * @brief ��ȡ�������
		 * @param CString& ����
		*/
		void GetTable(CString& cstrTable)
		{
			BufferString::std_string_to_CString(cstrTable, m_strTable);

			return;
		}

		/*!
		 * @brief ����������ʷ����
		 * @param CString ����
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetHisTable(CString cstrHisTable)
		{
			std::string strHisTable = "";

			BufferString::CString_to_std_string(strHisTable, cstrHisTable);

			return SetHisTable(strHisTable);
		}

		/*!
		 * @brief ��ȡ������ʷ����
		 * @param CString& ����
		*/
		void GetHisTable(CString& cstrHisTable)
		{
			BufferString::std_string_to_CString(cstrHisTable, m_strHisTable);

			return;
		}

		/*!
		 * @brief ����������ֶ���
		 * @param CString �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetNo(CString cstrNo)
		{
			std::string strNo = "";
			BufferString::CString_to_std_string(strNo, cstrNo);

			return SetNo(strNo);
		}

		/*!
		 * @brief ��ȡ������ֶ���
		 * @param CString& �ֶ���
		*/
		void GetNo(CString& cstrNo)
		{
			BufferString::std_string_to_CString(cstrNo, m_strNo);

			return;
		}

		/*!
		 * @brief ���ý׶κ��ֶ���
		 * @param CString �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetLevel(CString cstrLevel)
		{
			std::string strLevel = "";

			BufferString::CString_to_std_string(strLevel, cstrLevel);

			return SetLevel(strLevel);
		}

		/*!
		 * @brief ��ȡ�׶κ��ֶ���
		 * @param CSstring& �ֶ���
		*/
		void GetLevel(CString& cstrLevel)
		{
			BufferString::std_string_to_CString(cstrLevel, m_strTable);

			return;
		}

		/*!
		 * @brief �������������ֶ���
		 * @param CString �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetTarget(CString cstrTarget)
		{
			std::string strTarget = "";

			BufferString::CString_to_std_string(strTarget, cstrTarget);

			return SetTarget(cstrTarget);
		}

		/*!
		 * @brief ��ȡ���������ֶ���
		 * @param CString& �ֶ���
		*/
		void GetTarget(CString& cstrTarget)
		{
			BufferString::std_string_to_CString(cstrTarget, m_strTarget);

			return;
		}

		/*!
		 * @brief ������ʼ�����ֶ���
		 * @param CString �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetBegin(CString cstrBegin)
		{
			std::string strBegin = "";

			BufferString::CString_to_std_string(strBegin, cstrBegin);

			return SetBegin(strBegin);
		}

		/*!
		 * @brief ��ȡ��ʼ�����ֶ���
		 * @param CString& �ֶ���
		*/
		void GetBegin(CString& cstrBegin)
		{
			BufferString::std_string_to_CString(cstrBegin, m_strBegin);

			return;
		}

		/*!
		 * @brief �����������ֶ���
		 * @param cstring �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetAction(CString cstrAction)
		{
			std::string strAction = "";

			BufferString::CString_to_std_string(strAction, cstrAction);

			return SetAction(strAction);
		}

		/*!
		 * @brief ��ȡ�������ֶ���
		 * @param CString& �ֶ���
		*/
		void GetAction(CString& cstrAction)
		{
			BufferString::std_string_to_CString(cstrAction, m_strAction);

			return;
		}

		/*!
		 * @brief ��������״̬�ֶ���
		 * @param CString �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetStatus(CString cstrStatus)
		{
			std::string strStatus = "";

			BufferString::CString_to_std_string(strStatus, cstrStatus);

			return SetStatus(strStatus);
		}

		/*!
		 * @brief ��ȡ����״̬�ֶ���
		 * @param CString& �ֶ���
		*/
		void GetStatus(CString cstrStatus)
		{
			BufferString::std_string_to_CString(cstrStatus, m_strStatus);

			return;
		}

		/*!
		 * @brief �����쳣��Ϣ�ֶ���
		 * @param CString �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetError(CString cstrError)
		{
			std::string strError = "";

			BufferString::CString_to_std_string(strError, cstrError);

			return SetError(strError);
		}

		/*!
		 * @brief ��ȡ�쳣��Ϣ�ֶ���
		 * @param CString& �ֶ���
		*/
		void GetError(CString& cstrError)
		{
			BufferString::std_string_to_CString(cstrError, m_strError);

			return;
		}

		/*!
		 * @brief ���÷���ʱ���ֶ���
		 * @param CString �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetPublish(CString cstrPublish)
		{
			std::string strPublish = "";

			BufferString::CString_to_std_string(strPublish, cstrPublish);

			return SetPublish(strPublish);
		}

		/*!
		 * @brief ��ȡ����ʱ���ֶ���
		 * @param CString& �ֶ���
		*/
		void GetPublish(CString& cstrPublish)
		{
			BufferString::std_string_to_CString(cstrPublish, m_strPublish);

			return;
		}

		/*!
		 * @brief �������ʱ���ֶ���
		 * @param CString �ֶ���
		 * @return bool ���óɹ�����true,ʧ�ܷ���false
		*/
		bool SetFinish(CString cstrFinish)
		{
			std::string strFinish = "";

			BufferString::CString_to_std_string(strFinish, cstrFinish);

			return SetFinish(strFinish);
		}

		/*!
		 * @brief ��ȡ���ʱ���ֶ���
		 * @param CString& �ֶ���
		*/
		void GetFinish(CString& cstrFinish)
		{
			BufferString::std_string_to_CString(cstrFinish, m_strFinish);

			return;
		}
#endif // _AFX
	} TaskTable;

	static TaskTable g_taskTable;

	/*!
	 * @brief ���������
	 * @param TaskTable �����ṹ��
	*/
	static void SetTaskTable(const TaskTable task);
}

/*!< @brief ����AGV����Ļ����ṹ�� */
typedef struct _TaskStruct
{
protected:
	unsigned short m_usNo;		/*!< �����:Ϊ0ʱ������Ч */
	unsigned char m_byLevel;	/*!< �׶κ�:Ϊ0ʱ������Ч */
	std::string m_strAction;	/*!< ������:�ý׶�AGVӦִ�еĶ��� */
	unsigned char m_byStatus;	/*!< ����״̬:�ý׶�����ִ�е�״̬ */
	unsigned char m_byExecuter;	/*!< ����ִ����:ִ�иý׶������AGV��� */
	unsigned short m_usTarget;	/*!< ��������:ͨ��Ϊ��λ����RFID�����,Ϊ0ʱ������Ч */
	unsigned short m_usBegin;	/*!< ��ʼ����:ͨ��ΪAGV���ڵ���ʼ���꣬Ϊ0ʱ����ϵͳ����AGVִ������ */
	std::string m_strError;		/*!< �����쳣��Ϣ */

public:
	_TaskStruct()
	{
		m_usNo = 0;
		m_byLevel = 0;
		m_strAction = "";
		m_byStatus = Task::_TASKSTA_NONE;
		m_byExecuter = 0;
		m_usTarget = 0;
		m_usBegin = 0;
		m_strError = _TASKERROR_A_NONE;
	}

	_TaskStruct(unsigned short usNo, unsigned char byLevel, std::string strAction, unsigned short usTarget,
		unsigned short usBegin = 0,unsigned char byStatus = Task::_TASKSTA_NONE, unsigned char byExecuter = 0
		, std::string strError = _TASKERROR_A_NONE)
	{
		m_usNo = usNo;
		m_byLevel = byLevel;
		m_strAction = strAction;
		m_byStatus = byStatus;
		m_byExecuter = byExecuter;
		m_usTarget = usTarget;
		m_usBegin = usBegin;
		m_strError = strError;
	}

	_TaskStruct(unsigned short usNo, unsigned char byLevel, std::wstring wstrAction, unsigned short usTarget,
		unsigned short usBegin = 0, unsigned char byStatus = Task::_TASKSTA_NONE, unsigned char byExecuter = 0
		, std::string strError = _TASKERROR_A_NONE)
	{
		m_usNo = usNo;
		m_byLevel = byLevel;
		BufferString::std_wstring_to_string(m_strAction,wstrAction);
		m_byStatus = byStatus;
		m_byExecuter = byExecuter;
		m_usTarget = usTarget;
		m_usBegin = usBegin;
		m_strError = strError;
	}

#ifdef _AFX
	_TaskStruct(unsigned short usNo, unsigned char byLevel, CString cstrAction, unsigned short usTarget,
		unsigned short usBegin = 0, unsigned char byStatus = Task::_TASKSTA_NONE, unsigned char byExecuter = 0
		, std::string strError = _TASKERROR_A_NONE)
	{
		m_usNo = usNo;
		m_byLevel = byLevel;
		BufferString::CString_to_std_string(m_strAction,cstrAction);
		m_byStatus = byStatus;
		m_byExecuter = byExecuter;
		m_usTarget = usTarget;
		m_usBegin = usBegin;
		m_strError = strError;
	}
#endif

	_TaskStruct(const _TaskStruct& task)
	{
		m_usNo = task.m_usNo;
		m_byLevel = task.m_byLevel;
		m_strAction = task.m_strAction;
		m_byStatus = task.m_byStatus;
		m_byExecuter = task.m_byExecuter;
		m_usTarget = task.m_usTarget;
		m_usBegin = task.m_usBegin;
		m_strError = task.m_strError;
	}

	void operator=(const _TaskStruct& task)
	{
		m_usNo = task.m_usNo;
		m_byLevel = task.m_byLevel;
		m_strAction = task.m_strAction;
		m_byStatus = task.m_byStatus;
		m_byExecuter = task.m_byExecuter;
		m_usTarget = task.m_usTarget;
		m_usBegin = task.m_usBegin;
		m_strError = task.m_strError;
	}

public:
	/*!
	 * @brief ���񽻻�
	 *
	 * ������ִ�����Լ�ִ��״̬���ȫ����Ϣ
	 * �������쳣��Ϣ
	 * @param _TaskStruct& ����������ṹ��
	*/
	void Swop(_TaskStruct& task)
	{
		_TaskStruct tmp(task);

		task.m_usNo = m_usNo;
		task.m_byLevel = m_byLevel;
		task.m_strAction = m_strAction;
		task.m_usTarget = m_usTarget;
		task.m_usBegin = m_usBegin;
		task.m_strError = _TASKERROR_A_NONE;

		m_usNo = tmp.m_usNo;
		m_byLevel = tmp.m_byLevel;
		m_strAction = tmp.m_strAction;
		m_usTarget = tmp.m_usTarget;
		m_usBegin = tmp.m_usBegin;
		m_strError = _TASKERROR_A_NONE;

		return;
	}

	/*!
	 * @brief �������
	 * 
	 * ������������
	 * @param const _TaskStruct& ���µ�����ṹ��
	*/
	bool Update(_TaskStruct task)
	{
		if (m_usNo != task.GetNo() || m_byLevel != task.GetLevel())
		{
			return false;
		}

		std::string strAction = "";	/*!< ������ */
		std::string strError = "";	/*!< �쳣��Ϣ */
		task.GetAction(strAction);
		task.GetError(strError);

		if (SetTarget(task.GetTarget())
			|| SetBegin(task.GetBegin())
			|| SetAction(strAction)
			|| SetExecuter(task.GetExecuter())
			|| SetStatus(task.GetStatus())
			|| SetError(strError))
		{
			return true;
		}

		return false;
	}

	/*!
	 * @brief �����Ƿ���Ч
	 * @return bool ��Ч����true,��Ч����false
	*/
	bool IsNull()
	{
		if (m_usNo == 0 || m_byLevel == 0 || m_strAction == "")
		{
			return true;
		}

		return false;
	}

	/*!
	 * @brief ���������
	 * @param unsigned short �������
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetNo(unsigned short usNo)
	{
		if (usNo == 0)
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
	
	/*!
	 * @brief ��ȡ�����
	 * @return unsigned short �����
	*/
	unsigned short GetNo() const
	{
		return m_usNo;
	}
	
	/*!
	 * @brief ��ȡ�����
	 * @param string& �����
	*/
	void GetNo(std::string &strNo)
	{
		BufferString::std_string_format(strNo, "%d", m_usNo);

		return;
	}

	/*!
	 * @brief ��ȡ�����
	 * @param wstring& �����
	*/
	void GetNo(std::wstring& wstrNo)
	{
		std::string strNo = "";
		GetNo(strNo);

		BufferString::std_string_to_wstring(wstrNo, strNo);

		return;
	}

	/*!
	 * @brief ���ý׶κ�
	 * @param unsigned char �½׶κ�
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetLevel(unsigned char byLevel)
	{
		if (byLevel == 0)
		{
			return false;
		}

		if (m_byLevel == byLevel)
		{
			return false;
		}

		m_byLevel = byLevel;
		
		return true;
	}

	/*!
	 * @brief ��ȡ�׶κ�
	 * @return unsigned char �׶κ�
	*/
	unsigned char GetLevel() const
	{
		return m_byLevel;
	}

	/*!
	 * @brief ��ȡ�׶κ�
	 * @param string& �׶κ�
	*/
	void GetLevel(std::string& strLevel)
	{
		BufferString::std_string_format(strLevel, "%d", m_byLevel);

		return;
	}

	/*!
	 * @brief ��ȡ�׶κ�
	 * @param wstring& �׶κ�
	*/
	void GetLevel(std::wstring& wstrLevel)
	{
		std::string strLevel = "";
		GetLevel(strLevel);

		BufferString::std_string_to_wstring(wstrLevel, strLevel);

		return;
	}

	/*!
	 * @brief ���ö���
	 * @param string ����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetAction(std::string strAction)
	{
		if (m_strAction == strAction)
		{
			return false;
		}

		m_strAction = strAction;

		return true;
	}

	/*!
	 * @brief ���ö���
	 * @param wstring ����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetAction(std::wstring wstrAction)
	{
		std::string strAction = "";
		BufferString::std_wstring_to_string(strAction, wstrAction);

		return SetAction(strAction);
	}

	/*!
	 * @brief ��ȡ����
	 * @param string& ����
	*/
	void GetAction(std::string &strAction)
	{
		strAction = m_strAction;

		return;
	}

	/*!
	 * @brief ��ȡ����
	 * @param wstring& ����
	*/
	void GetAction(std::wstring &wstrAction)
	{
		std::string strAction = "";
		GetAction(strAction);

		BufferString::std_string_to_wstring(wstrAction, strAction);

		return;
	}

	/*!
	 * @brief ��ȡ����
	 * @return string ����
	*/
	std::string GetAction() const
	{
		return m_strAction;
	}

	/*!
	 * @brief ����״̬
	 * @param unsigned char ��ִ��״̬
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetStatus(unsigned char byStatus)
	{
		if (m_byStatus == byStatus)
		{
			return false;
		}

		if (m_strAction == _TASKACTION_A_END && (byStatus == Task::_TASKSTA_PAUSE || byStatus == Task::_TASKSTA_CANCEL))
		{
			return false;
		}

		m_byStatus = byStatus;

		return true;
	}

	/*!
	 * @brief ��ȡ״̬
	 * @return unsigned char ִ��״̬
	*/
	unsigned char GetStatus() const
	{
		return m_byStatus;
	}

	/*
	 * @brief ��ȡ״̬
	 * @param string& ִ��״̬
	*/
	void GetStatus(std::string& strStatus)
	{
		GetStatus(m_byStatus, strStatus);
		switch (m_byStatus)
		{
		case Task::_TASKSTA_FINISH:
			if (m_strAction != _TASKACTION_A_END)
			{
				strStatus = "�������,�ȴ�ִ����һ�׶�����";
			}
			else
			{
				strStatus = "�����������";
			}
			break;
		case Task::_TASKSTA_CANCEL:
			if (m_strAction != _TASKACTION_A_END)
			{
				strStatus = "����ȡ��,������һ�׶�";
			}
			else
			{
				strStatus = "����ȡ��";
			}	
			break;
		}

		return;
	}

	/*
	 * @brief ��ȡ״̬
	 * @param wstring& ִ��״̬
	*/
	void GetStatus(std::wstring& wstrStatus)
	{
		std::string strStatus = "";
		GetStatus(strStatus);

		BufferString::std_string_to_wstring(wstrStatus, strStatus);

		return;
	}

	/*!
	 * @brief ����ִ����
	 * @param unsigned char ��ִ���߱��
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetExecuter(unsigned char byExecuter)
	{
		if (m_byExecuter == byExecuter)
		{
			return false;
		}

		m_byExecuter = byExecuter;

		return true;
	}

	/*!
	 * @brief ��ȡִ����
	 * @return unsigned char ִ���߱��
	*/
	unsigned char GetExecuter() const
	{
		return m_byExecuter;
	}

	/*!
	 * @brief ��ȡִ����
	 * @param string& ִ���߱��
	*/
	void GetExecuter(std::string& strExecuter)
	{
		if (m_byExecuter == 0)
		{
			strExecuter = "��";
		}
		else
		{
			BufferString::std_string_format(strExecuter, "%d", m_byExecuter);
		}

		return;
	}

	/*!
	 * @brief ��ȡִ����
	 * @param wstring& ִ���߱��
	*/
	void GetExecuter(std::wstring& wstrExecuter)
	{
		std::string strExecuter = "";
		GetExecuter(strExecuter);

		BufferString::std_string_to_wstring(wstrExecuter, strExecuter);

		return;
	}

	/*!
	 * @brief ������������
	 * @param unsigned short ����������
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetTarget(unsigned short usTarget)
	{
		if (usTarget == 0)
		{
			return false;
		}

		if (m_usTarget == usTarget)
		{
			return false;
		}

		m_usTarget = usTarget;

		return true;
	}

	/*!
	 * @brief ��ȡ��������
	 * @return unsigned short ��������
	*/
	unsigned short GetTarget() const
	{
		return m_usTarget;
	}

	/*!
	 * @brief ��ȡ��������
	 * @param string ��������
	*/
	void GetTarget(std::string& strTarget)
	{
		BufferString::std_string_format(strTarget, "%d", m_usTarget);

		return;
	}

	/*!
	 * @brief ��ȡ��������
	 * @param wstring ��������
	*/
	void GetTarget(std::wstring& wstrTarget)
	{
		std::string strTarget = "";
		GetTarget(strTarget);

		BufferString::std_string_to_wstring(wstrTarget, strTarget);

		return;
	}

	/*!
	 * @brief �����쳣��Ϣ
	 * @param string �쳣��Ϣ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetError(std::string strError)
	{
		if (strError == "")
		{
			return false;
		}

		if (m_strError == strError)
		{
			return false;
		}

		m_strError = strError;

		return true;
	}

	/*!
	 * @brief �����쳣��Ϣ
	 * @param wstring �쳣��Ϣ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetError(std::wstring wstrError)
	{
		std::string strError = "";
		BufferString::std_wstring_to_string(strError, wstrError);

		return SetError(strError);
	}

	/*!
	 * @brief ��ȡ�쳣��Ϣ
	 * @param string& �쳣��Ϣ
	*/
	void GetError(std::string& strError)
	{
		strError = m_strError;

		return;
	}

	/*!
	 * @brief ��ȡ�쳣��Ϣ
	 * @param wstring& �쳣��Ϣ
	*/
	void GetError(std::wstring& wstrError)
	{
		std::string strError = "";
		GetError(strError);

		BufferString::std_string_to_wstring(wstrError, strError);

		return;
	}

	/*!
	 * @brief ��ȡ�쳣��Ϣ
	 * @return string �쳣��Ϣ
	*/
	std::string GetError() const
	{
		return m_strError;
	}

	/*!
	 * @brief ������ʼ����
	 * @param unsigned short ����ʼ����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetBegin(unsigned short usBegin)
	{
		if (m_usBegin == usBegin)
		{
			return false;
		}

		m_usBegin = usBegin;

		return true;
	}

	/*!
	 * @brief ��ȡ��ʼ����
	 * @return unsigned short ��ʼ����
	*/
	unsigned short GetBegin() const
	{
		return m_usBegin;
	}

	/*!
	 * @brief ��ȡ��ʼ����
	 * @param string& ��ʼ����
	*/
	void GetBegin(std::string& strBegin)
	{
		if (m_usBegin == 0)
		{
			strBegin = "�ɵ���ϵͳ����AGV";
		}
		else
		{
			BufferString::std_string_format(strBegin, "%d", m_usBegin);
		}

		return;
	}

	/*!
	 * @brief ��ȡ��ʼ����
	 * @param wstring& ��ʼ����
	*/
	void GetBegin(std::wstring& wstrBegin)
	{
		std::string strBegin = "";
		GetBegin(strBegin);

		BufferString::std_string_to_wstring(wstrBegin, strBegin);

		return;
	}

	/*!
	 * @brief ��ȡ����״̬
	 * @param string ����״̬
	 * @return unsigned char ״̬��
	*/
	static unsigned char GetStatusCode(std::string strStatus)
	{
		if (strStatus == "δִ������")
		{
			return Task::_TASKSTA_NONE;
		}
		else if (strStatus == "ִ��������")
		{
			return Task::_TASKSTA_EXECUTE;
		}
		else if (strStatus == "�������,�ȴ�ִ����һ�׶�����" || strStatus == "�����������" || strStatus == "�������")
		{
			return Task::_TASKSTA_FINISH;
		}
		else if (strStatus == "������ͣ")
		{
			return Task::_TASKSTA_PAUSE;
		}
		else if (strStatus == "����ȡ��,������һ�׶�" || strStatus == "����ȡ��")
		{
			return Task::_TASKSTA_CANCEL;
		}

		return 0xFF;
	}

	/*!
	 * @brief ��ȡ����״̬
	 * @param wstring ����״̬
	 * @return unsigned char ״̬��
	*/
	static unsigned char GetStatusCode(std::wstring wstrStatus)
	{
		std::string strStatus = "";
		BufferString::std_wstring_to_string(strStatus, wstrStatus);

		return GetStatusCode(strStatus);
	}

	/*!
	 * @brief ��ȡ״̬
	 * @param unsigned char ״̬��
	 * @param string& ״̬
	*/
	static void GetStatus(unsigned char byStatus, std::string& strStatus)
	{
		switch (byStatus)
		{
		case Task::_TASKSTA_NONE:
			strStatus = "δִ������";
			break;
		case Task::_TASKSTA_EXECUTE:
			strStatus = "ִ��������";
			break;
		case Task::_TASKSTA_FINISH:
			strStatus = "�������";
			break;
		case Task::_TASKSTA_PAUSE:
			strStatus = "������ͣ";
			break;
		case Task::_TASKSTA_CANCEL:
			strStatus = "����ȡ��";
			break;
		default:
			strStatus = "δ֪����״̬";
			break;
		}

		return;
	}

	/*!
	 * @brief ��ȡ״̬
	 * @param unsigned char ״̬��
	 * @param wstring& ״̬
	*/
	static void GetStatus(unsigned char byStatus, std::wstring& wstrStatus)
	{
		std::string strStatus = "";
		GetStatus(byStatus, strStatus);

		return BufferString::std_string_to_wstring(wstrStatus, strStatus);
	}

#ifdef _AFX
	/*!
	 * @brief ��ȡ�����
	 * @param CString& �����
	*/
	void GetNo(CString& cstrNo)
	{
		std::string strNo = "";
		GetNo(strNo);

		BufferString::std_string_to_CString(cstrNo, strNo);

		return;
	}

	/*!
	 * @brief ��ȡ�׶κ�
	 * @oaram CString& �׶κ�
	*/
	void GetLevel(CString& cstrLevel)
	{
		std::string strLevel = "";
		GetLevel(strLevel);

		BufferString::std_string_to_CString(cstrLevel, strLevel);

		return;
	}

	/*!
	 * @brief ���ö���
	 * @param CString ����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetAction(CString cstrAction)
	{
		std::string strAction = "";
		BufferString::CString_to_std_string(strAction, cstrAction);

		return SetAction(strAction);
	}

	/*!
	 * @brief ��ȡ����
	 * @param CString& ����
	*/
	void GetAction(CString& cstrAction)
	{
		std::string strAction = "";
		GetAction(strAction);

		BufferString::std_string_to_CString(cstrAction, strAction);

		return;
	}

	/*!
	 * @brief ��ȡ����
	 * @param CString& ִ��״̬
	*/
	void GetStatus(CString& cstrStatus)
	{
		std::string strStatus = "";
		GetStatus(strStatus);

		BufferString::std_string_to_CString(cstrStatus, strStatus);

		return;
	}

	/*!
	 * @brief ��ȡִ����
	 * @param CString& ִ���߱��
	*/
	void GetExecuter(CString& cstrExecuter)
	{
		std::string strExecuter = "";
		GetExecuter(strExecuter);

		BufferString::std_string_to_CString(cstrExecuter, strExecuter);

		return;
	}

	/*!
	 * @brief ��ȡ��������
	 * @param CString& cstrTarget
	*/
	void GetTarget(CString& cstrTarget)
	{
		std::string strTarget = "";
		GetTarget(strTarget);

		BufferString::std_string_to_CString(cstrTarget, strTarget);

		return;
	}

	/*!
	 * @brief �����쳣��Ϣ
	 * @param CString �쳣��Ϣ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetError(CString cstrError)
	{
		std::string strError = "";
		BufferString::CString_to_std_string(strError, cstrError);

		return SetError(strError);
	}

	/*!
	 * @brief ��ȡ�쳣��Ϣ
	 * @param CString& �쳣��Ϣ
	*/
	void GetError(CString& cstrError,COLORREF color)
	{
		std::string strError = "";
		GetError(strError);

		BufferString::std_string_to_CString(cstrError, strError);

		if (strError != _TASKERROR_A_NONE)
		{
			color = COLOR_RED;
		}

		return;
	}

	/*!
	 * @brief ��ȡ��ʼ����
	 * @param CString& ��ʼ����
	*/
	void GetBegin(CString& cstrBegin)
	{
		std::string strBegin = "";
		GetBegin(strBegin);

		BufferString::std_string_to_CString(cstrBegin, strBegin);

		return;
	}

	/*!
	 * @brief ��ȡ����״̬
	 * @param CString ����״̬
	 * @return unsigned char ״̬��
	*/
	static unsigned char GetStatusCode(CString cstrStatus)
	{
		std::string strStatus = "";
		BufferString::CString_to_std_string(strStatus, cstrStatus);

		return GetStatusCode(strStatus);
	}

	/*!
	 * @brief ��ȡ״̬
	 * @param unsigned char ״̬��
	 * @param CString& ״̬
	*/
	static void GetStatus(unsigned char byStatus, CString& cstrStatus)
	{
		std::string strStatus = "";
		GetStatus(byStatus, strStatus);

		return BufferString::std_string_to_CString(cstrStatus, strStatus);
	}

#endif // !_AFX
} AGVTask;

/*!< @brief ����AGVһ������Ļ����ṹ�� */
typedef struct _TaskArrayStruct
{
protected:
	unsigned short m_usNo;						/*!< ����� */
	std::map<unsigned char,AGVTask*> m_mapTask;	/*!< �����Ա�б� */

public:
	_TaskArrayStruct()
	{
		m_usNo = 0;
	}

	_TaskArrayStruct(unsigned short usNo)
	{
		m_usNo = usNo;
	}

	_TaskArrayStruct(unsigned short usNo, std::map<unsigned char, AGVTask*> mapTask)
	{
		m_usNo = usNo;
		m_mapTask = mapTask;
	}

	_TaskArrayStruct(const _TaskArrayStruct& taskArr)
	{
		m_usNo = taskArr.m_usNo;
		m_mapTask = taskArr.m_mapTask;
	}

	~_TaskArrayStruct()
	{
		Clear();
	}

	void operator=(const _TaskArrayStruct& taskArr)
	{
		m_usNo = taskArr.m_usNo;
		m_mapTask = taskArr.m_mapTask;
	}

public:
	/*!
	 * @brief �������Ƿ���Ч
	 * @return bool ��Ч����true,��Ч����false
	*/
	bool IsNull()
	{
		if (m_mapTask.size() > 0)
		{
			return false;
		}

		return true;
	}

	/*!
	 * @brief ���������
	 *
	 * ����������ųɹ�,��ʧȥԭ����ŵ���������
	 * @param unsigned short �������
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetNo(unsigned short usNo)
	{
		if (usNo == 0)
		{
			return false;
		}

		if (m_usNo == usNo)
		{
			return false;
		}

		Clear();

		m_usNo = usNo;

		return true;
	}

	/*!
	 * @brief ��ȡ�����
	 * @return unsigned short �����
	*/
	unsigned short GetNo() const
	{
		return m_usNo;
	}

	/*!
	 * @brief ��ȡ�����Ա�б�
	 * @return map<unsigend char,AGVTask*>&  �����Ա�б�
	*/
	std::map<unsigned char, AGVTask*>& GetTaskList() 
	{
		return m_mapTask;
	}

	/*!
	 * @brief �������
	 * @param AGVTask ����ṹ��
	 * @return bool ��ӳɹ�����true,ʧ�ܷ���false
	*/
	bool AddTask(AGVTask task)
	{
		unsigned short usNo = task.GetNo();	/*!< ����� */
		if (usNo == 0)
		{
			return false;
		}

		unsigned char byLevel = task.GetLevel();	/*!< �׶κ� */
		if (byLevel == 0)
		{
			return false;
		}

		if (m_usNo != usNo)
		{
			return false;
		}

		if (m_mapTask.find(byLevel) != m_mapTask.end())
		{
			return false;
		}

		m_mapTask[byLevel] = new AGVTask(task);

		return true;
	}

	/*!
	 * @brief ��ȡ����
	 * @param unsigned char ����׶κ�
	 * @return AGVTask* ����ָ��
	*/
	AGVTask* GetTask(unsigned char byLevel)
	{
		if (m_mapTask.find(byLevel) == m_mapTask.end())
		{
			return nullptr;
		}

		return m_mapTask[byLevel];
	}

	/*!
	 * @brief ��������
	 *
	 * �����������,���������
	 * @param AGVTask ����ṹ��
	 * @return bool ���³ɹ�����true,ʧ�ܷ���false
	*/
	bool UpdateTask(AGVTask task)
	{
		unsigned char byLevel = task.GetLevel();	/*!< �׶κ� */
		if (m_mapTask.find(byLevel) != m_mapTask.end())
		{
			return m_mapTask[byLevel]->Update(task);
			
		}
		else
		{
			return AddTask(task);
		}

		return true;
	}

	/*!
	 * @brief �������
	*/
	void Clear()
	{
		for (std::map<unsigned char, AGVTask*>::iterator it = m_mapTask.begin(); it != m_mapTask.end();)
		{
			delete it->second;
			it = m_mapTask.erase(it);
		}
	}
	
} AGVTaskArr;

/*!
 * @class CTaskManager
 * @brief �������������
 *
 * ��������������������Ե���
*/
class CTaskManager
{
public:
	CTaskManager();
	~CTaskManager();

protected:
	std::map<unsigned short, AGVTaskArr*> m_mapTaskList; /*!< �����б� */
	ADOConn m_ado;										 /*!< �������ݿ�ADO���� */
	std::thread* m_pTheadUpdate;						 /*!< ��������߳� */
	HANDLE m_hShutdownEvent;							 /*!< �ر��¼� */
	std::mutex m_mutexLock;								 /*!< ������ */
#ifdef _AFX
	HWND m_hParent;										 /*!< �����ھ�� */
#endif // _AFX

public:
	/*!
	 * @brief ����������
	 * @param const ADOConn �������ݿ�ADO����
	 * @return bool �����ɹ�����true,ʧ�ܷ���false
	*/
	bool OpenManager(const ADOConn ado);

#ifdef _AFX
	/*!
	 * @brief ����������
	 * @param const ADOConn �������ݿ�ADO����
	 * @return bool �����ɹ�����true,ʧ�ܷ���false
	*/
	bool OpenManager(const ADOConn ado, HWND hParent);
#endif // _AFX

	/*!
	 * @brief �رչ�����
	*/
	void CloseManager();

protected:
	/*!
	 * @brief ������������߳�
	*/
	void StartUpdateThread();

	/*!
	 * @brief �ر���������߳�
	*/
	void EndUpdateThread();

	/*!
	 * @brief ��������߳�
	*/
	void UpdateThread();

public:
	/*!
	 * @brief ��ȡ����
	 * @param unsigned short �����
	 * @param unsigend char ����׶κ�
	 * @return AGVTask ����
	*/
	AGVTask Get(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief ��ȡ�����Ա�б�
	 * @param unsigned short �����
	 * @return AGVTaskArr �����Ա�б�
	*/
	AGVTaskArr GetArray(unsigned short usNo);

	/*!
	 * @brief ��ȡ�����б�
	 * @param map<unsigned short, AGVTaskArr> �����б�
	*/
	std::map<unsigned short, AGVTaskArr> GetList();

	/*!
	 * @brief ��������
	 * @param AGVTask ����ṹ��
	 * @return bool �����ɹ�����true,ʧ�ܷ���false
	*/
	bool Publish(AGVTask task);

	/*!
	 * @brief ��������
	 * @param AGVTask ����ṹ��
	 * @return bool ���³ɹ�����true,ʧ�ܷ���false
	*/
	bool Update(AGVTask task);

	/*!
	 * @brief ɾ������
	 * @param unsigned short �����
	 * @return bool ɾ���ɹ�����true,ʧ�ܷ���false
	*/
	bool Delete(unsigned short usNo);

	/*!
	 * @brief �༭����
	 * @param AGVTask ����ṹ��
	 * @return bool �༭�ɹ�����true,ʧ�ܷ���false
	 */
	bool Edit(AGVTask task);

	/*!
	 * @brief ȡ������
	 * @param unsigned short �����
	 * @param unsigned char �׶κ�
	 * @return bool ȡ���ɹ�����true,ʧ�ܷ���false
	*/
	bool Cancel(unsigned short usNo,unsigned char byLevel);

	/*!
	 * @breif ��ͣ����
	 * @param unsigned short �����
	 * @param unsigned char �׶κ�
	 * @return bool ��ͣ�ɹ�����true,ʧ�ܷ���false
	*/
	bool Pause(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief ǿ���������
	 * @param unsigned short �����
	 * @param unsigned char �׶κ�
	 * @return bool ǿ����ɳɹ�����true,ʧ�ܷ���false
	*/
	bool Finish(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief ��������
	 * @param unsigned short �����
	 * @param unsigned char �׶κ�
	 * @return bool ǿ����ɳɹ�����true,ʧ�ܷ���false
	*/
	bool Reset(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief ȡ��ȫ������
	 * @return bool ȡ���ɹ�����true,ʧ�ܷ���false
	*/
	bool CancelAll();

	/*!
	 * @brief ��ͣȫ������
	 * @return bool ��ͣ�ɹ�����true,ʧ�ܷ���false
	*/
	bool PauseAll();

	/*!
	 * @brief ����ȫ������
	 * @return bool ǿ����ɳɹ�����true,ʧ�ܷ���false
	*/
	bool ResetAll();

	/*!
	 * @brief ��������
	 *
	 * �������������ָ�������ǰ
	 * @param unsigned short ����������
	 * @param unsigned char �����׶κ�
	 * @param AGVTask ������
	 * @return bool ����ɹ�����true,ʧ�ܷ���false
	*/
	bool Insert(unsigned short usNo, unsigned char byLevel, AGVTask newTask);

	/*!
	 * @brief �������
	 * @param AGVTask ������
	 * @return bool ��ӳɹ�����true,ʧ�ܷ���false
	*/
	bool Add(AGVTask newTask);

	/*!
	 * @brief ��ȡ��ǰ��������
	 * @return unsigned short ��ǰ�������� 
	*/
	unsigned short GetCurMaxNo();
protected:

	/*!
	 * @brief ��ȡ����
	 * @param unsigned short �����
	 * @param unsigend char ����׶κ�
	 * @return AGVTask* ����ָ��
	*/
	AGVTask* GetTask(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief ��ȡ�����Ա�б�
	 * @param unsigned short �����
	 * @return AGVTaskArr* �����Ա�б�ָ��
	*/
	AGVTaskArr* GetTaskArray(unsigned short usNo);

	/*!
	 * @brief ��ȡ�����б�
	 * @param map<unsigned short, AGVTaskArr*>& �����б�
	*/
	std::map<unsigned short, AGVTaskArr*>& GetTaskList();

	/*!
	 * @brief ��������
	 * @param AGVTask ����ṹ��
	 * @return bool �����ɹ�����true,ʧ�ܷ���false
	*/
	bool PublishTask(AGVTask task);

	/*!
	 * @brief ��������
	 * @param AGVTask ����ṹ��
	 * @return bool ���³ɹ�����true,ʧ�ܷ���false
	*/
	bool UpdateTask(AGVTask task);

	/*!
	 * @brief ɾ������
	 * @param unsigned short �����
	*/
	void DeleteTask(unsigned short usNo);

	/*!
	 * @brief ������������
	*/
	void Clear();
};

#endif // !_TASK_H