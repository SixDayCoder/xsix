#include "xsix/utils/file_utils.h"
#include "xsix/utils/string_utils.h"
#include <algorithm>
#include <fstream>

namespace xsix
{

#if defined(_XSIX_WINDOWS)
	static bool __path_exists_windows(const std::string& path, WIN32_FIND_DATA* wf_data)
	{
		if (!wf_data)
		{
			return false;
		}

		WCHAR wpath[256] = { 0 };
		mbstowcs(wpath, path.c_str(), 256);
		HANDLE hFind = FindFirstFile(wpath, wf_data);
		bool ret = (hFind == INVALID_HANDLE_VALUE) ? false : true;
		FindClose(hFind);

		return ret;
	}
#endif

	bool is_path_exists(const std::string& path)
	{
#if defined(_XSIX_WINDOWS)

		WIN32_FIND_DATA wf_data;
		return __path_exists_windows(path, &wf_data);

#elif defined(_XSIX_LINUX)

		return access(path.c_str(), F_OK) == 0 ? true : false;

#endif

		return false;
	}

	bool is_file_exists(const std::string& path)
	{
#if   defined(_XSIX_WINDOWS)

		WIN32_FIND_DATA wf_data;
		if (!__path_exists_windows(path, &wf_data))
		{
			return false;
		}

		return !(wf_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

#elif defined(_XSIX_LINUX)

		struct stat info;
		int32_t rc = stat(path.c_str(), &info);
		if (rc == 0)
		{
			return !S_ISDIR(info.st_mode);
		}
		return false;

#endif

		return false;
	}

	bool is_dir_exists(const std::string& path)
	{
#if   defined(_XSIX_WINDOWS)

		WIN32_FIND_DATA wf_data;
		if (!__path_exists_windows(path, &wf_data))
		{
			return false;
		}

		return wf_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

#elif defined(_XSIX_LINUX)

		struct stat info;
		int32_t rc = stat(path.c_str(), &info);
		if (rc == 0)
		{
			return S_ISDIR(info.st_mode);
		}
		return false;

#endif

		return false;
	}

	std::string get_cwd()
	{
#if   defined(_XSIX_WINDOWS)
		char buffer[MAX_PATH] = { 0 };
		_getcwd(buffer, MAX_PATH);
		return std::string(buffer);

#elif defined(_XSIX_LINUX)

		char buffer[256] = { 0 };
		getcwd(buffer, 256);
		return std::string(buffer);

#endif
		return "";
	}

	std::string get_file_base_name(const std::string& path)
	{
		std::string tmp_path = xsix::get_std_path(path);
		auto start_pos = tmp_path.find_last_of("/");
		if (start_pos == std::string::npos)
		{
			return "";
		}
		std::string ext = get_file_ext(tmp_path);
		if (ext == "")
		{
			return "";
		}
		auto end_pos = tmp_path.find_last_of(ext);
		if (end_pos == std::string::npos)
		{
			return "";
		}
		return tmp_path.substr(start_pos + 1, end_pos - start_pos - ext.length());
	}

	std::string get_file_ext(const std::string& path)
	{
		static std::string special_ext[] = {
			".tar.gz"
		};
		for (const auto& ext : special_ext)
		{
			if (xsix::has_suffix(path, ext))
			{
				return ext;
			}
		}
		auto pos = path.find_last_of(".");
		if (pos == std::string::npos)
		{
			return "";
		}
		return path.substr(pos, path.length() - pos);
	}

	std::string get_std_path(const std::string& get_std_path)
	{
		std::string tmp_path(get_std_path);
		std::replace(tmp_path.begin(), tmp_path.end(), '\\', '/');
		return tmp_path;
	}

	void make_std_path(std::string* path)
	{
		if (path)
		{
			std::replace(path->begin(), path->end(), '\\', '/');
		}
	}

	bool create_dir(const std::string& path)
	{
		//TODO:
		return false;
	}

	bool create_file(const std::string& path)
	{
		//TODO:
		return false;
	}

	bool delete_file(const std::string& path)
	{
		//TODO:
		return false;
	}
}