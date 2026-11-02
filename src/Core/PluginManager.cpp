#include <Engine/Core/PluginManager.hpp>

#include <Engine/Core/Plugin.hpp>
#include <Engine/Core/PluginHost.hpp>
#include <Engine/Core/PluginLoader.hpp>
#include <Engine/Core/PluginLoaderInProcess.hpp>

#include <vector>
#include <memory>


namespace eng::core
{

void PluginManager::setup(const PluginHost& host)
{
	for (auto& plugin_loader : m_plugin_loaders)
		plugin_loader->get()->onSetup(host);
}


void PluginManager::loadPlugin(std::unique_ptr<Plugin> plugin_in_process)
{
	auto loader = std::make_unique<PluginLoaderInProcess>();
	loader->load(std::move(plugin_in_process));
	m_plugin_loaders.push_back(std::move(loader));
}

void PluginManager::loadPlugin(const std::string& path_to_plugin_library)
{
	//TODO
}

} //namespace eng::core