#ifndef RENDER_STATES_CLASS_HEADER
#define RENDER_STATES_CLASS_HEADER

#include <Graphics/View.hpp>
#include <Math/Transform3.hpp>


namespace gfx
{
	class Texture;
	class Shader;

	struct RenderStates
	{
		RenderStates();

		View m_view;
		mth::Transform3 m_transform;
		Texture* m_texture;
		Shader* m_shader;
	};
}

#endif //RENDER_STATES_CLASS_HEADER