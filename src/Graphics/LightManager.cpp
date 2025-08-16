#include <Engine/Graphics/LightManager.hpp>

#include <Engine/Math/Vec3.hpp>

#include <cmath>
#include <iostream>


gfx::LightManager::DirectionalLight gfx::LightManager::s_directional_light;


void gfx::LightManager::initialize()
{
	disableDirectionalLight();
}

void gfx::LightManager::finalize() {}


void gfx::LightManager::enableDirectionalLight(DirectionalLight light)
{
	s_directional_light = light;
}

void gfx::LightManager::disableDirectionalLight()
{
	s_directional_light = {{0, 0, 0}, {0, 0, 0}};
}

gfx::LightManager::DirectionalLight gfx::LightManager::getDirectionalLight()
{
	return s_directional_light;
}