#include <Engine/Core/Node.hpp>

#include <Engine/Core/Logger.hpp>
#include <Engine/Math/Transform2.hpp>
#include <Engine/Math/Transform3.hpp>

#include <optional>
#include <string>
#include <vector>


namespace eng
{

core::Node::Node():
	m_name(""),
	m_parent(nullptr)
{
	m_children.clear();
}

core::Node::~Node()
{
	destroy();
}


void core::Node::setup()
{
	Logger::debug("START setup of node \"" + getNamePath() + "\" START");

	onSetup();
	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->setup();

	Logger::debug("END   setup of node \"" + getNamePath() + "\" END");
}

void core::Node::update(float delta)
{
	onUpdate(delta);
	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->update(delta);
}

void core::Node::destroy()
{
	onDestroy();
	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->destroy();
}


void core::Node::onSetup() {}
void core::Node::onUpdate(float delta) {}
void core::Node::onDestroy() {}


void core::Node::setParent(Node* new_parent)
{
	m_parent = new_parent;
}

void core::Node::setName(std::string new_name)
{
	m_name = new_name;
}


core::Node* core::Node::getParent()
{
	return m_parent;
}

core::Node* core::Node::getChildByName(const std::string& name)
{
	for (unsigned int i = 0; i < m_children.size(); i++)
		if (m_children[i]->getName() == name)
			return m_children[i].get();
	return nullptr;
}

std::string core::Node::getName()
{
	return m_name;
}

std::string core::Node::getNamePath()
{

	return (m_parent ? m_parent->getNamePath() : "") + ("/" + m_name);
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