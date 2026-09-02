#include <Engine/Graphics/GL/Api.hpp>

#include <Engine/Graphics/GL/TexturePool.hpp>

#include <memory>

#include "Null/Api.hpp"
#include "OpenGL33/Api.hpp"


namespace eng::gfx::gl
{

std::unique_ptr<Api>         Api::s_instance     = nullptr;
std::unique_ptr<TexturePool> Api::s_texture_pool = nullptr;


Api* Api::getInstance()
{
	if (!s_instance)
		createInstance(Type::AUTO);
	return s_instance.get();
}

void Api::createInstance(Type type)
{
	s_instance = createImpl(type);
}


void Api::finalize()
{
	s_texture_pool.reset();
}


TexturePool* Api::getTexturePool()
{
	if (!s_texture_pool)
		s_texture_pool = std::make_unique<TexturePool>();
	return s_texture_pool.get();
}


std::unique_ptr<Api> Api::createImpl(Type type)
{
	if (type == Type::AUTO)
	{
		#ifdef _WIN32
			#ifdef USE_OPENGL_3_3
				return std::make_unique<OpenGL33::Api>();
			#endif
		#endif

		#ifdef __linux__
			#ifdef USE_OPENGL_3_3
				return std::make_unique<OpenGL33::Api>();
			#endif
		#endif

		#ifdef USE_OPENGL_33
			return std::make_unique<OpenGL33::Api>();
		#endif
		
		return std::make_unique<Null::Api>();
	}
	
	switch (type)
	{
		#ifdef USE_OPENGL_3_3
		case Type::OPENGL_3_3:
			return std::make_unique<OpenGL33::Api>();
		#endif
		
		default:
			return std::make_unique<Null::Api>();
	}
}

} //namespace eng::gfx::gl