#pragma once

#include <stdio.h>
#include <string.h>
#include <string>
#include <stdexcept>

namespace xsix
{
	//===================template specialization begin===================

	template<typename ValueT>
	inline int v2s(char* buf, int size, ValueT v)
	{
		int rc = snprintf(buf, size, "%s", std::to_string(v).c_str());
		return rc;
	}

	template<>
	inline int v2s(char* buf, int size, bool v)
	{
		std::string tmp = v ? "true" : "false";
		int rc = snprintf(buf, size, "%s", tmp.c_str());
		return rc;
	}

	template<>
	inline int v2s(char* buf, int size, std::string v)
	{
		int rc = snprintf(buf, size, "%s", v.c_str());
		return rc;
	}

	template<>
	inline int v2s(char* buf, int size, const char* v)
	{
		std::string tmp(v);
		int rc = snprintf(buf, size, "%s", tmp.c_str());
		return rc;
	}

	template<>
	inline int v2s(char* buf, int size, char* v)
	{
		std::string tmp(v);
		int rc = snprintf(buf, size, "%s", tmp.c_str());
		return rc;
	}

	//===================template specialization end===================

	//recursion end check
	inline void fmt(char* buf, int buf_size, const char* format)
	{
		//check '%' count
		const char* format_curr_it = format;
		while (*format_curr_it)
		{
			//invalid '%' count
			if (*format_curr_it == '%' && *(format_curr_it + 1) != '%')
			{
				throw std::runtime_error("invalid format string: missing arguments");
			}
			format_curr_it++;
		}
		
		//copy to buf
		char* buf_curr_it = buf;
		char* buf_end_it = buf + buf_size - 1;
		format_curr_it = format;
		while ((buf_curr_it < buf_end_it) && (*format_curr_it != '\0'))
		{
			*buf_curr_it++ = *format_curr_it++;
		}

		//end of buf string
		if (buf_curr_it < buf_end_it)
		{
			*buf_curr_it = '\0';
		}
		*buf_end_it = '\0';
	}

	template<typename T, typename ...Args>
	void fmt(char* buf, int buf_size, const char* format, T val, Args ...args)
	{
		auto format_size = strlen(format);
		if (format_size <= 0 || buf_size <= 0)
		{
			return;
		}

		char* buf_curr_it = buf;
		char* buf_end_it = buf + buf_size - 1;

		const char* format_curr_it = format;
		while ((buf_curr_it < buf_end_it) && (*format_curr_it != '\0'))
		{
			if (*format_curr_it == '%')
			{
				if (*(format_curr_it + 1) == '%')
				{
					*buf_curr_it = '%';
					format_curr_it += 2;
				}
				else
				{
					int buf_remain_size = static_cast<int>(buf_end_it - buf_curr_it + 1);
					int value_size = v2s(buf_curr_it, buf_remain_size, val);
					int step = value_size < (buf_remain_size - 1) ? value_size : (buf_remain_size - 1);

					format_curr_it++;
					buf_curr_it += step;
					buf_remain_size = static_cast<int>(buf_end_it - buf_curr_it + 1);
	
					return xsix::fmt(buf_curr_it, buf_remain_size, format_curr_it, args...);
				}
			}
			else
			{
				*buf_curr_it++ = *format_curr_it++;
			}
		}
	}

	template<typename ...Args>
	void fmt(std::string& buf, const char* format, Args ...args)
	{
		if (format)
		{
			char tmp[4096] = { 0 };
			xsix::fmt(tmp, 4096, format, args...);
			buf = tmp;
		}
	}
}