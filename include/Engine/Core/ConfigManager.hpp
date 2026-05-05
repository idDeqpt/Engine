#ifndef CONFIG_MANAGER_CLASS_HEADER
#define CONFIG_MANAGER_CLASS_HEADER

#include <Engine/Math/Vec2.hpp>

#include <unordered_map>
#include <variant>
#include <string>


namespace eng::core
{
	class SignalBus;

	class ConfigManager
	{
	public:
		using ValueContainer = std::variant<bool, int, float, double, std::string, mth::Vec2>;

		ConfigManager(SignalBus& sbus);

		void setDefault(bool clear = true);

		void notifyOne(const std::string& key);
		void notifyAll();

		template <class T>
		void set(const std::string& key, T value);

		template <class T>
		T get(const std::string& key) const;

	protected:
		SignalBus& m_sbus;
		std::unordered_map<std::string, ValueContainer> m_configs;
	};

	template <class T>
	void ConfigManager::set(const std::string& key, T value)
	{
		m_configs[key] = value;
		notifyOne(key);
	}

	template <class T>
	T ConfigManager::get(const std::string& key) const
	{
		auto it = m_configs.find(key);
		if (it == m_configs.end())
			throw std::out_of_range("Config key not found: " + key);
		else if (!std::holds_alternative<T>(it->second))
			throw std::bad_variant_access();

		return std::get<T>(it->second);
	}
}

#endif //CONFIG_MANAGER_CLASS_HEADER