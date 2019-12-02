/*!
* @file DebugPrint.h
* @brief ���������Ϣ
* @author FanKaiyu
* @date 2018-04-19
* @version 1.0
*/

#ifndef _DEBUGPRINT_H
#define _DEBUGPRINT_H
#pragma once
//#include <stdio.h>
#include <stdarg.h>
//#include <Windows.h>

static const int MAX_DBG_MSG_LEN = 4096;
static const int MAX_TIME_LEN = 255;

/*!
* @brief ������Ϣ���
* @param clpszFormat �ַ�����ʽ����Ϣ
*/
static void DebugPrint(const char *clpszFormat, ...)
{
	SYSTEMTIME sys; /*!< ϵͳʱ��*/
	char achBuf[MAX_DBG_MSG_LEN]; /*!< ������Ϣ*/
	char achStr[MAX_TIME_LEN + MAX_DBG_MSG_LEN]; /*!< ������ַ���*/

	// ��ʼ��
	memset(achBuf, 0, MAX_DBG_MSG_LEN);
	memset(achStr, 0, MAX_TIME_LEN + MAX_DBG_MSG_LEN);

	// ��ʽ��
	va_list ap;
	va_start(ap, clpszFormat);
	_vsnprintf_s(achBuf, sizeof(achBuf), clpszFormat, ap);
	va_end(ap);

	// ��ȡϵͳ��ǰʱ��
	GetLocalTime(&sys);
	wsprintfA(achStr, "%04d/%02d/%02d %02d:%02d:%02d %s.\r\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, achBuf);

	// ����ַ���
	OutputDebugStringA(achStr);
	

	return;
}

#endif // !_DEBUGPRINT_H