#include "stdafx.h"
#include "CProcessData.h"
#include "CRC.h"

#ifdef _PROCESSDATA_H

CProcessData::CProcessData(byte byType)
{
	m_byType = byType;
}

CProcessData::~CProcessData()
{
}

BufferString::Buffer CProcessData::Transfer(BufferString::Buffer bfSource)
{
	return Transfer(bfSource.m_pszBuffer, bfSource.m_unSize);
}

BufferString::Buffer CProcessData::Transfer(byte * bySource, unsigned int unLen)
{
	if (bySource == nullptr || unLen == 0)
	{
		return BufferString::Buffer();
	}

	std::string strDataSource = "";	/*!< Դ���� */
	std::string strDataTrans = "";	/*!< ת������ */
	std::string strDataHex = "";	/*!< ����16�����ַ��� */

	byte *byData = new byte[unLen * 2];	/*!< ת�����ݿ� */

	// ��ʼ�����ݿ�
	memset(byData, 0, unLen * 2);

	// ���汨��ͷ
	byData[0] = bySource[0];

	// ��ȡ16���������ַ���
	BufferString::std_string_format(strDataHex, "%02X ", byData[0]);

	strDataSource += strDataHex;
	strDataTrans += strDataHex;

	int nCount = 1;	/*!< ת������λ�� */

	// ����ת��
	for (unsigned int i = 1; i < unLen-1; i++,nCount++)
	{
		// ��ȡ16���������ַ���
		BufferString::std_string_format(strDataHex, "%02X ", bySource[i]);
		strDataSource += strDataHex;

		if (bySource[i] == 0xB0)
		{
			byData[nCount] = bySource[i];

			nCount++;
			byData[nCount] = 0x01;

			strDataTrans += "B0 01 ";
		}
		else if (bySource[i] == 0xBA)
		{
			byData[nCount] = 0xB0;

			nCount++;
			byData[nCount] = 0x02;

			strDataTrans += "B0 02 ";
		}
		else if (bySource[i] == 0xBB)
		{
			byData[nCount] = 0xB0;

			nCount++;
			byData[nCount] = 0x03;

			strDataTrans += "B0 03 ";
		}
		else
		{
			byData[nCount] = bySource[i];

			// ��ȡ16���������ַ���
			BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);
			strDataTrans += strDataHex;
		}
	}

	// ���汨��β
	byData[nCount] = bySource[unLen - 1];

	// ��ȡ16���������ַ���
	BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);

	strDataSource += strDataHex;
	strDataTrans += strDataHex;

	DebugPrint("AGVControlSystem/CProcessData.cpp/Transfer Դ����:%s.�ַ���:%s.\nת�������:%s.�ַ���:%s.", strDataSource.c_str(), bySource, strDataTrans.c_str(), byData);

	BufferString::Buffer bfTrans(byData, nCount);

	delete[] byData;

	return bfTrans;
}

BufferString::Buffer CProcessData::Retransfer(BufferString::Buffer bfSource)
{
	return Retransfer(bfSource.m_pszBuffer, bfSource.m_unSize);
}

