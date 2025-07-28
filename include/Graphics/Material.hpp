#ifndef MATERIAL_CLASS_HEADER
#define MATERIAL_CLASS_HEADER

#include <Math/Vec3.hpp>

namespace gfx
{
	struct Material
	{
		mth::Vec3 ambient;
		mth::Vec3 diffuse;
		mth::Vec3 specular;
		float     shininess;

		Material();
		Material(mth::Vec3 ambient, mth::Vec3 diffuse, mth::Vec3 specular, float shininess);
	};
}

#endif MATERIAL_CLASS_HEADER