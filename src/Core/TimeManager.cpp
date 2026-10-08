#include "Engine/Core/TimeManager.hpp"

#include <thread>
#include <chrono>
#include <mutex>

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
	#include <intrin.h>
#elif defined(__i386__) || defined(__x86_64__)
	#include <immintrin.h>
#endif

#if defined(_WIN32)
	#define NOMINMAX
	#include <windows.h>
	#include <mmsystem.h>
	#pragma comment(lib, "winmm.lib")
#endif

namespace
{

constexpr auto kBusyWaitThreshold = std::chrono::microseconds(2000);
constexpr auto kSleepOvershoot    = std::chrono::microseconds(1500);

inline void cpuRelax() noexcept
{
	#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
		YieldProcessor();
	#elif defined(__i386__) || defined(__x86_64__)
		_mm_pause();
	#else
		std::this_thread::yield();
	#endif
}

void busyWaitUntil(std::chrono::steady_clock::time_point deadline) noexcept
{
	while (std::chrono::steady_clock::now() < deadline)
		cpuRelax();
}

#if defined(_WIN32)
	void ensureHighResTimer()
	{
		static std::once_flag flag;
		std::call_once(flag, [] { timeBeginPeriod(1); });
	}
#endif

} // namespace

namespace eng::core
{

TimeManager::TimeManager()
{
	m_app_start_time = std::chrono::steady_clock::now();
}

void TimeManager::sleepSeconds(float seconds)
{
	if (seconds <= 0.0f)
		return;

	auto total = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::duration<float>(seconds));
	auto deadline = std::chrono::steady_clock::now() + total;

	if (total < kBusyWaitThreshold)
	{
		busyWaitUntil(deadline);
		return;
	}

	#if defined(_WIN32)
		ensureHighResTimer();
	#endif

	std::this_thread::sleep_for(total - kSleepOvershoot);
	busyWaitUntil(deadline);
}

float TimeManager::getAppSeconds()
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_app_start_time).count();
}

} // namespace eng::core