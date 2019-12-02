/*!
 * @file CCA.h
 * @brief ����è�豸�����ļ�
 * ������������èCA�����Ļ�������
 * ������������è���ߺ������Ļ�������
 * ������������è�����ֱ�Ļ�������
 *
 * @author FanKaiyu
 * @date 2019-01-24
 * @version 2.1
*/
#pragma once
#include <string>
#include <list>
#include <mutex>
#include <map>
#include <thread>
#include "CTCPClient.h"
#include "Buffer.h"
#include "CAGV.h"
#include "CTaskManager.h"

#define CA_MAX_CLIENT_ADDR 0x8999	/*!< �ͻ�������ַ */
#define CA_SERVER_ADDR 0x9999		/*!< ��������ַ */
#define CA_TCPCMD_LOGON 0x01		/*!< ע������� */
#define CA_TCPCMD_HEARTBEAT 0x02	/*!< ��������� */
#define CA_TCPCMD_DATACOMM 0x04		/*!< ����ͨ������� */
#define CA_USBCMD_SEND 0xb1			/*!< ������������� */
#define CA_USBCMD_UPLOAD_SEND 0xd1	/*!< �ϴ��ѷ��͵���������� */
#define CA_USBCMD_UPLOAD_CALL 0xd2	/*!< ��������������������� */
#define CA_USBCMD_UPLOAD_RECV 0xd4	/*!< �ϴ����յ���������� */

#ifndef _CA_H
#define _CA_H

