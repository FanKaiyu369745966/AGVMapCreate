/*!
 * @file CAGV.h
 * @brief AGV�����ļ�
 *
 * AGV��Ļ��������ļ�
 * @author FanKaiyu
 * @date 2019-01-06
 * @version 2.0
*/
#pragma once
#include <string>

#ifndef _AGV_H
#define _AGV_H

/*!
 * AGV����
 * ���ݹ��ܷ��࣬�ɷ�Ϊ��
 * 1������ʽ��Transfer��
 * 2��ǣ��ʽ��Pull��
 * 3��Ǳ��ʽ��Steal��
 * 4���泵ʽ��Forklift��
 * 5����е��ʽ��Arm��
 * 6������ʽ��Lift��
 *
 * ���ݵ�����ʽ���࣬�ɷ�Ϊ��
 * 1���Ź쵼��(Magnetic Track)
 * 2�����⵼��(Laser)
 * 3����ά�뵼��(QR code)
 *
 * ���ݸ���ͨ�ŷ�ʽ���࣬�ɷ�Ϊ��
 * 1��TCP/IP SOCKETͨ��
 * 2��TCP/IP HTTPͨ��
 * 3�����ڹ㲥ͨ��
*/

/*! @brief AGV���� */
enum _AGVTYPE
{
	_AGVTYPE_ARM = 'A',		/*!< ��е��ʽAGV */
	_AGVTYPE_FORK = 'F',	/*!< �泵ʽAGV */
	_AGVTYPE_LIFT = 'L',	/*!< ����ʽAGV */
	_AGVTYPE_PULL = 'P',	/*!< ǣ��ʽAGV */
	_AGVTYPE_SNEAK = 'S',	/*!< Ǳ��ʽAGV */
	_AGVTYPE_TRAVEL = 'T',	/*!< ����ʽAGV */
};

/*! @brief AGV���ƶ����� */
enum _AGVDRIVER
{
	_AGVDRIVER_SINGLE = 'S',	/*! ���� */
	_AGVDRIVER_DOUBLE = 'D',	/*! ˫�� */
	_AGVDRIVER_FULL = 'F',		/*! ȫ�� */
};

/*! @brief AGV״̬��Ϣ�� */
enum _AGVSTATUS
{
	_AGVSTA_WAIT,		/*!< ����״̬.
						��λ�����յ���״̬,��ΪAGV�Ѿ�׼���ò����Խ�����λ���Ŀ��Ƶ���.
						��ʱ��λ�����Խ��ղ�������λ�����͵��������͵ı���. 
						*/
	_AGVSTA_RUN,		/*!< ����״̬.
						��λ�����յ���״̬,��ΪAGV���ƶ�������.��ʱ��λ�����˻���������״̬�ⲻ���κδ���.
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.���ƶ�������������뽫AGV״̬�ָ�Ϊ����״̬.
						*/
	_AGVSTA_STOP,		/*!< ֹͣ״̬.
						��λ�����յ���״̬,��ΪAGV����Ϊֹͣ.�����ֶ�����;��ʱ��λ�����˻���������״̬�ⲻ���κδ���.
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.�û������ֶ�����AGV,���û�������������뽫AGV״̬�ָ�Ϊ����״̬.
						*/
	_AGVSTA_SCREAM,		/*!< ��ͣ״̬.
						��λ�����յ���״̬,��ΪAGV������ֹͣ����,�����ֶ�����.��ʱ��λ�����ϱ��������û�,���ҳ�������״̬�ⲻ��������.
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.���û�������������뽫AGV״̬�ָ�Ϊ����״̬.
						*/
	_AGVSTA_FIND,		/*!< Ѱ��/��λ״̬.
						��λ�����յ���״̬,��ΪAGV������.��ʱ��λ�����˻���������״̬�ⲻ���κδ���.
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.��λ��Ӧ����Ѱ��/��λ����,��һ��ʱ����Ѱ��/��λʧ�ܺ�,Ӧ��AGV״̬��Ϊֹͣ,�����쳣״̬��Ϊ�Ѵ�/�޷���λ.
						*/
	_AGVSTA_OBSDOWN,	/*!< �ǽӴ�ʽ���ϼ���״̬.
						��λ�����յ���״̬,��ΪAGV�����ǽӴ�ʽ���ϲ����ڼ�������״̬,��ʱ��λ�����˻���������״̬�ⲻ���κδ���.
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.
						*/
	_AGVSTA_OBSSTOP,	/*!< �ǽӴ�ʽ����ֹͣ״̬.
						��λ�����յ���״̬,��ΪAGV�����ǽӴ�ʽ���ϲ����ڼ�������״̬,��ʱ��λ�����˻���������״̬�ⲻ���κδ���.
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.
						*/
	_AGVSTA_MOBSSTOP,	/*!< �Ӵ�ʽ����ֹͣ״̬.
						��λ�����յ���״̬,��ΪAGV�����Ӵ�ʽ���ϲ�����ֹͣ״̬,��ʱ��λ���ϱ��쳣���û�,���ҳ��˻���������״̬�ⲻ���κδ���.
						��ʱ��λ�����˴�����λ����������������,�������ľ����Բ�����.
						*/
	_AGVSTA_TRAFFIC,	/*!< ��ͨ����ֹͣ״̬.
						��λ�����յ���״̬,��ΪAGV���ڽ�ͨ���Ƶ�ȴ���λ������,��ʱ��λ�����ݽ�ͨ���ƹ��������״̬��AGV�Ƿ�����ͨ��.
						��ʱ��λ�����˴�����λ�����������������Լ���ͨ���ƿ��Ʊ���,�������ľ����Բ�����.
						*/
};

