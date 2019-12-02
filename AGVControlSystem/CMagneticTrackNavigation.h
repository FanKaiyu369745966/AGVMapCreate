/*!
 * @file CMagneticTrackNavigation.h
 * @brief �Ź쵼�����������ļ�
 * ��������RFID���Ļ�������
 * ����������ͨ��������Ļ�������
 * ����������λ��Ϣ�Ļ������� 
 *
 * @author FanKaiyu
 * @date 2019-01-07
 * @version 1.0
*/

#pragma once
#include <list>
#include <set>
#include <map>

#ifndef _MAGNETICTRACK_H
#define _MAGNETICTRACK_H
/*!
 * @class CRFID
 * @brief RFID���Ļ���������
*/
class CRFID
{
public:
	CRFID(unsigned short usNo = 0);
	CRFID(const CRFID& rfid);
	~CRFID();

protected:
	unsigned short m_usNo;					/*!< RFID����� */
	void *m_pUser;							/*!< ʹ��RFID�����û�ָ��,ͨ��ΪAGVָ�� */
#ifdef _WIN64
	unsigned long long m_ullUseTime;		/*!< ��ʼʹ��RFID����ʱ�䣨ϵͳ����ʱ�䣩 */
#elif _WIN32
	unsigned long m_ulUseTime;				/*!< ��ʼʹ��RFID����ʱ�䣨ϵͳ����ʱ�䣩 */
#endif //  _WIN64
	void *m_pLocker;						/*!< ��סRFID�����û�ָ�룬ͨ��ΪAGVָ�롣���ڷ�ֹ�������û�ʹ��RFID�� */

public:
	/*!
	 * @brief ��ȡ���
	 * @return unsigned short ��ȡRFID�������Ϣ
	*/
	unsigned short GetNo() const;

	/*!
	 * @brief ���ñ��
	 * @param unsigned short ��RFID�����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetNo(unsigned short usNo);

	/*!
	 * @brief ��ʼʹ��RFID��
	 * @param void* ʹ��RFID�����û�ָ��
	 * @return void* ����ʹ��RFID���û�ָ��
	*/
	void* Use(void* pUser);

	/*!
	 * @brief ����ʹ��RFID��
	 * @param void* ʹ��RFID�����û�ָ��
	*/
	void Finish(void* pUser);

	/*!
	 * @brief ��ȡʹ����
	 * @return void* ʹ��RFID�����û�ָ��
	*/
	void* GetUser() const;

#ifdef  _WIN64
	/*!
	 * @brief ��ȡʹ����
	 * @param unsigned long long& �û���ʼʹ��RFID����ʱ�䣨ϵͳ����ʱ�䣩
	 * @return void* ʹ��RFID�����û�ָ��
	*/
	void* GetUser(unsigned long long& ullUseTime) const;
#elif _WIN32
	/*!
	 * @brief ��ȡʹ����
	 * @param unsigned long& �û���ʼʹ��RFID����ʱ�䣨ϵͳ����ʱ�䣩
	 * @return void* ʹ��RFID�����û�ָ��
	*/
	void* GetUser(unsigned long& ulUseTime) const;
#endif //  _WIN64

	/*!
	 * @brief ����RFID��
	 * @param void* ����RFID�����û�ָ��
	 * @return void* �Ѿ�����RFID���û�ָ��
	*/
	void* Lock(void* pLocker);

	/*!
	 * @brief ���RFID������
	 * @param void* ����RFID�����û�ָ��
	*/
	void Unlock(void* pLocker);

	/*!
	 * @brief ��ȡ������
	 * @return void* ����RFID�����û�ָ��
	*/
	void* GetLocker() const;

	void operator=(const CRFID& rfid);
};

/*!
 * @class CMagneticTrackLocation
 * @brief �Ź쵼����ʽ������λ������
 *
 * ����Ϊ��λ������λ�����λ�ȸ��趨λ��Ϣ
*/
class CMagneticTrackLocation
{
public:
	CMagneticTrackLocation(unsigned short usRFID);
	~CMagneticTrackLocation();

protected:
	unsigned short m_usLocateRFID; /*!< ��λRFID�������Ϣ */

public:
	/*!
	 * @brief ���ö�λ��Ϣ
	 * @param unsigend short �¶�λRFID�������Ϣ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetLocation(unsigned short usRFID);

	/*!
	 * @brief ��ȡ��λ��Ϣ
	 * @return unsigned short ��λRFID�������Ϣ
	*/
	unsigned short GetLocation() const; 

public:
	/*!
	 * @brief ��ȡ��λ��Ϣ
	 * @param string& ��λRFID�������Ϣ
	*/
	void GetLocation(std::string& strRFID);

