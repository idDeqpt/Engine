#include <Graphics/Material.hpp>

#include <Math/Vec3.hpp>


gfx::Material::Material() : ambient(1), diffuse(1), specular(1), shininess(1) {}

gfx::Material::Material(mth::Vec3 ambient, mth::Vec3 diffuse, mth::Vec3 specular, float shininess):
	ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}