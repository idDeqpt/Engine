#ifndef VIEW_CLASS_HEADER
#define VIEW_CLASS_HEADER

#include <Math/Vec3.hpp>
#include <Math/Mat4.hpp>
#include <Math/Transformable3.hpp>

namespace gfx
{
	class View : public mth::Transformable3
	{
	public:
		View();
		View(View& view);

		void move(const mth::Vec3& offset);

		void setOrtho(float left, float right, float bottom, float top, float near, float far);
		void setPerspective(float fovy, float aspect_ratio, float near, float far);

		void setPosition(const mth::Vec3& new_position);

		mth::Mat4 getProjectionMatrix();

	protected:
		mth::Mat4 m_projection;


	};
}

#endif //VIEW_CLASS_HEADER