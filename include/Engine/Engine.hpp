#ifndef ENGINE_CLASS_HEADER
#define ENGINE_CLASS_HEADER

#include <Engine/Core/Context.hpp>
#include <Engine/Core/Node.hpp>
#include <Engine/Core/SignalBus.hpp>
#include <Engine/Core/PluginHost.hpp>
#include <Engine/Core/PluginManager.hpp>

#include <vector>
#include <memory>

namespace eng
{
	namespace sys {class Window;}

	class Engine
	{
	public:
		Engine();
		Engine(std::unique_ptr<core::Node> root);
		~Engine();

		void setup();
		void mainLoop();

		core::PluginManager& getPluginManager();

	protected:
		float m_framerate;
		std::unique_ptr<core::Node> m_root_node;
		std::shared_ptr<sys::Window> m_window;
		core::Context m_context;
		std::vector<core::SubscriptionId> m_subscriptions;
		std::unique_ptr<core::PluginHost> m_plugin_host;
		std::unique_ptr<core::PluginManager> m_plugin_manager;

		void init();
	};
} //namespace eng

#endif //ENGINE_CLASS_HEADER