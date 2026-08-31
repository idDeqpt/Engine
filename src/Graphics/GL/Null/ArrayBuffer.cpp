#include "ArrayBuffer.hpp"

#include <Engine/Graphics/GL/ArrayBuffer.hpp>
#include <Engine/Graphics/PrimitiveType.hpp>


namespace eng::gfx::gl::Null
{

void ArrayBuffer::loadData(const void* data, unsigned int size, gl::ArrayBuffer::Usage usage) {}
void ArrayBuffer::loadIndexes(const void* data, unsigned int count, Usage usage) {}
void ArrayBuffer::loadInstances(mth::Mat4* transforms, unsigned int count) {}

void ArrayBuffer::configureAttrib(unsigned int index, unsigned int components, bool normalized, unsigned int stride, unsigned int offset) {}
void ArrayBuffer::configureInstanceAttrib(unsigned int index, unsigned int divisor, unsigned int components, bool normalized, unsigned int stride, unsigned int offset) {}

void ArrayBuffer::draw(PrimitiveType p_type, unsigned int first, unsigned int count) {}

} //namespace eng::gfx::gl::Null