/*!
 * @class CCA
 * @brief ����TCPͨ�ŵĶ���èCA������
 *
 * ������������èCA�����Ļ�������
*/
class CCA
	:public CTCPClient
{
public:
	CCA(unsigned short usNo, std::string strIP,short sPort = 0, std::string strText = "");
	CCA(unsigned short usNo, std::wstring wstrIP, short sPort = 0, std::wstring wstrText = L"");
#ifdef _AFX
	CCA(unsigned short usNo, CString cstrIP, short sPort = 0, CString strText = _T(""));
#endif // !_AFX
	~CCA();
protected:
	unsigned short m_usNo;						/*!< CA������:��ͨ��ʱΪĿ�ĵ�ַ,ȡֵ��ΧΪ0-8999 */
	std::string m_strText;						/*!< ��ע��Ϣ:ͨ������������CA��λ���Լ�ʵ������ */

protected:
	unsigned int m_unLoginCheck;				/*!< ע��У���� */
	unsigned int m_unDisTime;					/*!< ����ʱ����(s) */
	std::thread *m_pThreadHeart;				/*!< �����߳�ָ�� */
	HANDLE m_hShutdownEvent;					/*!< �ر��¼� */
	bool m_bHeart;								/*!< �������ͱ�ʶ */

protected:
	static std::list<std::string> g_listRecv;	/*!< �ѽ��յ���Ϣ�б� */
	static std::mutex g_lock;					/*!< ������ */

public:
	/*!
	 * @brief �����������
	 * @param unsigend short ���������
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetNo(unsigned short usNo);

	/*!
	 * @brief ��ȡ�������
	 * @return unsigned short �������
	*/
	unsigned short GetNo() const;

	/*!
	 * @brief ��ȡ�������
	 * @param string �������
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief ��ȡ�������
	 * @param wstring �������
	*/
	void GetNo(std::wstring& wstrNo);

	/*!
	 * @brief ���ñ�ע��Ϣ
	 * @param string �����±�ע��Ϣ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetText(std::string strText);

	/*!
	 * @brief ���ñ�ע��Ϣ
	 * @param wstring �����±�ע��Ϣ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetText(std::wstring wstrText);

	/*!
	 * @brief ��ȡ��ע��Ϣ
	 * @param string& ������ע��Ϣ
	*/
	void GetText(std::string& strText);

	/*!
	 * @brief ��ȡ��ע��Ϣ
	 * @param wstring& ������ע��Ϣ
	*/
	void GetText(std::wstring& wstrText);

#ifdef _AFX
	/*!
	 * @brief ��ȡ�������
	 * @param CString �������
	*/
	void GetNo(CString& cstrNo);

	/*!
	 * @brief ���ñ�ע��Ϣ
	 * @param CString �����±�ע��Ϣ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetText(CString cstrText);

	/*!
	 * @brief ��ȡ��ע��Ϣ
	 * @param CString& ������ע��Ϣ
	*/
	void GetText(CString& cstrText);
#endif //!_AFX

public:
	/*!
	 * @brief �����豸
	 * @param SOCKET �ͻ���SOCKET������
	 * @return bool ���ӳɹ�����true,ʧ�ܷ���false
	*/
	bool Connect(SOCKET socket);

	/*!
	 * @brief �ж����豸������
	*/
	void Break();

	/*!
	 * @brief ��ȡ����״̬
	 * @return boll �����ӷ���true,δ���ӷ���false
	*/
	bool IsConnected();

	/*!
	 * @brief ��ȡ���յ���Ϣ
	 * @return �ӽ�����Ϣ�б��з���1����Ϣ
	*/
	static std::string GetRecvMsg();

	/*!
	 * @brief ���ֱ�����Ϣ
	 * @param unsigned char Ŀ���ֱ���
	 * @param string �ı���Ϣ.�ı���ϢӦС��40���ֽ�.
	 * @return bool ���뷢���б���true,ʧ�ܷ���false
	*/
	bool SendToWatch(unsigned char byWatch, std::string strText);

	/*!
	 * @brief ���ֱ�����Ϣ
	 * @param unsigned int Ŀ���ֱ���
	 * @param wstring �ı���Ϣ.�ı���ϢӦС��40���ֽ�.
	 * @return bool ���뷢���б���true,ʧ�ܷ���false
	*/
	bool SendToWatch(unsigned char byWatch, std::wstring wstrText);

#ifdef _AFX
	/*!
	 * @brief ���ֱ�����Ϣ
	 * @param unsigned int Ŀ���ֱ���
	 * @param CString �ı���Ϣ.�ı���ϢӦС��40���ֽ�.
	 * @return bool ���뷢���б���true,ʧ�ܷ���false
	*/
	bool SendToWatch(unsigned char byWatch, CString cstrText);
#endif //!_AFX
protected:
	/*!
	 * @brief �ϳ�ע����
	 * @param unsigned int ���غϳɺ��ע����
	 * @param unsigned int ���غϳɺ��ע����У��ֵ
	*/
	void CreateLogonMessage(unsigned int& unLogin, unsigned int& unCheck);

	/*!
	 * @brief ����ע��
	*/
	void AskLogon();

	/*!
	 * @brief �ϳ�У����
	 * @param unsigned char* ��ҪУ������ݿ�
	 * @param unsigned int ��ҪУ������ݳ���
	 * @return unsigned short ���غϳɵ�У����Ϣ
	*/
	unsigned short CreateCheck(unsigned char* pData, unsigned int unLen);

	/*!
	 * @brief �ϳ�����ͨ�ű���
	 * @param unsigned char ������
	 * @param unsigned short Ŀ�ĵ�ַ
	 * @param unsigned char* ���ݰ�ָ��
	 * @param unsigned int ���ݰ�����
	 * @return Buffer �ϳɺ�ı��İ�
	*/
	BufferString::Buffer CreatePackage(unsigned char byCmd, unsigned short usTarget, unsigned char *pData, unsigned int unLen);

	/*!
	 * @brief ����
	 *
	 * ��������������CA����
	 * @param unsigned int �������ʱ�� 0-990
	*/
	void HeartBeat(unsigned int unDisTime);

	/*!
	 * @brief ����ת��
	 *
	 * ������ͷβ��
	 * ������������0x1A ת��Ϊ 0x1A + 0x01
	 * ������������0x1A ת��Ϊ 0x1A + 0x02
	 * @param Buffer& ��Ҫת��ı���
	 * @return Buffer& ת���ı���
	*/
	BufferString::Buffer Translate(BufferString::Buffer&);

	/*!
	 * @brief ���ķ�ת��
	 *
	 * ������ͷβ��
	 * ������������0x1A + 0x01 ת��Ϊ 0x1A
	 * ������������0x1A + 0x02 ת��Ϊ 0x1B
	 * @param Buffer ��Ҫ��ת��ı���
	 * @return Buffer ��ת���ı���
	*/
	BufferString::Buffer Analysis(BufferString::Buffer&);

	/*!
	 * @brief ���ݴ���
	 *
	 * �����Ĵӽ��յ�������ȡ������ʣ������ݷ���
	 * @param buffer ���յ�����
	 * @post buffer������ӦΪ������ʣ�������.
	*/
	void ProcessData(BufferString::Buffer &buffer);

	/*!
	 * @brief ���Ĵ���
	 *
	 * ���ݱ������ݽ������Ĺ���
	 * @param unsigned char* ��������ָ��
	 * @param unsigned int ���ĳ���
	*/
	void ProcessPackage(unsigned char* pData, unsigned int unLen);

	/*!
	 * @brief �����߳�
	*/
	void StartThread();

	/*!
	 * @brief �ر��߳�
	*/
	void CloseThread();

	/*!
	 * @brief ���ݴ����߳�
	 *
	 * ��ʱ��������
	*/
	void ProcessDataThread();
};

