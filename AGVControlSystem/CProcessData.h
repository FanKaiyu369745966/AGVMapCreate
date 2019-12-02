/*!
 * @file CProcessData.h
 * @brief ���ݴ�������ļ�
 *
 * @author FanKaiyu
 * @date 2019-01-09
 * @version 1.2
 *
 * @update 2019-01-18
 * @author FanKaiyu
 * @brief ���Ӷ�·�����ĵĺϳɡ�������ת�塢��ת��.
 *
 * @update 2019-01-21
 * @author FanKaiyu
 * @brief ���Ľṹ�����������ֽ�,���ڴ����ݵ�ƴ��.
*/
#pragma once
#include "Buffer.h"
#include <list>
#include "CRC.h"

#ifndef _PROCESSDATA_H
#define _PROCESSDATA_H

/*! @brief ƴ�����ݿ� */
typedef struct _JointBufferStruct
{
public:
	unsigned short m_usGroup;	/*!< ��� */
	unsigned short m_usTotal;	/*!< �ܰ��� */
	BufferString::Buffer *m_pArray;			/*!< ƴ�����ݰ����� */

public:
	_JointBufferStruct(unsigned short usGroup, unsigned short usTotal)
	{
		m_usGroup = usGroup;
		m_usTotal = usTotal;
		m_pArray = new BufferString::Buffer[m_usTotal];
	}
	          
	_JointBufferStruct(const _JointBufferStruct& joint)
	{		
		m_usGroup = joint.m_usGroup;
		m_usTotal = joint.m_usTotal;

		m_pArray = new BufferString::Buffer[m_usTotal];

		for (int i = 0; i < m_usTotal; i++)
		{
			m_pArray[i] = joint.m_pArray[i];
		}
	}

	~_JointBufferStruct()
	{
		if (m_pArray)
		{
			delete[] m_pArray;
		}
	}

	void operator=(const _JointBufferStruct& joint)
	{
		if (m_pArray)
		{
			delete[] m_pArray;
		}

		m_usGroup = joint.m_usGroup;
		m_usTotal = joint.m_usTotal;

		m_pArray = new BufferString::Buffer[m_usTotal];

		for (int i = 0; i < m_usTotal; i++)
		{
			m_pArray[i] = joint.m_pArray[i];
		}
	}

public:
	/*!
	 * @brief �����Ƿ��Ѿ�ƴ������
	 * @return bool ������������true,���򷵻�false
	*/
	bool IsFull() const
	{
		if (m_pArray == nullptr)
		{
			return false;
		}

		for (int i = 0; i < m_usTotal; i++)
		{
			if (m_pArray[i].m_unSize == 0 || m_pArray[i].m_pszBuffer == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	/*!
	 * @brief �����Ƿ��Ѿ�ƴ������
	 * @param Buffer& ���������ݿ�
	 * @return bool ������������true,���򷵻�false
	*/
	bool IsFull(BufferString::Buffer& bfData) const
	{
		if (IsFull())
		{
			bfData = CreateFullData();

			if (bfData.m_unSize == 0)
			{
				return false;
			}

			return true;
		}

		return false;
	}

	/*!
	 * @brief �ϳ�����������
	 * @return Buffer ���������ݿ�
	*/
	BufferString::Buffer CreateFullData() const
	{
		BufferString::Buffer fullData;

		if (m_pArray == nullptr)
		{
			return fullData;
		}

		unsigned int unDefaultLen = 14;		/*!< Ĭ�ϱ��ĳ��� */

		int nIndex = 11;	/*!< �±� */

		for (int i = 0; i < m_usTotal; i++)
		{
			if (m_pArray[i].m_unSize == 0 || m_pArray[i].m_pszBuffer == nullptr)
			{
				return fullData;
			}

			fullData += BufferString::Buffer(&m_pArray[i].m_pszBuffer[nIndex], m_pArray->m_unSize - unDefaultLen);	/*!< ��������� */
		}

		unsigned short usGroup = CRC::CRC16(fullData.m_pszBuffer, fullData.m_unSize);	/*!< ���� */

		fullData = BufferString::Buffer(&m_pArray[0].m_pszBuffer[nIndex - 2], 2) + fullData;	/*!< �������� */

		if (usGroup != m_usGroup)
		{
			return BufferString::Buffer();
		}

		return fullData;
	}

	/*!
	 * @brief ���ƴ�����ݰ�
	 * @return ��ӳɹ�����true,ʧ�ܷ���false
	*/
	bool AddJointData(BufferString::Buffer bfData)
	{
		int nIndex = 3;	/*!< �±� */

		unsigned short usGroup = (bfData.m_pszBuffer[nIndex] << 8) | bfData.m_pszBuffer[++nIndex];	/*!< ���� */

		if (usGroup != m_usGroup)
		{
			return false;
		}

		unsigned short usTotal = (bfData.m_pszBuffer[++nIndex] << 8) | bfData.m_pszBuffer[++nIndex];	/*!< �ܰ��� */
		unsigned short usCur = (bfData.m_pszBuffer[++nIndex] << 8) | bfData.m_pszBuffer[++nIndex];		/*!< ��ǰ���� */

		if (m_usTotal != usTotal)
		{
			if (m_pArray)
			{
				delete[] m_pArray;
			}

			m_usTotal = usTotal;
			m_pArray = new BufferString::Buffer[usTotal];
		}

		m_pArray[usCur - 1] = bfData;

		return true;
	}
} JointBuffer;

/*!
 * @class CProcessData
 * @brief ���ݴ������
 *
 * ���ݱ���ָ�����򣬽����ݽ���ת�塢��ת�塢�������ϳɵȲ���
*/
class CProcessData
{
public:
	CProcessData(byte byType);
	~CProcessData();
protected:
	byte m_byType; /*!< �豸���� */

protected:
	/*!
	 * @brief ת��
	 * @param Buffer Դ���ݿ�
	 * @return Buffer ת�������
	*/
	BufferString::Buffer Transfer(BufferString::Buffer bfSource);

	/*!
	 * @brief ת��
	 * @param byte* Դ���ݿ��׵�ַָ��
	 * @param unsigned ine Դ���ݿ鳤��
	 * @return Buffer ת�������
	*/
	BufferString::Buffer Transfer(byte* bySource, unsigned int unLen);

	/*!
	 * @brief ��ת��
	 * @param Buffer Դ����
	 * @return Buffer ��ת��������
	*/
	BufferString::Buffer Retransfer(BufferString::Buffer bfSource);

	/*!
	 * @brief ��ת��
	 * @param byte* Դ���ݿ��׵�ַָ��
	 * @param unsigned ine Դ���ݿ鳤��
	 * @return Buffer ��ת��������
	*/
	BufferString::Buffer Retransfer(byte* bySource, unsigned int unLen);

	/*!
	 * @brief �ϳ�
	 * @param Buffer Դ���ݿ�
	 * @param unsigned char �豸���
	 * @return std::list<Buffer> �ϳɺ�ı��ļ���
	*/
	std::list<BufferString::Buffer> CreatePackge(BufferString::Buffer bfSource,unsigned char byNo);

	/*!
	 * @brief �ϳ�
	 * @param byte* Դ���ݿ��׵�ַָ��
	 * @param unsigned ine Դ���ݿ鳤��
	 * @param unsigned char �豸���
	 * @return std::list<Buffer> �ϳɺ�ı��ļ���
	*/
	std::list<BufferString::Buffer> CreatePackge(byte* bySource,unsigned int unLen, unsigned char byNo);

	/*!
	 * @brief ���ݺ�У��
	 *
	 * �����ݰ��ֽ��������
	 * ����ӵó��ĺ�ȡ��
	 * @param Buffer Դ���ݿ�
	 * @return unsigned int У����
	*/
	unsigned int Sum(BufferString::Buffer bfSource);

	/*!
	 * @brief ���ݺ�У��
	 *
	 * �����ݰ��ֽ��������
	 * ����ӵó��ĺ�ȡ��
	 * @param byte* Դ���ݿ��׵�ַָ��
	 * @param unsigned ine Դ���ݿ鳤��
	 * @return unsigned int У����
	*/
	unsigned int Sum(byte* bySource, unsigned int unLen);

	/*!
	 * @brief CRC16У��
	 * @param Buffer Դ���ݿ�
	 * @return unsigned short У����
	*/
	unsigned short CRC16(BufferString::Buffer bfSource);

	/*!
	 * @brief CRC16У��
	 * @param byte* Դ���ݿ��׵�ַָ��
	 * @param unsigned ine Դ���ݿ鳤��
	 * @return unsigned short У����
	*/
	unsigned short CRC16(byte* bySource, unsigned int unLen);

public:
	/*!
	 * @brief ����
	 * @param Buffer Դ���ݿ�
	 * @param Buffer& ʣ�����ݿ�
	 * @return �������ı��ļ���
	*/
	std::list<BufferString::Buffer> Process(BufferString::Buffer bfSource, BufferString::Buffer &bfResidue);

	/*!
	 * @brief ����
	 * @param byte* Դ���ݿ��׵�ַָ��
	 * @param unsigned ine Դ���ݿ鳤��
	 * @param Buffer& ʣ�����ݿ�
	 * @return �������ı��ļ���
	*/
	std::list<BufferString::Buffer> Process(byte* bySource, unsigned int unLen, BufferString::Buffer &bfResidue);

	/*!
	 * @brief ƴ��
	 * @param std::list<Buffer> ��ƴ�ӵ����ݼ���
	 * @param std::list<Buffer>& ʣ��δƴ�ӵ����ݼ���
	 * @return std::list<Buffer> ƴ����ɵ����ݼ���
	*/
	std::list<BufferString::Buffer> Joint(std::list<BufferString::Buffer> bfList, std::list<BufferString::Buffer>& bfLiftList);

/*-------------------------------------��������·������-----------------------------------------------*/
protected:
	/*!
	 * @brief ת��·����
	 * @param Buffer Դ���ݿ�
	 * @return Buffer ת�������
	*/
	BufferString::Buffer TransferPath(BufferString::Buffer bfSource);

	/*!
	 * @brief ת��·����
	 * @param byte* Դ���ݿ��׵�ַָ��
	 * @param unsigned ine Դ���ݿ鳤��
	 * @return Buffer ת�������
	*/
	BufferString::Buffer TransferPath(byte* bySource, unsigned int unLen);

	/*!
	 * @brief ��ת��·����
	 * @param Buffer Դ����
	 * @return Buffer ��ת��������
	*/
	BufferString::Buffer RetransferPath(BufferString::Buffer bfSource);

	/*!
	 * @brief ��ת��·����
	 * @param byte* Դ���ݿ��׵�ַָ��
	 * @param unsigned ine Դ���ݿ鳤��
	 * @return Buffer ��ת��������
	*/
	BufferString::Buffer RetransferPath(byte* bySource, unsigned int unLen);

	/*!
	 * @brief �ϳ�·����
	 * @param Buffer Դ���ݿ�
	 * @param unsigned char �豸���
	 * @return Buffer �ϳɺ�ı���
	*/
	BufferString::Buffer CreatePackgePath(BufferString::Buffer bfSource, unsigned char byNo);

	/*!
	 * @brief �ϳ�·����
	 * @param byte* Դ���ݿ��׵�ַָ��
	 * @param unsigned ine Դ���ݿ鳤��
	 * @param unsigned char �豸���
	 * @return Buffer �ϳɺ�ı���
	*/
	BufferString::Buffer CreatePackgePath(byte* bySource, unsigned int unLen, unsigned char byNo);

public:
	/*!
	 * @brief ����·����
	 * @param Buffer Դ���ݿ�
	 * @param Buffer& ʣ�����ݿ�
	 * @return �������ı��ļ���
	*/
	std::list<BufferString::Buffer> ProcessPath(BufferString::Buffer bfSource, BufferString::Buffer &bfResidue);

	/*!
	 * @brief ����·����
	 * @param byte* Դ���ݿ��׵�ַָ��
	 * @param unsigned ine Դ���ݿ鳤��
	 * @param Buffer& ʣ�����ݿ�
	 * @return �������ı��ļ���
	*/
	std::list<BufferString::Buffer> ProcessPath(byte* bySource, unsigned int unLen, BufferString::Buffer &bfResidue);
};

#endif // !_PROCESSDATA_H