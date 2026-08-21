#ifndef GRAPHICS_OPEN_GL_3_3_API_CLASS_HEADER
#define GRAPHICS_OPEN_GL_3_3_API_CLASS_HEADER

#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Graphics/GL/ArrayBuffer.hpp>

namespace eng::gfx::gl::OpenGL33
{
	class Api : public gl::Api
	{
		static bool isAvailable();

		#ifdef USE_OPENGL_3_3

		void init() override;

		void enable(const Capability& capability);
		void disable(const Capability& capability);

		void useDepthMask(bool flag) override;
		void setDepthFunction(const DepthFunction& function) override;
		void setBlendFunction(const BlendFactor& source, const BlendFactor& destination) override;

		void setActiveTexture(unsigned int texrure_i) override;

		void useShader(unsigned int shader_id) override;
		unsigned int compileVertexShader(const char* vertex_buffer, char* log_buffer, unsigned int log_buffer_size) override;
		unsigned int compileFrahmentShader(const char* fragment_buffer, char* log_buffer, unsigned int log_buffer_size) override;
		unsigned int linkShader(unsigned int vertex_id, unsigned int fragment_id, char* log_buffer, unsigned int log_buffer_size) override;
		bool setUniform1i(unsigned int shader_id, const char* name, int value) override;
		bool setUniform1ui(unsigned int shader_id, const char* name, unsigned int value) override;
		bool setUniform1f(unsigned int shader_id, const char* name, float value) override;
		bool setUniform3fv(unsigned int shader_id, const char* name, const float* values_ptr) override;
		bool setUniform4fv(unsigned int shader_id, const char* name, const float* values_ptr) override;
		bool setUniformMatrix3fv(unsigned int shader_id, const char* name, const float* matrix_ptr) override;
		bool setUniformMatrix4fv(unsigned int shader_id, const char* name, const float* matrix_ptr) override;

		std::unique_ptr<ArrayBuffer> createArrayBuffer() override;

		#endif
	};
}

#endif //GRAPHICS_OPEN_GL_3_3_API_CLASS_HEADER