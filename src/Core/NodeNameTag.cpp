#include <Engine/Core/NodeNameTag.hpp>

#include <string>
#include <functional>


namespace eng
{

core::NodeNameTag::NodeNameTag():
	m_index(0)
{
	updateHashes();
}

core::NodeNameTag::NodeNameTag(const std::string& name, unsigned int index, const std::string& parent_path):
	m_name(name),
	m_index(index),
	m_path(parent_path + "/" + name + (index ? ("_" + std::to_string(index)) : ""))
{
	updateHashes();
}


const std::string& core::NodeNameTag::getName() const
{
	return m_name;
}

const std::string& core::NodeNameTag::getPath() const
{
	return m_path;
}

unsigned int core::NodeNameTag::getIndex() const
{
	return m_index;
}


unsigned int core::NodeNameTag::getNameHash() const
{
	return m_name_hash;
}

unsigned int core::NodeNameTag::getPathHash() const
{
	return m_path_hash;
}



void core::NodeNameTag::updateHashes()
{
	m_name_hash = std::hash<std::string>{}(m_name);
	m_path_hash = std::hash<std::string>{}(m_path);
}

} //namespace eng