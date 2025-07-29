#include <Graphics/Material.hpp>

#include <Graphics/TextureManager.hpp>


gfx::Material::Material() : diffuse(0), specular(0), shininess(1) {}

gfx::Material::Material(TextureId diffuse, TextureId specular, float shininess):
	diffuse(diffuse), specular(specular), shininess(shininess) {}