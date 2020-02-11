#include "xsix/xsix_file_utils.h"
#include "xsix/xsix_string_utils.h"
#include <algorithm>
#include <fstream>

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
		std::string szTempPath = GetStdPath(szPath);
		auto startPos = szTempPath.find_last_of("/");
		if (startPos == std::string::npos)
		{
			return "";
		}
		std::string ext = GetFileExtension(szTempPath);
		if (ext == "")
		{
			return "";
		}
		auto endPos = szTempPath.find_last_of(ext);
		if (endPos == std::string::npos)
		{
			return "";
		}
		return szTempPath.substr(startPos + 1, endPos - startPos - ext.length());
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

	std::string FileUtils::GetStdPath(const std::string& szPath)
	{
		std::string szTempPath(szPath);
		std::replace(szTempPath.begin(), szTempPath.end(), '\\', '/');
		return szTempPath;
	}

	void FileUtils::MakeStdPath(std::string* szPath)
	{
		if (!szPath)
		{
			return;
		}
		std::replace(szPath->begin(), szPath->end(), '\\', '/');
	}

	bool FileUtils::Mkdirs(const std::string& szPath)
	{
		if (szPath.length() > MAX_PATH)
		{
			return false;
		}
		if (IsDirExists(szPath))
		{
			return true;
		}
#if   defined(_XSIX_WINDOWS)
		std::string szDirPath = GetStdPath(szPath);
		char szTempPath[MAX_PATH] = { 0 };
		if (szDirPath[szDirPath.length() - 1] != '/')
		{
			szDirPath.push_back('/');
		}
		for (std::size_t i = 0; i < szDirPath.length(); ++i)
		{
			if (szDirPath[i] == '/' && i != 0)
			{
				memcpy(szTempPath, szDirPath.c_str(), i);
				if (!IsDirExists(szTempPath))
				{
					int32_t nRetCode = _mkdir(szTempPath);
					if (nRetCode != 0)
					{
						return false;
					}
				}
			}
		}
		return true;
#elif defined(_XSIX_LINUX)
		//TODO:
		return false;
#endif
		return false;
	}

	bool FileUtils::CreateFileEx(const std::string& szPath)
	{
		std::string szStdPath = GetStdPath(szPath);
		std::fstream file;
		file.open(szStdPath.c_str(), std::ios::out);
		if (!file)
		{
			return false;
		}
		file.close();
		return true;
	}

	bool FileUtils::DeleteFileEx(const std::string& szPath)
	{
		std::string szStdPath = GetStdPath(szPath);
		if (!IsPathExists(szStdPath))
		{
			return true;
		}
		int32_t nRet = remove(szStdPath.c_str());
		return nRet == 0 ? true : false;
	}

}