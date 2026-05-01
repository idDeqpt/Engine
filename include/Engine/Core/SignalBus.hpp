#ifndef SIGNAL_BUS_CLASS_HEADER
#define SIGNAL_BUS_CLASS_HEADER

#include <unordered_map>
#include <functional>
#include <typeindex>
#include <algorithm>
#include <vector>
#include <memory>
#include <atomic>
#include <string>


namespace eng::core
{
	class CallbackBase
	{
	public:
		virtual ~CallbackBase() = default;
	};

	template <typename... Args>
	class Callback : public CallbackBase
	{
	public:
		using FuncType = std::function<void(Args...)>;

		Callback(FuncType func): m_func(std::move(func)) {}

		void invoke(Args... args) const {m_func(args...);}

	private:
		FuncType m_func;
	};


	using SubscriptionId = unsigned int;

	class SignalBus
	{
	public:
		template <typename... Args>
		SubscriptionId subscribe(const std::string& signal_name, std::function<void(Args...)> callback);

		template <typename Func>
		SubscriptionId subscribe(const std::string& signal_name, Func&& callback);

		void unsubscribe(SubscriptionId id);

		void clear(const std::string& signal_name);
		void clearAll();

		template <typename... Args>
		void emit(const std::string& signal_name, Args... args) const;

	private:
		mutable std::unordered_map<std::string,
			std::unordered_map<std::type_index,
				std::vector<std::pair<SubscriptionId, std::shared_ptr<CallbackBase>>>>>
			m_subscribers; //map of <signal name, subscribers>

		mutable std::atomic<SubscriptionId> m_next_id{1};
	};


	template <typename... Args>
	SubscriptionId SignalBus::subscribe(const std::string& signal_name, std::function<void(Args...)> callback)
	{
		auto& type_map = m_subscribers[signal_name];
		auto& callbacks = type_map[std::type_index(typeid(void(Args...)))];
		
		SubscriptionId id = m_next_id++;
		callbacks.emplace_back(id, std::make_shared<Callback<Args...>>(std::move(callback)));
		return id;
	}

	template <typename Func>
	SubscriptionId SignalBus::subscribe(const std::string& signal_name, Func&& callback)
	{
		return subscribe(signal_name, std::function{std::forward<Func>(callback)});
	}

	template <typename... Args>
	void SignalBus::emit(const std::string& signal_name, Args... args) const
	{
		auto it = m_subscribers.find(signal_name);
		if (it == m_subscribers.end()) return;

		const auto& type_map = it->second;
		auto type_it = type_map.find(std::type_index(typeid(void(Args...))));
		if (type_it == type_map.end()) return;

		auto callbacks_copy = type_it->second; // vector<pair<SubscriptionId, shared_ptr<CallbackBase>>>
		for (const auto& [id, cb_ptr] : callbacks_copy)
		{
			auto typed_cb = std::dynamic_pointer_cast<Callback<Args...>>(cb_ptr);
			if (typed_cb)
				typed_cb->invoke(args...);
		}
	}
}

#endif //SIGNAL_BUS_CLASS_HEADER