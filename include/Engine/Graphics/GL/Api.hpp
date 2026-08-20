#ifndef GRAPHICS_API_CLASS_HEADER
#define GRAPHICS_API_CLASS_HEADER

#include <memory>

namespace eng::gfx::gl
{
	enum Capability
	{
		DEPTH_TEST,
		BLEND,
		SCISSOR_TEST,
		CULL_FACE,
		POLYGON_OFFSET_FILL,
		STENCIL_TEST,
		DITHER,
		MULTISAMPLE
	};

	class Api
	{
	public:
		enum class Type
		{
			OPENGL_3_3,
			AUTO
		};

		static Api* getInstance();
		static void createInstance(Type type = Type::AUTO);

		virtual ~Api() = default;

		virtual void init() = 0;

		virtual void enable(Capability capability) = 0;
		virtual void disable(Capability capability) = 0;

		virtual void setActiveTexture(unsigned int texrure_i) = 0;

		virtual void useShader(unsigned int shader_id) = 0;
		virtual unsigned int compileVertexShader(const char* vertex_buffer, char* log_buffer, unsigned int log_buffer_size) = 0;
		virtual unsigned int compileFrahmentShader(const char* fragment_buffer, char* log_buffer, unsigned int log_buffer_size) = 0;
		virtual unsigned int linkShader(unsigned int vertex_id, unsigned int fragment_id, char* log_buffer, unsigned int log_buffer_size) = 0;
		virtual bool setUniform1i(unsigned int shader_id, const char* name, int value) = 0;
		virtual bool setUniform1ui(unsigned int shader_id, const char* name, unsigned int value) = 0;
		virtual bool setUniform1f(unsigned int shader_id, const char* name, float value) = 0;
		virtual bool setUniform3fv(unsigned int shader_id, const char* name, const float* values_ptr) = 0;
		virtual bool setUniform4fv(unsigned int shader_id, const char* name, const float* values_ptr) = 0;
		virtual bool setUniformMatrix3fv(unsigned int shader_id, const char* name, const float* matrix_ptr) = 0;
		virtual bool setUniformMatrix4fv(unsigned int shader_id, const char* name, const float* matrix_ptr) = 0;

	protected:
		static std::unique_ptr<Api> s_instance;

	private:
		static std::unique_ptr<Api> createImpl(Type type);
	};
}

#endif //GRAPHICS_API_CLASS_HEADER