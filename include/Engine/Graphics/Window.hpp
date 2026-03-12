#ifndef WINDOW_CLASS_HEADER
#define WINDOW_CLASS_HEADER

#include <string>

#include <Engine/Graphics/RenderTarget.hpp>


struct GLFWwindow;

namespace eng::gfx
{
	class Window : public RenderTarget
	{
	public:
		Window();
		Window(int width, int height, std::string title);
		~Window();

		GLFWwindow* getHandler();
		bool isOpen();
		void close();

		void display();
		void destroy();

	protected:
		GLFWwindow* window_ptr;
	};
}

#endif //WINDOW_CLASS_HEADER