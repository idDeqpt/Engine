#ifndef SHAPE_CLASS_HEADER
#define SHAPE_CLASS_HEADER

#include <vector>

#include <Math/Vec3.hpp>
#include <Math/Transformable3.hpp>

#include <Graphics/Color.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Drawable.hpp>
#include <Graphics/VertexBuffer.hpp>
#include <Graphics/RenderStates.hpp>


namespace gfx
{
	class Window;

	class Shape : public Drawable, public mth::Transformable3
	{
	public:
		Shape();

		void setColor(const Color& color);
		void setTexture(TextureId texture);

		void addPoint(const mth::Vec3& vertex_pos, const mth::Vec2& vertex_tex_pos = mth::Vec2(0));
		void update();

		void draw(Window* window, RenderStates& states) override;

	protected:
		std::vector<Vertex> m_vertices;
		Color m_color;
		VertexBuffer m_vertices_buffer;
		TextureId m_texture;
	};
}

#endif //SHAPE_CLASS_HEADER