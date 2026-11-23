#ifndef GRAPHICS_GL_SHADER_CLASS_HEADER
#define GRAPHICS_GL_SHADER_CLASS_HEADER

#include <Engine/Math/Vec.hpp>
#include <Engine/Math/Mat.hpp>

#include <string>

namespace eng::gfx::gl
{
	class Shader
	{
	public:
		struct Error
		{
			unsigned int code;
			std::string log;
		};

        Shader() = default;
        virtual ~Shader() = default;

		virtual Error loadFromBuffer(const char* vertex_buffer, const char* fragment_buffer) = 0;

		virtual bool setUniform(const std::string& name, bool value) = 0;
		virtual bool setUniform(const std::string& name, int value) = 0;
		virtual bool setUniform(const std::string& name, unsigned int value) = 0;
		virtual bool setUniform(const std::string& name, float value) = 0;
		virtual bool setUniform(const std::string& name, const mth::Vec3& vec) = 0;
		virtual bool setUniform(const std::string& name, const mth::Mat3& mat) = 0;
		virtual bool setUniform(const std::string& name, const mth::Mat4& mat) = 0;
		virtual bool setUniform3fv(const std::string& name, const float* values_ptr) = 0;
		virtual bool setUniform4fv(const std::string& name, const float* values_ptr) = 0;

		virtual void use() = 0;
	};
}

#endif //GRAPHICS_GL_SHADER_CLASS_HEADER