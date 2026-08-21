#ifndef GRAPHICS_API_CLASS_HEADER
#define GRAPHICS_API_CLASS_HEADER

#include <Engine/Graphics/GL/ArrayBuffer.hpp>
#include <memory>

namespace eng::gfx::gl
{
	enum class Capability
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

	enum class DepthFunction
	{
		NEVER,
		LESS,
		EQUAL,
		LEQUAL,
		GREATER,
		NOTEQUAL,
		GEQUAL,
		ALWAYS
	};

	enum class BlendFactor
	{
		ZERO,
		ONE,
		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,
		DST_COLOR,
		ONE_MINUS_DST_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_COLOR,
		CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA,
		SRC_ALPHA_SATURATE
	};

	class Api
	{
	public:
		enum class Type
		{
			NULL_API,
			OPENGL_3_3,
			AUTO
		};

		static Api* getInstance();
		static void createInstance(Type type = Type::AUTO);

		virtual ~Api() = default;

		virtual void init() = 0;

		virtual void enable(const Capability& capability) = 0;
		virtual void disable(const Capability& capability) = 0;

		virtual void useDepthMask(bool flag) = 0;
		virtual void setDepthFunction(const DepthFunction& function) = 0;
		virtual void setBlendFunction(const BlendFactor& source, const BlendFactor& destination) = 0;

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

		virtual std::unique_ptr<ArrayBuffer> createArrayBuffer() = 0;

	protected:
		static std::unique_ptr<Api> s_instance;

	private:
		static std::unique_ptr<Api> createImpl(Type type);
	};
}

#endif //GRAPHICS_API_CLASS_HEADER