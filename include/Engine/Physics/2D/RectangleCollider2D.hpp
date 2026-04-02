#ifndef RECTANGLE_COLLIDER_2D_CLASS_HEADER
#define RECTANGLE_COLLIDER_2D_CLASS_HEADER

#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Math/Vec2.hpp>

#include <vector>


namespace eng::phy
{

	class RectangleCollider2D : public Collider2D
	{
	public:
		RectangleCollider2D();

		void setSize(const mth::Vec2& size);

		Type getType() override;
		mth::Vec2 getSize();

	protected:
		mth::Vec2 m_size;
	};
}

#endif //RECTANGLE_COLLIDER_2D_CLASS_HEADER