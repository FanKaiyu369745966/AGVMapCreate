#pragma once
#include <map>

/*!
 * ��ѡ����
*/
#include "CMagneticTrackNavigation.h"
#include "CTCPMNavAGV.h"
#include "CMNavStation.h"
#include "CMNavRestPosition.h"
#include "CTCPServer.h"
#include "ADO/ADOConn.h"
#include "AES.h"

/*!
 * ��ѡ����
*/
#include "CMNavCharger.h"
#include "CCA.h"
#include "CChargerControler.h"

/*!
 * ҵ������
*/
#include "CTaskManager.h"

#ifndef _SERVICE_H
#define _SERVICE_H

#define _SELECT_FUNC			/*!< ȫ����ѡ���� */
#define _SELECT_AUTO_GOUP		/*!< �Զ���λ */

#ifdef _AFX
#define WM_SERVICE_UPDATE	WM_USER+3
#define ID_DATA_AGV 1
#endif //_AFX

class CService
{
public:
	CService();
	~CService();

protected:
	/*!
	 * ͨ������
	*/
	AES m_aes;															/*!< AES���ܹ� */
	CTCPServer *m_pServer;												/*!< TCP����� */
	std::thread *m_pThreadFunc;											/*!< �����߳�ָ�� */
	HANDLE m_hShutdownEvent;											/*!< �ر��¼� */
	std::mutex m_mutexLock;												/*!< ������ */
	ADOConn m_adoDB;													/*!< ADO���ݿ����� */
	bool m_bPermission;													/*!< �û�ʹ��Ȩ�� */
#ifdef _AFX
	HWND m_hParent;														/*!< ���ھ�� */
#endif // _AFC

#ifdef _SELECT_FUNC
protected:
#ifdef _CHARGERCONTROLER_H
	std::map<unsigned char, CChargerControler*> m_mapChargerCtrlers;	/*!< ���������� */
#endif // !_CHARGERCONTROLER_H

#ifdef _CA_H
	std::map<unsigned short, CCA*> m_mapCAs;							/*!< ���ߺ�������(����èCA����)�� */
	std::map<unsigned char, CAWatch*> m_mapCAWatchs;					/*!< ����(����è)�ֱ�ṹ���� */
#endif // !_CA_H

#ifdef _LED_H
	/*!< LED�� */
#endif // !_LED_H

#endif // !_SELECT_FUNC

protected:
	/*!
	 * ͨ�ù���
	*/
	/*!
	 * @brief �������ò���
	*/
	void DownLoadConfig();

	/*!
	 * @brief �ϴ����ò���
	*/
	void UpLoadConfig();

	/*!
	 * @brief ����XML�е����ò���
	*/
	void LoadXMLConfig();

	/*!
	 * @brief �������ݿ��е����ò���
	*/
	void LoadDatabaseConfig();

	/*!
	 * @brief �豸����
	*/
	void Connect();

	/*!
	 * @brief �豸�ж�
	*/
	void Disconnect();

	/*!
	 * @brief �����߳�
	*/
	void FunctionThread();

	/*!
	 * @brief �����߳�
	*/
	void StartThread();

	/*!
	 * @brief �����߳�
	*/
	void EndThread();

	/*!
	 * @brief ��������
	*/
	void DestroyData();

#ifdef _SELECT_FUNC
protected:
	/*!
	 * ͨ�ÿ�ѡ����
	*/

#ifdef _CA_H
	/*!
	 * @brief ���������Ϣ
	*/
	void ProcessCallMsg();

	/*!
	 * @brief ��Ϣ֪ͨ
	 * @param string ��Ϣ�ı�
	 * @param unsigned char ��λ���
	*/
	void SendMsgToWatch(std::string strMsg,unsigned char byStation);

	/*!
	 * @brief ��Ϣ֪ͨ
	 * @param wstring ��Ϣ�ı�
	 * @param unsigned char ��λ���
	*/
	void SendMsgToWatch(std::wstring wstrMsg, unsigned char byStation);

#ifdef _AFX
	/*!
	 * @brief ��Ϣ֪ͨ
	 * @param CString ��Ϣ�ı�
	 * @param unsigned char ��λ���
	*/
	void SendMsgToWatch(CString cstrMsg, unsigned char byStation);
#endif // _AFX
#endif // !_CA_H

#ifdef _LED_H
	/*!
	 * @brief ����ı���LED
	 * @param string ��Ϣ�ı�
	 * @param unsigned char LED���
	*/
	void SendMsgToLED(std::string strMsg, unsigned char byLed);

