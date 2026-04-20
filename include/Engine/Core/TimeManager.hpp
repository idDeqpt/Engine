#ifndef TIME_MANAGER_CLASS_HEADER
#define TIME_MANAGER_CLASS_HEADER

#include <chrono>

namespace eng::core
{
	class TimeManager
	{
	public:
		TimeManager();

		void sleepSeconds(float seconds);
		float getAppSeconds();

	private:
		std::chrono::steady_clock::time_point m_app_start_time;
	};
}

#endif //TIME_MANAGER_CLASS_HEADER