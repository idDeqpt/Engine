#ifndef PLUGIN_LOADER_LIBRARY_CLASS_HEADER
#define PLUGIN_LOADER_LIBRARY_CLASS_HEADER

#include <Engine/System/DynamicLibrary.hpp>
#include <Engine/Core/PluginLoader.hpp>
#include <Engine/Core/Plugin.hpp>
#include <string>

namespace eng::core
{
	class PluginLoaderLibrary : public PluginLoader
	{
	public:
		PluginLoaderLibrary() = default;
		~PluginLoaderLibrary();

		bool load(const std::string& lib_path);
		void unload() override;

		Plugin* get() const override;

	protected:
		using CreateFn  = Plugin* (*)();
		using DestroyFn = void (*)(Plugin*);

		sys::DynamicLibrary m_library;
		Plugin* m_plugin = nullptr;
		DestroyFn m_destroy = nullptr;
	};
}

#endif //PLUGIN_LOADER_LIBRARY_CLASS_HEADER