#pragma once

#include "xsix/common_define.h"
#include <vector>
#include <string>

namespace xsix
{
	namespace utils
	{
		string_vec split(const std::string& src, const std::string& delim)
		{
			string_vec tokens;
			tokens.reserve(4);
			std::string::size_type lastPos = src.find_first_not_of(delim, 0);
			std::string::size_type pos = src.find_first_of(delim, lastPos);
			while (std::string::npos != pos || std::string::npos != lastPos)
			{
				tokens.emplace_back(src.substr(lastPos, pos - lastPos));
				lastPos = src.find_first_not_of(delim, pos);
				pos = src.find_first_of(delim, lastPos);
			}
			return tokens;
		}

		void split(const std::string& src, const std::string& delim, string_vec& tokens)
		{
			tokens.clear();
			std::string::size_type lastPos = src.find_first_not_of(delim, 0);
			std::string::size_type pos = src.find_first_of(delim, lastPos);
			while (std::string::npos != pos || std::string::npos != lastPos)
			{
				tokens.emplace_back(src.substr(lastPos, pos - lastPos));
				lastPos = src.find_first_not_of(delim, pos);
				pos = src.find_first_of(delim, lastPos);
			}
		}

		std::string	ltrim_copy(const std::string& src)
		{
			std::string tmp(src);
			ltrim(tmp);
			return tmp;
		}

		std::string	rtrim_copy(const std::string& src)
		{
			std::string tmp(src);
			rtrim(tmp);
			return tmp;
		}

		std::string	trim_copy(const std::string& src)
		{
			std::string tmp(src);
			trim(tmp);
			return tmp;
		}

		void ltrim(std::string& src)
		{
			src.erase(src.begin(), std::find_if(
					src.begin(),
					src.end(),
					[](int ch) { return ch != ' ' && ch != '\t' && ch != '\n'; }
				)
			);
		}

		void rtrim(std::string& src)
		{
			src.erase(std::find_if(
					src.rbegin(),
					src.rend(),
					[](int ch) { return ch != ' ' && ch != '\t' && ch != '\n'; }
				).base(), src.end()
			);
		}

		void trim(std::string& src)
		{
			ltrim(src);
			rtrim(src);
		}

		bool has_prefix(const std::string& src, const std::string& prefix)
		{
			if (prefix.length() > src.length())
			{
				return false;
			}

			for (std::size_t i = 0; i < prefix.length(); ++i)
			{
				if (prefix[i] != src[i])
				{
					return false;
				}
			}

			return true;
		}

		bool has_suffix(const std::string& src, const std::string& suffix)
		{
			if (suffix.length() > src.length())
			{
				return false;
			}

			std::size_t offset = src.length() - suffix.length();
			for (std::size_t i = 0; i < suffix.length(); ++i)
			{
				std::size_t index = i + offset;
				if (index >= src.length())
				{
					return false;
				}
				if (suffix[i] != src[index])
				{
					return false;
				}
			}

			return true;
		}
	}
}