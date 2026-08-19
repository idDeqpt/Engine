#ifndef GRAPHICS_OPEN_GL_3_3_API_CLASS_HEADER
#define GRAPHICS_OPEN_GL_3_3_API_CLASS_HEADER

#include <Engine/Graphics/GL/Api.hpp>

namespace eng::gfx::gl::OpenGL33
{
	class Api : public gl::Api
	{
		static bool isAvailable();

		#ifdef USE_OPENGL_3_3

		void init() override;

		#endif
	};
}

#endif //GRAPHICS_OPEN_GL_3_3_API_CLASS_HEADER