#include <Engine/Graphics/Material.hpp>

#include <Engine/Graphics/Texture.hpp>


gfx::Material::Material() : diffuse(nullptr), specular(nullptr), shininess(1) {}

gfx::Material::Material(Texture* diffuse, Texture* specular, float shininess):
	diffuse(diffuse), specular(specular), shininess(shininess) {}