#include <Engine/Core/Logger.hpp>

#include <condition_variable>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>


namespace eng::core
{

Logger& Logger::getInstance()
{
	static Logger instance("Engine.log");
	return instance;
}


Logger::Logger(const std::string& out_path):
	m_process(true)
{
	setOutFile(out_path);
	m_thread = std::thread(&Logger::processThread, this);
}

Logger::~Logger()
{
	m_process = false;
	m_cv.notify_one();
	m_thread.join();
	if (m_out_file.is_open())
		m_out_file.close();
}


void Logger::debug(const String& str)
{
	getInstance().log(Logger::LogLevel::DEBUG, str);
}

void Logger::info(const String& str)
{
	getInstance().log(Logger::LogLevel::INFO, str);
}

void Logger::warning(const String& str)
{
	getInstance().log(Logger::LogLevel::WARNING, str);
}

void Logger::error(const String& str)
{
	getInstance().log(Logger::LogLevel::ERROR, str);
}


void Logger::log(LogLevel level, const String& str)
{
	{
		std::lock_guard<std::mutex> lock(m_queue_mutex);
		m_write_queue.push({
			std::chrono::system_clock::now(),
			level,
			str.getString()
		});
	}
	m_cv.notify_one();
}

void Logger::setOutFile(const std::string& file_path)
{
	std::lock_guard<std::mutex> lock(m_file_mutex);
	if (m_out_file.is_open())
		m_out_file.close();
	m_out_file.open(file_path, std::ios::app);
}



std::string Logger::formatTime(const Logger::LogEntry& entry)
{
	auto time = std::chrono::system_clock::to_time_t(entry.time);
    auto ms   = std::chrono::duration_cast<std::chrono::microseconds>(entry.time.time_since_epoch()) % 1'000'000;
	
	std::stringstream ss;
	ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
	ss << '.' << std::setfill('0') << std::setw(6) << ms.count();

	return ss.str();
}

std::string Logger::formatString(const Logger::LogEntry& entry)
{
	constexpr char* levelStr[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

	std::stringstream ss;
	ss << "[" << formatTime(entry)                       << "] ";
	ss << "[" << levelStr[static_cast<int>(entry.level)] << "] ";
	ss << entry.text;

	return ss.str();
}

void Logger::writeData(const std::string& msg)
{
	std::cout << msg << std::endl;
	if (m_out_file.is_open())
		m_out_file << msg << "\n";
	m_out_file.flush();
}


void Logger::processThread()
{
	while (m_process || !m_write_queue.empty())
	{
		LogEntry entry;
		{
			std::unique_lock<std::mutex> lock(m_queue_mutex);
			m_cv.wait(lock, [this] {return !m_write_queue.empty() || !m_process;});
			if (!m_write_queue.empty())
			{
				entry = m_write_queue.front();
				m_write_queue.pop();
			}
		}
		
		std::unique_lock<std::mutex> lock(m_file_mutex);
		writeData(formatString(entry));
	}
}

} //namespace eng