#include <Engine/Core/Node.hpp>

#include <Engine/Context.hpp>
#include <Engine/Core/Logger.hpp>
#include <Engine/Core/NameTag.hpp>
#include <Engine/Core/NodeNameTag.hpp>

#include <Engine/Math/Transform2.hpp>
#include <Engine/Math/Transform3.hpp>

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

	m_context = context;
	onSetup();
	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->setup(m_context);
	m_setuped = true;

	Logger::debug("END   setup of node \"" + m_tag.getPath() + "\" END");
}

void core::Node::update(float delta)
{
	onUpdate(delta);
	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->update(delta);
}

void core::Node::destroy()
{
	Logger::debug("START destroy of node \"" + m_tag.getPath() + "\" START");

	onDestroy();
	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->destroy();
	m_children.clear();
	
	Logger::debug("END   destroy of node \"" + m_tag.getPath() + "\" END");
}


void core::Node::onSetup() {}
void core::Node::onUpdate(float delta) {}
void core::Node::onDestroy() {}


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
	NameTag tag(name);
	for (unsigned int i = 0; i < m_children.size(); i++)
		if (m_children[i]->getTag().getNameHash() == tag.getHash())
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


bool core::Node::isChild(Node* node)
{
	for (unsigned int i = 0; i < m_children.size(); i++)
		if (node == m_children[i].get())
			return true;
	return false;
}

void core::Node::removeChild(Node* node)
{
	for (unsigned int i = 0; i < m_children.size(); i++)
		if (node == m_children[i].get())
		{
			node->destroy();
			m_children.erase(m_children.begin() + i);
			return;
		}
}

}