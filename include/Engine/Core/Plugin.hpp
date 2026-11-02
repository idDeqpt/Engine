#ifndef PLUGIN_CLASS_HEADER
#define PLUGIN_CLASS_HEADER

#include <Engine/Core/PluginHost.hpp>

namespace eng::core
{
	class Plugin
	{
	public:
		virtual ~Plugin() = default;

		virtual void onSetup(const PluginHost& host) {}
	};
}

#endif //PLUGIN_CLASS_HEADER