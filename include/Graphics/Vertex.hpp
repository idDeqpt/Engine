#ifndef VERTEX_STRUCT_HEADER
#define VERTEX_STRUCT_HEADER

#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>
#include "Color.hpp"


namespace gfx
{
	struct Vertex
	{
		mth::Vec3 position;
		Color color;
		mth::Vec2 tex_coords;
	};
}

#endif //VERTEX_STRUCT_HEADER