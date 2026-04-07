#ifndef STRING_CLASS_HEADER
#define STRING_CLASS_HEADER

#include <string>

namespace eng::core
{
	class String
	{
	public:
		String();
		String(const char* v);
		String(const std::string& v);
		String(int v);
		String(unsigned int v);
		String(float v);

		const std::string& getString() const;

		String& operator<<(const char* v);
		String& operator<<(const std::string& v);
		String& operator<<(int v);
		String& operator<<(unsigned int v);
		String& operator<<(float v);

	protected:
		std::string m_str;
	};
}

#endif //STRING_CLASS_HEADER