BufferString::Buffer CProcessData::Retransfer(byte * bySource, unsigned int unLen)
{
	if (bySource == nullptr || unLen == 0)
	{
		return BufferString::Buffer();
	}

	std::string strDataSource = "";		/*!< Դ���� */
	std::string strDataRetrans = "";	/*!< ��ת������ */
	std::string strDataHex = "";		/*!< ����16�����ַ��� */

	byte *byData = new byte[unLen];	/*!< ��ת�����ݿ� */

	// ��ʼ�����ݿ�
	memset(byData, 0, unLen);

	// ���汨��ͷ
	byData[0] = bySource[0];

	// ��ȡ16���������ַ���
	BufferString::std_string_format(strDataHex, "%02X ", byData[0]);

	strDataSource += strDataHex;
	strDataRetrans += strDataHex;

	int nCount = 1;	/*!< ��ת������λ�� */

	// ���ݷ�ת��
	for (unsigned int i = 1; i < unLen - 1; i++, nCount++)
	{
		// ��ȡ16���������ַ���
		BufferString::std_string_format(strDataHex, "%02X ", bySource[i]);
		strDataSource += strDataHex;

		if (bySource[i] == 0xB0)
		{
			i++;
			switch (bySource[i])
			{
			case 0x01:
				byData[nCount] = 0xB0;

				strDataRetrans += "B0 ";
				break;
			case 0x02:
				byData[nCount] = 0xBA;

				strDataRetrans += "BA ";
				break;
			case 0x03:

				byData[nCount] = 0xBB;

				strDataRetrans += "BB ";
				break;
			}
		}
		else
		{
			byData[nCount] = bySource[i];

			// ��ȡ16���������ַ���
			BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);
			strDataRetrans += strDataHex;
		}
	}

	// ���汨��β
	byData[nCount] = bySource[unLen - 1];

	// ��ȡ16���������ַ���
	BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);

	strDataSource += strDataHex;
	strDataRetrans += strDataHex;

	DebugPrint("AGVControlSystem/CProcessData.cpp/Retransfer Դ����:%s.�ַ���:%s.\n��ת�������:%s.�ַ���:%s.", strDataSource.c_str(), bySource, strDataRetrans.c_str(), byData);

	BufferString::Buffer bfTrans(byData, nCount);

	delete[] byData;

	return bfTrans;
}

std::list<BufferString::Buffer> CProcessData::CreatePackge(BufferString::Buffer bfSource, unsigned char byNo)
{
	return CreatePackge(bfSource.m_pszBuffer, bfSource.m_unSize, byNo);
}

std::list<BufferString::Buffer> CProcessData::CreatePackge(byte * bySource, unsigned int unLen, unsigned char byNo)
{
	if (bySource == nullptr || unLen == 0)
	{
		return std::list<BufferString::Buffer>();
	}

	unsigned int unMaxLen = 500;		/*!< ������󳤶� */
	unsigned int unDefaultLen = 14;		/*!< Ĭ�ϱ��ĳ��� */

	std::list<BufferString::Buffer> listPackage;

	unsigned int unTotalPackage = unLen / (unMaxLen - unDefaultLen);	/*!< �����ݰ��� */
	unsigned int unLiftLen = unLen % (unMaxLen - unDefaultLen);			/*!< ʣ�����ݳ��� */

	if (unLiftLen > 0)
	{
		unTotalPackage++;
	}

	unsigned short usGroup = 0;			/*!< ���� */
	usGroup = CRC16(bySource, unLen);

	for (unsigned int i = 0; i < unTotalPackage; i++)
	{
		unsigned int unDataLen = unMaxLen - unDefaultLen;		/*!< ���ݳ��� */

		if (i == unTotalPackage - 1)
		{
			if (unLiftLen > 0)
			{
				unDataLen = unLiftLen;
			}	
		}

		unsigned int unPackageLen = unDataLen + unDefaultLen;	/*!< ���ݰ��ܳ� */

		int nIndex = 0;	/*!< �±� */

		byte *byPackage = new byte[unPackageLen];	/*!< �������� */
		// ��ʼ��
		memset(byPackage, 0, unPackageLen);

		byPackage[nIndex] = 0xBA;													/*!< ����ͷ */
		byPackage[++nIndex] = (unPackageLen >> 8) & 0xFF;							/*!< ���ݳ���(��) */
		byPackage[++nIndex] = unPackageLen & 0xFF;									/*!< ���ݳ���(��) */
		byPackage[++nIndex] = (usGroup >> 8) & 0xFF;								/*!< ����(��) */
		byPackage[++nIndex] = usGroup & 0xFF;										/*!< ����(��) */
		byPackage[++nIndex] = (unTotalPackage >> 8) & 0xFF;							/*!< �ܰ���(��) */
		byPackage[++nIndex] = unTotalPackage & 0xFF;								/*!< �ܰ���(��) */
		byPackage[++nIndex] = ((i + 1) >> 8) & 0xFF;								/*!< ��ǰ����(��) */
		byPackage[++nIndex] = (i + 1) & 0xFF;										/*!< ��ǰ����(��) */
		byPackage[++nIndex] = m_byType;												/*!< �豸���� */
		byPackage[++nIndex] = byNo;													/*!< �豸��� */

		memcpy_s(&byPackage[++nIndex], unPackageLen, bySource + (i * (unMaxLen - unDefaultLen)), unDataLen);	/*!< ������ */

		unsigned short usCRC16 = CRC16(byPackage, unPackageLen-3);					/*!< У���� */
		byPackage[unPackageLen-3] = (usCRC16 >> 8) & 0xFF;							/*!< У��λ(��) */
		byPackage[unPackageLen-2] = usCRC16 & 0xFF;									/*!< У��λ(��) */
		byPackage[unPackageLen-1] = 0xBB;											/*!< ����β */

		std::string strDataRecv = "";
		for (unsigned int d = 0; d < unPackageLen; d++)
		{
			std::string strDataHex = "";
			BufferString::std_string_format(strDataHex, "%02X ", byPackage[d]);
			strDataRecv += strDataRecv;
		}

		DebugPrint("AGVControlSystem/CProcessData.cpp/CreatePackge ��������:%s.�ַ���:%s.", strDataRecv.c_str(), byPackage);

		// ת��
		listPackage.push_back(Transfer(byPackage, unPackageLen));
		delete[] byPackage;
	}

	return listPackage;
}

