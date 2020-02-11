#pragma once
#include "xsix/xsix_common_define.h"
#include <string>

namespace xsix
{
	class FileUtils
	{
	public:

		static bool IsPathExists(const std::string& szPath);

		static bool IsFileExists(const std::string& szPath);

		static bool IsDirExists(const std::string& szPath);

		static std::string GetFileBaseName(const std::string& szPath);

		static std::string GetFileExtension(const std::string& szPath);

		static std::string GetCWD();

		static std::string GetStdPath(const std::string& szPath);

		static void MakeStdPath(std::string* szPath);

		static bool Mkdirs(const std::string& szPath);

		static bool CreateFileEx(const std::string& szPath);

		static bool DeleteFileEx(const std::string& szPath);
	};
}