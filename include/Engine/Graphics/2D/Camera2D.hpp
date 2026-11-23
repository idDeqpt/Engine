#ifndef CAMERA_2D_CLASS_HEADER
#define CAMERA_2D_CLASS_HEADER

#include <Engine/Core/Node2D.hpp>
#include <Engine/Math/Vec.hpp>
#include <Engine/Math/Mat.hpp>
#include <Engine/System/Window.hpp>


namespace eng::gfx
{
	class Camera2D : public core::Node2D
	{
	public:
		Camera2D();

		void setSize(mth::Vec2 new_size);
		void setRect(const float& left, const float& right, const float& bottom, const float& top);
		void setPerspective(mth::Vec2 perspective);

		mth::Mat3 getProjectionMatrix();
		mth::Mat3 getViewMatrix();
		mth::Mat3 getProjViewMatrix();

		mth::Vec2 convertWindowPoint(mth::Vec2 point, const RenderTarget& rt);

	protected:
		mth::Vec2 m_perspective;
		mth::Mat3 m_projection;
	};
}

#endif //CAMERA_2D_CLASS_HEADER