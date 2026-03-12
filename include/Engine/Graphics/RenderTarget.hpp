#ifndef RENDER_TARGET_CLASS_HEADER
#define RENDER_TARGET_CLASS_HEADER

#include <Engine/Math/Vec2.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/RenderStates.hpp>
#include <Engine/Graphics/Drawable.hpp>


namespace eng::gfx
{
	class RenderTarget
	{
	public:
		RenderTarget();
		RenderTarget(unsigned int buffers_count, Texture::PixelFormat* formats);
		~RenderTarget();

		void setViewport(int x, int y, int width, int height);

		void bind();
		void clear(const Color& color);
		void draw(Drawable& drawable, RenderStates& states);

		Texture& getTexture(unsigned int index);

	protected:
		mth::Vec2 m_viewport_pos;
		mth::Vec2 m_viewport_size;
		unsigned int m_native_handle;
		unsigned int m_depth_buffer_handle;
		unsigned int m_color_buffers_count;
		Texture* m_buffer_textures;

	};
}

#endif //RENDER_TARGET_CLASS_HEADER