	/*!
	 * @brief ����ı���LED
	 * @param wstring ��Ϣ�ı�
	 * @param unsigned char LED���
	*/
	void SendMsgToLED(std::wstring strMsg, unsigned char byLed);

#ifdef _AFX
	/*!
	 * @brief ����ı���LED
	 * @param CString ��Ϣ�ı�
	 * @param unsigned char LED���
	*/
	void SendMsgToLED(CString cstrMsg, unsigned char byLed);
#endif // _AFX

	/*!
	 * @brief LED��ʾ
	*/

#endif // !_LED_H

#ifdef _CHARGERCONTROLER_H
	/*!
	 * @brief ���������������
	 *
	 * ���Ƴ���������ʱ��ȡ�����״̬
	 * ��������������ص���Ϣ,�����³����״̬
	*/
	void ProcessChargerControlerData();

#endif // !_CHARGERCONTROLER_H

#endif // !_SELECT_FUNC

public:
	/*!
	 * ���ݸ���
	*/

	/*!
	 * @brief AGVʵʱ״̬����
	*/

	/*!
	 * @brief �豸״̬����
	*/

	/*!
	 * @brief �����״̬����
	*/

	/*!
	 * @brief ����״̬����
	*/

public:
	/*!
	 * �������
	*/
	bool Open();
#ifdef _AFX
	/*!
	 * @brief ����
	 * @param HWND ������ָ��
	*/
	bool Open(const HWND hParent);
#endif
	/*!
	 * @brief �ر�
	*/
	void Close();

	/*!
	 * @brief ����
	*/
	void Destroy();

	/*!
	 * @brief ����Ա�����֤
	 * @param string ����Ա�ܳ�(����)
	 * @return bool ��֤�ɹ�����true,ʧ�ܷ���false
	*/
	bool GetAdmin(std::string strPassword);

	/*!
	 * @brief ����Ա�����֤
	 * @param wstring ����Ա�ܳ�(����)
	 * @return bool ��֤�ɹ�����true,ʧ�ܷ���false
	*/
	bool GetAdmin(std::wstring wstrPassword);

#ifdef _AFX
	/*!
	 * @brief ����Ա�����֤
	 * @param CString ����Ա�ܳ�(����)
	 * @return bool ��֤�ɹ�����true,ʧ�ܷ���false
	*/
	bool GetAdmin(CString cstrPassword);
#endif //_AFX

	/*!
	 * @brief ����Ա��������
	 * @param string �ɹ���Ա�ܳ�
	 * @param string �¹���Ա�ܳ�
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetAdmin(std::string strOldPassword,std::string strNewPassword);

	/*!
	 * @brief ����Ա��������
	 * @param wstring �ɹ���Ա�ܳ�
	 * @param wstring �¹���Ա�ܳ�
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetAdmin(std::wstring wstrOldPassword, std::wstring wstrNewPassword);
#ifdef _AFX

	/*!
	 * @brief ����Ա��������
	 * @param CSstring �ɹ���Ա�ܳ�
	 * @param CString �¹���Ա�ܳ�
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetAdmin(CString cstrOldPassword, CString cstrNewPassword);
#endif //_AFX


#ifdef _MAGNETICTRACK_H
	/*!
	 * @brief �Ź쵼��ר������
	*/
protected:
	/*!
	 * ��������
	*/
	std::map<unsigned char, CTCPMNavAGV*> m_mapAGVs;				/*!< AGV�� */
	std::map<unsigned short, CRFID*> m_mapRFIDs;					/*!< RFID���� */
	std::map<unsigned char, CMNavStation*> m_mapStations;			/*!< ��λ�� */
	std::map<unsigned char, CMNavRestPosition*> m_mapRests;			/*!< ����λ�� */
	std::map<unsigned short, CMTTrafficCtrlArea*> m_mapTraffic;		/*!< ��ͨ������*/
	CTaskManager m_TaskManager;										/*!< ��������� */

#ifdef _SELECT_FUNC
protected:
	/*!
	 * ��ѡ����
	*/
	std::map<unsigned char, CMNavCharger*> m_mapChargers;			/*!< ���λ�� */

#ifdef _CA_H
	std::map<unsigned short, CACall*> m_mapCACalls;					/*!< ����(����è)�������ṹ���� */
#endif // _CA_H

#endif // !_SELECT_FUNC

protected:
	/*!
	 * ��������
	*/

