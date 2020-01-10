#pragma once
#include "xsix/xsix_common_define.h"
#include <string>

namespace xsix
{
	template<uint32_t SlotCount>
	class bits
	{
	private:

		enum
		{
			BITS_COUNT =  SlotCount,
			BYTE_COUNT = (BITS_COUNT + 7) / 8
		};

	public:

		bits()
		{
			ClearAll();
		}

		~bits()
		{

		}
	
	public:

		bits(const bits<SlotCount>& rhs)
		{
			memcpy(m_InnerBytes, rhs.m_InnerBytes, sizeof(m_InnerBytes));
		}

		bits<SlotCount>& operator=(const bits<SlotCount>& rhs)
		{
			if (this != &rhs)
			{
				memcpy(m_InnerBytes, rhs.m_InnerBytes, sizeof(m_InnerBytes));
			}
			return *this;
		}
		
	public:
		
		void Set(int32_t nIndex, bool bValue)
		{
			if (nIndex >= 0 && nIndex < BITS_COUNT)
			{
				int32_t nByteIndex = nIndex / 8;
				int32_t nInnerBitIndex = nIndex % 8;
				if (bValue)
				{
					m_InnerBytes[nByteIndex] |= (0x01 << nInnerBitIndex);
				}
				else
				{
					m_InnerBytes[nByteIndex] &= ~(0x01 << nInnerBitIndex);
				}
			}			
		}
		
		bool Get(int32_t nIndex)
		{
			if (nIndex >= 0 && nIndex < BITS_COUNT)
			{
				int32_t nByteIndex = nIndex / 8;
				int32_t nInnerBitIndex = nIndex % 8;
				return ( m_InnerBytes[nByteIndex] & (0x01 << nInnerBitIndex) ) ? true : false;
			}
			return false;
		}

		void Toggle(int32_t nIndex)
		{
			if (nIndex >= 0 && nIndex < BITS_COUNT)
			{
				Set(nIndex, !Get(nIndex));
			}	
		}

		std::string GetBitString()
		{
			std::string result;
			result.reserve(BITS_COUNT);
			for (int32_t i = 0; i < BITS_COUNT; ++i)
			{
				if (Get(i))
				{
					result.push_back('1');
				}
				else
				{
					result.push_back('0');
				}
			}
			return result;
		}

		int32_t Length() const
		{
			return BITS_COUNT;
		}

	private:

		void ClearAll()
		{
			for (int32_t i = 0; i < BYTE_COUNT; i++)
			{
				memset(&m_InnerBytes[i], 0x00, sizeof(uint8_t));
			}
		}

		void SetAll()
		{
			for (int32_t i = 0; i < BYTE_COUNT; i++)
			{
				memset(&m_InnerBytes[i], 0xFF, sizeof(uint8_t));
			}
		}

	private:

		uint8_t m_InnerBytes[BYTE_COUNT];
	};
}