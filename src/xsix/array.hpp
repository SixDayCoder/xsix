#pragma once

namespace xsix
{

	template<typename ElementT, int XArraySize>
	class xarray
	{
	public:

		xarray() : m_length(0) {}
		~xarray() = default;


	public:

		bool empty() const { return m_length == 0; }
		bool full() const  { return m_length == XArraySize; }
		int  size()   const { return XArraySize; }
		int  length() const { return m_length; }
		int  remain_length() const { return XArraySize - m_length; }
		ElementT* data() { return &m_container[0]; }

	public:

		bool append(const ElementT& elem)
		{
			return append(&elem, 1);
		}

		bool append(const ElementT* src, int cnt)
		{
			int index = 0;
			while ((m_length < XArraySize) && (index < cnt))
			{
				m_container[m_length] = src[index];
				m_length++;
				index++;
			}
			return index == cnt;
		}

	public:

		const ElementT& operator[](int index) const
		{
			//XASSERT(index >= 0 && index < size());
			return m_container[index];
		}

		ElementT& operator[](int index)
		{
			//XASSERT(index >= 0 && index < size());
			return m_container[index];
		}
		
	private:

		int		 m_length;
		ElementT m_container[XArraySize];
	};

}