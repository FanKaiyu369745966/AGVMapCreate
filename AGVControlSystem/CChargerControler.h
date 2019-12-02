/*!
 * @file CChargerControler.h
 * @brief ����TCPͨ�ŵĳ����������ļ�
 *
 * �����˳��������Ļ���������Ϣ
 * @author FanKaiyu
 * @date 2019-02-14
 * @version 1.0
*/

#pragma once
#include "CTCPClient.h"
#include "CProcessData.h"
#include <list>

#ifndef _CHARGERCONTROLER_H
#define _CHARGERCONTROLER_H

/*!
 * @class CTCPMNavAGV
 * @brief ����TCPͨ�ŵĳ���������
 *
 * �����˳��������Ļ���������Ϣ
*/
class CChargerControler :
	public CTCPClient, CProcessData
{
public:
	CChargerControler(unsigned char byNo,std::string strIP,short sPort = -1);
	CChargerControler(unsigned char byNo, std::wstring wstrIP, short sPort = -1);
#ifdef _AFX
	CChargerControler(unsigned char byNo, CString cstrIP, short sPort = -1);
#endif
	~CChargerControler();
protected:
	unsigned char m_byNo;	/*!< ��� */

protected:
	std::list<BufferString::Buffer> m_listSend;	/*!< ���ݷ����б� */
	std::thread *m_pThreadSend;					/*!< ���ݷ����߳� */
	std::mutex m_mutexLock;						/*!< ������ */
	HANDLE m_hShutdownEvent;					/*!< �ر��¼� */

public:
	/*!
	 * @brief ���ñ��
	 * @param unsigned char ���������±��
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetNo(unsigned char byNo);

	/*!
	 * @brief ��ȡ���
	 * @return unsigned char �����������
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief ��ȡ���
	 * @param string& �����������
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief ��ȡ���
	 * @param wstring& �����������
	*/
	void GetNo(std::wstring& wstrNo);

#ifdef _AFX
	/*!
	 * @brief ��ȡ���
	 * @param CString& �����������
	*/
	void GetNo(CString& cstrNo);
#endif

public:
	/*!
	 * @brief ���Ƴ�������
	 * @param unsigned char ��������
	*/
	void Charge(unsigned char byNo);

	/*!
	 * @brief ���Ƴ�����ϵ�
	 * @param unsigned char ��������
	*/
	void Uncharge(unsigned char byNo);

	/*!
	 * @brief ��ȡ�����״̬
	 * @param unsigend char ��������
	*/
	void GetChargerStatus(unsigned char byNo);

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
	void SendPackage(byte *byPackage, unsigned int unPackageLen);

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

#endif // !_CHARGERCONTROLER_H