#include "xsix/xsix_string_utils.h"
#include <algorithm>

namespace xsix
{

	std::vector<std::string> StringUtils::Split(const std::string& src, const std::string& delim)
	{
		std::vector<std::string> tokens;
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

	void StringUtils::Split(const std::string& src, const std::string& delim, std::vector<std::string>& tokens)
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

	std::string StringUtils::LTrimCopy(const std::string& src)
	{
		std::string tmp(src);
		LTrim(tmp);
		return tmp;
	}

	void StringUtils::LTrim(std::string& src)
	{
		src.erase(src.begin(), std::find_if(
				src.begin(), 
				src.end(), 
				[](int ch) { return ch != ' ' && ch != '\t' && ch != '\n'; }
			)
		);
	}

	std::string StringUtils::RTrimCopy(const std::string& src)
	{
		std::string tmp(src);
		RTrim(tmp);
		return tmp;
	}

	void StringUtils::RTrim(std::string& src)
	{
		src.erase(std::find_if(
				src.rbegin(),
				src.rend(),
				[](int ch) { return ch != ' ' && ch != '\t' && ch != '\n'; }
			).base(), src.end()
		);
	}

	std::string StringUtils::TrimCopy(const std::string& src)
	{
		std::string tmp(src);
		Trim(tmp);
		return tmp;
	}

	void StringUtils::Trim(std::string& src)
	{
		LTrim(src);
		RTrim(src);
	}

	bool StringUtils::HasPrefix(const std::string& src, const std::string& prefix)
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

	bool StringUtils::HasSuffix(const std::string& src, const std::string& suffix)
	{
		if (suffix.length() > src.length())
		{
			return false;
		}
		std::size_t nOffset = src.length() - suffix.length();
		for (std::size_t i = 0; i < suffix.length(); ++i)
		{
			std::size_t nIndex = i + nOffset;
			if (nIndex >= src.length())
			{
				return false;
			}
			if (suffix[i] != src[nIndex])
			{
				return false;
			}
		}
		return true;
	}
}