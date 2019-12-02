/*!
 * @file Buffer.h
 * @brief ����ת��,���ݴ洢
 * @author FanKaiyu
 * @date 2018-04-19
 * @version 1.0
 */

#ifndef _BUFFER_H
#define _BUFFER_H
#pragma once
#include <string>
#include <wchar.h>
#include <stdarg.h>

namespace BufferString
{
	/*!
	 * @brief ASCII���ַ�ת��Ϊ16������
	 * @param chAscii ��Ҫת����ASCII���ַ�
	 * @return char ת�����16������
	*/
	static char HexChar(char chAscii)
	{
		if ((chAscii >= '0') && (chAscii <= '9'))
		{
			return chAscii - 0x30;
		}
		else if ((chAscii >= 'A') && (chAscii <= 'F'))
		{
			return chAscii - 'A' + 10;
		}
		else if ((chAscii >= 'a') && (chAscii <= 'f'))
		{
			return chAscii - 'a' + 10;
		}
		else
		{
			return 0x10;
		}
	}


	/*!
	* @brief �ַ���ת��Ϊ16������
	* @param lpszStr ��Ҫת�����ַ���
	* @param lpszData ת������ַ���
	* @return int ת�����16������
	*/
	static int StrToHex(std::string lpszStr, char *lpszData)
	{
		int t, t1;
		int rlen = 0, len = lpszStr.length();

		if (len / 2 != 0)
		{
			lpszStr += '0';
			len += 1;
		}

		for (int i = 0; i < len;)
		{
			char l, h = lpszStr[i];

			if (h == ' ')
			{
				i++;
				continue;
			}

			i++;

			if (i >= len)
			{
				break;
			}

			l = lpszStr[i];
			t = HexChar(h);
			t1 = HexChar(l);

			if ((t == 16) || (t1 == 16))
			{
				break;
			}
			else
			{
				t = t * 16 + t1;
			}

			i++;
			lpszData[rlen] = (char)t;
			rlen++;
		}

		return rlen;
	}


	/*!
	* @brief �ַ���ת��Ϊ10������
	* @param lpszStr ��Ҫת�����ַ���
	* @return int ת�����10������
	*/
	static int HexToDem(std::string lpszStr)
	{
		int dem = 0;

		for (unsigned int i = 0; i < lpszStr.length(); i++)
		{
			dem = dem * 16;

			//0~9֮����ַ�
			if ((lpszStr[i] <= '9') && (lpszStr[i] >= '0'))
			{
				dem += lpszStr[i] - '0';
			}
			//A~F֮����ַ�
			else if ((lpszStr[i] <= 'F') && (lpszStr[i] >= 'A'))
			{
				dem += lpszStr[i] - 'A' + 10;
			}
			//a~f֮����ַ�
			else if ((lpszStr[i] <= 'f') && (lpszStr[i] >= 'a'))
			{
				dem += lpszStr[i] - 'a' + 10;
			}
			//����ʱ����-1
			else
			{
				return -1;
			}
		}

		return dem;
	}


