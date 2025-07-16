#ifndef WINDOW_CLASS_HEADER
#define WINDOW_CLASS_HEADER

#include <string>

#include "Color.hpp"


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

		void clear(const Color& color);
		void display();
		void destroy();

	protected:
		GLFWwindow* window_ptr;
	};
}

#endif //WINDOW_CLASS_HEADER