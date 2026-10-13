#ifndef LOGGER_STATIC_CLASS_HEADER
#define LOGGER_STATIC_CLASS_HEADER

#include <Engine/Core/String.hpp>

#include <condition_variable>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>


namespace eng::core
{
	class Logger
	{
	public:
		enum LogLevel
		{
			DEBUG,
			INFO,
			WARNING,
			ERROR
		};

		struct LogEntry
		{
			std::chrono::time_point<std::chrono::system_clock> time;
			LogLevel level;
			std::string text;
		};

		static Logger& getInstance();

		Logger();
		Logger(const std::string& out_path);
		~Logger();

		static void debug(const String& str);
		static void info(const String& str);
		static void warning(const String& str);
		static void error(const String& str);

		void log(LogLevel level, const String& str);
		void setOutFile(const std::string& file_path);

	protected:
		bool m_process;
		std::thread m_thread;
		std::mutex m_queue_mutex;
		std::mutex m_file_mutex;
		std::condition_variable m_cv;
		std::queue<LogEntry> m_write_queue;
		std::ofstream m_out_file;

		std::string formatTime(const LogEntry& entry);
		std::string formatString(const LogEntry& entry);
		void writeData(const std::string& msg);

		void processThread();
	};
}

#endif //LOGGER_STATIC_CLASS_HEADER