	/*!
	 * @brief ��ͨ����
	*/
	void TrafficControl();

	/*!
	 * @brief ����AGV״̬
	 * @param unsigned char AGV���
	 * @param unsigned char AGV��ǰ����
	 * @param char AGV��ǰ�ٶ�
	 * @param unsigned char AGV��ǰ״̬
	 * @param char AGV�쳣��Ϣ
	 * @param unsigned short AGV����
	 * @param unsigned char AGV����״̬
	 * @param unsigned char AGV�ػ�״̬
	 * @param unsigned short AGV��ǰ����
	 * @param unsigned short AGV��һ����
	*/
	void UpdateAGVStatus(unsigned char byNo,unsigned char byPower, char chSpeed, unsigned char byStatus, 
		char chError, unsigned short usAction, unsigned char byActStatus, unsigned char byCargo,
		unsigned short usRFID, unsigned short usNextRFID = 0);

	/*!
	 * @brief ����AGV�쳣
	 * @param unsigned char AGV���
	 * @param char AGV�쳣��Ϣ
	*/
	void UpdateAGVError(unsigned char byNo, char chError);

	/*!
	 * @brief ����AGV RFID��
	 * @param unsigned char AGV���
	 * @param unsigned short AGV��ǰRFID�����
	*/
	void UpdateAGVRFID(unsigned char byNo, unsigned short usRFID);

#ifdef _TASK_H
protected:
	/*!
	 * �������
	*/
	/*!
	 * @brief ������
	*/
	void ProcessTask();

	/*!
	 * @brief ��������
	*/

	/*!
	 * @brief ��������
	*/

	/*!
	 * @brief ɾ������
	*/

	/*!
	 * @brief ִ������
	*/

	/*!
	 * @brief ��ȡ��������
	*/

	/*!
	 * @brief ��ȡ������
	*/
#endif _TASK_H

#ifdef _SELECT_FUNC
protected:
	/*!
	 * ��ѡ��������
	*/
#ifdef _SELECT_AUTO_GOUP
	/*!
	 * @brief ������λ
	*/
	void RestPositionControl();

#endif // !_SELECT_AUTO_GOUP

#ifdef _CHARGER_H
	/*!
	 * @brief �Զ����
	*/
	void ChargerControl();

#endif // !_CHARGER_H

#ifdef _CA_H
	/*!
	 * @brief ��������
	 * @param unsigned int ���������
	 * @param string ��������Я����������Ϣ
	*/
	void PublishCallTask(unsigned int unNo, std::string strMsg);

	/*!
	 * @brief ��������
	 * @param unsigned int ���������
	 * @param wstring ��������Я����������Ϣ
	*/
	void PublishCallTask(unsigned int unNo, std::wstring strMsg);

#ifdef _AFX
	/*!
	 * @brief ��������
	 * @param unsigned int ���������
	 * @param cstring ��������Я����������Ϣ
	*/
	void PublishCallTask(unsigned int unNo, CString strMsg);
#endif // _AFX
#endif // _CA_H

#endif // !_SELECT_FUNC

public:
	/*!
	 * @brief �û�����
	*/

