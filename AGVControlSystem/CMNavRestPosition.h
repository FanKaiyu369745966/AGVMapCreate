/*!
 * @file CMNavRestPosition.h
 * @brief �ŵ�������λ�ļ�
 *
 * @author FanKaiyu
 * @date 2019-01-17
 * @version 1.0
*/

#pragma once
#include "CMagneticTrackNavigation.h"
#include "CRestPosition.h"

#ifdef _RESTPOSITION_H

/*!
 * @class CMNavRestPosition
 * @brief �ŵ�������λ��
*/
class CMNavRestPosition
	:public CMagneticTrackLocation, public CRestPosition
{
public:
	CMNavRestPosition(unsigned char byNo, unsigned short usLocation,std::string strName = "");
	CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::string strName = "");
	CMNavRestPosition(unsigned char byNo, unsigned short usLocation,std::set<unsigned short> setStopList, std::string strName = "");
	CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::set<unsigned short> setStopList, std::string strName = "");
	CMNavRestPosition(unsigned char byNo, unsigned short usLocation, std::wstring wstrName = L"");
	CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::wstring wstrName = L"");
	CMNavRestPosition(unsigned char byNo, unsigned short usLocation, std::set<unsigned short> setStopList, std::wstring wstrName = L"");
	CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::set<unsigned short> setStopList, std::wstring wstrName = L"");
#ifdef _AFX
	CMNavRestPosition(unsigned char byNo, unsigned short usLocation, CString cstrName = _T(""));
	CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation,CString cstrName = _T(""));
	CMNavRestPosition(unsigned char byNo, unsigned short usLocation, std::set<unsigned short> setStopList, CString cstrName = _T(""));
	CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::set<unsigned short> setStopList, CString cstrName = _T(""));
#endif // _AFX
	CMNavRestPosition(const CMNavRestPosition& rest);
	~CMNavRestPosition();
	void operator=(const CMNavRestPosition& rest);

protected:
	std::set<unsigned short> m_setStop;	/*!< ����RFID����ż��� */

public:
	/*!
	 * @brief ��ӽ���RFID��
	 * @param unsigned short �½���RFID�������Ϣ
	 * @return bool ��ӳɹ�����true,ʧ�ܷ���false
	*/
	bool AddStopRFID(unsigned short usRFID);

	/*!
	 * @brief ɾ������RFID��
	 * @param unsigned short ����RFID�������Ϣ
	*/
	void DeleteStopRFID(unsigned short usRFID);

	/*!
	 * @brief ��ȡ����RFID������
	 * @return set<unsigned short> ����RFID�������Ϣ����
	*/
	std::set<unsigned short> GetStopRFIDList() const;

public:
	/*!
	 * @brief ��ȡ����RFID������
	 * @param string& ����RFID�������Ϣ����
	*/
	void GetStopRFIDList(std::string& strRFIDList);

	/*!
	 * @brief ��ȡ����RFID������
	 * @param wstring& ����RFID�������Ϣ����
	*/
	void GetStopRFIDList(std::wstring& wstrRFIDList);

	/*!
	 * @brief ���ý���RFID������
	 * @param string ����RFID�������Ϣ����
	*/
	void SetStopRFIDList(std::string strRFIDList);

	/*!
	 * @brief ���ý���RFID������
	 * @param wstring ����RFID�������Ϣ����
	*/
	void SetStopRFIDList(std::wstring wstrRFIDList);

#ifdef _AFX
	/*!
	 * @brief ��ȡ����RFID������
	 * @param CString& ����RFID�������Ϣ����
	*/
	void GetStopRFIDList(CString& cstrRFIDList);

	/*!
	 * @brief ���ý���RFID������
	 * @param CString ����RFID�������Ϣ����
	*/
	void SetStopRFIDList(CString cstrRFIDList);
#endif // !_AFX
};

#endif // !_RESTPOSITION_H