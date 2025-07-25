#ifndef WINDOW_CLASS_HEADER
#define WINDOW_CLASS_HEADER

#include <string>

#include "Color.hpp"
#include "VertexBuffer.hpp"
#include "Shader.hpp"
#include "RenderStates.hpp"
#include "Drawable.hpp"


struct GLFWwindow;

namespace gfx
{
	class Window
	{
	public:
		Window();
		Window(int width, int height, std::string title);
		~Window();

		GLFWwindow* getHandler();

		void setViewport(int x, int y, int width, int height);
		bool isOpen();
		void close();

		void clear(const Color& color);
		void display();
		void destroy();

		void draw(Drawable& drawable, RenderStates& states);
		void draw(VertexBuffer& vertex_buffer, RenderStates& states);

	protected:
		GLFWwindow* window_ptr;
	};
}

#endif //WINDOW_CLASS_HEADER