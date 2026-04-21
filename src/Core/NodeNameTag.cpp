#include <Engine/Core/NodeNameTag.hpp>

#include <string>
#include <functional>


namespace eng
{

core::NodeNameTag::NodeNameTag(): NodeNameTag("", 0, "") {}

core::NodeNameTag::NodeNameTag(const std::string& name, unsigned int index, const std::string& parent_path):
	m_name(name),
	m_index(index),
	m_path(parent_path + "/" + name + (index ? ("_" + std::to_string(index)) : "")) {}


const std::string& core::NodeNameTag::getName() const
{
	return m_name.getString();
}

const std::string& core::NodeNameTag::getPath() const
{
	return m_path.getString();
}

unsigned int core::NodeNameTag::getIndex() const
{
	return m_index;
}


unsigned int core::NodeNameTag::getNameHash() const
{
	return m_name.getHash();
}

unsigned int core::NodeNameTag::getPathHash() const
{
	return m_path.getHash();
}

} //namespace eng