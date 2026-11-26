#ifndef WINDOW_CLASS_HEADER
#define WINDOW_CLASS_HEADER

#include <Engine/Graphics/RenderTarget.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Math/Vec.hpp>

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

		static void initialize();
		static void finalize();

		Window();
		Window(int width, int height, const std::string& title);

		void setTitle(const std::string& new_title);
		GLFWwindow* getHandler();
		bool isOpen() const;
		void close();

		void resize(mth::Vec2 new_size);
		void setViewportSize(mth::Vec2 new_size);
		void setViewportCentering(mth::Vec2 ratio);
		void setViewportScaling(ViewportScaling mode);
		void updateViewport();

		void clear(const gfx::Color& color) override;

		void display();

		static void* (*getProcAddressLoader())(const char*);

	protected:
		GLFWwindow* window_ptr;
		mth::Vec2 m_size;
		mth::Vec2 m_window_viewport_size;
		mth::Vec2 m_centering_ratio;
		ViewportScaling m_scaling_mode;
	};
}

#endif //WINDOW_CLASS_HEADER