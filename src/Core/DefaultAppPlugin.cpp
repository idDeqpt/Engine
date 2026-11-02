#include <Engine/Core/DefaultAppPlugin.hpp>

#include <Engine/Core/PluginInProcess.hpp>
#include <Engine/Core/PluginHost.hpp>
#include <Engine/Core/Node.hpp>

#include <vector>
#include <memory>


namespace eng::core
{

DefaultAppPlugin::DefaultAppPlugin(std::unique_ptr<Node> root):
    m_root(std::move(root))
{}

void DefaultAppPlugin::onSetup(const PluginHost& host)
{
    host.root.addChild(std::move(m_root), "game");
}

} //namespace eng::core