	/*! @brief ���ݿ� */
	typedef struct _BufferStruct
	{
		unsigned char* m_pszBuffer; /*!< ָ�����ݿ��ַ��ָ��*/
		unsigned int m_unSize; /*!< ���ݿ�Ĵ�С*/

		/*!
		* @brief �޲ι��캯��
		*/
		_BufferStruct()
		{
			m_pszBuffer = nullptr;
			m_unSize = 0;
		}

		/*!
		* @brief �вι��캯��
		* @param clpszBuffer ָ�����ݿ��׵�ַ��ָ��
		* @param cunSize ���ݿ��С
		*/
		_BufferStruct(const unsigned char* clpszBuffer, const unsigned int cunSize)
		{
			m_pszBuffer = nullptr;
			m_unSize = 0;

			// ��������ݿ��ֽ�����Ϊ0
			if (cunSize > 0)
			{
				m_pszBuffer = new unsigned char[cunSize];
				m_unSize = cunSize;
				memset(m_pszBuffer, 0, m_unSize);
				memcpy(m_pszBuffer, clpszBuffer, m_unSize);
			}
		}

		/*!
		* @brief �������캯��
		* @param cBuffer ���������ݿ�
		*/
		_BufferStruct(const _BufferStruct &cBuffer)
		{
			m_pszBuffer = nullptr;
			m_unSize = 0;

			// ���������ݿ��ֽ�����Ϊ0
			if (cBuffer.m_unSize > 0)
			{
				m_pszBuffer = new unsigned char[cBuffer.m_unSize];
				m_unSize = cBuffer.m_unSize;
				memset(m_pszBuffer, 0, m_unSize);
				memcpy(m_pszBuffer, cBuffer.m_pszBuffer, m_unSize);
			}
		}

		/*!
		* @brief ��������
		*/
		~_BufferStruct()
		{
			// ���ݿ����ֽ�����Ϊ0
			if (m_unSize > 0)
			{
				delete[] m_pszBuffer;
				m_pszBuffer = nullptr;
				m_unSize = 0;
			}
		}

		/*!
		* @brief ����=������
		*
		* ��ָ�����ݿ��ڵ����ݿ�������ǰ���ݿ�
		* @param cBuffer ���������ݿ�
		*/
		void operator= (const _BufferStruct &cBuffer)
		{
			if (m_unSize > 0)
			{
				delete[] m_pszBuffer;
				m_pszBuffer = nullptr;
				m_unSize = 0;
			}

			if (cBuffer.m_unSize > 0)
			{
				m_pszBuffer = new unsigned char[cBuffer.m_unSize];
				m_unSize = cBuffer.m_unSize;
				memset(m_pszBuffer, 0, m_unSize);
				memcpy(m_pszBuffer, cBuffer.m_pszBuffer, m_unSize);
			}
		}

		/*!
		* @brief ����+������
		*
		* ����ǰ���ݿ���ָ�����ݿ������ݺϲ�,�����غϲ���������ݿ�
		* @param cBuffer �ϲ������ݿ�
		* @return _BufferStruct �ϲ�������ݿ�
		*/
		_BufferStruct operator+ (const _BufferStruct &cBuffer)
		{
			_BufferStruct newBuffer(m_pszBuffer, m_unSize);
			newBuffer += cBuffer;

			return newBuffer;
		}

		/*!
		* @brief ����+=������
		*
		* ����ǰ���ݿ���ָ�����ݿ������ݺϲ�,���滻��ǰ���ݿ��ڵ�����
		* @param cBuffer �ϲ������ݿ�
		*/
		void operator+= (const _BufferStruct &cBuffer)
		{
			if (cBuffer.m_unSize == 0)
			{
				return;
			}

			unsigned int unLen = m_unSize + cBuffer.m_unSize; /*!< �ϲ�������ݿ��С */

			if (unLen == 0)
			{
				return;
			}

			unsigned char* lpszData = new unsigned char[unLen]; /*!< �ϲ�������ݿ��ڴ��׵�ַ*/
			unsigned int unDataLen = 0;/*!< �ϲ�ǰ�����ݿ��С*/
			memset(lpszData, 0, unLen);

			if (m_unSize > 0)
			{
				memcpy(lpszData, m_pszBuffer, m_unSize);
				delete[] m_pszBuffer;
				m_pszBuffer = nullptr;
				unDataLen = m_unSize;
				m_unSize = 0;
			}

			if (cBuffer.m_unSize > 0)
			{
				memcpy(lpszData + unDataLen, cBuffer.m_pszBuffer, cBuffer.m_unSize);
			}

			m_pszBuffer = lpszData;
			m_unSize = unLen;
			return;
		}

		/*!
		* @brief ����==������
		*
		* �Ƚ��������ݿ��ڵ������Ƿ����
		* @param cBuffer ��֮�Ƚϵ����ݿ�
		* @return bool ��ȷ���true,����ȷ���false
		*/
		bool operator== (const _BufferStruct &cBuffer)
		{
			if (m_unSize != cBuffer.m_unSize)
			{
				return false;
			}

			for (unsigned int i = 0; i < m_unSize; i++)
			{
				if (m_pszBuffer[i] != cBuffer.m_pszBuffer[i])
				{
					return false;
				}
			}
			return true;
		}

		/*!
		* @brief �ͷ����ݿ��ڴ�ռ�
		*/
		void Clear()
		{
			if (m_unSize > 0)
			{
				delete[] m_pszBuffer;
				m_pszBuffer = nullptr;
				m_unSize = 0;
			}
		}

	} Buffer;

	/*!
	* @brief �����ݿ��ڲ���ָ�������ݿ�
	* @param lpszPacket ����Դ���ݿ�
	* @param unPacketSize ����Դ���ݿ��С
	* @param lpszData ָ�����ҵ����ݿ�
	* @param unDataSize ָ�����ҵ����ݿ��С
	* @return unsigned char* ��һ�γ���ָ�����ݿ�������ݵĵ�ַ
	*/
	static unsigned char* FindData(unsigned char *lpszPacket, unsigned int unPacketSize, unsigned char *lpszData, unsigned int unDataSize)
	{
		if (unPacketSize < unDataSize)
		{
			return nullptr;
		}

		for (unsigned int d = 0; d < unPacketSize; d += unDataSize)
		{
			bool b_find = true;

			for (unsigned int f = 0; f < unDataSize; f++)
			{
				if (lpszPacket[d + f] != lpszData[f])
				{
					b_find = false;
					break;
				}
			}

			if (b_find)
			{
				return &lpszPacket[d];
			}
		}

		return nullptr;
	}

