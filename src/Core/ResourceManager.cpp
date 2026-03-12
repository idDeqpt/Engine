#include <Engine/Core/ResourceManager.hpp>

#include <Engine/Core/Resource.hpp>

#include <unordered_map>
#include <string>


namespace eng
{

std::unordered_map<std::string, eng::core::Resource*> core::ResourceManager::s_resources;


void core::ResourceManager::initialize()
{
	finalize();
}

void core::ResourceManager::finalize()
{
	for (auto& [key, value] : s_resources)
		if (value)
				delete value;
	s_resources.clear();
}


void core::ResourceManager::unload(const std::string& key)
{
	auto it = s_resources.find(key);
	if (it != s_resources.end())
	{
		delete it->second;
		s_resources.erase(it);
	}
}

} //namespace eng