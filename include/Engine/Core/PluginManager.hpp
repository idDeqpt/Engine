#ifndef PLUGIN_MANAGER_CLASS_HEADER
#define PLUGIN_MANAGER_CLASS_HEADER

#include <Engine/Core/Plugin.hpp>
#include <Engine/Core/PluginLoader.hpp>
#include <Engine/Core/PluginHost.hpp>

#include <vector>
#include <memory>

namespace eng::core
{
	class PluginManager
	{
	public:
		PluginManager() = default;

		void loadPlugin(std::unique_ptr<Plugin> plugin_in_process);
		void loadPlugin(const std::string& path_to_plugin_library);

		void setup(const PluginHost& host);

	protected:
		std::vector<std::unique_ptr<PluginLoader>> m_plugin_loaders;
	};
}

#endif //PLUGIN_MANAGER_CLASS_HEADER