#ifndef RENDER_STATES_CLASS_HEADER
#define RENDER_STATES_CLASS_HEADER

#include <Math/Transform3.hpp>


namespace gfx
{
	class Shader;

	struct RenderStates
	{
		RenderStates();
		RenderStates(mth::Transform3& transform, Shader& shader);

		mth::Transform3 m_transform;
		Shader* m_shader;
	};
}

#endif //RENDER_STATES_CLASS_HEADER