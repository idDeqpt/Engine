#ifndef VIEW_CLASS_HEADER
#define VIEW_CLASS_HEADER

#include <Engine/Math/Vec3.hpp>
#include <Engine/Math/Mat4.hpp>
#include <Engine/Math/Transformable3.hpp>

namespace gfx
{
	class View : public mth::Transformable3
	{
	public:
		View();
		View(View& view);

		void setOrtho(float left, float right, float bottom, float top, float near, float far);
		void setPerspective(float fovy, float aspect_ratio, float near, float far);

		mth::Mat4 getProjectionMatrix();
		mth::Mat4 getViewMatrix();

		static void setActive(View* target);
		static View* getActive();

	protected:
		mth::Mat4 m_projection;
		static View* m_active;
	};
}

#endif //VIEW_CLASS_HEADER