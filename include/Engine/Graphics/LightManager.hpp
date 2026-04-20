#ifndef LIGHT_MANAGER_CLASS_HEADER
#define LIGHT_MANAGER_CLASS_HEADER

#include <Engine/Math/Vec3.hpp>

namespace eng::gfx
{
	class LightManager
	{
	public:
		struct DirectionalLight
		{
			mth::Vec3 direction;
			mth::Vec3 color;
		};

		void enableDirectionalLight(DirectionalLight light);
		void disableDirectionalLight();

		DirectionalLight getDirectionalLight();

	protected:
		DirectionalLight m_directional_light;
	};
}

#endif //LIGHT_MANAGER_CLASS_HEADER