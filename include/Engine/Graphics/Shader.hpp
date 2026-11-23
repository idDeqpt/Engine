#ifndef SHADER_CLASS_HEADER
#define SHADER_CLASS_HEADER

#include <Engine/Graphics/GL/Shader.hpp>
#include <Engine/Core/Resource.hpp>
#include <Engine/Math/Vec.hpp>

#include <string>
#include <memory>


namespace eng::gfx
{
	class Shader : public core::Resource
	{
	public:
		enum Error
		{
			NO_ERROR = 0,
			UNINITED,
			VERTEX_FILE_NOT_EXIST,
			FRAGMENT_FILE_NOT_EXIST,
			VERTEX_COMPILE_FAILED,
			FRAGMENT_COMPILE_FAILED,
			PROGRAM_LINKING_FAILED
		};

		Shader();
		Shader(const char* vertex_buffer, const char* fragment_buffer);

		bool loadFromFile(std::initializer_list<std::string> paths) override;
		bool loadFromFile(const std::string& vertex_path, const std::string& fragment_path);
		bool loadFromBuffer(const char* vertex_buffer, const char* fragment_buffer) {return convertErrorToLog(m_shader->loadFromBuffer(vertex_buffer, fragment_buffer));}

		int getLastError() override;
		const std::string& getLastErrorLog();

		bool setUniform(const std::string& name, bool value)         {return m_shader->setUniform(name, value);}
		bool setUniform(const std::string& name, int value)          {return m_shader->setUniform(name, value);}
		bool setUniform(const std::string& name, unsigned int value) {return m_shader->setUniform(name, value);}
		bool setUniform(const std::string& name, float value)        {return m_shader->setUniform(name, value);}
		bool setUniform(const std::string& name, const mth::Vec3& vec) {return m_shader->setUniform(name, vec);}
		bool setUniform(const std::string& name, const mth::Mat3& mat) {return m_shader->setUniform(name, mat);}
		bool setUniform(const std::string& name, const mth::Mat4& mat) {return m_shader->setUniform(name, mat);}
		bool setUniform3fv(const std::string& name, const float* values_ptr) {return m_shader->setUniform3fv(name, values_ptr);}
		bool setUniform4fv(const std::string& name, const float* values_ptr) {return m_shader->setUniform4fv(name, values_ptr);}

		bool use();

	protected:
		std::unique_ptr<gl::Shader> m_shader;
		std::string m_last_error_log;

		bool convertErrorToLog(const gl::Shader::Error& error)
		{
			if (error.code == 0)	
		{
			m_last_error = Shader::Error::NO_ERROR;
			m_last_error_log.clear();
			return true;
		}

			m_last_error = error.code + 3;
			m_last_error_log = std::move(error.log);
			return false;
		}
	};
}

#endif //SHADER_CLASS_HEADER