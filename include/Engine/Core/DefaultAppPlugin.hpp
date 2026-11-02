#ifndef DEFAULT_APP_PLUGIN_CLASS_HEADER
#define DEFAULT_APP_PLUGIN_CLASS_HEADER

#include <Engine/Core/PluginInProcess.hpp>
#include <Engine/Core/PluginHost.hpp>
#include <Engine/Core/Node.hpp>

namespace eng::core
{
	class DefaultAppPlugin : public PluginInProcess
	{
	public:
		DefaultAppPlugin(std::unique_ptr<Node> root);

        void onSetup(const PluginHost& host) override;

    protected:
        std::unique_ptr<Node> m_root;
	};
}

#endif //DEFAULT_APP_PLUGIN_CLASS_HEADER