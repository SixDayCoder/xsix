#include "xsix/xsix_file_utils.h"
#include "xsix/xsix_string_utils.h"

namespace xsix
{
	static bool InnerPathExists_Windows(const std::string& szPath, WIN32_FIND_DATA& FindFileData)
	{
#if   defined(_XSIX_WINDOWS)
		HANDLE hFind;
		hFind = FindFirstFile(szPath.c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			FindClose(hFind);
			return false;
		}
		FindClose(hFind);
		return true;
#endif
		return false;
	}

	bool FileUtils::IsPathExists(const std::string& szPath)
	{
#if   defined(_XSIX_WINDOWS)
		WIN32_FIND_DATA FindFileData;
		return InnerPathExists_Windows(szPath, FindFileData);
#elif defined(_XSIX_LINUX)
		//TODO:
		return false;
#endif
		return false;
	}

	bool FileUtils::IsFileExists(const std::string& szPath)
	{
#if   defined(_XSIX_WINDOWS)
		WIN32_FIND_DATA FindFileData;
		if (!InnerPathExists_Windows(szPath, FindFileData))
		{
			return false;
		}
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			return true;
		}
		return false;
#elif defined(_XSIX_LINUX)
		//TODO:
		return false;
#endif
		return false;
	}

	bool FileUtils::IsDirExists(const std::string& szPath)
	{
#if   defined(_XSIX_WINDOWS)
		WIN32_FIND_DATA FindFileData;
		if (!InnerPathExists_Windows(szPath, FindFileData))
		{
			return false;
		}
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			return true;
		}
		return false;
#elif defined(_XSIX_LINUX)
		//TODO:
		return false;
#endif
		return false;
	}

	std::string FileUtils::GetFileBaseName(const std::string& szPath)
	{
		auto startPos = szPath.find_last_of("/");
		if (startPos == std::string::npos)
		{
			return "";
		}
		std::string ext = GetFileExtension(szPath);
		if (ext == "")
		{
			return "";
		}
		auto endPos = szPath.find_last_of(ext);
		if (endPos == std::string::npos)
		{
			return "";
		}
		return szPath.substr(startPos + 1, endPos - startPos - ext.length());
	}

	std::string FileUtils::GetFileExtension(const std::string& szPath)
	{
		static std::string specialExtension[] = {
			".tar.gz"
		};
		bool bSpecialExtension = false;
		for (const auto& ext : specialExtension) 
		{
			if (StringUtils::HasSuffix(szPath, ext))
			{
				return ext;
			}
		}
		auto pos = szPath.find_last_of(".");
		if (pos == std::string::npos)
		{
			return "";
		}
		return szPath.substr(pos, szPath.length() - pos);
	}

	std::string FileUtils::GetCWD()
	{
#if   defined(_XSIX_WINDOWS)
		char buffer[MAX_PATH] = { 0 };
		_getcwd(buffer, MAX_PATH);
		return std::string(buffer);
#elif defined(_XSIX_LINUX)
		//TODO:
		return "";
#endif
		return "";
	}

}