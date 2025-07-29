#ifndef TRANSFORMABLE_3_CLASS_HEADER
#define TRANSFORMABLE_3_CLASS_HEADER

#include <Math/Transform3.hpp>
#include <Math/Quaternion.hpp>
#include <Math/Vec3.hpp>

namespace mth
{
	class Transformable3
	{
	public:
		Transformable3();
		Transformable3(Transformable3* parent);

		void move(const Vec3& offset);
		void relativeMove(const Vec3& offset);
		void scale(const Vec3& scale_v);
		void rotate(const Quaternion& quat);

		void setOrigin(const Vec3& new_origin);
		void setPosition(const Vec3& new_position);
		void setScale(const Vec3& new_scale);
		void setRotation(const Quaternion& quat);
		void setParent(Transformable3& parent);

		Vec3 getPosition();
		Quaternion getRotation();

		Transformable3* getParent();
		Transform3 getLocalTransform();
		Transform3 getGlobalTransform();
		Transform3 getInvertLocalTransform();

	protected:
		Vec3 m_origin;
		Vec3 m_position;
		Vec3 m_scale;
		Quaternion m_rotation;
		Transform3 m_transform;
		Transform3 m_invert_transform;
		bool m_transform_need_update;
		bool m_invert_transform_need_update;
		Transformable3* m_parent;

		void computeTransform();
	};
}

#endif //TRANSFORMABLE_3_CLASS_HEADER