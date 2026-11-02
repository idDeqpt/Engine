#include <Engine/Core/PluginManager.hpp>

#include <Engine/Core/Plugin.hpp>
#include <Engine/Core/PluginHost.hpp>

#include <vector>
#include <memory>


namespace eng::core
{

void PluginManager::setup(const PluginHost& host)
{
    for (auto& plugin : m_plugins)
        plugin->onSetup(host);
}

void PluginManager::addPlugin(std::unique_ptr<Plugin> plugin)
{
    m_plugins.push_back(std::move(plugin));
}

} //namespace eng::core