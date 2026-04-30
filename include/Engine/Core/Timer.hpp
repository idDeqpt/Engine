#ifndef TIMER_CLASS_HEADER
#define TIMER_CLASS_HEADER

#include <chrono>

namespace eng::core
{
	class Timer
	{
	public:
		Timer();

		void restart();
		double getElapsedSeconds();

	protected:
		std::chrono::steady_clock::time_point m_start_time;
	};
}

#endif //TIMER_CLASS_HEADER