/*!
 * @file CLED.h
 * @brief LED�豸�����ļ�
 *
 * ������LED�豸�Ļ���������Ϣ
 * @author FanKaiyu
 * @date 2019-02-14
 * @version 1.0
*/

#pragma once
#include <string>

#ifndef _LED_H
#define _LED_H

/*!
 * @class CLED
 * @brief LED�豸����
 *
 * ������LED�豸�Ļ���������Ϣ
*/
class CLED
{
public:
	CLED(unsigned char byNo,unsigned short usWidth,unsigned short usHeight,std::string strIP,unsigned short usPort,std::string strText = "");
	CLED(unsigned char byNo, unsigned short usWidth, unsigned short usHeight, std::wstring wstrIP, unsigned short usPort, std::wstring wstrText = L"");
#ifdef _AFX
	CLED(unsigned char byNo, unsigned short usWidth, unsigned short usHeight, CString cstrIP, unsigned short usPort, CString cstrText = _T(""));
#endif //!_AFC
	~CLED();
protected:
	unsigned char m_byNo;		/*!< ��� */
	unsigned short m_usWidth;	/*!< �� */
	unsigned short m_usHeight;	/*!< �� */
	std::string m_strIP;		/*!< IP��ַ */
	unsigned short m_usPort;	/*!< �˿� */
	std::string m_strText;		/*!< ��ע��Ϣ��ͨ������������LED��λ���Լ�ʵ������ */

public:
	/*!
	 * @brief ���ñ��
	 * @param unsigend char LED�±��
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetNo(unsigned char byNo);

	/*!
	 * @brief ��ȡ���
	 * @return unsigned char LED���
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief ��ȡ���
	 * @param string& LED���
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief ��ȡ���
	 * @param wstring& LED���
	*/
	void GetNo(std::wstring& wstrNo);

	/*!
	 * @brief ���ÿ�
	 * @param unsigned short LED�¿�
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetWidth(unsigned short usWidth);

	/*!
	 * @brief ��ȡ��
	 * @return unsigned short LED��
	*/
	unsigned short GetWidth() const;

	/*!
	 * @brief ��ȡ��
	 * @param string& LED��
	*/
	void GetWidth(std::string& strWidth);

	/*!
	 * @brief ��ȡ��
	 * @param wstring& LED��
	*/
	void GetWidth(std::wstring& wstrWidth);

	/*!
	 * @brief ���ø�
	 * @param unsigned short LED�¸�
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetHeight(unsigned short usHeight);

	/*!
	 * @brief ��ȡ��
	 * @return unsigned short LED��
	*/
	unsigned short GetHeight() const;

	/*!
	 * @brief ��ȡ��
	 * @param string& LED��
	*/
	void GetHeight(std::string strHeight);

	/*!
	 * @brief ��ȡ��
	 * @param wstring& LED��
	*/
	void GetHeight(std::wstring wstrHeight);

	/*!
	 * @brief ����IP��ַ
	 * @param string LED��IP��ַ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetIP(std::string strIP);

	/*!
	 * @brief ����IP
	 * @param wstring LED��IP��ַ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetIP(std::wstring wstrIP);

	/*!
	 * @brief ��ȡIP
	 * @param string& LEDIP��ַ
	*/
	void GetIP(std::string& strIP);

	/*!
	 * @brief ��ȡIP
	 * @param wstring& LEDIP��ַ
	*/
	void GetIP(std::wstring& wstrIP);

	/*!
	 * @brief ���ö˿�
	 * @param unsigned short LED�¶˿�
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetPort(unsigned short usPort);

	/*!
	 * @brief ��ȡ�˿�
	 * @return unsigned short LED�˿�
	*/
	unsigned short GetPort() const;

	/*!
	 * @brief ��ȡ�˿�
	 * @param string& LED�˿�
	*/
	void GetPort(std::string& strPort);

	/*!
	 * @brief ��ȡ�˿�
	 * @param wstring& LED�˿�
	*/
	void GetPort(std::wstring wstrPort);

	/*!
	 * @brief ���ñ�ע
	 * @param string LED�±�ע��Ϣ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetText(std::string strText);

	/*!
	 * @brief ���ñ�ע
	 * @param wstring LED�±�ע��Ϣ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetText(std::wstring wstrText);
	
	/*!
	 * @brief ��ȡ��ע
	 * @param string& LED��ע��Ϣ
	*/
	void GetText(std::string& strText);

	/*!
	 * @brief ��ȡ��ע
	 * @param wstring& LED��ע��Ϣ
	*/
	void GetText(std::wstring& wstrText);

#ifdef _AFX
	/*!
	 * @brief ��ȡ���
	 * @param CString& LED���
	*/
	void GetNo(CString& cstrNo);

	/*!
	 * @brief ��ȡ��
	 * @param CString& LED��
	*/
	void GetWidth(CString& cstrWidth);

	/*!
	 * @brief ��ȡ��
	 * @param CString& LED��
	*/
	void GetHeight(CString& cstrHeight);

	/*!
	 * @brief ����IP
	 * @param CString LED��IP��ַ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetIP(CString cstrIP);

	/*!
	 * @brief ��ȡIP
	 * @param CString& LEDIP��ַ
	*/
	void GetIP(CString& cstrIP);

	/*!
	 * @brief ��ȡ�˿�
	 * @param CString& LED�˿�
	*/
	void GetPort(CString& cstrPort);

	/*!
	 * @brief ���ñ�ע
	 * @param CString LED�±�ע��Ϣ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetText(CString cstrText);

	/*!
	 * @brief ��ȡ��ע
	 * @param CString& LED��ע��Ϣ
	*/
	void GetText(CString& cstrText);
#endif //!_AFX
};

#endif // !_LED_H