#include <Engine/Graphics/Material.hpp>

#include <Engine/Graphics/Texture.hpp>


gfx::Material::Material() : diffuse(nullptr), specular(nullptr), normal(nullptr), shininess(1) {}

gfx::Material::Material(Texture* diffuse, Texture* specular, Texture* normal, float shininess):
	diffuse(diffuse), specular(specular), normal(normal), shininess(shininess) {}