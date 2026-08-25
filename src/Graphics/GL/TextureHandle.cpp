#include <Engine/Graphics/GL/TextureHandle.hpp>


namespace eng::gfx::gl
{

TextureHandle::TextureHandle():
	m_id(0),
	m_generation(0) {}


TextureHandle::TextureHandle(unsigned int id, unsigned int generation):
	m_id(id),
	m_generation(generation) {}


unsigned int TextureHandle::getId() const
{
	return m_id;
}

unsigned int TextureHandle::getGeneration() const
{
	return m_generation;
}

} //namespace eng::gfx::gl