	/*!
	 * @brief string�ַ���תʮ��������
	 * @param string Դ�ַ���
	 * @return int ת�����ʮ��������
	*/
	static int std_string_to_int(std::string _str)
	{
		int nSum = 0;
		double x = 0.0f;
		for (int i = _str.length() - 1; i > -1; i--, x += 1)
		{
			if (_str[i] == '+' || _str[i] == '-')
			{
				nSum = -nSum;
				break;
			}
			else if (_str[i] < '0' || _str[i] > '9')
			{
				break;
			}
			else
			{
				double dbDem = pow(10, x);
				nSum += (int)((_str[i] - '0') * dbDem);
			}
		}

		return nSum;
	}

	/*!
	 * @brief string�ַ����滻
	 * @param string& Դ�ַ���
	 * @param char Ҫ�滻���ַ�
	 * @param char �滻����ַ�
	*/
	static void std_string_replace(std::string& _str, char c, char r)
	{
		for (unsigned int i = 0; i < _str.length(); i++)
		{
			if (_str[i] == c)
			{
				_str[i] = r;
			}
		}

		return;
	}

	/*!
	 * @brief string�ַ����滻
	 * @param string& Դ�ַ���
	 * @param int Ҫ�滻���ַ��������ַ���
	 * @param char �滻����ַ�
	*/
	static void std_string_replace(std::string& _str, int c, char r)
	{
		for (unsigned int i = 0; i < _str.length(); i++)
		{
			if (_str[i] == c)
			{
				_str[i] = r;
			}
		}

		return;
	}

	/*!
	 * @brief string�ַ�����ʽ�����
	 * @param string& ��ʽ��
	 * @param string ��ʽ�����string�ַ���
	*/
	static void std_string_format(std::string & _str, const char * _Format, ...)
	{
		std::string tmp;

		va_list marker = NULL;
		va_start(marker, _Format);

		size_t num_of_chars = _vscprintf(_Format, marker);

		if (num_of_chars > tmp.capacity()) {
			tmp.resize(num_of_chars + 1);
		}

		vsprintf_s((char *)tmp.data(), tmp.capacity(), _Format, marker);

		va_end(marker);

		_str = tmp.c_str();
		return;
	}

	static void std_string_to_wstring(std::wstring& _wstr, std::string _str)
	{
#ifdef _ATL
		_wstr = (wchar_t*)_bstr_t(_str.c_str());
#else
		//��ȡ��������С��������ռ䣬��������С���ַ�����  
		int len = MultiByteToWideChar(CP_ACP, 0, _str.c_str(), _str.size(), NULL, 0);
		wchar_t* buffer = new wchar_t[len + 1];
		//���ֽڱ���ת���ɿ��ֽڱ���  
		MultiByteToWideChar(CP_ACP, 0, _str.c_str(), _str.size(), buffer, len);
		buffer[len] = '\0';             //����ַ�����β  
		//ɾ��������������ֵ  
		_wstr.append(buffer);
		delete[] buffer;
#endif // !_ATL

		return;
	}

	static void std_wstring_to_string(std::string& _str, std::wstring _wstr)
	{
#ifdef _ATL
		_str = (char*)_bstr_t(_wstr.c_str());
#else
		//��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
		int len = WideCharToMultiByte(CP_ACP, 0, _wstr.c_str(), _wstr.size(), NULL, 0, NULL, NULL);
		char* buffer = new char[len + 1];
		//���ֽڱ���ת���ɶ��ֽڱ���  
		WideCharToMultiByte(CP_ACP, 0, _wstr.c_str(), _wstr.size(), buffer, len, NULL, NULL);
		buffer[len] = '\0';
		//ɾ��������������ֵ  
		_str.append(buffer);
		delete[] buffer;
#endif // !_ATL

		return;
	}

#ifdef _AFX
	static void CString_to_std_string(std::string& _str, CString _cstr)
	{
#ifdef _ATL
		_str = (char*)_bstr_t(_cstr);
#elif _UNICODE
		_str = CT2A(_cstr.GetBuffer());
#else
		_str = _cstr.GetBuffer();
#endif // !_ATL

		return;
	}

	static void std_string_to_CString(CString& _cstr, std::string _str)
	{
#ifdef _UNICODE
		std::wstring _wstr = L"";
		std_string_to_wstring(_wstr, _str);
		_cstr.Format(_T("%s"), _wstr.c_str());
#else
		_cstr.Format(_T("%s"), _str.c_str());
#endif
		return;
	}
#endif // _AFX
}

#endif // !_BUFFER_H