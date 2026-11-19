#include <Engine/Graphics/RenderStates.hpp>

#include <Engine/Math/Transform3.hpp>
#include <Engine/Math/Mat.hpp>


namespace eng
{

gfx::RenderStates::RenderStates():
	m_transform(mth::Mat4::getIdentity()),
	m_shader(nullptr) {}

} //namespace eng