unsigned int CProcessData::Sum(BufferString::Buffer bfSource)
{
	return Sum(bfSource.m_pszBuffer, bfSource.m_unSize);
}

unsigned int CProcessData::Sum(byte * bySource, unsigned int unLen)
{
	std::string strDataRecv = "";
	for (unsigned int i = 0; i < unLen; i++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", bySource[i]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("AGVControlSystem/CProcessData.cpp/Sum Դ����:%s.�ַ�����%s.", strDataRecv.c_str(), bySource);

	int nSum = 0;

	for (unsigned int i = 0; i < unLen; i++)
	{
		nSum += bySource[i];
	}

	nSum = ~nSum;

	DebugPrint("----AGVControlSystem/CProcessData.cpp/Sum У����:%04X.ʮ����:%d.", nSum, nSum);

	return nSum;
}

unsigned short CProcessData::CRC16(BufferString::Buffer bfSource)
{
	return CRC::CRC16(bfSource.m_pszBuffer, bfSource.m_unSize);
}

unsigned short CProcessData::CRC16(byte * bySource, unsigned int unLen)
{
	std::string strDataRecv = "";
	for (unsigned int i = 0; i < unLen; i++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", bySource[i]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("AGVControlSystem/CProcessData.cpp/CRC16 Դ����:%s.�ַ�����%s.", strDataRecv.c_str(), bySource);

	return CRC::CRC16(bySource, unLen);
}

std::list<BufferString::Buffer> CProcessData::Process(BufferString::Buffer bfSource, BufferString::Buffer & bfResidue)
{
	return Process(bfSource.m_pszBuffer, bfSource.m_unSize, bfResidue);
}

std::list<BufferString::Buffer> CProcessData::Process(byte * bySource, unsigned int unLen, BufferString::Buffer & bfResidue)
{
	if (bySource == nullptr || unLen == 0)
	{
		return std::list<BufferString::Buffer>();
	}

	std::list<BufferString::Buffer> m_listPackage;	/*!< ���ļ��� */

	byte* byResidue = bySource;			/*!< ʣ�����ݿ� */
	unsigned int unResidueLen = unLen;	/*!< ʣ�����ݿ��С */

	while (byResidue)
	{
		byte* byHead = (byte*)memchr(byResidue, 0xBA, unResidueLen);	/*!< ����ͷ */

		// δ�����ݿ����ҵ�����ͷ
		if (byHead == nullptr)
		{
			byResidue = nullptr;
			unResidueLen = 0;
			break;
		}

		byResidue = byHead;
		unResidueLen = unLen - (byResidue - bySource);

		byte* byTail = (byte*)memchr(byResidue, 0xBB, unResidueLen);	/*!< ����β */

		// δ�����ݿ����ҵ�����ͷ
		if (byTail == nullptr)
		{
			break;
		}

		// ����β�Ѿ������ݿ�ĩβ
		if (byTail == &bySource[unLen - 1])
		{
			byResidue = nullptr;
			unResidueLen = 0;
		}
		else
		{
			byResidue = byTail + 1;
			unResidueLen = unLen - (byResidue - bySource);
		}

		BufferString::Buffer bfTrans = Retransfer(byHead, (byTail - byHead) + 1);		/*!< ��ת������ */

		unsigned short usCRC16 = CRC16(byHead, (byTail - byHead) - 1);	/*!< CRCУ���� */
		unsigned short usCheck = *(byTail - 2) << 8 | *(byTail - 1);	/*!< ������У��ֵ */

		DebugPrint("AGVControlSystem/CProcessData.cpp/Process �豸����У��:%04X:%04X.", m_byType, *(byHead + 6));

		// �豸����У��
		if (*(byHead + 6) != m_byType)
		{
			continue;
		}

		DebugPrint("----AGVControlSystem/CProcessData.cpp/Process ����У��:%04X:%04X.", usCRC16, usCheck);

		// У����У��
		if (usCRC16 != usCheck)
		{
			continue;
		}

		/*!
		 * ɸ����ȫ��ͬ�ı���
		*/
		bool bSame = false;
		for (std::list<BufferString::Buffer>::iterator it = m_listPackage.begin(); it != m_listPackage.end();it++)
		{
			if (*it == bfTrans)
			{
				bSame = true;
				break;
			}
		}

		if (bSame)
		{
			continue;
		}

		m_listPackage.push_back(bfTrans);

		std::string strDataRecv = "";
		for (unsigned int i = 0; i < bfTrans.m_unSize; i++)
		{
			std::string strDataHex = "";
			BufferString::std_string_format(strDataHex, "%02X ", bfTrans.m_pszBuffer[i]);
			strDataRecv += strDataRecv;
		}

		DebugPrint("--------AGVControlSystem/CProcessData.cpp/Process ���İ�����:%s.�ַ���:%s.", strDataRecv.c_str(), byHead);
	}

	if (unResidueLen > 0)
	{
		bfResidue = BufferString::Buffer(byResidue, unResidueLen);
	}
	else
	{
		bfResidue = BufferString::Buffer();
	}

	return m_listPackage;
}

std::list<BufferString::Buffer> CProcessData::Joint(std::list<BufferString::Buffer> bfList, std::list<BufferString::Buffer>& bfLiftList)
{
	std::list<BufferString::Buffer> listFull;
	std::list<JointBuffer> listJoint;

	for (std::list<BufferString::Buffer>::iterator it = bfList.begin();it != bfList.end();)
	{
		BufferString::Buffer& bfPackage = *it;		/*!< ���İ� */
		int nIndex = 3;				/*!< �±� */

		unsigned int unGroup = (bfPackage.m_pszBuffer[nIndex] << 8) | bfPackage.m_pszBuffer[++nIndex];		/*!< ���� */
		unsigned int unTotal = (bfPackage.m_pszBuffer[++nIndex] << 8) | bfPackage.m_pszBuffer[++nIndex];	/*!< �ܰ��� */
		unsigned int unCur = (bfPackage.m_pszBuffer[++nIndex] << 8) | bfPackage.m_pszBuffer[++nIndex];		/*!< ��ǰ���� */

		BufferString::Buffer bfData(&bfPackage.m_pszBuffer[++nIndex], bfPackage.m_unSize - 12);	/*!< ������ */

		// �������ݰ�
		if (unTotal == unCur && unTotal == 1)
		{
			listFull.push_back(bfData);
		}
		// ������ݰ�
		else
		{
			// ���ݽ��ղ���ȫ
			if (unTotal > bfList.size())
			{
				break;
			}

			bool bJoint = false;

			for (std::list<JointBuffer>::iterator itJoint = listJoint.begin(); itJoint != listJoint.end();)
			{
				JointBuffer& pJoint = *itJoint;

				if (pJoint.AddJointData(bfPackage))
				{
					bJoint = true;

					BufferString::Buffer bfFull;
					if (pJoint.IsFull(bfFull))
					{
						listFull.push_back(bfFull);
					}

					break;
				}
			}

			if (bJoint == false)
			{
				JointBuffer joint(unGroup, unTotal);

				joint.AddJointData(bfPackage);

				listJoint.push_back(joint);
			}
		}

		it = bfList.erase(it);
	}

	bfLiftList = bfList;

	return listFull;
}

/*-------------------------------------��������·������-----------------------------------------------*/
BufferString::Buffer CProcessData::TransferPath(BufferString::Buffer bfSource)
{
	return TransferPath(bfSource.m_pszBuffer, bfSource.m_unSize);
}

BufferString::Buffer CProcessData::TransferPath(byte * bySource, unsigned int unLen)
{
	if (bySource == nullptr || unLen == 0)
	{
		return BufferString::Buffer();
	}

	std::string strDataSource = "";	/*!< Դ���� */
	std::string strDataTrans = "";	/*!< ת������ */
	std::string strDataHex = "";	/*!< ����16�����ַ��� */

	byte *byData = new byte[unLen * 2];	/*!< ת�����ݿ� */

	// ��ʼ�����ݿ�
	memset(byData, 0, unLen * 2);

	// ���汨��ͷ
	byData[0] = bySource[0];

	// ��ȡ16���������ַ���
	BufferString::std_string_format(strDataHex, "%02X ", byData[0]);

	strDataSource += strDataHex;
	strDataTrans += strDataHex;

	int nCount = 1;	/*!< ת������λ�� */

	// ����ת��
	for (unsigned int i = 1; i < unLen - 1; i++, nCount++)
	{
		// ��ȡ16���������ַ���
		BufferString::std_string_format(strDataHex, "%02X ", bySource[i]);
		strDataSource += strDataHex;

		if (bySource[i] == 0xA0)
		{
			byData[nCount] = bySource[i];

			nCount++;
			byData[nCount] = 0x01;

			strDataTrans += "A0 01 ";
		}
		else if (bySource[i] == 0xAA)
		{
			byData[nCount] = 0xA0;

			nCount++;
			byData[nCount] = 0x02;

			strDataTrans += "A0 02 ";
		}
		else if (bySource[i] == 0xAB)
		{
			byData[nCount] = 0xA0;

			nCount++;
			byData[nCount] = 0x03;

			strDataTrans += "A0 03 ";
		}
		else
		{
			byData[nCount] = bySource[i];

			// ��ȡ16���������ַ���
			BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);
			strDataTrans += strDataHex;
		}
	}

	// ���汨��β
	byData[nCount] = bySource[unLen - 1];

	// ��ȡ16���������ַ���
	BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);

	strDataSource += strDataHex;
	strDataTrans += strDataHex;

	DebugPrint("AGVControlSystem/CProcessData.cpp/TransferPath Դ����:%s.�ַ���:%s.\nת�������:%s.�ַ���:%s.", strDataSource.c_str(), bySource, strDataTrans.c_str(), byData);

	BufferString::Buffer bfTrans(byData, nCount);

	delete[] byData;

	return bfTrans;
}