	/*!
	 * @brief ��ȡ��λ��Ϣ
	 * @param wstring& ��λRFID�������Ϣ
	*/
	void GetLocation(std::wstring& wstrRFID);

#ifdef _AFX
	/*!
	 * @brief ��ȡ��λ��Ϣ
	 * @param CString& ��λRFID�������Ϣ
	*/
	void GetLocation(CString& cstrRFID);
#endif // !_AFX
};

typedef class CMagneticTrackLocation CMTLocation;

/*!
 * @class CMagneticTrackNavigation
 * brief �Ź쵼����ʽ����������
*/
class CMagneticTrackNavigation
{
public:
	CMagneticTrackNavigation();
	~CMagneticTrackNavigation();

protected:
	unsigned short m_usCurRFID;					/*!< ��ǰRFID�������Ϣ */
	unsigned short m_usOldRFID;					/*!< ��ȥRFID�������Ϣ */
	unsigned short m_usNextRFID;				/*!< ��һRFID�������Ϣ */
	unsigned short m_usEndRFID;					/*!< ��ֹRFID�������Ϣ */
	std::list<unsigned short> m_listOldRFID;	/*!< ��ȥRFID������ */

public:
	/*!
	 * @brief ���õ�ǰRFID�������Ϣ
	 * @param unsigned short ��RFID�������Ϣ
	 * @return bool ����ɹ�����true,ʧ�ܷ���false
	*/
	bool SetCurRFID(unsigned short usRFID);

	/*!
	 * @brief ���õ�ǰRFID�������Ϣ
	 * @param unsigned short ��RFID�������Ϣ
	 * @param unsigned short& ��RFID�������Ϣ
	 * @return bool ����ɹ�����true,ʧ�ܷ���false
	*/
	bool SetCurRFID(unsigned short usRFID,unsigned short& usOldRFID);

	/*!
	 * @brief ��ȡ��ǰRFID�������Ϣ
	 * @return unsigned short ��ǰRFID�������Ϣ
	*/
	unsigned short GetCurRFID() const;

	/*!
	 * @brief ��ȡ��ȥRFID�������Ϣ
	 * @return unsigned short ��ȥRFID�������Ϣ
	*/
	unsigned short GetOldRFID() const;

	/*!
	 * @brief ������һRFID�������Ϣ
	 * @param unsigned short ����һRFID�������Ϣ
	 * @return bool ����ɹ�����true,ʧ�ܷ���false
	*/
	bool SetNextRFID(unsigned short usRFID);

	/*!
	 * @brief ��ȡ��һRFID�������Ϣ
	 * @return unsigned short ��һRFID�������Ϣ
	*/
	unsigned short GetNextRFID() const;

	/*!
	 * @brief ������ֹRFID�������Ϣ
	 * @param unsigend short ����ֹRFID�������Ϣ
	 * @return bool ����ɹ�����true,ʧ�ܷ���false
	*/
	bool SetEndRFID(unsigned short usRFID);

	/*!
	 * @brief ��ȡ��ֹRFID�������Ϣ
	 * @return unsigned short ��ֹRFID�������Ϣ
	*/
	unsigned short GetEndRFID() const;

	/*!
	 * @brief ��ȡ��ȥRFID������
	 * @return list<unsigned short> ��ȥRFID������
	*/
	std::list<unsigned short> GetOldRFIDList() const;

public:
	/*!
	 * @brief ��ȡ��ǰRFID�������Ϣ
	 * @param std::string& ��ǰRFID�������Ϣ
	*/
	void GetCurRFID(std::string& strRFID);

	/*!
	 * @brief ��ȡ��ǰRFID�������Ϣ
	 * @param std::wstring& ��ǰRFID�������Ϣ
	*/
	void GetCurRFID(std::wstring& wstrRFID);

