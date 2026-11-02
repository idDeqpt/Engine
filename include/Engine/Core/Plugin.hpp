#ifndef PLUGIN_CLASS_HEADER
#define PLUGIN_CLASS_HEADER

#include <Engine/Core/PluginHost.hpp>
//#include <Engine/System/DynamicLibrary.hpp>

namespace eng::core
{
	class Plugin
	{
	public:
		virtual ~Plugin() = default;

		virtual void onSetup(const PluginHost& host) {}
	};

	/*class PluginLibrary : public Plugin
	{
		void load(const std::string& path) {}
	protected:
		sys::DynamicLibrary m_library;
	};*/
}

#endif //PLUGIN_CLASS_HEADER