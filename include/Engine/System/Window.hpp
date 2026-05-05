#ifndef WINDOW_CLASS_HEADER
#define WINDOW_CLASS_HEADER

#include <Engine/Graphics/RenderTarget.hpp>
#include <Engine/Math/Vec2.hpp>

#include <string>


struct GLFWwindow;

namespace eng::sys
{
	class Window : public gfx::RenderTarget
	{
	public:
		enum ViewportScaling
		{
			FIXED,
			STRETCH,
		};

		Window();
		Window(int width, int height, std::string title);
		~Window();

		GLFWwindow* getHandler();
		bool isOpen();
		void close();

		void resize(const mth::Vec2& new_size);
		void setViewportCentering(const mth::Vec2& ratio);
		void setViewportScaling(ViewportScaling mode);

		void display();
		void destroy();

	protected:
		GLFWwindow* window_ptr;
		mth::Vec2 m_size;
		mth::Vec2 m_centering_ratio;
		ViewportScaling m_scaling_mode;
	};
}

#endif //WINDOW_CLASS_HEADER