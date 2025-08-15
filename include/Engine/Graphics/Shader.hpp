#ifndef SHADER_CLASS_HEADER
#define SHADER_CLASS_HEADER

#include <string>
#include <Engine/Math/Vec3.hpp>


typedef unsigned int GLuint;
typedef char GLchar;

namespace gfx
{
	class Shader
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
		Shader(const GLchar* vertex_buffer, const GLchar* fragment_buffer);

		bool loadFromBuffer(const GLchar* vertex_buffer, const GLchar* fragment_buffer);
		bool loadFromFile(std::string vertex_path, std::string fragment_path);

		Error getLastError();
		std::string getLastErrorLog();

		bool setUniform1i(std::string name, int value);
		bool setUniform1f(std::string name, float value);
		bool setUniformVec3(std::string name, const mth::Vec3& vec);
		bool setUniform3fv(std::string name, int count, float* values_ptr);
		bool setUniform4fv(std::string name, int count, float* values_ptr);
		bool setUniformMatrix3fv(std::string name, float* mat_ptr);
		bool setUniformMatrix4fv(std::string name, float* mat_ptr);

		bool use();

		static void setActive(Shader* target);
		static Shader* getActive();

	protected:
		GLuint shader_program_id;
		Error last_error;
		std::string last_error_log;
		static Shader* active;
	};
}

#endif //SHADER_CLASS_HEADER