#pragma once
#include "stdafx.h"
#include <string>
#include <list>
#include <map>
#include "ADOConn.h"

using namespace std;

/*!
 * WMS�����ṹ
 * ���� WMS_CARGO_TABLE
 * �ֶ� WMSCargo_No ���� int ˵�������ܱ��
 * �ֶ� WMSCargo_Row ���� int ˵����������
 * �ֶ� WMSCargo_Col ���� int  ˵����������
 * �ֶ� WMSCargo_Unit ���� vchar ˵������λ
 * �ֶ� WMSCargo_Type ���� vchar ˵������������
 * �ֶ� WMSCargo_Number ���� int ˵��������
 * �ֶ� WMSCargo_Intime ���� datetime ˵�������ʱ��
*/

/*!< @brief �����Ϣ�ṹ�� */
typedef struct _CargoStruct
{
	unsigned int m_unNo;		/*!< ���ܱ�� */
	unsigned int m_unRow;		/*!< �� */
	unsigned int m_unCol;		/*!< �� */
	string m_strType;			/*!< �������� */
	string m_strUnit;			/*!< ��λ */
	unsigned int m_unNumber;	/*!< ���� */
	string m_strIntime;			/*!< ���ʱ�� */

	_CargoStruct()
	{
		m_unNo = 0;
		m_unRow = 0;
		m_unCol = 0;
		m_strType = "";
		m_strUnit = "";
		m_unNumber = 0;
		m_strIntime = "";
	}

	_CargoStruct(const _CargoStruct& cargo)
	{
		m_unNo = cargo.m_unNo;
		m_unRow = cargo.m_unRow;
		m_unCol = cargo.m_unCol;
		m_strType = cargo.m_strType;
		m_strUnit = cargo.m_strUnit;
		m_unNumber = cargo.m_unNumber;
		m_strIntime = cargo.m_strIntime;
	}

	void operator=(const _CargoStruct& cargo)
	{
		m_unNo = cargo.m_unNo;
		m_unRow = cargo.m_unRow;
		m_unCol = cargo.m_unCol;
		m_strType = cargo.m_strType;
		m_strUnit = cargo.m_strUnit;
		m_unNumber = cargo.m_unNumber;
		m_strIntime = cargo.m_strIntime;
	}

	bool IsNull()
	{
		if (m_unNo == 0 || m_unRow == 0 || m_unCol == 0 || m_strType == "" || m_strUnit == "" || m_unNumber == 0 || m_strIntime == "")
		{
			return true;
		}

		return false;
	}
} _CARGO;

