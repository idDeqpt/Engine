#ifndef GRAPHICS_GL_ARRAY_BUFFER_CLASS_HEADER
#define GRAPHICS_GL_ARRAY_BUFFER_CLASS_HEADER

#include <Engine/Graphics/PrimitiveType.hpp>

namespace eng::gfx::gl
{
	class ArrayBuffer
	{
	public:
        enum class Usage
        {
            STATIC_DRAW,
            DYNAMIC_DRAW,
            STREAM_DRAW
        };

        ArrayBuffer() = default;
		virtual ~ArrayBuffer() = default;

		virtual void loadData(const void* data, unsigned int size, Usage usage) = 0;
		virtual void configureAttrib(unsigned int index, unsigned int components, bool normalized, unsigned int stride, unsigned int offset) = 0;

        virtual void draw(PrimitiveType p_type, unsigned int first, unsigned int count) = 0;
	};
}

#endif //GRAPHICS_GL_ARRAY_BUFFER_CLASS_HEADER