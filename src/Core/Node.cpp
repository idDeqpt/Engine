#include <Engine/Core/Node.hpp>

#include <Engine/Context.hpp>
#include <Engine/Core/Logger.hpp>
#include <Engine/Core/NameTag.hpp>
#include <Engine/Core/NodeNameTag.hpp>

#include <Engine/Math/Transform2.hpp>
#include <Engine/Math/Transform3.hpp>

#include <string>
#include <vector>


namespace eng::core
{

bool Node::isDestroyed()
{
	return m_destroyed;
}


void Node::build()
{
	if (m_built) return;
	Logger::debug("START build of node \"" + m_tag.getPath() + "\" START");

	onBuild();
	m_built = true;
	for (auto& child : m_children)
		child->build();

	Logger::debug("END   build of node \"" + m_tag.getPath() + "\" END");
}

void Node::setup(Context& context)
{
	if (!m_built) build();
	Logger::debug("START setup of node \"" + m_tag.getPath() + "\" START");

	m_context = context;
	onSetup();
	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->setup(m_context);
	m_setuped = true;
	onReady();

	Logger::debug("END   setup of node \"" + m_tag.getPath() + "\" END");
}

void Node::update(const Delta& delta)
{
	onUpdate(delta);
	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->update(delta);
}

void Node::destroy()
{
	if (m_destroyed) return;
	
	Logger::debug("START destroy of node \"" + m_tag.getPath() + "\" START");

	m_destroyed = true;
	onDestroy();
	for (unsigned int i = 0; i < m_children.size(); i++)
		m_children[i]->destroy();
	
	Logger::debug("END   destroy of node \"" + m_tag.getPath() + "\" END");
}


void Node::cleanupDestroyed()
{
	for (auto it = m_children.begin(); it != m_children.end(); )
		if ((*it)->m_destroyed)
			it = m_children.erase(it);
		else
		{
			(*it)->cleanupDestroyed();
			it++;
		}
}


void Node::onBuild() {}
void Node::onSetup() {}
void Node::onReady() {}
void Node::onUpdate(const Delta& delta) {}
void Node::onDestroy() {}


void Node::setParent(Node* new_parent)
{
	m_parent = new_parent;
}

void Node::setName(const std::string& new_name)
{
	m_tag = NodeNameTag(new_name, m_tag.getIndex(), m_parent ? m_parent->getTag().getPath() : "");
}


Node* Node::getParent()
{
	return m_parent;
}

Node* Node::getChildByName(const std::string& name)
{
	NameTag tag(name);
	for (unsigned int i = 0; i < m_children.size(); i++)
		if (m_children[i]->getTag().getNameHash() == tag.getHash())
			return m_children[i].get();
	return nullptr;
}

const NodeNameTag& Node::getTag()
{
	return m_tag;
}


bool Node::isChild(Node* node)
{
	for (unsigned int i = 0; i < m_children.size(); i++)
		if (node == m_children[i].get())
			return true;
	return false;
}

void Node::removeChild(Node* node)
{
	for (unsigned int i = 0; i < m_children.size(); i++)
		if (node == m_children[i].get())
		{
			node->destroy();
			return;
		}
}

} //namespace eng::core