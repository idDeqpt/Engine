#ifndef BASE_NODE_CLASS_HEADER
#define BASE_NODE_CLASS_HEADER

#include <Engine/Math/Transform2.hpp>
#include <Engine/Math/Transform3.hpp>
#include <Engine/Core/NodeNameTag.hpp>

#include <optional>
#include <string>
#include <vector>
#include <memory>


namespace eng::core
{
	class Node
	{
	public:
		Node();
		virtual ~Node();

		void setup();
		void update(float delta);
		void destroy();

		virtual void onSetup();
		virtual void onUpdate(float delta);
		virtual void onDestroy();

		void setParent(Node* new_parent);
		void setName(const std::string& new_name);

		Node* getParent();
		Node* getChildByName(const std::string& name);
		const NodeNameTag& getTag();


		virtual std::optional<mth::Transform2> getGlobalTransform2D();
		virtual std::optional<mth::Transform3> getGlobalTransform3D();

		template <class T, typename... Args>
		T* addChild(std::string name, Args&&... args);

	protected:
		NodeNameTag m_tag;
		Node* m_parent;
		std::vector <std::unique_ptr<Node>> m_children;
	};
}


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
	
	T* ptr = child.get();
	m_children.push_back(std::move(child));
	return ptr;
}

#endif //BASE_NODE_CLASS_HEADER