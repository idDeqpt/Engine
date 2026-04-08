#ifndef RESOURCE_MANAGER_STATIC_CLASS_HEADER
#define RESOURCE_MANAGER_STATIC_CLASS_HEADER

#include <Engine/Core/Resource.hpp>
#include <Engine/Core/Logger.hpp>

#include <initializer_list>
#include <unordered_map>
#include <utility>
#include <string>
#include <iostream>


namespace eng::core
{
	class ResourceManager
	{
	public:
		static void initialize();
		static void finalize();

		template<typename T>
		static std::pair<std::string, T*> load(std::initializer_list<std::string> paths);

		static void unload(const std::string& key);

	protected:
		static std::unordered_map<std::string, Resource*> s_resources;
	};
}


template<typename T>
static std::pair<std::string, T*> eng::core::ResourceManager::load(std::initializer_list<std::string> paths)
{
	static_assert(std::is_base_of<Resource, T>::value, "T must inherit from Resource");

	std::string key = "";
	for (std::string path : paths)
		key += path;

	auto it = s_resources.find(key);

	T* res = nullptr;
	if (it == s_resources.end())
	{
		res = new T();
		bool loaded = res->loadFromFile(paths);
		if (!loaded)
		{
			for (std::string path : paths)
				Logger::error(String("Failure loading from path \"" + path + "\""));
			return std::make_pair(key, nullptr);
		}
		for (std::string path : paths)
			Logger::debug(String("Success loading from path \"" + path + "\""));
		s_resources[key] = res;
	}
	else
	{
		res = dynamic_cast<T*>(it->second);
		if (!res)
		{
			for (std::string path : paths)
				Logger::error("Type mismatch for Resource from path \"" + path + "\"");
			return std::make_pair(key, nullptr);
		}
		for (std::string path : paths)
			Logger::debug("Resource have been loaded early from path \"" + path + "\"");
	}

	return std::make_pair(key, res);
}

#endif //RESOURCE_MANAGER_STATIC_CLASS_HEADER