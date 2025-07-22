#include <Graphics/RenderStates.hpp>


gfx::RenderStates::RenderStates():
	m_transform(nullptr),
	m_shader(nullptr) {}

gfx::RenderStates::RenderStates(mth::Transform3& transform, Shader& shader):
	m_transform(&transform),
	m_shader(&shader) {}