/*! @brief AGV������Ϣ�� */
enum _AGVACTION
{
	_AGVACT_NONE,		/*!< ��/ֹͣ����.
						δִ��״̬:AGV�յ��˶���,��AGV����ִ����������,��ֹͣ������ִ��.
						ִ��״̬:������.
						ִ�����״̬:������.
						*/
	_AGVACT_LOAD,		/*!< ���϶���.
						δִ��״̬:������ʽAGV�����ǹ�Ͳִ�����϶���;
								����Ǳ��ʽ��ǣ��ʽAGV������������ִ����������;
								�Բ泵ʽAGV�����ǻ���ִ��������;
								�Ի�е��AGV�����ǻ�е��ִ��ץȡ����,����е��AGV��ͬһ������Ҫ��ɶ����ͬ�Ķ���ʱ,ͨ�����Ӷ�����ִ����Ӧ�Ķ���;
								ִ�ж���ǰAGVӦ�ж��Ƿ��ڿ�ִ�д˶�����ָ��λ����.������,��Ӧ����ǰ�ر꿨��Ϊ0���ϱ��쳣.
								����ʼִ�ж���,�򽫶���״̬��Ϊִ����.
						ִ��״̬:������ɺ󽫶���״̬��Ϊִ�����.ӵ�л��ϼ�⹦�ܵ�AGVӦ��ȷ�ϻ����Ѵ����AGV�Ϻ��ٽ�����״̬��Ϊִ�����.
						ִ�����״̬:������.
						*/
	_AGVACT_UNLOAD,		/*!< ���϶���
						δִ��״̬:������ʽAGV�����ǹ�Ͳִ�����϶���;
								����Ǳ��ʽ��ǣ��ʽAGV������������ִ���½�����;
								�Բ泵ʽAGV�����ǻ���ִ���½�����;
								�Ի�е��AGV�����ǻ�е��ִ���·Ŷ���,����е��AGV��ͬһ������Ҫ��ɶ����ͬ�Ķ���ʱ,ͨ�����Ӷ�����ִ����Ӧ�Ķ���;
								ִ�ж���ǰAGVӦ�ж��Ƿ��ڿ�ִ�д˶�����ָ��λ����.������,��Ӧ����ǰ�ر꿨��Ϊ0���ϱ��쳣.
								����ʼִ�ж���,�򽫶���״̬��Ϊִ����.
						ִ��״̬:������ɺ󽫶���״̬��Ϊִ�����.ӵ�л��ϼ�⹦�ܵ�AGVӦ��ȷ�ϻ������뿪AGV���ٽ�����״̬��Ϊִ�����.
						ִ�����״̬:������.
						*/
	_AGVACT_MOVE,		/*!< �ƶ����� 
						δִ��״̬:AGV�յ��˶���,��AGV��ʼ�ƶ�����ֹ�ر꿨.��AGV��ǰ��������ֹ������ͬ,�����ƶ�.������״̬��Ϊִ�����.���򽫶���״̬��Ϊִ����.
						ִ��״̬:��AGV��ǰ��������ֹ������ͬ,�����ƶ�,������״̬��Ϊִ�����.
						ִ�����״̬:������.
						*/
	_AGVACT_TRAFFIC,	/*!< ��ͨ���ƶ��� 
						δִ��״̬:������.
						ִ��״̬:������.
						ִ�����״̬:AGV�յ��˶���,��AGV�����뿪��ֹ�ر꿨.
						*/
	_AGVACT_REMOVE,		/*!< �����ƶ�����
						δִ��״̬:AGV�յ��˶���,��AGV�����ƶ�����ֹ����.��AGV��ǰ��������ֹ������ͬ�����ƶ�ʱ,��AGV�ɽ���ǰ�������ú������ƶ�.������״̬��Ϊִ����.
						ִ��״̬:��AGV��ǰ��������ֹ������ͬ,�����ƶ�.������״̬��Ϊִ�����.
						ִ�����״̬:������.
						*/
};

