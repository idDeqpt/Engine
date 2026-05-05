#ifndef ENGINE_CLASS_HEADER
#define ENGINE_CLASS_HEADER

#include <Engine/Context.hpp>
#include <Engine/Core/Node.hpp>
#include <Engine/Core/SignalBus.hpp>
#include <vector>


namespace eng::sys {class Window;}

namespace eng::core
{
	class Engine
	{
	public:
		Engine(Node& root);
		~Engine();

		void setup();
		void mainLoop();
		void destroy();

		void setFramerate(float framerate);

	public:
		float m_framerate;
		Node* m_root_node;
		sys::Window* m_window;
		Context m_context;
		std::vector<SubscriptionId> m_subscriptions;
	};
}

#endif //ENGINE_CLASS_HEADER