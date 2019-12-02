/*!
 * @file CService.h
 * @brief CService��ͷ�ļ�
 * @author FanKaiyu
 * @date 2018-11-20
 * @version 3.0
*/

#pragma once
#include <thread>
#include <map>
#include <list>
#include <mutex>
#include "CTcpServer.h"
#include "CTcpClient.h"
#include "CCA.h"
#include "CArm.h"
#include "CPipeline.h"
#include "CStocker.h"
#include "LEDCtrl.h"
#include "Xml.h"
#include "ADOConn.h"
#include "Task.h"
#include "Cargo.h"

#ifndef _SERVICE_H
#define _SERVICE_H

/*!
 * @class CService
 * @brief ����ҵ���߼�������
 *
 * ҵ����Ȳ���
*/
class CService
{
public:
	CService();
	~CService();

protected:
	CTcpServer *m_pTcpServer;						/*!< ����� */
	list<CTcpClient*> m_listTcpClients;				/*!< �ͻ��������б� */
	CCA *m_pCA;										/*!< CA����ָ�� */
	CArm *m_pArm;									/*!< ��е��ָ�� */
	CStocker *m_pStocker;							/*!< �Ѷ��ָ�� */
	map<unsigned int, CPipeline*> m_mapPipeline;	/*!< ��������ָ�� */
	CLEDCtrl m_led;									/*!< LED��ʾ�� */

protected:
	thread *m_pThreadService;						/*!< ҵ���߳�ָ�� */
	HANDLE m_hShutdownEvent;						/*!< �ر��¼� */
	mutex m_lock;									/*!< ������ */
	ADOConn m_adoDB;								/*!< ���ݿ�ADO���Ӵ� */
	string m_strLed[10];							/*!< LED����ı� */

public:
	unsigned int m_unNo;							/*!< ���ܺ� */
	unsigned int m_unRow;							/*!< �к� */
	unsigned int m_unCol;							/*!< �к� */

public:
	/*!
	 * @brief ����ҵ����
	*/
	bool Open();

	/*!
	 * @brief �ر�ҵ����
	*/
	void Close();

	/*!
	 * @brief �Զ�����
	 * @param unsigned int ���ܺ�
	 * @param unsigned int ������
	 * @param unsigned int ������
	 * @return bool ����ɹ�����true,ʧ�ܷ���false
	*/
	bool AutoGetOut(unsigned int unNo, unsigned int unRow, unsigned int unCol);

	/*!
	 * @brief ����
	 * @param unsigned int ���ܺ�
	 * @param unsigned int ������
	 * @param unsigned int ������
	 * @return bool ����ɹ�����true,ʧ�ܷ���false
	*/
	bool GetOut(unsigned int unNo, unsigned int unRow, unsigned int unCol);

	/*!
	 * @brief ���
	 *
	 * ����Ŀ�������ֶ����ķ�ʽ�����������˹����������ϣ���ͨ�������¼�����ݿ���
	 * @param unsigned int ���ܺ�
	 * @param unsigned int ������
	 * @param unsigned int ������
	 * @return bool ���ɹ�����true,ʧ�ܷ���false
	*/
	bool PutIn(unsigned int unNo, unsigned int unRow, unsigned int unCol);

protected:
	/*!
	 * @biref ��ȡ������Ϣ
	 * @param wstring �����ļ�·��
	*/
	void LoadConfig(wstring wstrPath);

	/*!
	 * @brief �ͻ�������
	*/
	void ClientConnect();

	/*!
	 * @brief ������õ�TCP�ͻ�������
	*/
	void ClearFreeClient();

	/*!
	 * @brief �����߳�
	*/
	void StartThread();

	/*!
	 * @brief �ر��߳�
	*/
	void CloseThread();

	/*!
	 * @brief ҵ���߳�
	*/
	void ServiceThread();

	/*!
	 * @brief ������
	*/
	void ProcessTask();

	/*!
	 * @brief �����������
	 * @param _WMSTASK& WMS����ṹ��
	*/
	void ProcessTaskOut(_WMSTASK &task);

	/*!
	 * @brief �����������
	 * @param _WMSTASK& WMS����ṹ��
	*/
	void ProcessTaskIn(_WMSTASK &task);

	/*!
	 * @brief ��LED��Ļ
	*/
	void OpenScreen();

	/*!
	 * @brief �ر�LED��Ļ
	*/
	void CloseScreen();

	/*!
	 * @brief �����ı���LED
	 * @param string �ı���Ϣ
	*/
	void SendToLED(string strText);

	/*!
	 * @brief ��ȡ������Ϣ
	*/
	void GetCallMsg();
};

#endif // !_SERVICE_H