	/*!
	 * @brief ��ȡ��ȥRFID�������Ϣ
	 * @param std::string& ��ȥRFID�������Ϣ
	*/
	void GetOldRFID(std::string& strRFID);

	/*!
	 * @brief ��ȡ��ȥRFID�������Ϣ
	 * @param std::wstring& ��ȥRFID�������Ϣ
	*/
	void GetOldRFID(std::wstring& wstrRFID);

	/*!
	 * @brief ��ȡ��һRFID�������Ϣ
	 * @param string& ��һRFID�������Ϣ
	*/
	void GetNextRFID(std::string& strRFID);

	/*!
	 * @brief ��ȡ��һRFID�������Ϣ
	 * @param wstring& ��һRFID�������Ϣ
	*/
	void GetNextRFID(std::wstring& wstrRFID);

	/*!
	 * @brief ��ȡ��ֹRFID�������Ϣ
	 * @param string& ��ֹRFID�������Ϣ
	*/
	void GetEndRFID(std::string& strRFID);

	/*!
	 * @brief ��ȡ��ֹRFID�������Ϣ
	 * @param wstring& ��ֹRFID�������Ϣ
	*/
	void GetEndRFID(std::wstring& wstrRFID);

	/*!
	 * @brief ��ȡ��ȥRFID������
	 * @param string& ��ȥRFID������
	*/
	void GetOldRFIDList(std::string& strList);

	/*!
	 * @brief ��ȡ��ȥRFID������
	 * @param wstring& ��ȥRFID������
	*/
	void GetOldRFIDList(std::wstring& wstrList);

#ifdef _AFX
	/*!
	 * @brief ��ȡ��ǰRFID�������Ϣ
	 * @param CString& ��ǰRFID�������Ϣ
	*/
	void GetCurRFID(CString& cstrRFID);

	/*!
	 * @brief ��ȡ��ȥRFID�������Ϣ
	 * @param CString& ��ȥRFID�������Ϣ
	*/
	void GetOldRFID(CString& cstrRFID);

	/*!
	 * @brief ��ȡ��һRFID�������Ϣ
	 * @param CString& ��һRFID�������Ϣ
	*/
	void GetNextRFID(CString& cstrRFID);

	/*!
	 * @brief ��ȡ��ֹRFID�������Ϣ
	 * @param CString& ��ֹRFID�������Ϣ
	*/
	void GetEndRFID(CString& cstrRFID);

	/*!
	 * @brief ��ȡ��ȥRFID������
	 * @param CString& ��ȥRFID������
	*/
	void GetOldRFIDList(CString& cstrList);
#endif //!_AFX
};

typedef class CMagneticTrackNavigation CMTNav;

/*!
 * @class CMagneticTrackTrafficControlArea
 * @brief �Ź쵼����ʽ��ͨ���ƿ��������
 *
 * ������RFID�����ϴ���AGVʱ,���������AGV�����ڽ�ͨ����RFID�����н�ͨ����
 * ���򵱹�ǰRFID�����RFID���ϴ���AGVʱ,��ȶ�����鼯���ڵ��������бȶ�
 * �����ȵ����е�ԭ��,�жϴ˿���AGV�Ƿ�����ͨ��
*/
class CMagneticTrackTrafficControlArea
{
public:
	CMagneticTrackTrafficControlArea();
	CMagneticTrackTrafficControlArea(unsigned short usCtrl,unsigned short usBefore = 0);
	CMagneticTrackTrafficControlArea(unsigned short usCtrl, std::set<unsigned short> setStop, std::set<unsigned short> setComparison,unsigned short usBefore = 0);
	CMagneticTrackTrafficControlArea(const CMagneticTrackTrafficControlArea& area);
	~CMagneticTrackTrafficControlArea();

protected:
	unsigned short m_usBefore;					/*!< ��ͨ����֮ǰRFID�������Ϣ */
	unsigned short m_usCtrl;					/*!< ��ͨ����RFID�������Ϣ */
	std::set<unsigned short> m_setStop;			/*!< ����RFID������.��������RFID���ϴ���AGVʱ,��������Լ��뱾�����ȶԵ���������AGV��������ͨ��. */
	std::set<unsigned short> m_setComparison;	/*!< �ȶ�����鼯��.*/

public:
	/*!
	 * @brief ���ý�ͨ����֮ǰRFID�������Ϣ
	 * @param unsigned short �½�ͨ����֮ǰRFID�������Ϣ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetBeforeRFID(unsigned short usRFID);

	/*!
	 * @brief ��ȡ��ͨ����֮ǰRFID�������Ϣ
	 * @return unsigned short ��ͨ����֮ǰRFID�������Ϣ
	*/
	unsigned short GetBeforeRFID() const;

