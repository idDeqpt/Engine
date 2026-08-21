#include <Engine/Graphics/GL/Api.hpp>

#include <memory>

#include "Null/Api.hpp"
#include "OpenGL33/Api.hpp"


namespace eng::gfx::gl
{

std::unique_ptr<Api> Api::s_instance = nullptr;


Api* Api::getInstance()
{
	if (!s_instance)
		createInstance(Type::AUTO);
	return s_instance.get();
}

void Api::createInstance(Type type)
{
	s_instance = createImpl(type);
	if (s_instance) s_instance->init();
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