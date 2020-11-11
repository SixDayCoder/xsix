#pragma once

#include "xsix/common_define.h"
#include <vector>
#include <string>

namespace xsix
{
	namespace utils
	{

		string_vec		split(const std::string& src, const std::string& delim);

		void			split(const std::string& src, const std::string& delim, string_vec& tokens);

		std::string		ltrim_copy(const std::string& src);

		std::string		rtrim_copy(const std::string& src);

		std::string		trim_copy(const std::string& src);

		void			ltrim(std::string& src);

		void			rtrim(std::string& src);

		void			trim(std::string& src);

		bool			has_prefix(const std::string& src, const std::string& prefix);

		bool			has_suffix(const std::string& src, const std::string& suffix);
	}
}