	/*!
	 * @brief ���ý�ͨ����RFID�������Ϣ
	 * @param unsigned short �½�ͨ����RFID�������Ϣ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetCtrlRFID(unsigned short usRFID);

	/*!
	 * @brief ��ȡ��ͨ����RFID�������Ϣ
	 * @return unsigned short ��ͨ����RFID�������Ϣ
	*/
	unsigned short GetCtrlRFID() const;

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

	/*!
	 * @brief ��ӱȶ�����
	 * @param unsigned short ����ͨ����RFID�������Ϣ
	 * @return bool ��ӳɹ�����true,ʧ�ܷ���false
	*/
	bool AddCompareArea(unsigned short usAreaCtrl);

	/*!
	 * @brief ɾ���ȶ�����
 * @param unsigned short ����ͨ����RFID�������Ϣ
	*/
	void DeleteCompareArea(unsigned short usAreaCtrl);

	/*!
	 * @brief ��ȡ�ȶ����򼯺�
	 * @return set<unsigned short> �ȶ����򼯺�
	*/
	std::set<unsigned short> GetCompareAreaList() const;

	void operator=(const CMagneticTrackTrafficControlArea& area);

public:
	/*!
	 * @brief ��ȡ��ͨ����֮ǰRFID�������Ϣ
	 * @param string& ��ͨ����֮ǰRFID�������Ϣ
	*/
	void GetBeforeRFID(std::string& strRFID);

	/*!
	 * @brief ��ȡ��ͨ����֮ǰRFID�������Ϣ
	 * @param wstring& ��ͨ����֮ǰRFID�������Ϣ
	*/
	void GetBeforeRFID(std::wstring& wstrRFID);

	/*!
	 * @brief ��ȡ��ͨ����RFID�������Ϣ
	 * @param string& ��ͨ����RFID�������Ϣ
	*/
	void GetCtrlRFID(std::string& strRFID);

	/*!
	 * @brief ��ȡ��ͨ����RFID�������Ϣ
	 * @param wstring& ��ͨ����RFID�������Ϣ
	*/
	void GetCtrlRFID(std::wstring& wstrRFID);

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

	/*!
	 * @brief ��ȡ�ȶ����򼯺�
	 * @param string& �ȶ����򼯺�
	*/
	void GetCompareAreaList(std::string& strCompareList);

	/*!
	 * @brief ��ȡ�ȶ����򼯺�
	 * @param wstring& �ȶ����򼯺�
	*/
	void GetCompareAreaList(std::wstring& wstrCompareList);

	/*!
	 * @brief ���ñȶ����򼯺�
	 * @param string �ȶ����򼯺�
	*/
	void SetCompareAreaList(std::string strCompareList);

	/*!
	 * @brief ���ñȶ����򼯺�
	 * @param wstring �ȶ����򼯺�
	*/
	void SetCompareAreaList(std::wstring wstrCompareList);

#ifdef _AFX
	/*!
	 * @brief ��ȡ��ͨ����֮ǰRFID�������Ϣ
	 * @param CString& ��ͨ����֮ǰRFID�������Ϣ
	*/
	void GetBeforeRFID(CString& cstrRFID);

	/*!
	 * @brief ��ȡ��ͨ����RFID�������Ϣ
	 * @param CString& ��ͨ����RFID�������Ϣ
	*/
	void GetCtrlRFID(CString& cstrRFID);

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

	/*!
	 * @brief ��ȡ�ȶ����򼯺�
	 * @param CString& �ȶ����򼯺�
	*/
	void GetCompareAreaList(CString& cstrCompareList);

	/*!
	 * @brief ���ñȶ����򼯺�
	 * @param CString �ȶ����򼯺�
	*/
	void SetCompareAreaList(CString cstrCompareList);
#endif //!_AFX
};

typedef class CMagneticTrackTrafficControlArea CMTTrafficCtrlArea;

#endif // !_MAGNETICTRACK_H