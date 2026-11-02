#ifndef ENGINE_CLASS_HEADER
#define ENGINE_CLASS_HEADER

#include <Engine/Context.hpp>
#include <Engine/Core/Node.hpp>
#include <Engine/Core/SignalBus.hpp>
#include <Engine/Core/PluginHost.hpp>
#include <Engine/Core/PluginManager.hpp>

#include <vector>
#include <memory>


namespace eng::sys {class Window;}

namespace eng::core
{
	class Engine
	{
	public:
		Engine();
		Engine(std::unique_ptr<Node> root);
		~Engine();

		void setup();
		void mainLoop();

		PluginManager& getPluginManager();

	protected:
		float m_framerate;
		std::unique_ptr<Node> m_root_node;
		std::shared_ptr<sys::Window> m_window;
		Context m_context;
		std::vector<SubscriptionId> m_subscriptions;
		std::unique_ptr<PluginHost> m_plugin_host;
		std::unique_ptr<PluginManager> m_plugin_manager;

		void init();
	};
}

#endif //ENGINE_CLASS_HEADER