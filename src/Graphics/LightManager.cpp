#include <Engine/Graphics/LightManager.hpp>

#include <Engine/Math/Vec3.hpp>

#include <cmath>
#include <iostream>


namespace eng
{

void gfx::LightManager::enableDirectionalLight(DirectionalLight light)
{
	m_directional_light = light;
}

void gfx::LightManager::disableDirectionalLight()
{
	m_directional_light = {{0, 0, 0}, {0, 0, 0}};
}

gfx::LightManager::DirectionalLight gfx::LightManager::getDirectionalLight()
{
	return m_directional_light;
}

} //namespace eng