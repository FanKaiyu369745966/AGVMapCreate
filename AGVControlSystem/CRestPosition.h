/*!
 * @file CRestPosition.h
 * @brief ����λ�����ļ�
 *
 * ��������λ�Ļ����������������
 * @author FanKaiyu
 * @date 2019-01-15
 * @version 2.0
*/

#pragma once
#include <string>

#ifndef _RESTPOSITION_H
#define _RESTPOSITION_H

/*!
 * @class CRestPosition
 * @brief ����λ����
 *
 * ��������λ�Ļ����������������
*/
class CRestPosition
{
public:
	CRestPosition(unsigned char byNo, unsigned char byNext = 0, std::string strName = "");
	CRestPosition(unsigned char byNo, unsigned char byNext = 0, std::wstring wstrName = L"");
#ifdef _AFX
	CRestPosition(unsigned char byNo, unsigned char byNext = 0, CString cstrName = _T(""));
#endif //!_AFX
	CRestPosition(const CRestPosition& rest);
	~CRestPosition();
protected:
	unsigned char m_byNo;	/*!< ����λ��� */
	unsigned char m_byNext; /*!< ��һ����λ��ţ������ڴ���λ�Զ���λ���ܡ�*/
	std::string m_strName;	/*!< ���� */

public:
	/*!
	 * @brief ���ñ��
	 * @param unsigned char �´���λ���
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetNo(unsigned char byNo);

	/*!
	 * @brief ��ȡ���
	 * @return unsigned char ��ȡ����λ���
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief ������һλ���
	 * @param unsigned char ����һ����λ���
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetNext(unsigned char byNext);

	/*!
	 * @brief ��ȡ��һλ���
	 * @return unsigned char ��һ����λ���
	*/
	unsigned char GetNext() const;

	/*!
	 * @brief ��������
	 * @param string �´���λ����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetName(std::string strName);

	/*!
	 * @brief ��������
	 * @param wstring �´���λ����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetName(std::wstring wstrName);

#ifdef _AFX
	/*!
	 * @brief ��������
	 * @param CString �´���λ����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetName(CString cstrName);
#endif // !_AFX

	/*!
	 * @brief ��ȡ����λ����
	 * @param string& ��λ����
	*/
	void GetName(std::string& strName);

	/*!
	 * @brief ��ȡ����λ����
	 * @param wstring& ��λ����
	*/
	void GetName(std::wstring& wstrName);

#ifdef _AFX
	/*!
	 * @brief ��ȡ����λ����
	 * @param CString& ��λ����
	*/
	void GetName(CString& cstrName);
#endif // !_AFX

public:
	/*!
	 * @brief ��ȡ���
	 * @param string& ���
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief ��ȡ���
	 * @param wstring& ���
	*/
	void GetNo(std::wstring& wstrNo);

	/*!
	 * @brief ��ȡ��һλ���
	 * @param string& ��һλ���
	*/
	void GetNext(std::string& strNo);

	/*!
	 * @brief ��ȡ��һλ���
	 * @param string& ��һλ���
	*/
	void GetNext(std::wstring& wstrNo);

#ifdef _AFX
	/*!
	 * @brief ��ȡ���
	 * @param CString& ���
	*/
	void GetNo(CString& cstrNo);

	/*!
	 * @brief ��ȡ��һλ���
	 * @param string& ��һλ���
	*/
	void GetNext(CString& cstrNo);
#endif // !_AFX
};

#endif // !_RESTPOSITION_H