BufferString::Buffer CProcessData::RetransferPath(BufferString::Buffer bfSource)
{
	return Retransfer(bfSource.m_pszBuffer, bfSource.m_unSize);
}

BufferString::Buffer CProcessData::RetransferPath(byte * bySource, unsigned int unLen)
{
	if (bySource == nullptr || unLen == 0)
	{
		return BufferString::Buffer();
	}

	std::string strDataSource = "";		/*!< Դ���� */
	std::string strDataRetrans = "";	/*!< ��ת������ */
	std::string strDataHex = "";		/*!< ����16�����ַ��� */

	byte *byData = new byte[unLen];	/*!< ��ת�����ݿ� */

	// ��ʼ�����ݿ�
	memset(byData, 0, unLen);

	// ���汨��ͷ
	byData[0] = bySource[0];

	// ��ȡ16���������ַ���
	BufferString::std_string_format(strDataHex, "%02X ", byData[0]);

	strDataSource += strDataHex;
	strDataRetrans += strDataHex;

	int nCount = 1;	/*!< ��ת������λ�� */

	// ���ݷ�ת��
	for (unsigned int i = 1; i < unLen - 1; i++, nCount++)
	{
		// ��ȡ16���������ַ���
		BufferString::std_string_format(strDataHex, "%02X ", bySource[i]);
		strDataSource += strDataHex;

		if (bySource[i] == 0xA0)
		{
			i++;
			switch (bySource[i])
			{
			case 0x01:
				byData[nCount] = 0xA0;

				strDataRetrans += "A0 ";
				break;
			case 0x02:
				byData[nCount] = 0xAA;

				strDataRetrans += "AA ";
				break;
			case 0x03:

				byData[nCount] = 0xAB;

				strDataRetrans += "AB ";
				break;
			}
		}
		else
		{
			byData[nCount] = bySource[i];

			// ��ȡ16���������ַ���
			BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);
			strDataRetrans += strDataHex;
		}
	}

	// ���汨��β
	byData[nCount] = bySource[unLen - 1];

	// ��ȡ16���������ַ���
	BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);

	strDataSource += strDataHex;
	strDataRetrans += strDataHex;

	DebugPrint("AGVControlSystem/CProcessData.cpp/RetransferPath Դ����:%s.�ַ���:%s.\n��ת�������:%s.�ַ���:%s.", strDataSource.c_str(), bySource, strDataRetrans.c_str(), byData);

	BufferString::Buffer bfTrans(byData, nCount);

	delete[] byData;

	return bfTrans;
}

