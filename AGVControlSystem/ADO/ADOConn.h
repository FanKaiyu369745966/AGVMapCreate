/*!
* @file ADOConn.h
* @brief interface for the ADOConn class.
* @author 
* @date 
* @version 1.0
*/

#if !defined(AFX_ADOCONN_H__5FB9A9B2_8D94_44F7_A2DA_1F37A4F33D10__INCLUDED_)
#define AFX_ADOCONN_H__5FB9A9B2_8D94_44F7_A2DA_1F37A4F33D10__INCLUDED_

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace\
	rename("EOF", "adoEOF")rename("BOF", "adoBOF")

#if _MSC_VER > 1000

#pragma once
#include <map>
#include <mutex>
//using namespace std;
#endif // _MSC_VER > 1000

/*!
* @class ADOConn  
* @brief ���ݿ�ADO����
*
* ���ݿ�ADO������,������ѯ���޸����ݿ�Ȳ���
*/
class ADOConn  
{
public:
	/*!
	* @brief �޲ι���
	*/
	ADOConn();

	/*!
	* @brief ��������
	* @param cAdo ������ADO����
	*/
	ADOConn(const ADOConn& cAdo);

	/*!
	* @brief ��������
	*/
	virtual ~ADOConn();

protected:
	_ConnectionPtr m_pConnection; /*!< ���Ӷ���ָ��*/
	_RecordsetPtr m_pRecordset; /*!< �����ָ��*/

protected:
	std::mutex m_mutexLock; /*!< ������*/

public:
	_bstr_t m_bstrConnect; /*!< ���Ӵ� */
	_bstr_t m_bstrUser; /*!< �û��� */
	_bstr_t m_bstrPassword; /*!< �û����� */

public:
	/*!
	* @brief ��ʼ��ADO����
	* @return bool �ɹ�����true,ʧ�ܷ���false
	*/
	bool OnInitADOConn();

	/*!
	* @brief ��ѯ���ݿ�
	* @return _RecordsetPtr ���ز�ѯ�����
	*/
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);

	/*!
	* @brief ִ��SQL���
	* @return bool �ɹ�����true,ʧ�ܷ���false
	*/
    BOOL ExecuteSQL(_bstr_t bstrSQL);

	/*!
	* @brief �ر�ADO����
	*/
	void ExitConnect();

public:
	/*!
	* @brief ����=����
	* @param cAdo ������ADO����
	*/
	void operator=(const ADOConn& cAdo);
};

#endif // !defined(AFX_ADOCONN_H__5FB9A9B2_8D94_44F7_A2DA_1F37A4F33D10__INCLUDED_)
