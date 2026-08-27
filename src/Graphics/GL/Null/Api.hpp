#ifndef GRAPHICS_NULL_API_CLASS_HEADER
#define GRAPHICS_NULL_API_CLASS_HEADER

#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Graphics/GL/ArrayBuffer.hpp>
#include <Engine/Graphics/GL/FrameBuffer.hpp>
#include <Engine/Graphics/GL/TextureImpl.hpp>

namespace eng::gfx::gl::Null
{
	class Api : public gl::Api
	{
		static bool isAvailable();

		Type getType() override;
		void init() override;

		void enable(const Capability& capability);
		void disable(const Capability& capability);

		void useDepthMask(bool flag) override;
		void setDepthFunction(const DepthFunction& function) override;
		void setBlendFunction(const BlendFactor& source, const BlendFactor& destination) override;

		void setScissor(int x, int y, unsigned int width, unsigned int height) override;
		void setViewport(int x, int y, unsigned int width, unsigned int height) override;

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
		std::unique_ptr<FrameBuffer> createFrameBuffer() override;
		
		std::unique_ptr<TextureImpl> createTextureImpl() override;
	};
}

#endif //GRAPHICS_NULL_API_CLASS_HEADER