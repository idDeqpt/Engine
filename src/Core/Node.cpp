#include <Engine/Core/Node.hpp>

#include <Engine/Context.hpp>
#include <Engine/Core/Logger.hpp>
#include <Engine/Math/Transform2.hpp>
#include <Engine/Math/Transform3.hpp>
#include <Engine/Core/NodeNameTag.hpp>

#include <optional>
#include <string>
#include <vector>


namespace eng
{

core::Node::Node():
	m_setuped(false),
	m_parent(nullptr)
{
	m_children.clear();
}

core::Node::~Node()
{}


void core::Node::setup(Context& context)
{
	Logger::debug("START setup of node \"" + m_tag.getPath() + "\" START");

	onSetup(context);
	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->setup(context);
	m_setuped = true;

	Logger::debug("END   setup of node \"" + m_tag.getPath() + "\" END");
}

void core::Node::update(Context& context, float delta)
{
	onUpdate(context, delta);
	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->update(context, delta);
}

void core::Node::destroy(Context& context)
{
	onDestroy(context);
	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->destroy(context);
	m_children.clear();
}


void core::Node::onSetup(Context& context) {}
void core::Node::onUpdate(Context& context, float delta) {}
void core::Node::onDestroy(Context& context) {}


void core::Node::setParent(Node* new_parent)
{
	m_parent = new_parent;
}

void core::Node::setName(const std::string& new_name)
{
	m_tag = NodeNameTag(new_name, m_tag.getIndex(), m_parent ? m_parent->getTag().getPath() : "");
}


core::Node* core::Node::getParent()
{
	return m_parent;
}

core::Node* core::Node::getChildByName(const std::string& name)
{
	NodeNameTag tag(name, 0, "");
	for (unsigned int i = 0; i < m_children.size(); i++)
		if (m_children[i]->getTag().getNameHash() == tag.getNameHash())
			return m_children[i].get();
	return nullptr;
}

const core::NodeNameTag& core::Node::getTag()
{
	return m_tag;
}


std::optional<mth::Transform2> core::Node::getGlobalTransform2D()
{
	return m_parent ? m_parent->getGlobalTransform2D() : std::nullopt;
}

std::optional<mth::Transform3> core::Node::getGlobalTransform3D()
{
	return m_parent ? m_parent->getGlobalTransform3D() : std::nullopt;
}


}