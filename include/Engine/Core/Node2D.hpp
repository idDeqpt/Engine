#ifndef NODE_2D_CLASS_HEADER
#define NODE_2D_CLASS_HEADER

#include <Engine/Core/Node.hpp>
#include <Engine/Math/Transform2.hpp>
#include <Engine/Math/Vec.hpp>

namespace eng::core
{
	class Node2D : public Node
	{
	public:
		Node2D() = default;

		virtual void onTreeChanged() override;

		void move(const mth::Vec2& offset);
		void scale(const mth::Vec2& scale_v);
		void rotate(float angle); //radians

		void setOrigin(const mth::Vec2& new_origin);
		void setPosition(const mth::Vec2& new_position);
		void setScale(const mth::Vec2& new_scale);
		void setRotation(float new_angle); //radians

		const mth::Vec2& getOrigin() const;
		const mth::Vec2& getPosition() const;
		const mth::Vec2& getScale() const;
		float getRotation() const;

		mth::Vec2 getGlobalPosition();
		float getGlobalRotation();

		const mth::Transform2& getLocalTransform();
		const mth::Transform2& getGlobalTransform();

	protected:
		virtual void computeTransform();
		void invalidateTransform();

	private:
		mth::Vec2 m_origin;
		mth::Vec2 m_position;
		mth::Vec2 m_scale = mth::Vec2(1);
		float m_rotation = 0;
		mth::Transform2 m_local_transform;
		bool m_local_transform_need_update = true;

		bool m_global_valid = false;
		Node2D* m_nearest_2d = nullptr;
		unsigned int m_subtree_revision = 1;
		unsigned int m_cached_against_revision = 0;
		mth::Transform2 m_global_transform;
	};
}

#endif //NODE_2D_CLASS_HEADER