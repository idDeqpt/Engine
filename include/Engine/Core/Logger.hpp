#ifndef LOGGER_STATIC_CLASS_HEADER
#define LOGGER_STATIC_CLASS_HEADER

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

		static void debug(std::string text);
		static void info(std::string text);
		static void warning(std::string text);
		static void error(std::string text);

		void log(LogLevel level, std::string text);
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
		void writeData();

		void processThread();
	};
}

#endif //LOGGER_STATIC_CLASS_HEADER