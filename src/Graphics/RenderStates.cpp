#include <Engine/Graphics/RenderStates.hpp>

#include <Engine/Math/Transform3.hpp>


gfx::RenderStates::RenderStates():
	m_transform(mth::Transform3::getIdentity()),
	m_shader(nullptr) {}