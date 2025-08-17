#ifndef MATERIAL_CLASS_HEADER
#define MATERIAL_CLASS_HEADER

#include <Engine/Graphics/Texture.hpp>

namespace gfx
{
	struct Material
	{
		Texture* diffuse;
		Texture* specular;
		Texture* normal;
		float    shininess;

		Material();
		Material(Texture* diffuse, Texture* specular, Texture* normal, float shininess);
	};
}

#endif MATERIAL_CLASS_HEADER