#ifndef LIGHT_MANAGER_STATIC_CLASS_HEADER
#define LIGHT_MANAGER_STATIC_CLASS_HEADER

#include <Engine/Graphics/Texture.hpp>
#include <Engine/Math/Vec3.hpp>
#include <vector>

namespace gfx
{
	typedef unsigned int LightId;

	class LightManager
	{
	public:
		struct DirectionalLight
		{
			LightId id;
			mth::Vec3 direction;
			mth::Vec3 color;
		};
		
		static void initialize();
		static void finalize();

		static LightId addLight();
		static void setDirection(LightId id, const mth::Vec3& direction);
		static void setColor(LightId id, const mth::Vec3& color);

		static unsigned int getLightsCount();
		static Texture* getLightsTexture();

	protected:
		static std::vector<DirectionalLight> m_lights;
		static Texture* m_lights_texture;
		static bool m_need_update;
		static LightId m_id_counter;

		static void updateTexture();
	};
}

#endif //LIGHT_MANAGER_STATIC_CLASS_HEADER