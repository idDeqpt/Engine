#include <Graphics/LightManager.hpp>

#include <Math/Vec3.hpp>

#include <vector>
#include <cmath>
#include <iostream>


std::vector<gfx::LightManager::DirectionalLight> gfx::LightManager::m_lights;
gfx::Texture* gfx::LightManager::m_lights_texture;
bool gfx::LightManager::m_need_update;
gfx::LightId gfx::LightManager::m_id_counter;


void gfx::LightManager::initialize()
{
	m_lights.clear();
	m_need_update = true;
	m_id_counter = 1;
	if (m_lights_texture != nullptr) delete m_lights_texture;
	m_lights_texture = new Texture();
}

void gfx::LightManager::finalize()
{
	if (m_lights_texture != nullptr) delete m_lights_texture;
}


gfx::LightId gfx::LightManager::addLight()
{
	m_lights.push_back({m_id_counter, mth::Vec3(), mth::Vec3()});
	m_need_update = true;
	return m_id_counter++;
}

void gfx::LightManager::setDirection(LightId id, const mth::Vec3& direction)
{
	for (unsigned int i = 0; i < m_lights.size(); i++)
		if (m_lights[i].id == id)
		{
			m_lights[i].direction = direction;
			m_need_update = true;
			return;
		}
}

void gfx::LightManager::setColor(LightId id, const mth::Vec3& color)
{
	for (unsigned int i = 0; i < m_lights.size(); i++)
		if (m_lights[i].id == id)
		{
			m_lights[i].color = color;
			m_need_update = true;
			return;
		}
}


unsigned int gfx::LightManager::getLightsCount()
{
	return m_lights.size();
}

gfx::Texture* gfx::LightManager::getLightsTexture()
{
	updateTexture();
	return m_lights_texture;
}


void gfx::LightManager::updateTexture()
{
	if (m_need_update)
	{
		if (m_lights.size())
		{
			std::vector<float> pixels(m_lights.size()*2*3);
			for (unsigned int i = 0; i < m_lights.size(); i++)
			{
				pixels[i    ] = m_lights[i].direction.x;
				pixels[i + 1] = m_lights[i].direction.y;
				pixels[i + 2] = m_lights[i].direction.z;
				pixels[i + 3] = m_lights[i].color.x;
				pixels[i + 4] = m_lights[i].color.y;
				pixels[i + 5] = m_lights[i].color.z;
			}

			for (unsigned int i = 0; i < pixels.size(); i++)
				std::cout << pixels[i] << " ";
			std::cout << std::endl;

			m_lights_texture->create();
			m_lights_texture->loadFromBuffer((float*)pixels.data(), m_lights.size()*2, 1, Texture::PixelFormat::RGB32F);
		}

		m_need_update = false;
	}
}