#include <Engine/Core/SignalBus.hpp>

#include <unordered_map>
#include <functional>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>


namespace eng
{

void core::SignalBus::unsubscribe(SubscriptionId id)
{
	for (auto& [signal_name, type_map] : m_subscribers)
		for (auto& [type_idx, callbacks] : type_map)
		{
			auto it = std::find_if(callbacks.begin(), callbacks.end(),
				[id](const auto& pair) {return pair.first == id;});
			if (it != callbacks.end())
			{
				callbacks.erase(it);
				return;
			}
		}
}

void core::SignalBus::clear(const std::string& signal_name)
{
	m_subscribers.erase(signal_name);
}

void core::SignalBus::clearAll()
{
	m_subscribers.clear();
}

} //namespace eng