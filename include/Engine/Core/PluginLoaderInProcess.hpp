#ifndef PLUGIN_LOADER_IN_PROCESS_CLASS_HEADER
#define PLUGIN_LOADER_IN_PROCESS_CLASS_HEADER

#include <Engine/Core/PluginLoader.hpp>
#include <Engine/Core/Plugin.hpp>
#include <memory>

namespace eng::core
{
	class PluginLoaderInProcess : public PluginLoader
	{
	public:
		PluginLoaderInProcess() = default;
		~PluginLoaderInProcess();

		void load(std::unique_ptr<Plugin> plugin);
		void unload() override;

		Plugin* get() const override;

	protected:
		std::unique_ptr<Plugin> m_plugin;
	};
}

#endif //PLUGIN_LOADER_IN_PROCESS_CLASS_HEADER