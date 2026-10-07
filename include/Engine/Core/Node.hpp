#ifndef BASE_NODE_CLASS_HEADER
#define BASE_NODE_CLASS_HEADER

#include <Engine/Context.hpp>
#include <Engine/Core/NodeNameTag.hpp>
#include <Engine/Core/Delta.hpp>

#include <Engine/Math/Transform2.hpp>
#include <Engine/Math/Transform3.hpp>

#include <optional>
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


		virtual std::optional<mth::Transform2> getGlobalTransform2D();
		virtual std::optional<mth::Transform3> getGlobalTransform3D();

		bool isChild(Node* node);
		void removeChild(Node* node);

		template <class T, typename... Args>
		T* addChild(std::string name, Args&&... args);


	protected:
		Node* m_parent   = nullptr;
		bool m_built     = false;
		bool m_setuped   = false;
		bool m_destroyed = false;
		NodeNameTag m_tag;
		std::vector <std::unique_ptr<Node>> m_children;
		Context m_context;
	};
} //core
} //eng


template <class T, typename... Args>
T* eng::core::Node::addChild(std::string name, Args&&... args)
{
	static_assert(std::is_base_of_v<Node, T>, "T must be derived from Node");

	auto child = std::make_unique<T>(std::forward<Args>(args)...);
	child->setParent(this);

	NodeNameTag tag(name, 0, "");
	unsigned int count = 0;
	for (unsigned int i = 0; i < m_children.size(); i++)
		if (m_children[i]->getTag().getNameHash() == tag.getNameHash()) count++;

	child->m_tag = NodeNameTag(name, count, m_tag.getPath());
	if (m_setuped) child->setup(m_context);
	
	T* ptr = child.get();
	m_children.push_back(std::move(child));
	return ptr;
}

#endif //BASE_NODE_CLASS_HEADER