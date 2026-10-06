#ifndef ROOT_CLASS_HEADER
#define ROOT_CLASS_HEADER

#include <Engine/Core/Node.hpp>
#include <Engine/Graphics/3D/GeometricMesh.hpp>
#include <Engine/Graphics/3D/RenderWorld.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Math/Vec3.hpp>
#include <Engine/Math/Mat4.hpp>
#include <Engine/Context.hpp>

#include "Box.hpp"
#include "Camera.hpp"

class Root : public eng::core::Node
{
protected:
	Camera* m_camera = nullptr;
	Box* m_box = nullptr;

public:
	void onBuild() override
	{
		m_camera = addChild<Camera>("camera");
		m_box = addChild<Box>("box", eng::gfx::GeometricMesh::Type::PARALLELEPIPED);
	}

	void onSetup() override
	{
		m_context.get<eng::gfx::RenderWorld>().setActiveCamera(*m_camera);

		m_box->setSize(0.5);
		m_context.get<eng::gfx::RenderWorld>().addObject(*m_box);
		std::vector<eng::mth::Mat4> translations;
		for (int i = 0; i < 5; i++)
					translations.push_back(eng::mth::Mat4(1, 0, 0, i,
					                                      0, 1, 0, 0,
					                                      0, 0, 1, 0,
					                                      0, 0, 0, 1));
		for (int j = 0; j < 5; j++)
					translations.push_back(eng::mth::Mat4(1, 0, 0, 0,
					                                      0, 1, 0, j,
					                                      0, 0, 1, 0,
					                                      0, 0, 0, 1));
		for (int k = 0; k < 5; k++)
					translations.push_back(eng::mth::Mat4(1, 0, 0, 0,
					                                      0, 1, 0, 0,
					                                      0, 0, 1, k,
					                                      0, 0, 0, 1));
		m_box->loadInstances(translations.data(), translations.size());
	}
};

#endif //ROOT_CLASS_HEADER