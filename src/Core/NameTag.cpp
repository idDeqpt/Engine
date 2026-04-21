#include <Engine/Core/NameTag.hpp>

#include <string>
#include <functional>


namespace eng
{

core::NameTag::NameTag(): NameTag("") {}

core::NameTag::NameTag(const std::string& string):
	m_string(string),
	m_hash(std::hash<std::string>{}(m_string)) {}


const std::string& core::NameTag::getString() const
{
	return m_string;
}

unsigned int core::NameTag::getHash() const
{
	return m_hash;
}

} //namespace eng