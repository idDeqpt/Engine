#ifndef LOGGER_STATIC_CLASS_HEADER
#define LOGGER_STATIC_CLASS_HEADER

#include <Engine/Core/String.hpp>

#include <condition_variable>
#include <string>
#include <thread>
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

		static Logger& getInstance();

		static void debug(const String& str);
		static void info(const String& str);
		static void warning(const String& str);
		static void error(const String& str);

		void log(LogLevel level, const std::string& text);
		void setOutFile(const std::string& file_path);

	protected:
		bool m_process;
		std::thread m_thread;
		std::mutex m_mutex;
		std::condition_variable m_cv;
		std::queue<std::string> m_write_queue;
		std::string m_out_file_path;

		Logger();
		~Logger();

		std::string getCurrentTime();
		std::string formatString(LogLevel level, std::string text);
		void writeData(const std::string& msg);

		void processThread();
	};
}

#endif //LOGGER_STATIC_CLASS_HEADER