/*! @brief AGV����״̬��Ϣ�� */
enum _AGVACTIONSTATUS
{
	_AGVACTSTA_NONE,	/*!< ����δִ��״̬ */
	_AGVACTSTA_EXE,		/*!< ����ִ����״̬ */
	_AGVACTSTA_FINISH,	/*!< ����ִ�����״̬ */
};

/*! @brief AGV�쳣��Ϣ��*/
enum _AGVERROR
{
	_AGVERR_NONE,		/*!< ��/����
						˵��:AGV��������.
						��λ��:������.
						��λ��:������.
						*/
	_AGVERR_MISS,		/*!< �Ѵ�/�޷���λ
						˵��:�����û���AGV�ָ��������,��ֹ���Ѵ���ɵ��������̿���.
						��λ��:�ϱ��쳣���û�.
						��λ��:���״̬Ϊֹͣ.
						*/
	_AGVERR_PATH,		/*!< ·���쳣
						˵��:����λ�����͵��ƶ�ָ����󣨼�û�ж�Ӧ���ƶ�·���������.
						��λ��:�ϱ��쳣���û�.
						��λ��:���״̬Ϊֹͣ.
						*/
	_AGVERR_LOCATE,		/*!< �����쳣
						˵��:��AGVδ������λ��������ָ��λ�������.��λ��Ӧ���������ƶ����ƶ�����,ʹAGV���µ���ָ��λ��,��ִ֤�ж����İ�ȫ��.
						��λ��:�ϱ��쳣���û�.
						��λ��:����ǰ����������.
						*/
	_AGVERR_DIRECTION,	/*!< �����쳣
						˵��:��ȱ����Գ���������쳣��
						��λ��:�ϱ��쳣���û�.
						��λ��:���״̬Ϊֹͣ.
						*/
	_AGVERR_ACT = -2,	/*!< �����쳣
						˵��:��PC��λ��������ִ��ĳһ������ʱ��δ��������.
						��λ��:�ϱ��쳣���û�.
						��λ��:������.
						*/
	_AGVERR_COMM,		/*!< ͨ���쳣
						˵��:��PC��λ���������޷����ͱ��Ļ�ʱ��δ���ձ��������.
						��λ��:�ϱ��쳣���û�.
						��λ��:������.
						*/
};

