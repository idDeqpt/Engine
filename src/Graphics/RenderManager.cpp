#include <Engine/Graphics/RenderManager.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace eng
{

gfx::RenderManager::RenderManager()
{
	gladLoadGL(glfwGetProcAddress);
}

} // namespace eng