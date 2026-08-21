#include "Api.hpp"

#include <memory>

#include "ArrayBuffer.hpp"


namespace eng::gfx::gl::Null
{

bool Api::isAvailable() {return false;}

gl::Api::Type Api::getType() {return Api::Type::NULL_API;}

void Api::init() {}

void Api::enable(const Capability& capability) {}
void Api::disable(const Capability& capability) {}

void Api::useDepthMask(bool flag) {}
void Api::setDepthFunction(const DepthFunction& function) {}
void Api::setBlendFunction(const BlendFactor& source, const BlendFactor& destination) {}

void Api::setActiveTexture(unsigned int texrure_i) {}

void Api::useShader(unsigned int shader_id) {}
unsigned int Api::compileVertexShader(const char* vertex_buffer, char* log_buffer, unsigned int log_buffer_size) {return 0;}
unsigned int Api::compileFrahmentShader(const char* fragment_buffer, char* log_buffer, unsigned int log_buffer_size) {return 0;}
unsigned int Api::linkShader(unsigned int vertex_id, unsigned int fragment_id, char* log_buffer, unsigned int log_buffer_size) {return true;}

bool Api::setUniform1i(unsigned int shader_id, const char* name, int value) {return true;}
bool Api::setUniform1ui(unsigned int shader_id, const char* name, unsigned int value) {return true;}
bool Api::setUniform1f(unsigned int shader_id, const char* name, float value) {return true;}
bool Api::setUniform3fv(unsigned int shader_id, const char* name, const float* values_ptr) {return true;}
bool Api::setUniform4fv(unsigned int shader_id, const char* name, const float* values_ptr) {return true;}
bool Api::setUniformMatrix3fv(unsigned int shader_id, const char* name, const float* matrix_ptr) {return true;}
bool Api::setUniformMatrix4fv(unsigned int shader_id, const char* name, const float* matrix_ptr) {return true;}

std::unique_ptr<gl::ArrayBuffer> Api::createArrayBuffer()
{
	return std::make_unique<Null::ArrayBuffer>();
}

} //namespace eng::gfx::gl::Null