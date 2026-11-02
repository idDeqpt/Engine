#ifndef PLUGIN_LOADER_CLASS_HEADER
#define PLUGIN_LOADER_CLASS_HEADER

namespace eng::core
{
	class Plugin;

	class PluginLoader
	{
	public:
		virtual ~PluginLoader() = default;

		virtual void unload() = 0;
		virtual Plugin* get() const = 0;
	};
}

#endif //PLUGIN_LOADER_CLASS_HEADER