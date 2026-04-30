#ifndef SPRITE_2D_CLASS_HEADER
#define SPRITE_2D_CLASS_HEADER

#include <Engine/Graphics/2D/CanvasItem.hpp>

namespace eng::gfx
{
	class Texture;

	class Sprite2D : public CanvasItem
	{
	public:
		enum class Type
		{
			RECTANGLE,
			CIRCLE,
			CUSTOM,
		};

		Sprite2D();

		void setTexture(Texture* new_texture) override;
	};
}

#endif //SPRITE_2D_CLASS_HEADER