#include <Graphics/RenderStates.hpp>

#include <Graphics/View.hpp>
#include <Math/Transform3.hpp>


gfx::RenderStates::RenderStates():
	m_view(View()),
	m_transform(mth::Transform3::getIdentity()),
	m_shader(nullptr) {}

gfx::RenderStates::RenderStates(View& view, mth::Transform3& transform, Shader& shader):
	m_view(view),
	m_transform(transform),
	m_shader(&shader) {}