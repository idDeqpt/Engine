#ifndef BOX_CLASS_HEADER
#define BOX_CLASS_HEADER

#include <Engine/Graphics/3D/GeometricMesh.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>
#include <Engine/Graphics/Material.hpp>
#include <Engine/Graphics/Texture.hpp>

class Box : public eng::gfx::GeometricMesh
{
public:
	Box(eng::gfx::GeometricMesh::Type t) : eng::gfx::GeometricMesh(t) {}

	void onSetup() override
	{
		unsigned char white[] = {255, 255, 255, 255};
		m_tex.create(eng::gfx::gl::PixelFormat::RGBA);
		m_tex.loadFromBuffer(white, 1, 1);
		eng::gfx::Material mat;
		mat.albedo = &m_tex;
		setMaterial(mat);
	}
protected:
	eng::gfx::Texture m_tex;
};

#endif //BOX_CLASS_HEADER