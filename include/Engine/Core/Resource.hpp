#ifndef RESOURCE_CLASS_HEADER
#define RESOURCE_CLASS_HEADER

#include <initializer_list>
#include <string>

namespace eng::core
{
	class Resource
	{
	public:
		virtual bool loadFromFile(std::initializer_list<std::string> paths) {return false;}

		int getLastError() {return 0;}

	protected:
		int m_last_error;
	};
}

#endif //RESOURCE_CLASS_HEADER