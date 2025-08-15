#ifndef SHAPE_2D_CLASS_HEADER
#define SHAPE_2D_CLASS_HEADER

#include <Engine/Graphics/CanvasItem.hpp>
#include <Engine/Math/Vec2.hpp>

namespace gfx
{
	class Shape2D : public CanvasItem
	{
	public:
		enum class Type
		{
			RECTANGLE,
			CIRCLE,
			CUSTOM,
		};

		Shape2D();
		Shape2D(Type type);

		void setType(const Type new_type);
		void setAccuracy(unsigned int accuracy);
		void setSize(const mth::Vec2& new_size);
		void setGenerator(void (*generator)(Shape2D*));

		void draw(Window* window, RenderStates& states);

	protected:
		Type         m_type;
		unsigned int m_accuracy;
		mth::Vec2    m_size;
		bool         m_need_update_points;
		void       (*m_generator)(Shape2D*) = rectangleGenerator;

		void updateShape();

		static void (*generators[])(Shape2D*);
		static void rectangleGenerator(Shape2D* shape);
		static void circleGenerator(Shape2D* shape);
	};
}

#endif //SHAPE_2D_CLASS_HEADER