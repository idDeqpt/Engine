#include <Engine/Graphics/GL/TexturePool.hpp>

#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Graphics/GL/TextureHandle.hpp>
#include <Engine/Graphics/GL/TextureImpl.hpp>

#include <algorithm>
#include <vector>
#include <memory>


namespace eng::gfx::gl
{

TextureHandle TexturePool::generateTexture()
{
	auto it = std::find_if(m_textures.begin(), m_textures.end(), [](const PoolSlot& slot) {
		return slot.texture == nullptr;
	});

	unsigned int index;
	if (it == m_textures.end())
	{
		index = m_textures.size();
		m_textures.push_back(PoolSlot{1, nullptr});
	}
	else
	{
		index = it - m_textures.begin();
		m_textures[index].generation++;
	}

	m_textures[index].texture = Api::getInstance()->createTextureImpl();
	return TextureHandle(index, m_textures[index].generation);
}

void TexturePool::releaseTexture(const TextureHandle& handle)
{
	if (checkTextureAccess(handle))
		m_textures[handle.getId()].texture = nullptr;
}


std::shared_ptr<TextureImpl> TexturePool::getTexture(const TextureHandle& handle) const
{
	return (checkTextureAccess(handle)) ? m_textures[handle.getId()].texture : nullptr;
}


bool TexturePool::checkTextureAccess(const TextureHandle& handle) const
{
	return ((handle.getId() < m_textures.size()) && (handle.getGeneration() == m_textures[handle.getId()].generation));
}

} //namespace eng::gfx::gl