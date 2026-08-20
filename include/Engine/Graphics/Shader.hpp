#ifndef SHADER_CLASS_HEADER
#define SHADER_CLASS_HEADER

#include <Engine/Core/Resource.hpp>
#include <Engine/Math/Vec3.hpp>

#include <string>


namespace eng::gfx
{
	class Shader : public core::Resource
	{
	public:
		enum Error
		{
			NO_ERROR,
			UNINITED,
			VERTEX_FILE_NOT_EXIST,
			FRAGMENT_FILE_NOT_EXIST,
			VERTEX_COMPILE_FAILED,
			FRAGMENT_COMPILE_FAILED,
			PROGRAM_LINKING_FAILED
		};

		Shader();
		Shader(const char* vertex_buffer, const char* fragment_buffer);

		bool loadFromFile(std::initializer_list<std::string> paths);
		bool loadFromFile(std::string vertex_path, std::string fragment_path);
		bool loadFromBuffer(const char* vertex_buffer, const char* fragment_buffer);

		int getLastError();
		std::string getLastErrorLog();

		bool setUniform1i(std::string name, int value);
		bool setUniform1ui(std::string name, unsigned int value);
		bool setUniform1f(std::string name, float value);
		bool setUniformVec3(std::string name, const mth::Vec3& vec);
		bool setUniform3fv(std::string name, const float* values_ptr);
		bool setUniform4fv(std::string name, const float* values_ptr);
		bool setUniformMatrix3fv(std::string name, const float* mat_ptr);
		bool setUniformMatrix4fv(std::string name, const float* mat_ptr);

		bool use();

		static void setActive(Shader* target);
		static Shader* getActive();

	protected:
		unsigned int m_shader_program_id;
		std::string m_last_error_log;
		static Shader* s_active;
	};
}

#endif //SHADER_CLASS_HEADER