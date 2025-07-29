#ifndef MATERIAL_CLASS_HEADER
#define MATERIAL_CLASS_HEADER

#include <Graphics/TextureManager.hpp>

namespace gfx
{
	struct Material
	{
		TextureId diffuse;
		TextureId specular;
		float     shininess;

		Material();
		Material(TextureId diffuse, TextureId specular, float shininess);
	};
}

#endif MATERIAL_CLASS_HEADER