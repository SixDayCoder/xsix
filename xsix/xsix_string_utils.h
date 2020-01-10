#pragma once

#include "xsix/xsix_common_define.h"
#include <vector>
#include <string>

namespace xsix
{
	class StringUtils
	{
	public:

		static std::vector<std::string> Split(const std::string& src, const std::string& delim);

		static void Split(const std::string& src, const std::string& delim, std::vector<std::string>& tokens);

		static std::string LTrimCopy(const std::string& src);

		static std::string RTrimCopy(const std::string& src);

		static std::string TrimCopy(const std::string& src);

		static void LTrim(std::string& src);

		static void RTrim(std::string& src);

		static void Trim(std::string& src);

		static bool HasPrefix(const std::string& src, const std::string& prefix);

		static bool HasSuffix(const std::string& src, const std::string& suffix);
	};
}