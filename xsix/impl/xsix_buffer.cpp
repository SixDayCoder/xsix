#include "xsix/xsix_buffer.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>


namespace xsix
{

	buffer::buffer(int32_t nInitSize /*= BUFFER_SIZE_SMALL*/)
	{
		Init();
		nInitSize = nInitSize > BUFFER_SIZE_SMALL ? nInitSize : BUFFER_SIZE_SMALL;
		nInitSize = nInitSize > BUFFER_SIZE_MAX ? BUFFER_SIZE_MAX : nInitSize;
		XASSERT(Resize(nInitSize));
	}

	buffer::~buffer()
	{
		CleanUp();
	}

	int32_t buffer::Write(const char* src, int32_t size)
	{
		XASSERT(src);

		if (src <= 0)
		{
			return 0;
		}

		if (FreeSize() < size)
		{
			if (!Resize(Len() + size))
			{
				return 0;
			}
		}

		if (m_nHead <= m_nTail)
		{
			// condition(1) just likes
			// |---------------------|---------------------------|
			// 0(head)				 tail                        size
			// <-------good----------><------------free------------>
			if (m_nHead == 0)
			{
				memcpy(&m_szData[m_nTail], src, size);
			}
			else
			{
				// condition(2) just likes
				// |-----------|-----------|---------------------------|
				// 0           head		   tail                        size
				// <--no data--><-- good----><------------free--------->
				int32_t nRightFree = m_nSize - m_nTail;
				if (size <= nRightFree)
				{
					memcpy(&m_szData[m_nTail], src, size);
				}
				else
				{
					memcpy(&m_szData[m_nTail], src, nRightFree);
					memcpy(m_szData, &src[nRightFree], size - nRightFree);
				}
			}
		}
		else
		{ 
			// condition(3) just likes
			// |---------|------------|---------------------------|
			// 0         tail         head                        size
			// <---good---><---free---><-------------good---------->
			memcpy(&m_szData[m_nTail], src, size);
		}

		m_nTail = (m_nTail + size) % m_nSize;
		return size;
	}

	int32_t buffer::Read(char* dst, int32_t size)
	{
		if (size == 0)
		{
			return 0;
		}

		if (Empty())
		{
			return 0;
		}

		if (Len() < size)
		{
			size = Len();
		}

		if (m_nHead < m_nTail)
		{
			memcpy(dst, &m_szData[m_nHead], size);
		}
		else
		{
			int32_t nRightLen = m_nSize - m_nHead;
			if (size <= nRightLen)
			{
				memcpy(dst, &m_szData[m_nHead], size);
			}
			else
			{
				memcpy(dst, &m_szData[m_nHead], nRightLen);
				memcpy(&dst[nRightLen], m_szData, size - nRightLen);
			}
		}

		m_nHead = (m_nHead + size) % m_nSize;
		return size;
	}

	void buffer::DebugLog()
	{
		char tmp[2048] = { 0 };
		if (m_nHead <= m_nTail)
		{
			//c(1)
			if (m_nHead == 0)
			{
				memcpy(tmp, &m_szData[0], Len());
				tmp[Len()] = '\0';
			}
			//c(2)
			else
			{
				memcpy(tmp, &m_szData[m_nHead], m_nTail - m_nHead);
				tmp[Len()] = '\0';
			}
		}
		//c(3)
		else
		{
			memcpy(tmp, &m_szData[m_nHead], m_nSize - m_nHead);
			memcpy(&tmp[m_nSize - m_nHead], &m_szData[0], m_nTail);
			tmp[Len()] = '\0';
		}
		printf("head : %d, tail : %d, free : %d, len : %d\ndata : %s\n", m_nHead, m_nTail, FreeSize(), Len(), tmp);
	}

	const int32_t buffer::Len() const
	{
		if (Empty())
		{
			return 0;
		}

		if (m_nHead < m_nTail)
		{
			return m_nTail - m_nHead;
		}

		return m_nSize - m_nHead + m_nTail;
	}

	void buffer::Init()
	{
		m_szData = nullptr;

		m_nSize = m_nHead = m_nTail = 0;
	}

	void buffer::CleanUp()
	{
		if (m_szData)
		{
			XFREE(m_szData);
		}
		Init();
	}

	bool buffer::Resize(int32_t nNewSize)
	{
		if (!IsValidSize(nNewSize))
		{
			return false;
		}
		if (Size() >= nNewSize)
		{
			return true;
		}
		nNewSize = CalcNewSize(nNewSize);
		if (!IsValidSize(nNewSize))
		{
			return false;
		}
		char* szNewData = (char*)XMALLOC(nNewSize);
		XASSERT(szNewData);
		if (m_nHead < m_nTail)
		{
			memcpy(szNewData, &m_szData[m_nHead], m_nTail - m_nHead);
		}
		else if(m_nHead > m_nTail)
		{
			memcpy(szNewData, &m_szData[m_nHead], m_nSize - m_nHead);
			memcpy(&szNewData[m_nSize - m_nHead], m_szData, m_nTail);
		}
		XFREE(m_szData);
		m_szData = szNewData;
		m_nHead = 0;
		m_nTail = Len();
		m_nSize = nNewSize;
		return true;
	}

	int32_t buffer::CalcNewSize(int32_t nNewSize)
	{
		if (nNewSize <= BUFFER_SIZE_SMALL)
		{
			return BUFFER_SIZE_SMALL;
		}
		int32_t nTmpSize = m_nSize;
		while (nTmpSize < nNewSize)
		{
			if (nTmpSize < BUFFER_SIZE_RAW)
			{
				nTmpSize *= 2;
			}
			else if (nTmpSize < BUFFER_SIZE_MAX)
			{
				nTmpSize += BUFFER_SIZE_RAW;
			}
		}
		nNewSize = nTmpSize;
		return nNewSize;
	}

}