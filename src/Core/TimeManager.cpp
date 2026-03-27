#include "Engine/Core/TimeManager.hpp"

#include <thread>
#include <chrono>


#ifdef _WIN32
	#define NOMINMAX
	#include <windows.h>
	#include <mmsystem.h>
	#pragma comment(lib, "winmm.lib")
#elif defined(__linux__) || defined(__unix__)
	#include <unistd.h>
	#include <time.h>
	#include <errno.h>
#elif defined(__APPLE__)
	#include <mach/mach_time.h>
	#include <mach/mach.h>
#endif


void busyWait(std::chrono::microseconds ms)
{
	auto start = std::chrono::steady_clock::now();
	auto target = start + std::chrono::microseconds(ms);
	
	while (std::chrono::steady_clock::now() < target)
	{
		#ifdef _WIN32
			YieldProcessor();  // _mm_pause()
		#elif defined(__linux__) || defined(__APPLE__)
			__asm__ volatile ("pause" ::: "memory");
		#endif
	}
}


namespace eng
{

std::chrono::steady_clock::time_point core::TimeManager::s_app_start_time;


void core::TimeManager::initialize()
{
	s_app_start_time = std::chrono::steady_clock::now();
}

void core::TimeManager::finalize() {}


void core::TimeManager::sleepSeconds(float seconds)
{
	if (seconds <= 0.0f)
		return;
	
	auto microseconds = static_cast<long long>(seconds * 1'000'000.0);
	auto sleep_start = std::chrono::steady_clock::now();
	
	//short sleep
	if (microseconds < 2000)
	{
		auto target = sleep_start + std::chrono::microseconds(microseconds);
		
		while (std::chrono::steady_clock::now() < target)
		{
			#ifdef _WIN32
				YieldProcessor();  // _mm_pause()
			#elif defined(__linux__) || defined(__APPLE__)
				__asm__ volatile ("pause" ::: "memory");
			#endif
		}
		return;
	}
	
	//long sleep
	#ifdef _WIN32
		static bool high_res_initialized = false;
		if (!high_res_initialized)
		{
			timeBeginPeriod(1);
			high_res_initialized = true;
		}
	#endif
	
	auto sleep_microseconds = microseconds - 1500;
	if (sleep_microseconds > 0)
	{
		std::this_thread::sleep_for(
			std::chrono::microseconds(sleep_microseconds)
		);
	}
	
	auto target = sleep_start + std::chrono::microseconds(microseconds);
	
	while (std::chrono::steady_clock::now() < target)
	{
		#ifdef _WIN32
			YieldProcessor();
		#elif defined(__linux__) || defined(__APPLE__)
			__asm__ volatile ("pause" ::: "memory");
		#endif
	}
}

float core::TimeManager::getAppSeconds()
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - s_app_start_time).count();
}

} //namespace eng