	/*!
	 * @brief ��ͣ
	 * @param unsigned char AGV���
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVScream(unsigned char byAGVNo);

	/*!
	 * @brief ��λ
	 * @param unsigned char AGV���
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVRestart(unsigned char byAGVNo);

	/*!
	 * @breif �ƶ�
	 * @param unsigned char AGV���
	 * @param unsigned short �ƶ��յ����꼴RFID�����
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVMove(unsigned char byAGVNo,unsigned short usRFIDNo);

	/*!
	 * @brief ����ֹͣ
	 * @param unsigned char AGV���
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVStopAction(unsigned char byAGVNo);

	/*!
	 * @brief ����
	 * @param unsigned char AGV���
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVLoad(unsigned char byAGVNo);

	/*!
	 * @brief ����
	 * @param unsigned char AGV���
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVUnload(unsigned char byAGVNo);

	/*!
	 * @brief ��������
	 * @param unsigned char AGV���
	 * @param unsigned short ������
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVAction(unsigned char byAGVNo, unsigned short usActionCode);

	/*!
	 * @brief ����
	 * @param unsigned char AGV���
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVReset(unsigned char byAGVNo);

	/*!
	 * @brief ���õ�ǰ������״̬
	 * @param unsigned char AGV���
	 * @param unsigned short ��ǰ����
	 * @param unsigned char ��ǰ״̬
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVSet(unsigned char byAGVNo, unsigned short usRFIDNo, unsigned char byStatus);

	/*!
	 * @brief �����ƶ�
	 * @param unsigned char AGV���
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVRemove(unsigned char byAGVNo);

	/*!
	 * @brief ��ͨ����ͨ��
	 * @param unsigned char AGV���
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVTrafficPass(unsigned char byAGVNo);

	/*!
	 * @brief �ֶ�ģʽ
	 * @param unsigned char AGV���
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVHandleMode(unsigned char byAGVNo);

	/*!
	 * @brief �Զ�ģʽ
	 * @param unsigned char AGV���
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVAutoMode(unsigned char byAGVNo);

	/*!
	 * @brief �ػ�
	 * @param unsigned char AGV���
	 * @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool AGVShutdown(unsigned char byAGVNo);

#ifdef _TASK_H
	/*!
	 * @brief ��ȡ������Ϣ
	 * @param unsigned short �����
	 * @param unsigned char �׶κ�
	 * @return AGVTask ����ṹ��
	*/
	AGVTask GetTask(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief �޸�������Ϣ
	 * @param unsigned short �����
	 * @param unsigned char �׶κ�
	 * @param AGVTask ����ṹ��
	 * return bool �޸ĳɹ�����true��ʧ�ܷ���false
	*/
	bool EditTask(unsigned short usNo, unsigned char byLevel,AGVTask newTask);

	/*!
	 * @brief ȡ������
	 * @param unsigned short �����
	 * @param unsigned char �׶κ�
	 * @return bool ȡ���ɹ�����true,ʧ�ܷ���false
	*/
	bool CancelTask(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief ǿ���������
	 * @param unsigned short �����
	 * @param unsigned char �׶κ�
	 * @return bool ȡ���ɹ�����true,ʧ�ܷ���false
	*/
	bool FinishTask(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief ��ͣ����
	 * @param unsigned short �����
	 * @param unsigned char �׶κ�
	 * @return bool ȡ���ɹ�����true,ʧ�ܷ���false
	*/
	bool PauseTask(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief ��������
	 * @param unsigned short �����
	 * @param unsigned char �׶κ�
	 * @return bool ȡ���ɹ�����true,ʧ�ܷ���false
	*/
	bool ResetTask(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief ȡ��ȫ������
	*/
	void CancelAllTasks();

	/*!
	 * @brief ��ͣȫ������
	*/
	void PauseAllTasks();

	/*!
	 * @brief ����ȫ������
	*/
	void ResetAllTasks();

	/*!
	 * @brief ��������
	 *
	 * �������������ָ�������ǰ
	 * @param unsigned short ����������
	 * @param unsigned char �����׶κ�
	 * @param AGVTask ������
	 * @return bool ����ɹ�����true,ʧ�ܷ���false
	*/
	bool InsertTask(unsigned short usNo,unsigned char byLevel,AGVTask newTask);

	/*!
	 * @brief �������
	 * @param AGVTask ������
	 * @return bool ��ӳɹ�����true,ʧ�ܷ���false
	*/
	bool AddTask(AGVTask newTask);
#endif // _TASK_H

#endif // !_MAGNETICTRACK_H
};

#endif //!_SERVICE_H