/*! @brief ����èCA�������ߺ������ṹ�� */
typedef struct _CACallStruct
{
	unsigned short m_usNo;			/*!< ��� */
	unsigned short m_usEndPoint;	/*!< �յ����� ��AGV�Ķ����յ�,�˲���Ϊ0ʱ��Ч */
	unsigned short m_usStartPoint;	/*!< ������� ��AGV�Ķ������,ͨ��ͨ���˲���ѡ�����Ǹ������ϵ�AGVִ�����񣬴˲���Ϊ0�����ɵ���ϵͳѡ��AGV */
	char m_cType;					/*!< AGV���� ��ѡ������͵�AGV �˲���Ϊ0 ������AGV���� */
	std::string m_strMsg;			/*!< Я����Ϣ �ַ�����Ϣ����������ʾҲ���Ը��ݵ���ϵͳ�����ո�ʽ��д */
	std::string m_strAction;		/*!< ������ �������󶨵������� */

	_CACallStruct()
	{
		m_usNo = 0;
		m_usEndPoint = 0;
		m_usStartPoint = 0;
		m_cType = 0;
		m_strMsg = "";
	}

	_CACallStruct(unsigned short usNo,unsigned short usEndPoint,unsigned short usStartPoint = 0, char cType = 0,std::string strAction = _TASKACTION_A_MOVE,std::string strMsg = "")
	{
		m_usNo = usNo;
		m_usEndPoint = usEndPoint;
		m_usStartPoint = usStartPoint;
		m_cType = cType;
		m_strAction = strAction;
		m_strMsg = strMsg;
	}

	_CACallStruct(unsigned short usNo, unsigned short usEndPoint, unsigned short usStartPoint = 0, char cType = 0, std::wstring wstrAction = _TASKACTION_W_MOVE, std::wstring wstrMsg = L"")
	{
		m_usNo = usNo;
		m_usEndPoint = usEndPoint;
		m_usStartPoint = usStartPoint;
		m_cType = cType;
		BufferString::std_wstring_to_string(m_strAction, wstrAction);
		BufferString::std_wstring_to_string(m_strMsg, wstrMsg);
	}

#ifdef _AFX
	_CACallStruct(unsigned short usNo, unsigned short usEndPoint, unsigned short usStartPoint = 0, char cType = 0, CString cstrAction = _TASKACTION_MOVE, CString cstrMsg = _T(""))
	{
		m_usNo = usNo;
		m_usEndPoint = usEndPoint;
		m_usStartPoint = usStartPoint;
		m_cType = cType;
		BufferString::CString_to_std_string(m_strAction, cstrAction);
		BufferString::CString_to_std_string(m_strMsg, cstrMsg);
	}
#endif

	_CACallStruct(const _CACallStruct& call)
	{
		m_usNo = call.m_usNo;
		m_usEndPoint = call.m_usEndPoint;
		m_usStartPoint = call.m_usStartPoint;
		m_cType = call.m_cType;
		m_strMsg = call.m_strMsg;
	}

	void operator=(const _CACallStruct& call)
	{
		m_usNo = call.m_usNo;
		m_usEndPoint = call.m_usEndPoint;
		m_usStartPoint = call.m_usStartPoint;
		m_cType = call.m_cType;
		m_strMsg = call.m_strMsg;
	}

	/*!
	 * @brief ���ñ��
	 * @param unsigned short �������±��
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
	 * @brief ��ȡ���
	 * @return unsigned short ���������
	*/
	unsigned short GetNo() const
	{
		return m_usNo;
	}

	/*!
	 * @brief ��ȡ���
	 * @param string& ���������
	*/
	void GetNo(std::string& strNo)
	{
		BufferString::std_string_format(strNo, "%d", m_usNo);

		return;
	}

	/*!
	 * @brief ��ȡ���
	 * @param wstring& ���������
	*/
	void GetNo(std::wstring& wstrNo)
	{
		std::string strNo = "";
		GetNo(strNo);

		BufferString::std_string_to_wstring(wstrNo, strNo);

		return;
	}

	/*!
	 * @brief �����յ�����
	 * @param unsigned short �յ�������
	 * @return bool ���óɹ�����true,ʧ�ܷ���falsed
	*/
	bool SetEndPoint(unsigned short usEndPoint)
	{
		if (usEndPoint == 0)
		{
			return false;
		}

		if (m_usEndPoint == usEndPoint)
		{
			return false;
		}

		m_usEndPoint = usEndPoint;

		return true;
	}

	/*!
	 * @brief ��ȡ�յ�����
	 * @return unsigned short �յ�����
	*/
	unsigned short GetEndPoint() const
	{
		return m_usEndPoint;
	}

	/*!
	 * @brief ��ȡ�յ�����
	 * @param string& �յ�����
	*/
	void GetEndPoint(std::string& strEndPoint)
	{
		BufferString::std_string_format(strEndPoint, "%d", m_usEndPoint);

		return;
	}

	/*!
	 * @brief ��ȡ�յ�����
	 * @param string& �յ�����
	*/
	void GetEndPoint(std::wstring& wstrEndPoint)
	{
		std::string strEndPoint = "";
		GetEndPoint(strEndPoint);

		BufferString::std_string_to_wstring(wstrEndPoint, strEndPoint);

		return;
	}

	/*!
	 * @brief �����������
	 * @param unsigned short ���������
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetStartPoint(unsigned short usStartPoint)
	{
		if (m_usStartPoint == usStartPoint)
		{
			return false;
		}

		m_usStartPoint = usStartPoint;

		return true;
	}

	/*!
	 * @brief ��ȡ�������
	 * @return unsigned short �������
	*/
	unsigned short GetStartPoint() const
	{
		return m_usStartPoint;
	}

	/*!
	 * @brief ��ȡ�������
	 * @param std::string& �������
	*/
	void GetStartPoint(std::string &strStartPoint)
	{
		if (m_usStartPoint == 0)
		{
			strStartPoint = "�ɵ���ϵͳ����AGV";
		}
		else
		{
			BufferString::std_string_format(strStartPoint, "%d", m_usStartPoint);
		}

		return;
	}

	/*!
	 * @brief ��ȡ�������
	 * @param std::wstring& �������
	*/
	void GetStartPoint(std::wstring &wstrStartPoint)
	{
		std::string strStartPoint = "";
		GetStartPoint(strStartPoint);

		BufferString::std_string_to_wstring(wstrStartPoint, strStartPoint);

		return;
	}

	/*!
	 * @brief ��������
	 * @param char ��AGV����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetType(char cType)
	{
		if (m_cType == cType)
		{
			return false;
		}

		switch (cType)
		{
		case 0:
			break;
		case _AGVTYPE_ARM:
			break;
		case _AGVTYPE_FORK:
			break;
		case _AGVTYPE_LIFT:
			break;
		case _AGVTYPE_PULL:
			break;
		case _AGVTYPE_SNEAK:
			break;
		case _AGVTYPE_TRAVEL:
			break;
		default:
			return false;
		}

		m_cType = cType;
		return true;
	}

	/*!
	 * @brief ��ȡ����
	 * @return char AGV����
	*/
	char GetType() const
	{
		return m_cType;
	}

	/*!
	 * @brief ��ȡ����
	 * @param string& AGV����
	*/
	void GetType(std::string& strType)
	{
		switch (m_cType)
		{
		case 0:
			strType = "ȫ������";
			break;
		case _AGVTYPE_ARM:
			strType = "��е��ʽAGV";
			break;
		case _AGVTYPE_FORK:
			strType = "�泵ʽAGV";
			break;
		case _AGVTYPE_LIFT:
			strType = "����ʽAGV";
			break;
		case _AGVTYPE_PULL:
			strType = "ǣ��ʽAGV";
			break;
		case _AGVTYPE_SNEAK:
			strType = "Ǳ��ʽAGV";
			break;
		case _AGVTYPE_TRAVEL:
			strType = "����ʽAGV ";
			break;
		}

		return;
	}

	/*!
	 * @brief ��ȡ����
	 * @param wstring& AGV����
	*/
	void GetType(std::wstring& wstrType)
	{
		std::string strType = "";
		GetType(strType);

		BufferString::std_string_to_wstring(wstrType, strType);

		return;
	}

	/*!
	 * @brief ���ø�����Ϣ
	 * @param string �¸�����Ϣ
	 * @return bool ���óɹ�����trrue,ʧ�ܷ���false
	*/
	bool SetOverheadInfomation(std::string strMsg)
	{
		if (m_strMsg == strMsg)
		{
			return false;
		}

		m_strMsg = strMsg;

		return true;
	}

	/*!
	 * @brief ���ø�����Ϣ
	 * @param wstring �¸�����Ϣ
	 * @return bool ���óɹ�����trrue,ʧ�ܷ���false
	*/
	bool SetOverheadInfomation(std::wstring wstrMsg)
	{
		std::string strMsg = "";
		BufferString::std_wstring_to_string(strMsg, wstrMsg);

		return SetOverheadInfomation(strMsg);
	}

	/*!
	 * @brief ��ȡ������Ϣ
	 * @param string& ������Ϣ
	*/
	void GetOverheadInfomation(std::string& strMsg)
	{
		strMsg = m_strMsg;

		return;
	}

	/*!
	 * @brief ��ȡ������Ϣ
	 * @param wstring& ������Ϣ
	*/
	void GetOverheadInfomation(std::wstring& wstrMsg)
	{
		BufferString::std_string_to_wstring(wstrMsg, m_strMsg);

		return;
	}

	/*!
	 * @brief ���ö���
	 * @param string �¶���
	 * @return bool ���óɹ�����trrue,ʧ�ܷ���false
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
	 * @param wstring �¶���
	 * @return bool ���óɹ�����trrue,ʧ�ܷ���false
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
	void GetAction(std::string& strAction)
	{
		strAction = m_strAction;

		return;
	}

	/*!
	 * @brief ��ȡ����
	 * @param wstring& ����
	*/
	void GetAction(std::wstring& wstrAction)
	{
		BufferString::std_string_to_wstring(wstrAction, m_strAction);

		return;
	}

#ifdef _AFX
	/*!
	 * @brief ��ȡ���
	 * @param CString& ���������
	*/
	void GetNo(CString& cstrNo)
	{
		cstrNo.Format(_T("%d"), m_usNo);

		return;
	}

	/*!
	 * @brief ��ȡ�յ�����
	 * @param CString& �յ�����
	*/
	void GetEndPoint(CString& cstrEndPoint)
	{
		cstrEndPoint.Format(_T("%d"), m_usEndPoint);

		return;
	}

	/*!
	 * @brief ��ȡ�������
	 * @param CString& �������
	*/
	void GetStartPoint(CString &cstrStartPoint)
	{
		std::string strStartPoint = "";
		GetStartPoint(strStartPoint);

		BufferString::std_string_to_CString(cstrStartPoint, strStartPoint);

		return;
	}

	/*!
	 * @brief ��ȡ����
	 * @param CString& AGV����
	*/
	void GetType(CString& cstrType)
	{
		std::string strType = "";
		GetType(strType);

		BufferString::std_string_to_CString(cstrType, strType);

		return;
	}

	/*!
	 * @brief ���ø�����Ϣ
	 * @param CString �¸�����Ϣ
	 * @return bool ���óɹ�����trrue,ʧ�ܷ���false
	*/
	bool SetOverheadInfomation(CString cstrMsg)
	{
		std::string strMsg = "";
		BufferString::CString_to_std_string(strMsg, cstrMsg);

		return SetOverheadInfomation(strMsg);
	}

	/*!
	 * @brief ��ȡ������Ϣ
	 * @param CString& ������Ϣ
	*/
	void GetOverheadInfomation(CString& cstrMsg)
	{
		BufferString::std_string_to_CString(cstrMsg, m_strMsg);

		return;
	}

	/*!
	 * @brief ���ö���
	 * @param CString �¶���
	 * @return bool ���óɹ�����trrue,ʧ�ܷ���false
	*/
	bool SetAction(CString cstrAction)
	{
		std::string strAction = "";
		BufferString::CString_to_std_string(strAction, cstrAction);

		return SetAction(strAction);
	}

	/*!
	 * @brief ��ȡ����
	 * @param wstring& ����
	*/
	void GetAction(CString& cstrAction)
	{
		BufferString::std_string_to_CString(cstrAction, m_strAction);

		return;
	}
#endif
}CACall;

