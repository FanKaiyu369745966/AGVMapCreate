/*!
 * @file CRestPosition.h
 * @brief ��λ�����ļ�
 *
 * ������λ�Ļ����������������
 * @author FanKaiyu
 * @date 2019-01-15
 * @version 2.0
*/

#pragma once
#include <string>

#ifndef _STATION_H
#define _STATION_H

/*!
 * @class CStation
 * @brief ��λ����
 *
 * ������λ�Ļ����������������
*/
class CStation
{
public:
	CStation(unsigned char byNo,std::string strName = "");
	CStation(unsigned char byNo, std::wstring wstrName = L"");
#ifdef _AFX
	CStation(unsigned char byNo, CString cstrName = _T(""));
#endif //!_AFX
	~CStation();
protected:
	unsigned char m_byNo; /*!< ��� */
	std::string m_strName;	/*!< ���� */

public:
	/*!
	 * @brief ���ñ��
	 * @param unsigned char �¹�λ���
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetNo(unsigned char byNo);

	/*!
	 * @brief ��ȡ���
	 * @return unsigned char ��λ���
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief ��������
	 * @param string �¹�λ����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetName(std::string strName);

	/*!
	 * @brief ��������
	 * @param wstring �¹�λ����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetName(std::wstring wstrName);

#ifdef _AFX
	/*!
	 * @brief ��������
	 * @param CString �¹�λ����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetName(CString cstrName);
#endif // !_AFX

	/*!
	 * @brief ��ȡ��λ����
	 * @param string& ��λ����
	*/
	void GetName(std::string& strName);

	/*!
	 * @brief ��ȡ��λ����
	 * @param wstring& ��λ����
	*/
	void GetName(std::wstring& wstrName);

#ifdef _AFX
	/*!
	 * @brief ��ȡ��λ����
	 * @param CString& ��λ����
	*/
	void GetName(CString& cstrName);
#endif // !_AFX

public:
	/*!
	 * @brief ��ȡ���
	 * @param string& ��λ���
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief ��ȡ���
	 * @param wstring& ��λ���
	*/
	void GetNo(std::wstring& wstrNo);

#ifdef _AFX
	/*!
	 * @brief ��ȡ���
	 * @param CString& ��λ���
	*/
	void GetNo(CString& cstrNo);
#endif //!_AFX
};

#endif // !_STATION_H