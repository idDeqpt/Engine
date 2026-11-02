#ifndef BASE_NODE_CLASS_HEADER
#define BASE_NODE_CLASS_HEADER

#include <Engine/Context.hpp>
#include <Engine/Core/NodeNameTag.hpp>
#include <Engine/Core/Delta.hpp>

#include <Engine/Math/Transform2.hpp>
#include <Engine/Math/Transform3.hpp>

#include <string>
#include <vector>
#include <memory>


namespace eng
{
	class Context;

namespace core
{
	class Node
	{
	public:
		Node() = default;
		virtual ~Node() = default;

		bool isDestroyed();

		void build();
		void setup(Context& context);
		void update(const Delta& delta);
		void destroy();

		void cleanupDestroyed();

		virtual void onBuild(); //stage before children setups, for addChild and static initializations
		virtual void onSetup(); //stage before children setups, with acces to Context
		virtual void onReady(); //stage after children setups, children are fully initialized
		virtual void onUpdate(const Delta& delta);
		virtual void onDestroy();

		void setParent(Node* new_parent);
		void setName(const std::string& new_name);

		Node* getParent();
		Node* getChildByName(const std::string& name);
		const NodeNameTag& getTag();

		template <class T>
		T* getNearestParent();

		bool isChild(Node* node);
		void addChild(std::unique_ptr<Node> child, const std::string& name);
		void removeChild(Node* node);

		template <class T, typename... Args>
		T* addChild(const std::string& name, Args&&... args);


	protected:
		Node* m_parent   = nullptr;
		bool m_built     = false;
		bool m_setuped   = false;
		bool m_destroyed = false;
		NodeNameTag m_tag;
		std::vector <std::unique_ptr<Node>> m_children;
		Context m_context;
	};


	template <class T>
	T* Node::getNearestParent()
	{
		static_assert(std::is_base_of_v<Node, T>, "T must be derived from Node");

		if (m_parent == nullptr) return nullptr;

		T* par = dynamic_cast<T*>(m_parent);
		if (par != nullptr) return par;

		return m_parent->getNearestParent<T>();
	}

	template <class T, typename... Args>
	T* Node::addChild(const std::string& name, Args&&... args)
	{
		static_assert(std::is_base_of_v<Node, T>, "T must be derived from Node");

		auto child = std::make_unique<T>(std::forward<Args>(args)...);
		
		T* ptr = child.get();
		addChild(std::move(child), name);
		return ptr;
	}
} //core
} //eng

#endif //BASE_NODE_CLASS_HEADER