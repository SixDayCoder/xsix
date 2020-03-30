#pragma once

#include "xsix/xsix_common_define.h"
#include <string>

//file define
#if defined(_XSIX_WINDOWS)

	#include <io.h>
	#include <direct.h>
	#include <ShlObj.h>
	
#endif

#if defined(_XSIX_LINUX)

	#include <unistd.h>
	#include <sys/stat.h>
	
#endif

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