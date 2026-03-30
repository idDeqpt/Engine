#include <iostream>

#include <Engine/Core/Engine.hpp>
#include <Engine/Core/Node.hpp>
#include <Engine/Core/ResourceManager.hpp>
#include <Engine/System/EventManager.hpp>
#include <Engine/System/Keyboard.hpp>
#include <Engine/System/Mouse.hpp>
#include <Engine/Graphics/2D/Camera2D.hpp>
#include <Engine/Graphics/2D/Shape2D.hpp>
#include <Engine/Graphics/2D/Text2D.hpp>
#include <Engine/Graphics/3D/Camera3D.hpp>
#include <Engine/Graphics/3D/Mesh.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/RenderManager.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Math/Vec3.hpp>
#include <Engine/Math/Quaternion.hpp>

#include <GLFW/glfw3.h>


class UM : public eng::gfx::Mesh
{
public:
	void onSetup()
	{
		eng::gfx::RenderManager::getMainScene()->addObject3d(*this);

		std::string resources_dir = "E:/Programming/Projects/C++/Engine/tests/resources";
		eng::gfx::Texture* tex[2];
		std::vector<std::string>images = {
			"/bricks.jpg",
			"/bricks-normal.jpg",
		};

		std::cout << "TEXTURES\n";
		for (unsigned int i = 0; i < images.size(); i++)
		{
			auto load_result = eng::core::ResourceManager::load<eng::gfx::Texture>({resources_dir + images[i]});
			tex[i] = load_result.second;
			std::cout << "Tex" << i << ": " << tex[i]->getLastError() << std::endl;
		}

		const unsigned int floor_size     = 50;
		const unsigned int floor_accuracy = 30;
		const unsigned int floor_points_count  = floor_accuracy*floor_accuracy;
		const unsigned int floor_normals_count = (floor_accuracy - 1)*(floor_accuracy - 1)*2;
		const unsigned int floor_indexes_count = floor_normals_count*3;

		eng::mth::Vec3 floor_points    [floor_points_count];
		eng::mth::Vec2 floor_tex_coords[floor_points_count];
		eng::mth::Vec3 floor_normals   [floor_normals_count];
		eng::mth::Vec3 floor_tangents  [floor_normals_count];
		unsigned int floor_indexes[floor_indexes_count];
		unsigned int floor_normal_indexes[floor_indexes_count];

		unsigned int index = 0;
		for (unsigned int i = 0; i < floor_accuracy; i++)
			for (unsigned int j = 0; j < floor_accuracy; j++)
			{
				floor_points[index]       = eng::mth::Vec3(floor_size*float(i)/(floor_accuracy-1), (rand()%10)*0.0,           floor_size*float(j)/(floor_accuracy-1));
				floor_tex_coords[index++] = eng::mth::Vec2(float(i)/(floor_accuracy-1),            float(j)/(floor_accuracy-1));
			}
		index = 0;
		for (unsigned int i = 0; i < (floor_accuracy - 1); i++)
			for (unsigned int j = 0; j < (floor_accuracy - 1); j++)
			{
				unsigned int first  = i*floor_accuracy + j;
				unsigned int second = first + floor_accuracy;

				floor_normals[index/3]  = -(floor_points[second] - floor_points[first]).cross(floor_points[first + 1] - floor_points[first]).norm();
				floor_tangents[index/3] = eng::mth::Vec3(1, 0, 0);
				floor_normal_indexes[index] = index/3;
				floor_indexes[index++] = first;
				floor_normal_indexes[index] = index/3 + 1;
				floor_indexes[index++] = second;
				floor_normal_indexes[index] = index/3 + 2;
				floor_indexes[index++] = first + 1;

				floor_normals[index/3]  = -(floor_points[second] - floor_points[first + 1]).cross(floor_points[second + 1] - floor_points[second]).norm();
				floor_tangents[index/3] = eng::mth::Vec3(1, 0, 0);
				floor_normal_indexes[index] = index/3;
				floor_indexes[index++] = second;
				floor_normal_indexes[index] = index/3 + 1;
				floor_indexes[index++] = second + 1;
				floor_normal_indexes[index] = index/3 + 2;
				floor_indexes[index++] = first + 1;
			}

		std::cout << "SUCCESS: " << this->loadData({floor_points,     floor_points_count,  floor_indexes,
													floor_tex_coords, floor_points_count,  floor_indexes,
													floor_normals,    floor_normals_count, floor_indexes,
													floor_tangents,   floor_normals_count, floor_indexes, floor_indexes_count}) << std::endl;
		this->setMaterial({
			tex[0],
			tex[1],
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		});
	}
};