BufferString::Buffer CProcessData::CreatePackgePath(BufferString::Buffer bfSource, unsigned char byNo)
{
	return CreatePackgePath(bfSource.m_pszBuffer, bfSource.m_unSize, byNo);
}

BufferString::Buffer CProcessData::CreatePackgePath(byte * bySource, unsigned int unLen, unsigned char byNo)
{
	/* 2�ֽ� �ܵر꿨��  5�ֽ� �յ�ر꿨�� N*5�ֽ� �����ر꿨�� */
	if (bySource == nullptr || unLen == 0)
	{
		return BufferString::Buffer();
	}

	unsigned int unDefaultLen = 4;	/*!< Ĭ�ϱ��ĳ��� */

	unsigned int unPackageLen = unLen + unDefaultLen;
	unsigned int unTotal = unLen / 5; /*!< ��RFID�������� */

	byte *byPackage = new byte[unPackageLen];
	// ��ʼ��
	memset(byPackage, 0, unPackageLen);

	int nIndex = 0;

	byPackage[nIndex] = 0xAA;												/*!< ����ͷ */
	byPackage[++nIndex] = (unTotal >> 8) & 0xFF;							/*!< RFID������ */
	byPackage[++nIndex] = unTotal & 0xFF;									/*!< RFID������ */

	memcpy_s(&byPackage[++nIndex], unPackageLen, bySource, unLen);			/*!< ������ */

	unsigned short usCRC16 = CRC16(byPackage, unPackageLen - 3);			/*!< У���� */
	byPackage[unPackageLen - 3] = (usCRC16 >> 8) & 0xFF;					/*!< У��λ */
	byPackage[unPackageLen - 2] = usCRC16 & 0xFF;							/*!< У��λ */
	byPackage[unPackageLen - 1] = 0xAB;										/*!< ����β */

	std::string strDataRecv = "";
	for (unsigned int d = 0; d < unPackageLen; d++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", byPackage[d]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("AGVControlSystem/CProcessData.cpp/CreatePackgePath ��������:%s.�ַ���:%s.", strDataRecv.c_str(), byPackage);

	// ת��
	BufferString::Buffer tranf = Transfer(byPackage, unPackageLen);
	delete[] byPackage;

	return tranf;
}

std::list<BufferString::Buffer> CProcessData::ProcessPath(BufferString::Buffer bfSource, BufferString::Buffer & bfResidue)
{
	return Process(bfSource.m_pszBuffer, bfSource.m_unSize, bfResidue);
}

std::list<BufferString::Buffer> CProcessData::ProcessPath(byte * bySource, unsigned int unLen, BufferString::Buffer & bfResidue)
{
	if (bySource == nullptr || unLen == 0)
	{
		return std::list<BufferString::Buffer>();
	}

	std::list<BufferString::Buffer> m_listPackage;	/*!< ���ļ��� */

	byte* byResidue = bySource;			/*!< ʣ�����ݿ� */
	unsigned int unResidueLen = unLen;	/*!< ʣ�����ݿ��С */

	while (byResidue)
	{
		byte* byHead = (byte*)memchr(byResidue, 0xAA, unResidueLen);	/*!< ����ͷ */

		// δ�����ݿ����ҵ�����ͷ
		if (byHead == nullptr)
		{
			byResidue = nullptr;
			unResidueLen = 0;
			break;
		}

		byResidue = byHead;
		unResidueLen = unLen - (byResidue - bySource);

		byte* byTail = (byte*)memchr(byResidue, 0xAB, unResidueLen);	/*!< ����β */

		// δ�����ݿ����ҵ�����ͷ
		if (byTail == nullptr)
		{
			break;
		}

		// ����β�Ѿ������ݿ�ĩβ
		if (byTail == &bySource[unLen - 1])
		{
			byResidue = nullptr;
			unResidueLen = 0;
		}
		else
		{
			byResidue = byTail + 1;
			unResidueLen = unLen - (byResidue - bySource);
		}

		BufferString::Buffer bfTrans = Retransfer(byHead, (byTail - byHead) + 1);		/*!< ��ת������ */

		unsigned short usCRC16 = CRC16(byHead, (byTail - byHead) - 1);	/*!< CRCУ���� */
		unsigned short usCheck = *(byTail - 2) << 8 | *(byTail - 1);	/*!< ������У��ֵ */

		DebugPrint("----AGVControlSystem/CProcessData.cpp/ProcessPath ����У��:%04X:%04X.", usCRC16, usCheck);

		// У����У��
		if (usCRC16 != usCheck)
		{
			continue;
		}

		m_listPackage.push_back(bfTrans);

		std::string strDataRecv = "";
		for (unsigned int i = 0; i < bfTrans.m_unSize; i++)
		{
			std::string strDataHex = "";
			BufferString::std_string_format(strDataHex, "%02X ", bfTrans.m_pszBuffer[i]);
			strDataRecv += strDataRecv;
		}

		DebugPrint("--------AGVControlSystem/CProcessData.cpp/ProcessPath ���İ�����:%s.�ַ���:%s.", strDataRecv.c_str(), byHead);
	}

	if (unResidueLen > 0)
	{
		bfResidue = BufferString::Buffer(byResidue, unResidueLen);
	}
	else
	{
		bfResidue = BufferString::Buffer();
	}

	return m_listPackage;
}

#endif // !_PROCESSDATA_H