/*!
 * @brief ��¼�����Ϣ
 * @param ADOConn& ���ݿ����Ӵ�
 * @param unsigned int ���ܱ��
 * @param unsigned int ������
 * @param unsigned int ������
 * @param string ��������
 * @param string ��λ
 * @param unsigend int ����
 * @return bool ��¼�ɹ�����true,ʧ�ܷ���false
*/
static bool RecordIn(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol, string strType, string strUnit, unsigned int unNumber)
{
	CString cstrSql = _T("");
	cstrSql.Format(_T("insert into WMS_CARGO_TABLE([WMSCargo_No],[WMSCargo_Row],[WMSCargo_Col],[WMSCargo_Unit],[WMSCargo_Type],[WMSCargo_Number]) values(%d,%d,%d,'%s','%s',%d)"), unNo, unRow, unCol, strUnit.c_str(), strType.c_str(), unNumber);

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief ��������Ϣ
 * @param ADOConn& ���ݿ����Ӵ�
 * @param unsigned int ���ܱ��
 * @param unsigned int ������
 * @param unsigned int ������
 * @return bool ����ɹ�����true,ʧ�ܷ���false
*/
static bool ClearIn(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol)
{
	CString cstrSql = _T("");
	cstrSql.Format(_T("delete WMS_CARGO_TABLE where [WMSCargo_No]=%d and [WMSCargo_Row]=%d and [WMSCargo_Col]=%d"), unNo, unRow, unCol);

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief ���ȫ�������Ϣ
 * @param ADOConn& ���ݿ����Ӵ�
 * @return bool ����ɹ�����true,ʧ�ܷ���false
*/
static bool ClearAllCargo(ADOConn& ado)
{
	CString cstrSql = _T("delete WMS_CARGO_TABLE");

	return ado.ExecuteSQL(_bstr_t(cstrSql));
}

/*!
 * @brief ��ȡ�����Ϣ
 * @param ADOConn& ���ݿ����Ӵ�
 * @param unsigned int ���ܱ��
 * @param unsigned int ������
 * @param unsigned int ������
 * @return _CARGO �����Ϣ�ṹ��
*/
static _CARGO GetCargo(ADOConn& ado, unsigned int unNo, unsigned int unRow, unsigned int unCol)
{
	_CARGO cargo;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_CARGO_TABLE where [WMSCargo_No]=%d and [WMSCargo_Row]=%d and [WMSCargo_Col]=%d"), unNo, unRow, unCol);

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	if (pRecordset == nullptr || pRecordset->adoEOF)
	{
		return cargo;
	}

	_variant_t var;	/*!< �ֶ�ֵ */

	var = pRecordset->GetCollect(_T("WMSCargo_No"));
	if (var.vt != VT_NULL)
	{
		cargo.m_unNo = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSCargo_Row"));
	if (var.vt != VT_NULL)
	{
		cargo.m_unRow = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSCargo_Col"));
	if (var.vt != VT_NULL)
	{
		cargo.m_unCol = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
	if (var.vt != VT_NULL)
	{
		cargo.m_strUnit = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("WMSCargo_Type"));
	if (var.vt != VT_NULL)

	{
		cargo.m_strType = _bstr_t(var);
	}

	var = pRecordset->GetCollect(_T("WMSCargo_Number"));
	if (var.vt != VT_NULL)
	{
		cargo.m_unNumber = (unsigned int)var;
	}

	var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
	if (var.vt != VT_NULL)
	{
		cargo.m_strIntime = _bstr_t(var);
	}

	return cargo;
}

/*!
 * @brief ��ȡȫ�������Ϣ
 * @param ADOConn& ���ݿ����Ӵ�
 * @return list<_CARGO> �����Ϣ�б�
*/
static list<_CARGO> GetAllCargo(ADOConn& ado)
{
	list<_CARGO> listCargo;

	CString cstrSql = _T("select * from WMS_CARGO_TABLE");

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_CARGO cargo;

		_variant_t var;	/*!< �ֶ�ֵ */

		var = pRecordset->GetCollect(_T("WMSCargo_No"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Row"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Col"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strUnit = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Type"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Number"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNumber = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strIntime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listCargo.push_back(cargo);
	}

	return listCargo;
}

/*!
 * @brief ��ȡָ�����������п����Ϣ
 * @param ADOConn& ���ݿ����Ӵ�
 * @param unsigend int ���ܱ��
 * @return list<_CARGO> �����Ϣ�б�
*/
static list<_CARGO> GetAllCargo(ADOConn& ado, unsigned int unNo)
{
	list<_CARGO> listCargo;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_CARGO_TABLE where [WMSCargo_No]=%d"), unNo);

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_CARGO cargo;

		_variant_t var;	/*!< �ֶ�ֵ */

		var = pRecordset->GetCollect(_T("WMSCargo_No"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Row"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Col"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strUnit = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Type"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Number"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNumber = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strIntime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listCargo.push_back(cargo);
	}

	return listCargo;
}

/*!
 * @brief ��ȡָ�����������п����Ϣ
 * @param ADOConn& ���ݿ����Ӵ�
 * @param string ��λ
 * @param string ����
 * @return list<_CARGO> �����Ϣ�б�
*/
static list<_CARGO> GetAllCargo(ADOConn& ado, string strUnit, string strType)
{
	list<_CARGO> listCargo;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_CARGO_TABLE where [WMSCargo_Unit]='%s' and [WMSCargo_Type]='%s'"), strUnit.c_str(), strType.c_str());

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_CARGO cargo;

		_variant_t var;	/*!< �ֶ�ֵ */

		var = pRecordset->GetCollect(_T("WMSCargo_No"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Row"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Col"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strUnit = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Type"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Number"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNumber = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strIntime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listCargo.push_back(cargo);
	}

	return listCargo;
}

/*!
 * @brief ��ȡָ�����������п����Ϣ
 * @param ADOConn& ���ݿ����Ӵ�
 * @param unsigend int ���ܱ��
 * @param string ��λ
 * @param string ����
 * @return list<_CARGO> �����Ϣ�б�
*/
static list<_CARGO> GetAllCargo(ADOConn& ado, unsigned int unNo,string strUnit, string strType)
{
	list<_CARGO> listCargo;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_CARGO_TABLE where [WMSCargo_No]=%d [WMSCargo_Unit]='%s' and [WMSCargo_Type]='%s'"), unNo, strUnit.c_str(), strType.c_str());

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_CARGO cargo;

		_variant_t var;	/*!< �ֶ�ֵ */

		var = pRecordset->GetCollect(_T("WMSCargo_No"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Row"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Col"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strUnit = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Type"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Number"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNumber = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strIntime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listCargo.push_back(cargo);
	}

	return listCargo;
}

/*!
 * @brief unsigned int unNo
 * @param ADOConn& ���ݿ����Ӵ�
 * @param string ��ʼʱ��
 * @param string ����ʱ��
 * @return list<_CARGO> �����Ϣ�б�
*/
static list<_CARGO> GetAllCargo(ADOConn& ado, string strStartTime, string strEndTime)
{
	list<_CARGO> listCargo;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_CARGO_TABLE where [WMSCargo_Intime] between '%s' and '%s'"), strStartTime.c_str(), strEndTime.c_str());

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_CARGO cargo;

		_variant_t var;	/*!< �ֶ�ֵ */

		var = pRecordset->GetCollect(_T("WMSCargo_No"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Row"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Col"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strUnit = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Type"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Number"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNumber = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strIntime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listCargo.push_back(cargo);
	}

	return listCargo;
}

/*!
 * @brief unsigned int unNo
 * @param ADOConn& ���ݿ����Ӵ�
 * @param unsigned int ���ܱ��
 * @param string ��ʼʱ��
 * @param string ����ʱ��
 * @return list<_CARGO> �����Ϣ�б�
*/
static list<_CARGO> GetAllCargo(ADOConn& ado, unsigned int unNo, string strStartTime, string strEndTime)
{
	list<_CARGO> listCargo;

	CString cstrSql = _T("");
	cstrSql.Format(_T("select * from WMS_CARGO_TABLE where [WMSCargo_No]=%d and [WMSCargo_Intime] between '%s' and '%s'"), unNo, strStartTime.c_str(), strEndTime.c_str());

	_RecordsetPtr pRecordset = nullptr;

	pRecordset = ado.GetRecordSet(_bstr_t(cstrSql));

	while (pRecordset && !pRecordset->adoEOF)
	{
		_CARGO cargo;

		_variant_t var;	/*!< �ֶ�ֵ */

		var = pRecordset->GetCollect(_T("WMSCargo_No"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNo = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Row"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unRow = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Col"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unCol = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Unit"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strUnit = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Type"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strType = _bstr_t(var);
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Number"));
		if (var.vt != VT_NULL)
		{
			cargo.m_unNumber = (unsigned int)var;
		}

		var = pRecordset->GetCollect(_T("WMSCargo_Intime"));
		if (var.vt != VT_NULL)
		{
			cargo.m_strIntime = _bstr_t(var);
		}

		pRecordset->MoveNext();

		listCargo.push_back(cargo);
	}

	return listCargo;
}