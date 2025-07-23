#include <Graphics/Window.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Graphics/VertexBuffer.hpp>
#include <Graphics/TextureManager.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/View.hpp>
#include <Math/Transform3.hpp>


gfx::Window::Window() : Window(800, 600, "Window"){}

gfx::Window::Window(int width, int height, std::string title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	window_ptr = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window_ptr == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		destroy();
		return;
	}
	glfwMakeContextCurrent(window_ptr);
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
}

gfx::Window::~Window()
{
	destroy();
}


GLFWwindow* gfx::Window::getHandler()
{
	return window_ptr;
}


void gfx::Window::setViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

bool gfx::Window::isOpen()
{
	return !glfwWindowShouldClose(window_ptr);
}


void gfx::Window::clear(const gfx::Color& color)
{
	glClearColor(color.r/float(COLOR_MAX_VALUE), color.g/float(COLOR_MAX_VALUE), color.b/float(COLOR_MAX_VALUE), color.a/float(COLOR_MAX_VALUE));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gfx::Window::display()
{
	glfwSwapBuffers(window_ptr);
}

void gfx::Window::destroy()
{
	glfwTerminate();
}


void gfx::Window::draw(Drawable& drawable, RenderStates& states)
{
	drawable.draw(this, states);
}

void gfx::Window::draw(VertexBuffer& vertex_buffer, RenderStates& states)
{
	static constexpr int modes[] =
        {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN};
    const GLenum mode = modes[int(vertex_buffer.getPrimitiveType())];

    bool use_texture = states.m_texture != 0;

	states.m_shader->use();
	states.m_shader->setUniform1i("use_texture", use_texture);
	if (use_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, states.m_texture);
		states.m_shader->setUniformMatrix4fv("u_texture", 0);
	}
	states.m_shader->setUniformMatrix4fv("projection", states.m_view.getProjectionMatrix().getValuesPtr());
	states.m_shader->setUniformMatrix4fv("view", states.m_view.getGlobalTransform().getMatrix().getValuesPtr());
	states.m_shader->setUniformMatrix4fv("model", states.m_transform.getMatrix().getValuesPtr());

	VertexBuffer::bind(&vertex_buffer);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	if (vertex_buffer.getEBOHandle() == 0)
		glDrawArrays(mode, 0, vertex_buffer.getSize());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	VertexBuffer::bind(nullptr);
}