/*!
 * @class CAGV
 * @brief AGV����
 *
 * ���ݲ�ͬ���͵�AGV�������N��AGV��
 * ����ÿ��AGV������һЩ��������
 * ���ඨ����AGV�豸��ͨ�û���������Ϣ
*/
class CAGV
{
public:
	CAGV(unsigned char byNo,unsigned char byType, unsigned char byDriver,float fMaxSepped, bool bAuto = true);
	~CAGV();
protected:
	/*!
	 * ����������Ϣ
	*/
	unsigned char m_byNo;			/*!< AGV���,ÿ̨AGV������һ������ʶ��ĺ��뼴AGV��� */
	unsigned char m_byType;			/*!< AGV����,���ݲ�ͬ���͵�AGVִ���ض������� */
	unsigned char m_byDriver;		/*!< AGV���ƶ����� */
	float m_fMaxSpeed;				/*!< AGV������ٶ�ֵ������ٶ�ֵ X ��ǰ�ٶ� = ʵ���ٶȣ���λ m/min */
									/*!< AGV������Ϣ����ͬ��AGV�����ϴ���������Ϣ���ܲ�����ͬ�����Դ����Բ���AGV�豸��ͨ������ */
									/*!< AGVĿ��������Ϣ��ͬ����ͬ��AGV�����ϴ���������Ϣ������ͬ�����Դ����Բ���AGV�豸��ͨ�����ݡ������Զ������жϵ�ǰAGV�ƶ����յ��Ƿ�Ϊϵͳָ�����յ㣬ϵͳ�Ƿ�Ҫ�������ƶ�·�� */
	unsigned char m_byPower;		/*!< AGV������ÿ̨AGVʣ��ĵ�����Ϣ��ȡֵΪ0-100%������������AGV����Ĺ����Զ���繦�ܵ�ʵ�� */
	char m_chSpeed;					/*!< AGV�ٶȣ�ÿ̨AGV��ǰ���ٶȣ�ȡֵΪ-100-100%������ó�ʵ���ٶ�ֵ����ҪAGV����ٶ����� */
	unsigned char m_byStatus;		/*!< AGV״̬�������ж�AGV�����Ƿ��������Ƿ�ɽ���ϵͳ���Ƶ�״̬��Ϣ */
	char m_chError;					/*!< AGV�쳣��Ϣ����������ʾ�û�AGV�쳣�����û����쳣״�������Ų飬�쳣��Ϊ2�֣�1��ϵͳ�������쳣��2��AGV����������쳣 */
	unsigned short m_usAction;		/*!< AGV���������Լ�¼AGV��ǰ����ִ�еĶ������͵�״̬�룬�����Լ�¼�ƶ����ƣ���ͨ���ƣ������������� */
	unsigned char m_byActionStatus;	/*!< AGV����״̬�����Լ�¼AGV���������״̬������0��ʶ����δ��ʼִ�У�1��ʾ��������ִ���У�2��ʾ�����Ѿ�ִ����� */
	unsigned char m_byCargo;		/*!< AGV�ػ���0Ϊ���أ�255��0xFF��Ϊ���� */
	bool m_bAuto;					/*!< AGVģʽ��ΪtrueʱΪ�Զ�ģʽ��AGV��ͨ���������Զ�̿��ƣ�ΪfalseʱΪ�ֶ�ģʽ��AGVֻ��ͨ���û��ֶ��������п��� */
	void *m_pTask;					/*!< ���Լ�¼AGV��ǰִ�е���������,ͨ��Ϊ����ṹ�� */

public:
	/*!
	 * ������������
	*/
	/*!
	 * @brief ���ñ��
	 * @param unsigned char AGV�±��
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetNo(unsigned char byNo);

	/*!
	 * @brief ��ȡ���
	 * @return unsigned char AGV��� 
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief ��������
	 * @param unsigned char AGV������
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetType(unsigned char byType);

	/*!
	 * @brief ��ȡ����
	 * @return unsigned char AGV����
	*/
	unsigned char GetType() const;

	/*!
	 * @brief ���ÿ��ƶ�����
	 * @param unsigned char AGV�¿��ƶ�����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetDriver(unsigned char byDriver);

	/*!
	 * @brief ��ȡ���ƶ�����
	 * @return unsigned char AGV���ƶ�����
	*/
	unsigned char GetDriver() const;

	/*!
	 * @brief ��������ٶ�
	 * @param float AGV������ٶ�
	 * @return bool ���óɹ�����true,ʧ�ܷ���falses
	*/
	bool SetMaxSpeed(float fMaxSpeed);
	
	/*!
	 * @brief ��ȡ����ٶ�
	 * @return float AGV����ٶ�
	*/
	float GetMaxSpeed() const;

	/*!
	 * @brief ���õ���
	 * @param unsigned char AGV�µ���
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetPower(unsigned char byPower);

	/*!
	 * @brief ��ȡ����
	 * @return unsigned char AGV����
	*/
	unsigned char GetPower() const;

