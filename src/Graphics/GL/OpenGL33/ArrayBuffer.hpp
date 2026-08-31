#ifndef GRAPHICS_OPEN_GL_3_3_ARRAY_BUFFER_CLASS_HEADER
#define GRAPHICS_OPEN_GL_3_3_ARRAY_BUFFER_CLASS_HEADER

#include <Engine/Graphics/GL/ArrayBuffer.hpp>
#include <Engine/Graphics/PrimitiveType.hpp>

namespace eng
{
namespace mth
{
    class Mat4;
} //namespace mth

namespace gfx::gl::OpenGL33
{
	class ArrayBuffer : public gl::ArrayBuffer
	{
	public:
        ArrayBuffer();
		~ArrayBuffer() override;

		void loadData(const void* data, unsigned int size, gl::ArrayBuffer::Usage usage) override;
		void loadIndexes(const void* data, unsigned int count, Usage usage) override;
		void loadInstances(mth::Mat4* transforms, unsigned int count) override;

		void configureAttrib(unsigned int index, unsigned int components, bool normalized, unsigned int stride, unsigned int offset) override;
		void configureInstanceAttrib(unsigned int index, unsigned int divisor, unsigned int components, bool normalized, unsigned int stride, unsigned int offset) override;

        void draw(PrimitiveType p_type, unsigned int first, unsigned int count) override;

	protected:
        unsigned int m_VAO;
        unsigned int m_VBO;
        unsigned int m_EBO;
        unsigned int m_instance_VBO;
        unsigned int m_indexes_count;
        unsigned int m_instances_count;
	};
} //namespace gfx::gl::OpenGL33
} //namespace eng

#endif //GRAPHICS_OPEN_GL_3_3_ARRAY_BUFFER_CLASS_HEADER