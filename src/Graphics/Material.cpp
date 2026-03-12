#include <Engine/Graphics/Material.hpp>

#include <Engine/Graphics/Texture.hpp>


namespace eng
{

gfx::Material::Material():
	albedo(nullptr),
	normal(nullptr),
	metallic(nullptr),
	roughness(nullptr),
	height(nullptr),
	ao(nullptr),
	emission(nullptr) {}

gfx::Material::Material(Texture* albedo, Texture* normal, Texture* metallic, Texture* roughness, Texture* height, Texture* ao, Texture* emission):
	albedo(albedo),
	normal(normal),
	metallic(metallic),
	roughness(roughness),
	height(height),
	ao(ao),
	emission(emission) {}

} //namespace eng