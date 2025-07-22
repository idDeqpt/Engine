#ifndef TRANSFORMABLE_3_CLASS_HEADER
#define TRANSFORMABLE_3_CLASS_HEADER

#include "Transform3.hpp"
#include "Vec3.hpp"

namespace mth
{
	class Transformable3
	{
	public:
		Transformable3();
		Transformable3(Transformable3& parent);

		void move(const Vec3& offset);
		void scale(const Vec3& scale_v);

		void setOrigin(const Vec3& new_origin);
		void setPosition(const Vec3& new_position);
		void setScale(const Vec3& new_scale);
		void setRotation(const Vec3& new_rot_vec, float new_rot_angle);
		void setParent(Transformable3& parent);

		Transformable3* getParent();
		Transform3 getLocalTransform();
		Transform3 getGlobalTransform();

	protected:
		Vec3 m_origin;
		Vec3 m_position;
		Vec3 m_scale;
		Vec3 m_rot_vec;
		float m_rot_angle;
		Transform3 m_transform;
		Transformable3* m_parent;
		bool m_transform_need_update;
	};
}

#endif //TRANSFORMABLE_3_CLASS_HEADER