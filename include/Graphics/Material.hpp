#ifndef MATERIAL_CLASS_HEADER
#define MATERIAL_CLASS_HEADER

#include <Graphics/Texture.hpp>

namespace gfx
{
	struct Material
	{
		Texture* diffuse;
		Texture* specular;
		float     shininess;

		Material();
		Material(Texture* diffuse, Texture* specular, float shininess);
	};
}

#endif MATERIAL_CLASS_HEADER