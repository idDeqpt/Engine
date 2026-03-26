#ifndef CAMERA_2D_CLASS_HEADER
#define CAMERA_2D_CLASS_HEADER

#include <Engine/Core/Node2D.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Math/Mat3.hpp>


namespace eng::gfx
{
	class Camera2D : public core::Node2D
	{
	public:
		Camera2D();

		void setSize(mth::Vec2 new_size);
		void setRect(float left, float right, float bottom, float top);

		mth::Mat3 getProjectionMatrix();
		mth::Mat3 getViewMatrix();
		mth::Mat3 getProjViewMatrix();

		void setActive();

		static Camera2D& getActive();

	protected:
		mth::Mat3 m_projection;
		static Camera2D* m_active;
	};
}

#endif //CAMERA_2D_CLASS_HEADER