	/*!
	 * @brief ���õ�ǰ�ٶ�(%)
	 * @param char AGV�µ�ǰ�ٶ�
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetSpeed(char chSpeed);

	/*!
	 * @brief ��ȡ��ǰ�ٶ�(%)
	 * @return char AGV��ǰ�ٶ�
	*/
	char GetSpeed() const;

	/*!
	 * @brief ��ȡʵ���ٶ�(m/min)
	 * @return float AGV��ǰʵ���ٶ�
	*/
	float GetRealitySpeed() const;

	/*!
	 * @brief ����״̬
	 * @param unsigned char AGV��״̬
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetStatus(unsigned char byStatus);

	/*!
	 * @brief ��ȡ״̬
	 * @return unsinged char AGV״̬
	*/
	unsigned char GetStatus() const;

	/*!
	 * @brief ���ö���
	 * @param unsigned short AGV����
	 * @param unsigned char AGV�¶���״̬
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetAction(unsigned short usAction, unsigned char byStatus);

	/*!
	 * @brief ��ȡ����
	 * @param unsigned short& AGV����
	 * @param unsigned char& AGV����״̬
	*/
	void GetAction(unsigned short& usAction, unsigned char& byStatus);

	/*!
	 * @brief �����쳣
	 * @param char AGV���쳣��
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetError(char chError);

	/*!
	 * @brief ��ȡ�쳣
	 * @return char AGV�쳣��
	*/
	char GetError() const;

	/*!
	 * @brief �·�����
	 * @param void* ����ṹ��ָ��
	*/
	void DownloadTask(void* pTask);

	/*!
	 * @brief �������
	*/
	void ClearTask();

	/*!
	 * @brief ��ȡ����
	*/
	void* GetTask() const;

	/*!
	 * @brief ����ģʽ
	 * @param bool ΪtrueʱΪ�Զ�ģʽ��ΪfalseΪ�ֶ�ģʽ
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetMode(bool bAuto);

	/*!
	 * @brief ��ȡģʽ
	 * @return bool ΪtrueʱΪ�Զ�ģʽ��ΪfalseΪ�ֶ�ģʽ
	*/
	bool GetMode() const;

	/*!
	 * @brief �����ػ�����
	 * @param unsigned char ���ػ�����
	 * @return bool ���óɹ�����true,ʧ�ܷ���false
	*/
	bool SetCargo(unsigned char byCargo);

	/*!
	 * @brief ��ȡ�ػ�����
	 * @return unsigned char �ػ�����
	*/
	unsigned char GetCargo() const;

public:
	/*!
	 * @brief ��ȡ���
	 * @param string& AGV���
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief ��ȡ���
	 * @param wstring& AGV���
	*/
	void GetNo(std::wstring& wstrNo);

	/*!
	 * @brief ��ȡ����
	 * @param string& AGV����
	*/
	void GetType(std::string& strType);

	/*!
	 * @brief ��ȡ����
	 * @param wstring& AGV����
	*/
	void GetType(std::wstring& wstrType);

	/*!
	 * @brief ��ȡ���ƶ�����
	 * @param string& AGV���ƶ�����
	*/
	void GetDriver(std::string& strDriver);

	/*!
	 * @brief ��ȡ���ƶ�����
	 * @param wstring& AGV���ƶ�����
	*/
	void GetDriver(std::wstring& wstrDriver);

	/*!
	 * @brief ��ȡ����ٶ�(m/min)
	 * @param string& AGV����ٶ�(m/min)
	*/
	void GetMaxSpeed(std::string& strSpeed);

	/*!
	 * @brief ��ȡ����ٶ�(m/min)
	 * @param wstring& AGV����ٶ�(m/min)
	*/
	void GetMaxSpeed(std::wstring& wstrSpeed);

	/*!
	 * @brief ��ȡ��ǰ����(%)
	 * @param string& AGV��ǰ����(%)
	*/
	void GetPower(std::string& strPower);

	/*!
	 * @brief ��ȡ��ǰ����(%)
	 * @param wstring& AGV��ǰ����(%)
	*/
	void GetPower(std::wstring& wstrPower);

