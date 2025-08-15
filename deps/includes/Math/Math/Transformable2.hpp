#ifndef TRANSFORMABLE_2_CLASS_HEADER
#define TRANSFORMABLE_2_CLASS_HEADER

#include <Math/Transform2.hpp>
#include <Math/Quaternion.hpp>
#include <Math/Vec2.hpp>

namespace mth
{
	class Transformable2
	{
	public:
		Transformable2();
		Transformable2(Transformable2* parent);

		void move(const Vec2& offset);
		void scale(const Vec2& scale_v);
		void rotate(float angle); //radians

		void setOrigin(const Vec2& new_origin);
		void setPosition(const Vec2& new_position);
		void setScale(const Vec2& new_scale);
		void setRotation(float new_angle); //radians
		void setParent(Transformable2& parent);

		Vec2 getPosition();

		Transformable2* getParent();
		Transform2 getLocalTransform();
		Transform2 getGlobalTransform();
		Transform2 getInvertLocalTransform();

	protected:
		Vec2 m_origin;
		Vec2 m_position;
		Vec2 m_scale;
		float m_rotation;
		Transform2 m_transform;
		Transform2 m_invert_transform;
		bool m_transform_need_update;
		bool m_invert_transform_need_update;
		Transformable2* m_parent;

		void computeTransform();
	};
}

#endif //TRANSFORMABLE_2_CLASS_HEADER