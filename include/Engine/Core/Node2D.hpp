#ifndef NODE_2D_CLASS_HEADER
#define NODE_2D_CLASS_HEADER

#include <Engine/Core/Node.hpp>
#include <Engine/Math/Transform2.hpp>
#include <Engine/Math/Vec2.hpp>

namespace eng::core
{
	class Node2D : public Node
	{
	public:
		Node2D();

		void move(const mth::Vec2& offset);
		void scale(const mth::Vec2& scale_v);
		void rotate(float angle); //radians

		void setOrigin(const mth::Vec2& new_origin);
		void setPosition(const mth::Vec2& new_position);
		void setScale(const mth::Vec2& new_scale);
		void setRotation(float new_angle); //radians

		mth::Vec2 getOrigin();
		mth::Vec2 getPosition();
		mth::Vec2 getScale();

		mth::Vec2 getGlobalPosition();
		float getGlobalRotation();

		mth::Transform2 getLocalTransform();
		mth::Transform2 getInvertLocalTransform();
		std::optional<mth::Transform2> getGlobalTransform2D() override;

	protected:
		mth::Vec2 m_origin;
		mth::Vec2 m_position;
		mth::Vec2 m_scale;
		float m_rotation;
		mth::Transform2 m_transform;
		mth::Transform2 m_invert_transform;
		bool m_transform_need_update;
		bool m_invert_transform_need_update;

		virtual void computeTransform();
	};
}

#endif //NODE_2D_CLASS_HEADER