	/*!
	 * @brief ��ȡ��ǰ�ٶ�(%)
	 * @param string& AGV��ǰ�ٶ�(%)
	*/
	void GetSpeed(std::string& strSpeed);

	/*!
	 * @brief ��ȡ��ǰ�ٶ�(%)
	 * @param wstring& AGV��ǰ�ٶ�(%)
	*/
	void GetSpeed(std::wstring& wstrSpeed);

	/*!
	 * @brief ��ȡ��ǰʵ���ٶ�(m/min)
	 * @param string& AGVʵ���ٶ�(m/min)
	*/
	void GetRealitySpeed(std::string& strSpeed);

	/*!
	 * @brief ��ȡ��ǰʵ���ٶ�(m/min)
	 * @param wstring& AGVʵ���ٶ�(m/min)
	*/
	void GetRealitySpeed(std::wstring& wstrSpeed);

	/*!
	 * @brief ��ȡ״̬��Ϣ
	 * @param string& AGV״̬
	*/
	void GetStatus(std::string& strStatus);

	/*!
	 * @brief ��ȡ״̬��Ϣ
	 * @param wstring& AGV״̬
	*/
	void GetStatus(std::wstring& wstrStatus);

	/*!
	 * @brief ��ȡ������Ϣ
	 * @param string& AGV����
	 * @param string& AGV����״̬
	*/
	void GetAction(std::string& strAction, std::string& strStatus);

	/*!
	 * @brief ��ȡ������Ϣ
	 * @param wstring& AGV����
	 * @param wstring& AGV����״̬
	*/
	void GetAction(std::wstring& wstrAction, std::wstring& wstrStatus);

	/*!
	 * @brief ��ȡ�쳣��Ϣ
	 * @param string& AGV�쳣��Ϣ
	*/
	void GetError(std::string& strError);

	/*!
	 * @brief ��ȡ�쳣��Ϣ
	 * @param wstring& AGV�쳣��Ϣ
	*/
	void GetError(std::wstring& wstrError);

	/*!
	 * @brief ��ȡģʽ
	 * @param string& ģʽ
	*/
	void GetMode(std::string& strMode);

	/*!
	 * @brief ��ȡģʽ
	 * @param wstring& ģʽ
	*/
	void GetMode(std::wstring& wstrMode);

	/*!
	 * @brief ��ȡ�ػ�����
	 * @param string& �ػ�����
	*/
	void GetCargo(std::string& strCargo);

	/*!
	 * @brief ��ȡ�ػ�����
	 * @param wstring& �ػ�����
	*/
	void GetCargo(std::wstring& wstrCargo);

	/*!
	 * @brief ��ȡ������
	 * @param string ����
	 * @return unsigned char ������
	*/
	static unsigned char GetActionCode(std::string strAction);

	/*!
	 * @brief ��ȡ������
	 * @param wstring ����
	 * @return unsigned char ������
	*/
	static unsigned char GetActionCode(std::wstring wstrAction);

	/*!
	 * @brief ��ȡ����
	 * @param unsigned char ������
	 * @param string& ����
	*/
	static void GetAction(unsigned char byActCode, std::string& strAction);

	/*!
	 * @brief ��ȡ����
	 * @param unsigned char ������
	 * @param wstring& ����
	*/
	static void GetAction(unsigned char byActCode, std::wstring& wstrAction);

	/*!
	 * @brief ��ȡ״̬��
	 * @param string ״̬
	 * @return unsigned char ״̬��
	*/
	static unsigned char GetStatusCode(std::string strStatus);

	/*!
	 * @brief ��ȡ״̬��
	 * @param wstring ״̬
	 * @return unsigned char ״̬��
	*/
	static unsigned char GetStatusCode(std::wstring wstrStatus);

	/*!
	 * @brief ��ȡ״̬
	 * @param unsigned char ״̬��
	 * @param string& ״̬
	*/
	static void GetStatus(unsigned char byStaCode, std::string& strStatus);

	/*!
	 * @brief ��ȡ״̬
	 * @param unsigned char ״̬��
	 * @param wstring& ״̬
	*/
	static void GetStatus(unsigned char byStaCode, std::wstring& wstrStatus);

	/*!
	 * @brief ��ȡ�쳣��
	 * @param string �쳣��Ϣ
	 * @return char �쳣��
	*/
	static char GetErrorCode(std::string strError);

