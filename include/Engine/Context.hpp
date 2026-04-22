#ifndef CONTEXT_CLASS_HEADER
#define CONTEXT_CLASS_HEADER

#include <unordered_map>
#include <typeindex>
#include <stdexcept>
#include <utility>
#include <memory>


namespace eng
{
	class Context
	{
	public:
		Context() = default;
		Context(const Context& other) = default;

		Context& operator=(const Context& other) = default;

		template<typename T, typename... Args>
		std::shared_ptr<T> create(Args&&... args);

		template<typename T>
		std::shared_ptr<T> replace(std::shared_ptr<T> new_service);

		template<typename T>
		T& get() const;

		void shutdown()
		{
			for (auto it = m_services.rbegin(); it != m_services.rend(); ++it)
				it->reset();
			m_services.clear();
			m_indexes.clear();
		}

	private:
		std::vector<std::shared_ptr<void>> m_services;
		std::unordered_map<std::type_index, unsigned int> m_indexes;
	};

	template<typename T, typename... Args>
	std::shared_ptr<T> Context::create(Args&&... args)
	{
		auto idx = std::type_index(typeid(T));
		if (auto it = m_indexes.find(idx); it != m_indexes.end())
			m_services[it->second].reset();
		
		auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
		m_services.push_back(ptr);
		m_indexes[idx] = m_services.size() - 1;
		return ptr;
	}

	template<typename T>
	std::shared_ptr<T> Context::replace(std::shared_ptr<T> new_service)
	{
		auto idx = std::type_index(typeid(T));
		auto it = m_indexes.find(idx);
		
		std::shared_ptr<T> old;
		
		if (it != m_indexes.end())
		{
			unsigned int pos = it->second;
			old = std::static_pointer_cast<T>(m_services[pos]);
			m_services[pos] = new_service;
		}
		else
		{
			m_services.push_back(new_service);
			m_indexes[idx] = m_services.size() - 1;
		}
		
		return old;
	}

	template<typename T>
	T& Context::get() const
	{
		auto it = m_indexes.find(std::type_index(typeid(T)));
		if (it == m_indexes.end())
			throw std::runtime_error("Service not found");

		auto& ptr = m_services[it->second];
		if (!ptr) throw std::runtime_error("Service expired");
		return *std::static_pointer_cast<T>(ptr);
	}
}

#endif //CONTEXT_CLASS_HEADER