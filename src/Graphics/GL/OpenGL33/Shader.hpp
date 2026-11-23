#ifndef GRAPHICS_OPENGL_3_3_SHADER_CLASS_HEADER
#define GRAPHICS_OPENGL_3_3_SHADER_CLASS_HEADER

#include <Engine/Graphics/GL/Shader.hpp>
#include <Engine/Math/Vec.hpp>
#include <Engine/Math/Mat.hpp>

#include <string>

namespace eng::gfx::gl::OpenGL33
{
	class Shader : public gl::Shader
	{
	public:
		Shader() = default;
		~Shader() = default;

		Error loadFromBuffer(const char* vertex_buffer, const char* fragment_buffer) override;

		bool setUniform(const std::string& name, bool value)         override;
		bool setUniform(const std::string& name, int value)          override;
		bool setUniform(const std::string& name, unsigned int value) override;
		bool setUniform(const std::string& name, float value)        override;
		bool setUniform(const std::string& name, const mth::Vec3& vec) override;
		bool setUniform(const std::string& name, const mth::Mat3& mat) override;
		bool setUniform(const std::string& name, const mth::Mat4& mat) override;
		bool setUniform3fv(const std::string& name, const float* values_ptr) override;
		bool setUniform4fv(const std::string& name, const float* values_ptr) override;

		void use() override;

	protected:
		unsigned int m_shader_program_id = 0;
	};
}

#endif //GRAPHICS_OPEN_GL_3_3_SHADER_CLASS_HEADER