	/*!
	 * @brief ��ȡ�쳣��
	 * @param wstring �쳣��Ϣ
	 * @return char �쳣��
	*/
	static char GetErrorCode(std::wstring wstrError);

	/*!
	 * @brief ��ȡ�쳣��Ϣ
	 * @param char �쳣��
	 * @param string& �쳣��Ϣ
	*/
	static void GetErrorMessage(char chErrorCode, std::string& strErrorMsg);

	/*!
	 * @brief ��ȡ�쳣��Ϣ
	 * @param char �쳣��
	 * @param wstring& �쳣��Ϣ
	*/
	static void GetErrorMessage(char chErrorCode, std::wstring& wstrErrorMsg);

	/*!
	 * @brief ��ȡ����״̬��
	 * @param string ����״̬
	 * @return unsigned char ����״̬
	*/
	static unsigned char GetActionStatusCode(std::string strActStatus);

	/*!
	 * @brief ��ȡ����״̬��
	 * @param wstring ����״̬
	 * @return unsigned char ����״̬
	*/
	static unsigned char GetActionStatusCode(std::wstring wstrActStatus);

	/*!
	 * @brief ��ȡ����״̬
	 * @param unsigned char ����״̬��
	 * @param string& ����״̬
	*/
	static void GetActionStatus(unsigned char byActStaCode, std::string& strActStatus);

	/*!
	 * @brief ��ȡ����״̬
	 * @param unsigned char ����״̬��
	 * @param wstring& ����״̬
	*/
	static void GetActionStatus(unsigned char byActStaCode, std::wstring& wstrActStatus);

	/*!
	 * @brief ��ȡ������
	 * @param string& ����
	 * @return unsigned char ������
	*/
	static unsigned char GetTypeCode(std::string strType);

	/*!
	 * @brief ��ȡ������
	 * @param wstring& ����
	 * @return unsigned char ������
	*/
	static unsigned char GetTypeCode(std::wstring wstrType);

	/*!
	 * @brief ��ȡ����
	 * @param unsigned char ������
	 * @param string& ����
	*/
	static void GetType(unsigned char byTypeCode, std::string& strType);

	/*!
	 * @brief ��ȡ����
	 * @param unsigned char ������
	 * @param wstring& ����
	*/
	static void GetType(unsigned char byTypeCode, std::wstring& wstrType);

	/*!
	 * @brief ��ȡ�����з�����
	 * @param string �����з���
	 * @return unsigned char ���з���
	*/
	static unsigned char GetDriveCode(std::string strDrive);

	/*!
	 * @brief ��ȡ�����з�����
	 * @param wstring �����з���
	 * @return unsigned char ���з���
	*/
	static unsigned char GetDriveCode(std::wstring wstrDrive);

	/*!
	 * @brief ��ȡ�����з�����Ϣ
	 * @param unsigned char �����з���
	 * @param string& ���з�����Ϣ
	*/
	static void GetDrive(unsigned char byDriveCode, std::string& strDriveMsg);

	/*!
	 * @brief ��ȡ�����з�����Ϣ
	 * @param unsigned char �����з���
	 * @param wstring& ���з�����Ϣ
	*/
	static void GetDrive(unsigned char byDriveCode, std::wstring& wstrDriveMsg);

#ifdef _AFX
	/*!
	 * @brief ��ȡ���
	 * @param CString& AGV���
	*/
	void GetNo(CString& cstrNo);

	/*!
	 * @brief ��ȡ����
	 * @param CString& AGV����
	*/
	void GetType(CString& cstrType);

	/*!
	 * @brief ��ȡ���ƶ�����
	 * @param CString& AGV���ƶ�����
	*/
	void GetDriver(CString& cstrDriver);

	/*!
	 * @brief ��ȡ����ٶ�(m/min)
	 * @param CString& AGV����ٶ�(m/min)
	*/
	void GetMaxSpeed(CString& cstrSpeed);

	/*!
	 * @brief ��ȡ��ǰ����(%)
	 * @param CString& AGV��ǰ����(%)
	 * @param COLORREF& ������ʾ�û�����ɫ
	*/
	void GetPower(CString& cstrPower, COLORREF& color);

