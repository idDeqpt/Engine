#ifndef NAME_TAG_CLASS_HEADER
#define NAME_TAG_CLASS_HEADER

#include <string>

namespace eng::core
{
	class NameTag
	{
	public:
		NameTag();
		NameTag(const std::string& string);

		const std::string& getString() const;
		unsigned int       getHash()   const;

	protected:
		std::string  m_string;
		unsigned int m_hash;
	};
}

#endif //NAME_TAG_CLASS_HEADER