#ifndef GRAPHICS_NULL_SHADER_CLASS_HEADER
#define GRAPHICS_NULL_SHADER_CLASS_HEADER

#include <Engine/Graphics/GL/Shader.hpp>
#include <Engine/Math/Vec.hpp>
#include <Engine/Math/Mat.hpp>

#include <string>

namespace eng::gfx::gl::Null
{
	class Shader : public gl::Shader
	{
	public:
		Shader() = default;
		~Shader() = default;

		Error loadFromBuffer(const char* vertex_buffer, const char* fragment_buffer) override {return {0, ""};};

		bool setUniform(const std::string& name, bool value)         override {return true;};
		bool setUniform(const std::string& name, int value)          override {return true;};
		bool setUniform(const std::string& name, unsigned int value) override {return true;};
		bool setUniform(const std::string& name, float value)        override {return true;};
		bool setUniform(const std::string& name, const mth::Vec3& vec) override {return true;};
		bool setUniform(const std::string& name, const mth::Mat3& mat) override {return true;};
		bool setUniform(const std::string& name, const mth::Mat4& mat) override {return true;};
		bool setUniform3fv(const std::string& name, const float* values_ptr) override {return true;};
		bool setUniform4fv(const std::string& name, const float* values_ptr) override {return true;};

		void use() override {};
	};
}

#endif //GRAPHICS_NULL_SHADER_CLASS_HEADER