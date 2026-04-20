#include <Engine/Graphics/RenderManager.hpp>

#include <glad/glad.h>


namespace eng
{

gfx::RenderManager::RenderManager()
{
	gladLoadGL();
}

} // namespace eng