	/*!
	 * @brief ��ȡ��ǰ�ٶ�(%)
	 * @param CString& AGV��ǰ�ٶ�(%)
	 * @param COLORREF& ������ʾ�û�����ɫ
	*/
	void GetSpeed(CString& cstrSpeed, COLORREF& color);

	/*!
	 * @brief ��ȡ��ǰʵ���ٶ�(m/min)
	 * @param CString& AGVʵ���ٶ�(m/min)
	 * @param COLORREF& ������ʾ�û�����ɫ
	*/
	void GetRealitySpeed(CString& cstrSpeed, COLORREF& color);

	/*!
	 * @brief ��ȡ״̬��Ϣ
	 * @param CString& AGV״̬
	 * @param COLORREF& ������ʾ�û�����ɫ
	*/
	void GetStatus(CString& cstrStatus, COLORREF& color);

	/*!
	 * @brief ��ȡ������Ϣ
	 * @param CString& AGV����
	 * @param CString& AGV����״̬
	 * @param COLORREF& ������ʾ�û�����ɫ
	*/
	void GetAction(CString& cstrAction, CString& cstrStatus, COLORREF &color);

	/*!
	 * @brief ��ȡ�쳣��Ϣ
	 * @param CString& AGV�쳣��Ϣ
	 * @param COLORREF& ������ʾ�û�����ɫ
	*/
	void GetError(CString& cstrError, COLORREF& color);

	/*!
	 * @brief ��ȡģʽ
	 * @param CString& ģʽ
	*/
	void GetMode(CString& cstrMode);

	/*!
	 * @brief ��ȡ�ػ�����
	 * @param CString& �ػ�����
	*/
	void GetCargo(CString& cstrCargo);

	/*!
	 * @brief ��ȡ������
	 * @param CString ����
	 * @return unsigned char ������
	*/
	static unsigned char GetActionCode(CString cstrAction);

	/*!
	 * @brief ��ȡ����
	 * @param unsigned char ������
	 * @param CString& ����
	*/
	static void GetAction(unsigned char byActCode, CString& cstrAction);

	/*!
	 * @brief ��ȡ״̬��
	 * @param CString ״̬
	 * @return unsigned char ״̬��
	*/
	static unsigned char GetStatusCode(CString cstrStatus);

	/*!
	 * @brief ��ȡ״̬
	 * @param unsigned char ״̬��
	 * @param CString& ״̬
	*/
	static void GetStatus(unsigned char byStaCode, CString& cstrStatus);

	/*!
	 * @brief ��ȡ�쳣��
	 * @param CString �쳣��Ϣ
	 * @return char �쳣��
	*/
	static char GetErrorCode(CString cstrError);

	/*!
	 * @brief ��ȡ�쳣��Ϣ
	 * @param char �쳣��
	 * @param CString& �쳣��Ϣ
	*/
	static void GetErrorMessage(char chErrorCode, CString& cstrErrorMsg);

	/*!
	 * @brief ��ȡ����״̬��
	 * @param CString ����״̬
	 * @return unsigned char ����״̬
	*/
	static unsigned char GetActionStatusCode(CString cstrActStatus);

	/*!
	 * @brief ��ȡ����״̬
	 * @param unsigned char ����״̬��
	 * @param CString& ����״̬
	*/
	static void GetActionStatus(unsigned char byActStaCode, CString& cstrActStatus);

	/*!
	 * @brief ��ȡ������
	 * @param CString& ����
	 * @return unsigned char ������
	*/
	static unsigned char GetTypeCode(CString cstrType);

	/*!
	 * @brief ��ȡ����
	 * @param unsigned char ������
	 * @param CString& ����
	*/
	static void GetType(unsigned char byTypeCode, CString& cstrType);

	/*!
	 * @brief ��ȡ�����з�����
	 * @param CString �����з���
	 * @return unsigned char ���з���
	*/
	static unsigned char GetDriveCode(CString cstrDrive);

	/*!
	 * @brief ��ȡ�����з�����Ϣ
	 * @param unsigned char �����з���
	 * @param CString& ���з�����Ϣ
	*/
	static void GetDrive(unsigned char byDriveCode, CString& cstrDriveMsg);
#endif // !_AFX
};

#endif //!_AGV_H