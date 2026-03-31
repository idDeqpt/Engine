#ifndef BASE_NODE_CLASS_HEADER
#define BASE_NODE_CLASS_HEADER

#include <Engine/Math/Transform2.hpp>
#include <Engine/Math/Transform3.hpp>

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
		void setName(std::string new_name);

		Node* getParent();
		Node* getChildByName(const std::string& name);
		std::string getName();
		std::string getNamePath();


		virtual std::optional<mth::Transform2> getGlobalTransform2D();
		virtual std::optional<mth::Transform3> getGlobalTransform3D();

		template <class T, typename... Args>
		T* addChild(std::string name, Args&&... args);

	protected:
		std::string m_name;
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
	child->setName(name);

	unsigned int count = 0;
	for (unsigned int i = 0; i < m_children.size(); i++)
		if (m_children[i]->getName() == name) count++;
	if (count > 0)
		name += "_" + std::to_string(++count);
	
	T* ptr = child.get();
	m_children.push_back(std::move(child));
	return ptr;
}

#endif //BASE_NODE_CLASS_HEADER