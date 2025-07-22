#ifndef VERTEX_BUFFER_CLASS_HEADER
#define VERTEX_BUFFER_CLASS_HEADER

#include "Vertex.hpp"
#include "PrimitiveType.hpp"


namespace gfx
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		bool create(unsigned int vertex_count);
		bool update(Vertex* vertices);

		void setPrimitiveType(const PrimitiveType& type);

		unsigned int getVAOHandle();
		unsigned int getVBOHandle();
		unsigned int getEBOHandle();
		unsigned int getSize();
		PrimitiveType getPrimitiveType();

		static void bind(VertexBuffer* vertex_buffer);

	protected:
		unsigned int m_vao_id;
		unsigned int m_vbo_id;
		unsigned int m_ebo_id;
		bool using_ebo;
		unsigned int m_size;
		PrimitiveType m_prim_type;
	};
}

#endif //VERTEX_BUFFER_CLASS_HEADER