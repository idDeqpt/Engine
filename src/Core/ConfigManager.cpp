#include <Engine/Core/ConfigManager.hpp>

#include <Engine/Core/Logger.hpp>
#include <Engine/Core/SignalBus.hpp>
#include <Engine/Math/Vec2.hpp>

#include <unordered_map>
#include <variant>
#include <string>


namespace eng
{

core::ConfigManager::ConfigManager(SignalBus& sbus):
	m_sbus(sbus)
{
	setDefault();
}


void core::ConfigManager::setDefault(bool clear)
{
	if (clear) m_configs.clear();

	m_configs["window_title"] = std::string("Engine");
	m_configs["window_size"] = mth::Vec2(1600, 900);
	m_configs["window_viewport_size"] = mth::Vec2(1600, 900);
	m_configs["window_viewport_centering"] = mth::Vec2(0.5);
	m_configs["window_viewport_scaling"] = std::string("fixed");
}


void core::ConfigManager::notifyOne(const std::string& key)
{
	std::visit([&key, this](auto&& arg) {
		m_sbus.emit("on_change_config_" + key, arg);
	}, m_configs[key]);
}

void core::ConfigManager::notifyAll()
{
	for (const auto& [key, value] : m_configs)
		std::visit([&key, this](auto&& arg) {
			m_sbus.emit("on_change_config_" + key, arg);
		}, value);
}

} //namespace eng