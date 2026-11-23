#ifndef RENDER_STATES_STRUCT_HEADER
#define RENDER_STATES_STRUCT_HEADER

namespace eng::gfx
{
	class Shader;

	struct RenderStates
	{
		RenderStates() = default;

		Shader* shader = nullptr;
	};
}

#endif //RENDER_STATES_STRUCT_HEADER