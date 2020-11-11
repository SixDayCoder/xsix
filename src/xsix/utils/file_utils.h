#pragma once

#include "xsix/common_define.h"
#include <string>

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
	namespace utils
	{
		bool		is_path_exists(const std::string& path);

		bool		is_file_exists(const std::string& path);

		bool		is_dir_exists(const std::string& path);

		std::string get_cwd();

		std::string get_file_base_name(const std::string& path);

		std::string get_file_ext(const std::string& path);

		std::string get_std_path(const std::string& path);

		void		make_std_path(std::string* path);

		bool		create_dir(const std::string& path);

		bool		create_file(const std::string& path);

		bool		delete_file(const std::string& path);
	}
}