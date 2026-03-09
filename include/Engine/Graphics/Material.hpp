#ifndef MATERIAL_CLASS_HEADER
#define MATERIAL_CLASS_HEADER

#include <Engine/Graphics/Texture.hpp>

namespace gfx
{
	struct Material
	{
		Texture* albedo;
		Texture* normal;
		Texture* metallic;
		Texture* roughness;
		Texture* height;
		Texture* ao;
		Texture* emission;

		Material();
		Material(Texture* albedo, Texture* normal, Texture* metallic, Texture* roughness, Texture* height, Texture* ao, Texture* emission);
	};
}

#endif MATERIAL_CLASS_HEADER