#include <Graphics/RenderStates.hpp>

#include <Math/Transform3.hpp>


gfx::RenderStates::RenderStates():
	m_transform(mth::Transform3::getIdentity()),
	m_shader(nullptr) {}

gfx::RenderStates::RenderStates(mth::Transform3& transform, Shader& shader):
	m_transform(transform),
	m_shader(&shader) {}