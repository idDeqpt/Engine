#ifndef VIEW_CLASS_HEADER
#define VIEW_CLASS_HEADER

#include <Engine/Core/Node3D.hpp>
#include <Engine/Math/Vec3.hpp>
#include <Engine/Math/Mat4.hpp>

namespace eng::gfx
{
	class View : public core::Node3D
	{
	public:
		View();
		View(View& view);

		void setOrtho(float left, float right, float bottom, float top, float near, float far);
		void setPerspective(float fovy, float aspect_ratio, float near, float far);

		mth::Mat4 getProjectionMatrix();
		mth::Mat4 getViewMatrix();

		static void setActive2d(View* target);
		static View* getActive2d();
		
		static void setActive3d(View* target);
		static View* getActive3d();

	protected:
		mth::Mat4 m_projection;
		static View* m_active2d;
		static View* m_active3d;
	};
}

#endif //VIEW_CLASS_HEADER