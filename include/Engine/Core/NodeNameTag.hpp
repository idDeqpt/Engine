#ifndef NODE_NAME_TAG_CLASS_HEADER
#define NODE_NAME_TAG_CLASS_HEADER

#include <Engine/Core/NameTag.hpp>
#include <string>

namespace eng::core
{
	class NodeNameTag
	{
	public:
		NodeNameTag();
		NodeNameTag(const std::string& name, unsigned int index, const std::string& parent_path);

		const std::string& getName() const;
		const std::string& getPath() const;
		unsigned int getIndex() const;

		unsigned int getNameHash() const;
		unsigned int getPathHash() const;

	protected:
		NameTag m_name;
		unsigned int m_index;
		NameTag m_path;
	};
}

#endif //NODE_NAME_TAG_CLASS_HEADER