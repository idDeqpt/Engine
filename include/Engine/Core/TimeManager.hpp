#ifndef TIME_MANAGER_STATIC_CLASS_HEADER
#define TIME_MANAGER_STATIC_CLASS_HEADER

#include <chrono>

namespace eng::core
{
	class TimeManager
	{
	public:
		static void initialize();
		static void finalize();

		static void sleepSeconds(float seconds);
		static float getAppSeconds();

	private:
		static std::chrono::steady_clock::time_point s_app_start_time;
	};
}

#endif //TIME_MANAGER_STATIC_CLASS_HEADER