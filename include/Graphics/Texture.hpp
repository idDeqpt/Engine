#ifndef TEXTURE_CLASS_HEADER
#define TEXTURE_CLASS_HEADER

#include <string>


typedef unsigned int GLuint;

namespace gfx
{
	class Texture
	{
	public:
		enum Error
		{
			NO_ERROR,
			UNINITED,
			LOAD_FAILED
		};

		Texture();
		Texture(std::string path);

		bool loadFromFile(std::string path);

		GLuint getId();

		Error getLastError();

	protected:
		GLuint id;
		int width, height;
		Error last_error;
	};
}

#endif //TEXTURE_CLASS_HEADER