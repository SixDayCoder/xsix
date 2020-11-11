#include "xsix/log/logger.h"
#include "xsix/utils/file_utils.h"

namespace xsix
{
	Logger::Logger(const char* log_file_name, LoggerManager* logger_manager):
			m_log_file_name(log_file_name),
			m_logger_manager(logger_manager)
	{
		m_log_file_path = "./log/" + m_log_file_name + ".log";
		m_curr_cache_block = get_available_cache_block();

		if (m_logger_manager)
		{
			m_logger_manager->add_logger(this);
		}
	}

	void Logger::log(const char* logstr)
	{
		if (!logstr)
		{
			return;
		}

		char log_prefix[64] = { 0 };
		xsix::fmt(log_prefix, 64, R"({"logtime":"%", )", Timestamp::now().yymmhhmmss());

		char log_suffix[4] = { 0 };
		xsix::fmt(log_suffix, 4, "}\n");

		int logstr_size = strlen(logstr);
		int log_prefix_size = strlen(log_prefix);
		int log_suffix_size = strlen(log_suffix);
		int log_total_size = log_prefix_size + logstr_size + log_suffix_size;
		int curr_usable_size = 0;

		{
			std::lock_guard<std::mutex> lock(m_mutex);

			curr_usable_size = get_curr_cache_block_usable_size();
			if (log_total_size >= curr_usable_size)
			{
				swap_cache_block();
			}

			curr_usable_size = get_curr_cache_block_usable_size();
			if (log_total_size < curr_usable_size)
			{
				m_curr_cache_block->append(log_prefix, log_prefix_size);
				m_curr_cache_block->append(logstr, logstr_size);
				m_curr_cache_block->append(log_suffix, log_suffix_size);
			}

#if defined(_XSIX_WINDOWS)
			printf("%s%s%s", log_prefix, logstr, log_suffix);
#endif
		}
	}

	void Logger::flush()
	{
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			swap_cache_block();
		}

		FILE* fp = fopen(m_log_file_path.c_str(), "a+");
		if (fp)
		{
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				for (auto it = m_ready_flush_cache_block_list.begin(); it != m_ready_flush_cache_block_list.end(); ++it)
				{
					CacheBlockPtr ptr = *it;
					fwrite(ptr->data(), sizeof(char), ptr->length(), fp);
				}
				fclose(fp);
				m_ready_flush_cache_block_list.clear();
			}
		}
	}

	int Logger::get_curr_cache_block_usable_size()
	{
		if (!m_curr_cache_block)
		{
			return 0;
		}
		return m_curr_cache_block->size() - m_curr_cache_block->length() - 1;
	}

	Logger::CacheBlockPtr Logger::get_available_cache_block()
	{
		return std::make_shared<CacheBlock>();
	}

	void Logger::swap_cache_block()
	{
		if (m_curr_cache_block)
		{
			m_ready_flush_cache_block_list.push_back(m_curr_cache_block);
			m_curr_cache_block = get_available_cache_block();
		}
	}

	LoggerManager::LoggerManager(const char* log_root_dir_path):
				   m_log_root_path(log_root_dir_path)
	{
		const std::string cwd = utils::get_cwd();
	}

	void LoggerManager::add_logger(Logger* lg)
	{
		if (lg)
		{
			auto it = m_logger_map.find(lg->get_log_file_name());
			if (it == m_logger_map.end())
			{
				m_logger_map.insert(std::make_pair(lg->get_log_file_name(), lg));
			}
		}
	}

	void LoggerManager::remove_logger(const std::string& logger_name)
	{
		auto it = m_logger_map.find(logger_name);
		if (it != m_logger_map.end())
		{
			m_logger_map.erase(it);
		}
	}

	void LoggerManager::flush_all_logger()
	{
		for (auto it = m_logger_map.begin(); it != m_logger_map.end(); ++it)
		{
			it->second->flush();
		}
	}
}