/*! @brief ����èCA���������ֱ�ṹ��*/
typedef struct _CAWatchStruct
{
	unsigned char m_byNo;
	unsigned short m_usCA;
	unsigned char m_byStation;

	_CAWatchStruct()
	{
		m_byNo = 0;
		m_usCA = 0;
		m_byStation = 0;
	}

	_CAWatchStruct(unsigned char byNo, unsigned short usCA = 0, unsigned char byStation = 0)
	{
		m_byNo = byNo;
		m_usCA = usCA;
		m_byStation = byStation;
	}

	_CAWatchStruct(const _CAWatchStruct& watch)
	{
		m_byNo = watch.m_byNo;
		m_usCA = watch.m_usCA;
		m_byStation = watch.m_byStation;
	}

	void operator=(const _CAWatchStruct& watch)
	{
		m_byNo = watch.m_byNo;
		m_usCA = watch.m_usCA;
		m_byStation = watch.m_byStation;
	}

	/*!
	 * @brief ���ñ��
	 * @param unsigned char �ֱ��±��
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetNo(unsigned char byNo)
	{
		if (byNo == 0)
		{
			return false;
		}

		if (m_byNo == byNo)
		{
			return false;
		}

		m_byNo = byNo;

		return true;
	}

	/*!
	 * @brief ��ȡ���
	 * @return unsigned char �ֱ���
	*/
	unsigned char GetNo() const
	{
		return m_byNo;
	}

	/*!
	 * @brief ��ȡ���
	 * @param string& �ֱ���
	*/
	void GetNo(std::string& strNo)
	{
		BufferString::std_string_format(strNo, "%d", m_byNo);

		return;
	}

	/*!
	 * @brief ��ȡ���
	 * @param wstring& �ֱ���
	*/
	void GetNo(std::wstring& wstrNo)
	{
		std::string strNo = "";
		GetNo(strNo);

		BufferString::std_string_to_wstring(wstrNo, strNo);

		return;
	}

	/*!
	 * @brief �����ܿ�CA���
	 * @param unsigned short ��CA���
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetControlCA(unsigned short usCA)
	{
		if (m_usCA == usCA)
		{
			return false;
		}

		m_usCA = usCA;

		return true;
	}

	/*!
	 * @brief ��ȡ�ܿ�CA���
	 * @return unsigned short CA���
	*/
	unsigned short GetControlCA() const
	{
		return m_usCA;
	}

	/*!
	 * @brief ��ȡ�ܿ�CA���
	 * @param string& CA���
	*/
	void GetControlCA(std::string& strCA)
	{
		if(m_usCA == 0)
		{ 
			strCA = "ȫ������";
		}
		else
		{
			BufferString::std_string_format(strCA, "%d", m_usCA);
		}

		return;
	}

	/*!
	 * @brief ��ȡ�ܿ�CA���
	 * @param wstring& CA���
	*/
	void GetControlCA(std::wstring& wstrCA)
	{
		std::string strCA = "";
		GetControlCA(strCA);

		BufferString::std_string_to_wstring(wstrCA, strCA);

		return;
	}

	/*!
	 * @brief ���ù�λ
	 * @param unsigned char �ֱ��¹�λ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetStation(unsigned char byStation)
	{
		if (m_byStation == byStation)
		{
			return false;
		}

		m_byStation = byStation;

		return true;
	}

	/*!
	 * @brief ��ȡ��λ
	 * @return unsigned char ��λ
	*/
	unsigned char GetStation() const
	{
		return m_byStation;
	}

	/*!
	 * @brief ��ȡ��λ
	 * @param string& ��λ
	*/
	void GetStation(std::string& strStation)
	{
		if (m_byStation == 0)
		{
			strStation = "ȫ����λ";
		}
		else
		{
			BufferString::std_string_format(strStation, "%d", m_byStation);
		}
		
		return;
	}

	/*!
	 * @brief ��ȡ��λ
	 * @param wstring& ��λ
	*/
	void GetStation(std::wstring& wstrStation)
	{
		std::string strStation = "";
		GetStation(strStation);

		BufferString::std_string_to_wstring(wstrStation, strStation);

		return;
	}

#ifdef _AFX
	/*!
	 * @brief ��ȡ���
	 * @param CString& �ֱ���
	*/
	void GetNo(CString& cstrNo)
	{
		cstrNo.Format(_T("%d"), m_byNo);

		return;
	}

	/*!
	 * @brief ��ȡ�ܿ�CA���
	 * @param CString& �ֱ���
	*/
	void GetControlCA(CString& cstrCA)
	{
		std::string strCA = "";
		GetControlCA(strCA);

		BufferString::std_string_to_CString(cstrCA, strCA);

		return;
	}

	/*!
	 * @brief ��ȡ��λ
	 * @param CString& �ֱ���
	*/
	void GetStation(CString& cstrStation)
	{
		std::string strStation = "";
		GetStation(strStation);

		BufferString::std_string_to_CString(cstrStation, strStation);

		return;
	}
#endif 
}CAWatch;

#endif // !_CA_H