#include <Engine/Core/Timer.hpp>

#include <chrono>


namespace eng
{

core::Timer::Timer()
{
	restart();
}


void core::Timer::restart()
{
	m_start_time = std::chrono::steady_clock::now();
}

float core::Timer::getElapsedSeconds()
{
	std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
	return std::chrono::duration<float>(end_time - m_start_time).count();
}

} //namespace eng