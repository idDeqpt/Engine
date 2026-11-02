#ifndef PLUGIN_MANAGER_CLASS_HEADER
#define PLUGIN_MANAGER_CLASS_HEADER

#include <Engine/Core/Plugin.hpp>
#include <Engine/Core/PluginHost.hpp>

#include <vector>
#include <memory>

namespace eng::core
{
	class PluginManager
	{
	public:
		PluginManager() = default;

		void setup(const PluginHost& host);

        void addPlugin(std::unique_ptr<Plugin> plugin);

	protected:
        std::vector<std::unique_ptr<Plugin>> m_plugins;
	};
}

#endif //PLUGIN_MANAGER_CLASS_HEADER