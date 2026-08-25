#ifndef GRAPHICS_GL_TEXTURE_HANDLE_CLASS_HEADER
#define GRAPHICS_GL_TEXTURE_HANDLE_CLASS_HEADER

namespace eng::gfx::gl
{
	class TexturePool;

	class TextureHandle
	{
	public:
		TextureHandle();

	protected:
		unsigned int m_id;
		unsigned int m_generation;
		friend TexturePool;

		TextureHandle(unsigned int id, unsigned int generation);

		unsigned int getId() const;
		unsigned int getGeneration() const;
	};
}

#endif //GRAPHICS_GL_TEXTURE_HANDLE_CLASS_HEADER