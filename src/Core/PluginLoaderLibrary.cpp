#include <Engine/Core/PluginLoaderLibrary.hpp>

#include <Engine/System/DynamicLibrary.hpp>
#include <Engine/Core/PluginLoader.hpp>
#include <string>

namespace eng::core
{

PluginLoaderLibrary::~PluginLoaderLibrary()
{
	unload();
}


bool PluginLoaderLibrary::load(const std::string& lib_path)
{
	unload();
	if (!m_library.open(lib_path)) return false;

	return tryLoadAfterOpen();
}

bool PluginLoaderLibrary::load(const std::string& directory, const std::string& base_name)
{
	unload();
	if (!m_library.open(directory, base_name)) return false;

	return tryLoadAfterOpen();
}

void PluginLoaderLibrary::unload()
{
	if (m_plugin && m_destroy)
		m_destroy(m_plugin);
	m_plugin  = nullptr;
	m_destroy = nullptr;
	m_library.close();
}


Plugin* PluginLoaderLibrary::get() const
{
	return m_plugin;
}



bool PluginLoaderLibrary::tryLoadAfterOpen()
{
	m_destroy = reinterpret_cast<DestroyFn>(m_library.getSymbol("engine_plugin_destroy"));
	if (!m_destroy)
	{
		unload();
		return false;
	}

	auto create = reinterpret_cast<CreateFn>(m_library.getSymbol("engine_plugin_create"));
	if (!create)
	{
		unload();
		return false;
	}

	m_plugin = create();
	if (m_plugin == nullptr)
	{
		unload();
		return false;
	}
	return true;
}

} //namespace eng::core