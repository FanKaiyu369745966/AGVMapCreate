/*!
 * @file CAGV.h
 * @brief ����TCPͨ�š��ŵ�����AGV�����ļ�
 *
 * ���ڴ����ŵ���AGV��ģ���ļ�
 * @author FanKaiyu
 * @date 2018-11-10
 * @version 2.0
*/

#pragma once
#include "CAGV.h"
#include "CMagneticTrackNavigation.h"
#include "CTCPClient.h"
#include "CProcessData.h"

#ifdef _AGV_H

/*!
 * @class CTCPMNavAGV
 * @brief ����TCPͨ�š��ŵ�����AGV
 *
 * �����˴ŵ���AGV�Ŀ�ִ�ж���
*/
class CTCPMNavAGV :
	public CAGV,public CTCPClient,public CMagneticTrackNavigation, public CProcessData
{
public:
	CTCPMNavAGV(unsigned char byNo, unsigned char byType, unsigned char byDriver, float fMaxSpeed, std::string strIP,  short sPort = -1, unsigned char byDevType = 0xd1);
	CTCPMNavAGV(unsigned char byNo, unsigned char byType, unsigned char byDriver, float fMaxSpeed, std::wstring wstrIP,  short sPort = -1, unsigned char byDevType = 0xd1);
#ifdef _AFX
	CTCPMNavAGV(unsigned char byNo, unsigned char byType, unsigned char byDriver, float fMaxSpeed, CString cstrIP,  short sPort = -1, unsigned char byDevType = 0xd1);
#endif // !_AFX
	~CTCPMNavAGV();

protected:
	std::list<BufferString::Buffer> m_listSend;	/*!< ���ݷ����б� */
	std::thread *m_pThreadSend;					/*!< ���ݷ����߳� */
	std::mutex m_mutexLock;						/*!< ������ */
	HANDLE m_hShutdownEvent;					/*!< �ر��¼� */

protected:
	/*!
	 * @brief �������ݰ�
	 *
	 * �����ݰ��������ݰ������б�
	 * @param Buffer ���ݰ�
	*/
	void SendPackage(BufferString::Buffer bfPackage);

	/*!
	 * @brief �������ݰ�
	 *
	 * �����ݰ��������ݰ������б�
	 * @param byte* ���ݰ��׵�ַ
	 * @param unsigend int ���ݰ�����
	*/
	void SendPackage(byte *byPackage,unsigned int unPackageLen);

	/*!
	 * @brief ���������߳�
	*/
	void StartSendThread();

	/*!
	 * @brief �رշ����߳�
	*/
	void EndSendThread();

	/*!
	 * @brief �����߳�
	*/
	void SendThread();

	/*!
	 * @brief ����
	*/
	void Heartbeat();

public:
	/*!
	 * @brief �ƶ�
	 * @param unsigned short ��ֹRFID�����
	*/
	void MoveTo(unsigned short usRFID);

	/*!
	 * @brief ��ͨ���Ʒ���
	*/
	void TrafficPass();

	/*!
	 * @brief ��ͨ����ֹͣ
	 * @param unsigned short ����RFID�����
	*/
	void TrafficStop(unsigned short usRFID);

	/*!
	 * @brief ����
	*/
	void Load();

	/*!
	 * @brief ����
	*/
	void Unload();

	/*!
	 * @brief ��ͣ
	*/
	void Scream();

	/*!
	 * @brief ��λ
	*/
	void Restart();

	/*!
	 * @brief ����
	*/
	void Reset();

	/*!
	 * @brief �����ƶ�
	 * @param unsigned short ��ֹRFID�����
	*/
	void Remove(unsigned short usRFID);

	/*!
	 * @brief ֹͣ����
	*/
	void StopAction();

	/*!
	 * @brief ��������
	 * @param unsigned short ������
	*/
	void Action(unsigned short usAction);

	/*!
	 * @brief ��������
	 *
	 * Զ�̿���AGVִ�ж����ı���ָ��
	 * @param unsigned short ��ǰRFID�������Ϣ
	 * @param unsigned short ִ�ж�����RFID�������Ϣ
	 * @param unsigned short ������
	 * @param unsigend char ����״̬
	*/
	void Action(unsigned short usCurRFID,unsigned short usRFID, unsigned short usAction, unsigned char byActionStatus = _AGVACTSTA_NONE);

	/*!
	 * @brief �ػ�
	 *
	 * Զ�̿���AGV�ϵ�ػ��ı���ָ��
	*/
	void Shutdown();

	/*!
	 * @brief �ֶ�����
	 *
	 * ����AGV��ǰRFID����š�״̬��Ϣ
	 * @param unsigned short ��ǰRFID�������Ϣ
	 * @param unsigned char ״̬��Ϣ
	*/
	void ManualSet(unsigned short usRFID,unsigned char byStatus);

public:	
	/*!
	 * @brief ���ӿͻ���
	 * @param SOCKET �ͻ���socket������
	 * @return bool ���ӳɹ�����ture,ʧ�ܷ���false
	*/
	bool Connect(SOCKET socket);

	/*!
	 * @brief ���ӷ����
	 * @param string �����IP
	 * @param unsigned short �˿�
	 * @return bool ���ӳɹ�����ture,ʧ�ܷ���false
	*/
	bool Connect(std::string strIP, unsigned short usPort);

	/*!
	 * @brief ���ӷ����
	 * @param wstring �����IP
	 * @param unsigned short �˿�
	 * @return bool ���ӳɹ�����ture,ʧ�ܷ���false
	*/
	bool Connect(std::wstring wstrIP, unsigned short usPort);

#ifdef _AFX
	/*!
	 * @brief ���ӷ����
	 * @param CString �����IP
	 * @param unsigned short �˿�
	 * @return bool ���ӳɹ�����ture,ʧ�ܷ���false
	*/
	bool Connect(CString cstrIP, unsigned short usPort);
#endif // !_AFX

	/*!
	 * @brief �ж�����
	*/
	void Break();

	/*!
	 * @brief ��ȡ����״̬
	 * @return bool �����ӷ���true,δ���ӷ���false
	*/
	bool IsConnected() const;

	/*!
	 * @brief ��ȡ����״̬
	 * @param string& ����״̬
	*/
	void IsConnected(std::string& strConnect);

	/*!
	 * @brief ��ȡ����״̬
	 * @param wstring& ����״̬
	*/
	void IsConnected(std::wstring& wstrConnect);

#ifdef _AFX
	/*!
	 * @brief ��ȡ�ͻ�������״̬
	 * @param CString& �ͻ�������״̬
	 * @param COLORREF& ������ʾ�û�����ɫ
	*/
	void IsConnected(CString& cstrConnect, COLORREF& color);
#endif // !_AFX
};

#endif // !_AGV_H