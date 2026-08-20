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

		bool setUniform1i(unsigned int shader_id, const char* name, int value) override;
		bool setUniform1ui(unsigned int shader_id, const char* name, unsigned int value) override;
		bool setUniform1f(unsigned int shader_id, const char* name, float value) override;
		bool setUniform3fv(unsigned int shader_id, const char* name, const float* values_ptr) override;
		bool setUniform4fv(unsigned int shader_id, const char* name, const float* values_ptr) override;
		bool setUniformMatrix3fv(unsigned int shader_id, const char* name, const float* matrix_ptr) override;
		bool setUniformMatrix4fv(unsigned int shader_id, const char* name, const float* matrix_ptr) override;

		#endif
	};
}

#endif //GRAPHICS_OPEN_GL_3_3_API_CLASS_HEADER