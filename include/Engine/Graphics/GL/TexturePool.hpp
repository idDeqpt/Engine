#ifndef GRAPHICS_GL_TEXTURE_POOL_CLASS_HEADER
#define GRAPHICS_GL_TEXTURE_POOL_CLASS_HEADER

#include <Engine/Graphics/GL/TextureImpl.hpp>
#include <vector>
#include <memory>

namespace eng::gfx::gl
{
	class TextureHandle;

	class TexturePool
	{
	public:
		TexturePool() = default;
		~TexturePool() = default;

		TextureHandle generateTexture();
		void releaseTexture(const TextureHandle& handle);

		std::shared_ptr<TextureImpl> getTexture(const TextureHandle& handle) const;
		
	protected:
		struct PoolSlot
		{
			unsigned int generation = 1;
			std::shared_ptr<TextureImpl> texture = nullptr; 
		};

		std::vector<PoolSlot> m_textures;

		bool checkTextureAccess(const TextureHandle& handle) const;
	};
}

#endif //GRAPHICS_GL_TEXTURE_POOL_CLASS_HEADER