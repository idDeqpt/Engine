#ifndef DRAWABLE_CLASS_HEADER
#define DRAWABLE_CLASS_HEADER

namespace eng::gfx
{
	class RenderTarget;
	class RenderStates;

	class Drawable
	{
	public:
		Drawable() {m_visible = true;}

		void setVisible(bool flag) {m_visible = flag;}
		bool isVisible() {return m_visible;}

		virtual void draw(RenderTarget* target, RenderStates& states) = 0;

	protected:
		bool m_visible;
	};
}

#endif //DRAWABLE_CLASS_HEADER