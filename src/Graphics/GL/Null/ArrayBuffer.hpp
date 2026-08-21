#ifndef GRAPHICS_NULL_ARRAY_BUFFER_CLASS_HEADER
#define GRAPHICS_NULL_ARRAY_BUFFER_CLASS_HEADER

#include <Engine/Graphics/GL/ArrayBuffer.hpp>
#include <Engine/Graphics/PrimitiveType.hpp>

namespace eng::gfx::gl::Null
{
	class ArrayBuffer : public gl::ArrayBuffer
	{
	public:
        ArrayBuffer() = default;
		~ArrayBuffer() = default;

		void loadData(const void* data, unsigned int size, gl::ArrayBuffer::Usage usage) override;
		void configureAttrib(unsigned int index, unsigned int components, bool normalized, unsigned int stride, unsigned int offset) override;
        void draw(PrimitiveType p_type, unsigned int first, unsigned int count) override;

	protected:
        unsigned int m_VAO;
        unsigned int m_VBO;
	};
}

#endif //GRAPHICS_NULL_ARRAY_BUFFER_CLASS_HEADER