#include <Engine/Core/PluginLoaderInProcess.hpp>

#include <Engine/Core/PluginLoader.hpp>
#include <memory>

namespace eng::core
{

PluginLoaderInProcess::~PluginLoaderInProcess()
{
	unload();
}


void PluginLoaderInProcess::load(std::unique_ptr<Plugin> plugin)
{
	m_plugin = std::move(plugin);
}

void PluginLoaderInProcess::unload()
{
	m_plugin.reset();
}


Plugin* PluginLoaderInProcess::get() const
{
	return m_plugin.get();
}

} //namespace eng::core