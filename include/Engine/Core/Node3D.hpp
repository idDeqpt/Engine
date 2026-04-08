#ifndef NODE_3D_CLASS_HEADER
#define NODE_3D_CLASS_HEADER

#include <Engine/Core/Node.hpp>
#include <Engine/Math/Transform3.hpp>
#include <Engine/Math/Quaternion.hpp>
#include <Engine/Math/Vec3.hpp>

namespace eng::core
{
	class Node3D : public Node
	{
	public:
		Node3D();

		void move(const mth::Vec3& offset);
		void relativeMove(const mth::Vec3& offset);
		void scale(const mth::Vec3& scale_v);
		void rotate(const mth::Quaternion& quat);

		void setOrigin(const mth::Vec3& new_origin);
		void setPosition(const mth::Vec3& new_position);
		void setScale(const mth::Vec3& new_scale);
		void setRotation(const mth::Quaternion& quat);

		mth::Vec3 getPosition();
		mth::Quaternion getRotation();

		mth::Transform3 getLocalTransform();
		mth::Transform3 getInvertLocalTransform();
		std::optional<mth::Transform3> getGlobalTransform3D();

	protected:
		mth::Vec3 m_origin;
		mth::Vec3 m_position;
		mth::Vec3 m_scale;
		mth::Quaternion m_rotation;
		mth::Transform3 m_transform;
		mth::Transform3 m_invert_transform;
		bool m_transform_need_update;
		bool m_invert_transform_need_update;

		virtual void computeTransform();
	};
}

#endif //NODE_3D_CLASS_HEADER