#include "Api.hpp"

#include <memory>

#include "ArrayBuffer.hpp"
#include "FrameBuffer.hpp"
#include "Shader.hpp"
#include "TextureImpl.hpp"


namespace eng::gfx::gl::Null
{

bool Api::isAvailable() {return false;}

gl::Api::Type Api::getType() {return Api::Type::NULL_API;}

void Api::initialize(gl::Api::ProcAddressLoader loader) {}

void Api::setCapability(const Capability& capability) {}

void Api::useDepthMask(bool flag) {}
void Api::setDepthFunction(const DepthFunction& function) {}
void Api::setBlendFunction(const BlendFactor& source, const BlendFactor& destination) {}

void Api::setScissor(int x, int y, unsigned int width, unsigned int height) {}
void Api::setViewport(int x, int y, unsigned int width, unsigned int height) {}

void Api::setActiveTexture(unsigned int texrure_i) {}

std::unique_ptr<gl::ArrayBuffer> Api::createArrayBuffer()
{
	return std::make_unique<Null::ArrayBuffer>();
}

std::unique_ptr<gl::FrameBuffer> Api::createFrameBuffer()
{
	return std::make_unique<Null::FrameBuffer>();
}

std::unique_ptr<gl::Shader> Api::createShader()
{
	return std::make_unique<Null::Shader>();
}


std::unique_ptr<gl::TextureImpl> Api::createTextureImpl()
{
	return std::make_unique<Null::TextureImpl>();
}

} //namespace eng::gfx::gl::Null