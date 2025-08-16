#ifndef LIGHT_MANAGER_STATIC_CLASS_HEADER
#define LIGHT_MANAGER_STATIC_CLASS_HEADER

#include <Engine/Math/Vec3.hpp>

namespace gfx
{
	typedef unsigned int LightId;

	class LightManager
	{
	public:
		struct DirectionalLight
		{
			mth::Vec3 direction;
			mth::Vec3 color;
		};
		
		static void initialize();
		static void finalize();

		static void enableDirectionalLight(DirectionalLight light);
		static void disableDirectionalLight();

		static DirectionalLight getDirectionalLight();

	protected:
		static DirectionalLight s_directional_light;
	};
}

#endif //LIGHT_MANAGER_STATIC_CLASS_HEADER