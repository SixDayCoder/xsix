#pragma once

#include "xsix/common_define.h"
#include <string>

namespace xsix
{
	template<uint32_t SlotCount>
	class bits
	{
	private:

		enum
		{
			BITS_COUNT = SlotCount,
			BYTE_COUNT = (BITS_COUNT + 7) / 8
		};

	public:

		bits() { clear_all(); }

		bits(const bits<SlotCount>& rhs)
		{
			memcpy(m_bytes, rhs.m_bytes, sizeof(m_bytes));
		}

		bits<SlotCount>& operator=(const bits<SlotCount>& rhs)
		{
			if (this != &rhs)
			{
				memcpy(m_bytes, rhs.m_bytes, sizeof(m_bytes));
			}
			return *this;
		}

	public:

		void set(int32_t index, bool b)
		{
			if (index >= 0 && index < BITS_COUNT)
			{
				int32_t byte_index = index / 8;
				int32_t bit_index = index % 8;
				if (b)
				{
					m_bytes[byte_index] |= (0x01 << bit_index);
				}
				else
				{
					m_bytes[byte_index] &= ~(0x01 << bit_index);
				}
			}
		}

		bool get(int32_t index)
		{
			if (index >= 0 && index < BITS_COUNT)
			{
				int32_t byte_index = index / 8;
				int32_t bit_index = index % 8;
				return (m_bytes[byte_index] & (0x01 << bit_index)) ? true : false;
			}
			return false;
		}

		void toggle(int32_t index)
		{
			if (index >= 0 && index < BITS_COUNT)
			{
				set(index, !get(index));
			}
		}

		std::string get_bit_string()
		{
			std::string result;
			result.reserve(BITS_COUNT);
			for (int32_t i = 0; i < BITS_COUNT; ++i)
			{
				result.push_back(get(i) ? '1' : '0');
			}
			return result;
		}

		int32_t length() const
		{
			return BITS_COUNT;
		}

	private:

		void clear_all()
		{
			for (int32_t i = 0; i < BYTE_COUNT; i++)
			{
				memset(&m_bytes[i], 0x00, sizeof(uint8_t));
			}
		}

		void set_all()
		{
			for (int32_t i = 0; i < BYTE_COUNT; i++)
			{
				memset(&m_bytes[i], 0xFF, sizeof(uint8_t));
			}
		}

	private:

		uint8_t m_bytes[BYTE_COUNT];
	};
}