class UT : public eng::gfx::Shape2D
{
public:
	void onSetup()
	{
		eng::gfx::RenderManager::getMainScene()->addObject(*this);
		this->setPosition(eng::mth::Vec2(700, 400));
		this->setSize(eng::mth::Vec2(100, 100));
		auto t = eng::core::ResourceManager::load<eng::gfx::Texture>({"E:/Programming/Projects/C++/Engine/tests/resources/bricks.jpg"});
		this->setTexture(*t.second);
	}
};


class Camera : public eng::gfx::Camera3D
{
public:
	void onSetup()
	{
		this->setPerspective(3.14*0.25, float(900)/600, 1, 100);
		this->setPosition(eng::mth::Vec3(25, 20, 50));
		this->setRotation(eng::mth::Quaternion(eng::mth::Vec3(1, 0, 0), -0.8));
		this->setActive();
	}

	void onUpdate(float delta)
	{
		eng::mth::Vec3 vel;
		constexpr float speed = 30;
		if (eng::sys::EventManager::getKeyboard().isPressed(eng::sys::Keyboard::Key::W))
			vel.z -= speed;
		if (eng::sys::EventManager::getKeyboard().isPressed(eng::sys::Keyboard::Key::S))
			vel.z += speed;
		if (eng::sys::EventManager::getKeyboard().isPressed(eng::sys::Keyboard::Key::A))
			vel.x -= speed;
		if (eng::sys::EventManager::getKeyboard().isPressed(eng::sys::Keyboard::Key::D))
			vel.x += speed;
		if (eng::sys::EventManager::getKeyboard().isPressed(eng::sys::Keyboard::Key::LEFT_CONTROL))
			vel.y -= speed;
		if (eng::sys::EventManager::getKeyboard().isPressed(eng::sys::Keyboard::Key::SPACE))
			vel.y += speed;
		
		if (eng::sys::EventManager::getKeyboard().isPressed(eng::sys::Keyboard::Key::L))
			eng::sys::EventManager::setCursorMode(eng::sys::Mouse::CursorMode::DISABLE);
		if (eng::sys::EventManager::getKeyboard().isPressed(eng::sys::Keyboard::Key::K))
			eng::sys::EventManager::setCursorMode(eng::sys::Mouse::CursorMode::NORMAL);

		if (eng::sys::EventManager::getMouse().moved())
			m_rot_angles = m_rot_angles + eng::sys::EventManager::getMouse().getDelta();

		this->setRotation(eng::mth::Quaternion(eng::mth::Vec3(0, 1, 0), 0));
		this->rotate(eng::mth::Quaternion(eng::mth::Vec3(0, 1, 0), -m_rot_angles.x*0.01));
		this->rotate(eng::mth::Quaternion(eng::mth::Vec3(1, 0, 0), -m_rot_angles.y*0.01));

		if (vel.x || vel.y || vel.z)
			this->relativeMove(vel*delta);
	}

protected:
	eng::mth::Vec2 m_rot_angles;
};


class UText : public eng::gfx::Text2D
{
public:
	void onSetup()
	{
		eng::gfx::RenderManager::getMainScene()->addObject(*this);
		this->setFont(*eng::core::ResourceManager::load<eng::gfx::Font>({"E:/Programming/Projects/C++/Engine/tests/resources/GameFont.ttf"}).second);
	}

	void onUpdate(float delta)
	{
		this->setString(std::to_string(delta));
	}
};


class Root : public eng::core::Node
{
public:
	void onSetup()
	{
		addChild<Camera>("Camera3d");

		auto camera2d = addChild<eng::gfx::Camera2D>("Camera2d");
		camera2d->setSize(eng::mth::Vec2(900, 600));
		camera2d->setActive();

		addChild<UT>("shape");
		addChild<UM>("floor");
		addChild<UText>("text");
	}
};


int main()
{
	Root r;
	eng::core::Engine engine(r);
	engine.setup();
	engine.mainLoop();

	system("pause");
	return 0;
}