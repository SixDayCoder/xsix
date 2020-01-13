#pragma once
#include "xsix/xsix_common_define.h"

namespace xsix
{
	//1.ring buffer
	//2.fast alloc
	class buffer
	{
	public:

		enum 
		{
			BUFFER_SIZE_SMALL = 64,
			BUFFER_SIZE_RAW   = 1024,
			BUFFER_SIZE_MAX   = BUFFER_SIZE_RAW * BUFFER_SIZE_RAW,
		};

	public:

		buffer(int32_t nInitSize = BUFFER_SIZE_SMALL);
	
		~buffer();

	public:

		int32_t Write(const char* src, int32_t size);

		int32_t Read(char* dst, int32_t size);

	public:

		void DebugLog();

	public:

		bool  Empty() const { return m_nHead == m_nTail; }

		const int32_t Len()  const;

		const int32_t Size() const { return m_nSize; }

		const int32_t FreeSize() const  { return ((m_nHead <= m_nTail) ? (m_nSize - m_nTail + m_nHead - 1) : (m_nHead - m_nTail - 1)); }

	private:

		bool IsValidSize(int32_t nNewSize) { return (nNewSize > 0 && nNewSize <= BUFFER_SIZE_MAX); }

		void Init();

		void CleanUp();

		bool Resize(int32_t nNewSize);

		int32_t CalcNewSize(int32_t nNewSize);

	private:

		char* m_szData;

		int32_t m_nSize;

		int32_t m_nHead;

		int32_t m_nTail;
	};
}