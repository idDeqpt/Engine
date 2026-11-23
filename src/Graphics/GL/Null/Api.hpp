#ifndef GRAPHICS_NULL_API_CLASS_HEADER
#define GRAPHICS_NULL_API_CLASS_HEADER

#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Graphics/GL/ArrayBuffer.hpp>
#include <Engine/Graphics/GL/FrameBuffer.hpp>
#include <Engine/Graphics/GL/Shader.hpp>
#include <Engine/Graphics/GL/TextureImpl.hpp>

namespace eng::gfx::gl::Null
{
	class Api : public gl::Api
	{
		static bool isAvailable();

		Type getType() override;
		void initialize() override;

		void setCapability(const Capability& capability) override;

		void useDepthMask(bool flag) override;
		void setDepthFunction(const DepthFunction& function) override;
		void setBlendFunction(const BlendFactor& source, const BlendFactor& destination) override;

		void setScissor(int x, int y, unsigned int width, unsigned int height) override;
		void setViewport(int x, int y, unsigned int width, unsigned int height) override;

		void setActiveTexture(unsigned int texrure_i) override;

		std::unique_ptr<ArrayBuffer> createArrayBuffer() override;
		std::unique_ptr<FrameBuffer> createFrameBuffer() override;
		std::unique_ptr<Shader>      createShader()      override;
		
		std::unique_ptr<TextureImpl> createTextureImpl() override;
	};
}

#endif //GRAPHICS_NULL_API_CLASS_HEADER