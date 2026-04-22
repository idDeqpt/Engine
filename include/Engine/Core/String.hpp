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
		String(bool v);
		String(int v);
		String(unsigned int v);
		String(float v);
		String(const void* v);

		const std::string& getString() const;

		String& operator<<(const char* v);
		String& operator<<(const std::string& v);
		String& operator<<(bool v);
		String& operator<<(int v);
		String& operator<<(unsigned int v);
		String& operator<<(float v);
		String& operator<<(const void* v);

	protected:
		std::string m_str;
	};
}

#endif //STRING_CLASS_HEADER