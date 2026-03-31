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


namespace eng
{

core::Logger& core::Logger::getInstance()
{
	static Logger instance;
	return instance;
}


core::Logger::Logger():
	m_process(true)
{
	setOutFile("file.log");
	m_thread = std::thread(&Logger::processThread, this);
}

core::Logger::~Logger()
{
	m_process = false;
	m_thread.join();
}


void core::Logger::debug(std::string text)
{
	getInstance().log(Logger::LogLevel::DEBUG, text);
}

void core::Logger::info(std::string text)
{
	getInstance().log(Logger::LogLevel::INFO, text);
}

void core::Logger::warning(std::string text)
{
	getInstance().log(Logger::LogLevel::WARNING, text);
}

void core::Logger::error(std::string text)
{
	getInstance().log(Logger::LogLevel::ERROR, text);
}


void core::Logger::log(LogLevel level, std::string text)
{
	std::string formatted = formatString(level, text);
	
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_write_queue.push(formatted);
	}
	m_cv.notify_one();
}

void core::Logger::setOutFile(const std::string& file_path)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_out_file_path = file_path;
}



std::string core::Logger::getCurrentTime()
{
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 10'000;
	
	std::stringstream ss;
	ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
	ss << '.' << std::setfill('0') << std::setw(4) << ms.count();

	return ss.str();
}

std::string core::Logger::formatString(Logger::LogLevel level, std::string text)
{
	constexpr char* levelStr[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

	std::stringstream ss;
	ss << "[" << getCurrentTime() << "] ";
	ss << "[" << levelStr[static_cast<int>(level)] << "] ";
	ss << text;

	return ss.str();
}

void core::Logger::writeData()
{
	while (!m_write_queue.empty())
	{
		std::cout << m_write_queue.front() << std::endl;
		std::ofstream file(m_out_file_path, std::ios::app);
		if (file.is_open())
			file << m_write_queue.front() << std::endl;
		file.close();
		m_write_queue.pop();
	}
}


void core::Logger::processThread()
{
	while (m_process)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock, [this] {return !m_write_queue.empty() || !m_process;});
		
		writeData();
	}
}

} //namespace eng