#ifndef RENDER_STATES_CLASS_HEADER
#define RENDER_STATES_CLASS_HEADER

#include <Graphics/View.hpp>
#include <Graphics/TextureManager.hpp>
#include <Math/Transform3.hpp>


namespace gfx
{
	class Shader;

	struct RenderStates
	{
		RenderStates();

		mth::Transform3 m_transform;
		TextureId m_texture;
		Shader* m_shader;
	};
}

#endif //RENDER_STATES_CLASS_HEADER