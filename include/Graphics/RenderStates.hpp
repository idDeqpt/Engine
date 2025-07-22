#ifndef RENDER_STATES_CLASS_HEADER
#define RENDER_STATES_CLASS_HEADER

#include <Graphics/View.hpp>
#include <Math/Transform3.hpp>


namespace gfx
{
	class Shader;

	struct RenderStates
	{
		RenderStates();
		RenderStates(View& view, mth::Transform3& transform, Shader& shader);

		View m_view;
		mth::Transform3 m_transform;
		Shader* m_shader;
	};
}

#endif //RENDER_STATES_CLASS_HEADER