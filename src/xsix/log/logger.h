#pragma once

#include "xsix/noncopyable.h"
#include "xsix/fmt.hpp"
#include "xsix/time/timestamp.h"

#include <memory>
#include <mutex>
#include <list>
#include <map>
#include <algorithm>
#include <array>
#include <stdio.h>

namespace xsix
{
	class Logger;
	
	class LoggerManager;

	class LoggerManager
	{
	public:

		explicit LoggerManager(const char* log_root_dir_path);

	public:

		void add_logger(Logger* lg);

		void remove_logger(const std::string& logger_name);

		void flush_all_logger();

	public:

		std::size_t		   get_logger_count() const { return m_logger_map.size(); }

		const std::string& get_log_root_path() const { return m_log_root_path; }

	private:

		std::string					   m_log_root_path;

		std::map<std::string, Logger*> m_logger_map;
	};

	class Logger : public INonCopyable
	{
	private:

		using CacheBlock = std::array<char, 1024 * 1024>;

		using CacheBlockPtr = std::shared_ptr<CacheBlock>;

	public:

		explicit Logger(const char* log_file_name, LoggerManager* logger_manager);

		~Logger() { flush(); }

	public:

		void  log(const char* logstr);

		void  flush();

		const LoggerManager* get_logger_manager() const { return m_logger_manager; }

		const std::string&   get_log_file_name() const { return m_log_file_name; }

		const std::string&   get_log_file_path() const { return m_log_file_path; }

	private:

		CacheBlockPtr get_available_cache_block();

		int			  get_curr_cache_block_usable_size();

		void		  swap_cache_block();

	private:

		LoggerManager*			 m_logger_manager;

		std::mutex				 m_mutex;

		std::string				 m_log_file_name;

		std::string				 m_log_file_path;
		
		CacheBlockPtr			 m_curr_cache_block;

		std::list<CacheBlockPtr> m_ready_flush_cache_block_list;
	};
}


namespace xsix
{
	template<typename LoggerInstance, typename ...Args>
	void log(LoggerInstance& lg, const char* format, Args ...args)
	{
		char tmpbuf[4096] = { 0 };
		xsix::fmt(tmpbuf, 4096, format, args...);
		lg.log(tmpbuf);
	}
}

#define LOGGER_MANAGER \
		g_logger_manager

#define LOGGER_MANAGER_PTR \
		&g_logger_manager

#define LOGGER_MANAGER_DECL \
		extern xsix::LoggerManager g_logger_manager

#define LOGGER_MANAGER_IMPL(LOG_ROOT_PATH) \
		xsix::LoggerManager g_logger_manager(LOG_ROOT_PATH)

#define LOGGER_MANAGER_FLUSH \
		LOGGER_MANAGER.flush_all_logger()

#define LOGGER_MANAGER_LOGGER_COUNT \
		LOGGER_MANAGER.get_logger_count()

#define LOGGER(NAME) \
		g_##NAME##_logger

#define LOGGER_DECL(NAME)\
		extern xsix::Logger LOGGER(NAME);\

#define LOGGER_IMPL(NAME)\
		xsix::Logger LOGGER(NAME)(#NAME, LOGGER_MANAGER_PTR)

