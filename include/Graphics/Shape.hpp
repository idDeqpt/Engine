#ifndef SHAPE_CLASS_HEADER
#define SHAPE_CLASS_HEADER

#include <vector>
#include <Graphics/Shader.hpp>
#include <Graphics/Drawable.hpp>
#include <Math/Vec3.hpp>


namespace gfx
{
	class Shape : public Drawable
	{
	public:
		Shape();

		void addPoint(mth::Vec3 new_point);

		void draw(Window* window) const override;

	protected:
		std::vector<mth::Vec3> m_points;
		static Shader default_shader;

		void update();
	};
}

#endif //SHAPE_CLASS_HEADER