#ifndef CAMERA_3D_CLASS_HEADER
#define CAMERA_3D_CLASS_HEADER

#include <Engine/Core/Node3D.hpp>
#include <Engine/Math/Vec3.hpp>
#include <Engine/Math/Mat4.hpp>

namespace eng::gfx
{
	class Camera3D : public core::Node3D
	{
	public:
		Camera3D();

		void setOrtho(float left, float right, float bottom, float top, float near, float far);
		void setPerspective(float fovy, float aspect_ratio, float near, float far);

		mth::Mat4 getProjectionMatrix();
		mth::Mat4 getViewMatrix();
		mth::Mat4 getProjViewMatrix();

		void setActive();

		static Camera3D& getActive();

	protected:
		mth::Mat4 m_projection;
		static Camera3D* m_active;
	};
}

#endif //CAMERA_3D_CLASS_HEADER