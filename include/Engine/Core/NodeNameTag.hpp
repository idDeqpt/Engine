#ifndef NODE_NAME_TAG_CLASS_HEADER
#define NODE_NAME_TAG_CLASS_HEADER

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
		std::string m_name;
		unsigned int m_index;
		std::string m_path;
		unsigned int m_name_hash;
		unsigned int m_path_hash;

		void updateHashes();
	};
}

#endif //NODE_NAME_TAG_CLASS_HEADER