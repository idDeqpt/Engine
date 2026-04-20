#include <Engine/Core/ResourceManager.hpp>

#include <Engine/Core/Resource.hpp>

#include <unordered_map>
#include <string>


namespace eng
{

core::ResourceManager::ResourceManager() {}

core::ResourceManager::~ResourceManager()
{
	for (auto& [key, value] : m_resources)
		if (value)
				delete value;
	m_resources.clear();
}


void core::ResourceManager::unload(const std::string& key)
{
	auto it = m_resources.find(key);
	if (it != m_resources.end())
	{
		delete it->second;
		m_resources.erase(it);
	}
}

} //namespace eng