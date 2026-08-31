#ifndef GRAPHICS_GL_ARRAY_BUFFER_CLASS_HEADER
#define GRAPHICS_GL_ARRAY_BUFFER_CLASS_HEADER

#include <Engine/Graphics/PrimitiveType.hpp>

namespace eng
{
namespace mth
{
    class Mat4;
} //namespace mth

namespace gfx::gl
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
		virtual void loadIndexes(const void* data, unsigned int count, Usage usage) = 0;
		virtual void loadInstances(mth::Mat4* transforms, unsigned int count) = 0;

		virtual void configureAttrib(unsigned int index, unsigned int components, bool normalized, unsigned int stride, unsigned int offset) = 0;
		virtual void configureInstanceAttrib(unsigned int index, unsigned int divisor, unsigned int components, bool normalized, unsigned int stride, unsigned int offset) = 0;

        virtual void draw(PrimitiveType p_type, unsigned int first, unsigned int count) = 0;
	};
} //namespace gfx::gl
} //namespace eng

#endif //GRAPHICS_GL_ARRAY_BUFFER_CLASS_HEADER