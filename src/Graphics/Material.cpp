#include <Engine/Graphics/Material.hpp>

#include <Engine/Graphics/Texture.hpp>


gfx::Material::Material() : diffuse(nullptr), specular(nullptr), normal(nullptr), parallax(nullptr), shininess(1) {}

gfx::Material::Material(Texture* diffuse, Texture* specular, Texture* normal, Texture* parallax, float shininess):
	diffuse(diffuse), specular(specular), normal(normal), parallax(parallax), shininess(shininess) {}