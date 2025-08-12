#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Graphics/Color.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Text.hpp>
#include <Graphics/Text2D.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/FontManager.hpp>
#include <Graphics/LightManager.hpp>
#include <Graphics/EventManager.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/GeometricMesh.hpp>
#include <Graphics/Vertex.hpp>
#include <Graphics/RenderStates.hpp>
#include <Graphics/PrimitiveType.hpp>
#include <Graphics/CanvasItem.hpp>
#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>
#include <Math/Vec4.hpp>
#include <Math/Quaternion.hpp>
#include <Math/Transform3.hpp>
#include <Math/Transformable3.hpp>
#include <cmath>
#include <vector>
#include <random>

void print(const mth::Mat4& mat)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
			std::cout << mat[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void print(const mth::Mat3& mat)
{
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
			std::cout << mat[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


void print(const mth::Quaternion& q)
{
	std::cout << q.w << " ";
	std::cout << q.x << " ";
	std::cout << q.y << " ";
	std::cout << q.z << " ";
	std::cout << std::endl;
}

void print(const mth::Vec4& vec)
{
	std::cout << vec.x << " ";
	std::cout << vec.y << " ";
	std::cout << vec.z << " ";
	std::cout << vec.w << " ";
	std::cout << std::endl;
}

void print(const mth::Vec3& vec)
{
	std::cout << vec.x << " ";
	std::cout << vec.y << " ";
	std::cout << vec.z << " ";
	std::cout << std::endl;
}

mth::Vec4 mult(const mth::Mat4& mat, const mth::Vec4& vec)
{
	mth::Vec4 result;
	for (int i = 0; i < 4; i++)
	{
		result.x += mat[i][0]*vec.x;
		result.y += mat[i][1]*vec.y;
		result.z += mat[i][2]*vec.z;
		result.w += mat[i][3]*vec.w;
	}
	return result;
}


int main()
{
	unsigned int width = 1600;
	unsigned int height = 900;
	srand(0);
	gfx::Window window(width, height, "LearnOpenGL");

	gfx::Shader shader3d;
	shader3d.loadFromFile("C:/Projects/C++/libraries/Engine/Graphics/include/Graphics/shaders/default3d.vert",
						"C:/Projects/C++/libraries/Engine/Graphics/include/Graphics/shaders/default3d.frag");
	std::cout << "Shader error: " << shader3d.getLastError() << std::endl;
	if (shader3d.getLastError() != gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader error log: " << shader3d.getLastErrorLog() << std::endl;
	gfx::Shader shader2d;
	shader2d.loadFromFile("C:/Projects/C++/libraries/Engine/Graphics/include/Graphics/shaders/default2d.vert",
						"C:/Projects/C++/libraries/Engine/Graphics/include/Graphics/shaders/default2d.frag");
	std::cout << "Shader error: " << shader2d.getLastError() << std::endl;
	if (shader2d.getLastError() != gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader error log: " << shader2d.getLastErrorLog() << std::endl;

	gfx::FontManager::initialize();
	gfx::LightManager::initialize();
	gfx::EventManager::initialize(window.getHandler());

	gfx::Texture tex[4] = {gfx::Texture(), gfx::Texture(), gfx::Texture(), gfx::Texture()};
	std::cout << "TEXTURE\n";
	std::cout << "Tex1: " << tex[0].loadFromFile("C:/Projects/C++/libraries/Engine/Graphics/tests/image1.png") << std::endl;
	std::cout << "Tex2: " << tex[1].loadFromFile("C:/Projects/C++/libraries/Engine/Graphics/tests/image2.png") << std::endl;
	std::cout << "Tex3: " << tex[2].loadFromFile("C:/Projects/C++/libraries/Engine/Graphics/tests/box.png") << std::endl;
	std::cout << "Tex4: " << tex[3].loadFromFile("C:/Projects/C++/libraries/Engine/Graphics/tests/box-map.png") << std::endl;
	std::cout << "Err: " << glGetError() << std::endl;

	gfx::FontId fontid = gfx::FontManager::loadFromFile("C:/Projects/C++/libraries/Engine/Graphics/tests/GameFont.ttf", 24);
	std::cout << "Font: " << fontid << std::endl;

	const unsigned int floor_size = 20;
	const unsigned int floor_accuracy = 20;
	const unsigned int floor_points_count = floor_accuracy*floor_accuracy;
	const unsigned int floor_normals_count = (floor_accuracy - 1)*(floor_accuracy - 1)*2;
	const unsigned int floor_indexes_count = floor_normals_count*3;

	mth::Vec3 floor_points    [floor_points_count];
	mth::Vec2 floor_tex_coords[floor_points_count];
	mth::Vec3 floor_normals   [floor_normals_count];
	unsigned int floor_indexes[floor_indexes_count];
	unsigned int floor_normal_indexes[floor_indexes_count];

	unsigned int index = 0;
	for (unsigned int i = 0; i < floor_accuracy; i++)
		for (unsigned int j = 0; j < floor_accuracy; j++)
		{
			floor_points[index] = mth::Vec3(floor_size*float(i)/(floor_accuracy-1), (rand()%10)*0.05, floor_size*float(j)/(floor_accuracy-1));
			floor_tex_coords[index++] = mth::Vec2(float(i)/(floor_accuracy-1), float(j)/(floor_accuracy-1));
		}
	index = 0;
	for (unsigned int i = 0; i < (floor_accuracy - 1); i++)
		for (unsigned int j = 0; j < (floor_accuracy - 1); j++)
		{
			unsigned int first = i*floor_accuracy + j;
			unsigned int second = first + floor_accuracy;

			floor_normals[index/3] = -(floor_points[second] - floor_points[first]).cross(floor_points[first + 1] - floor_points[first]).norm();
			floor_normal_indexes[index] = index/3;
			floor_indexes[index++] = first;
			floor_normal_indexes[index] = index/3 + 1;
			floor_indexes[index++] = second;
			floor_normal_indexes[index] = index/3 + 2;
			floor_indexes[index++] = first + 1;

			floor_normals[index/3] = -(floor_points[second] - floor_points[first + 1]).cross(floor_points[second + 1] - floor_points[second]).norm();
			floor_normal_indexes[index] = index/3;
			floor_indexes[index++] = second;
			floor_normal_indexes[index] = index/3 + 1;
			floor_indexes[index++] = second + 1;
			floor_normal_indexes[index] = index/3 + 2;
			floor_indexes[index++] = first + 1;
		}

	gfx::Mesh floor;
	std::cout << "SUCCESS: " << floor.loadData({floor_points, floor_points_count, floor_indexes,
												floor_tex_coords, floor_points_count, floor_indexes,
												floor_normals, floor_normals_count, floor_indexes, floor_indexes_count}) << std::endl;
	floor.setMaterial({
		&tex[2],
		&tex[3],
		128*0.4
	});

	gfx::GeometricMesh obj(gfx::GeometricMesh::Type::PARALLELEPIPED);
	//gfx::GeometricMesh obj(gfx::GeometricMesh::Type::ELLIPSOID);
	obj.setMaterial({
		&tex[2],
		&tex[3],
		128*0.4
	});

	gfx::RenderStates states;
	gfx::View view3d;
	//view3d.setOrtho(0, 10, 10, 0, -30, 30);
	view3d.setPerspective(3.14*0.25, float(width)/height, 1, 100);
	view3d.setPosition(mth::Vec3(0, 1, 2));
	gfx::View view2d;
	view2d.setOrtho(0, width, height, 0, -10, 10);

	std::vector<mth::Vec3> positions = {
		{0.0f,  0.0f,  0.0f}, 
		{2.0f,  5.0f, -15.0f}, 
		{-1.5f, -2.2f, -2.5f}, 
		{-3.8f, -2.0f, -12.3f}, 
		{2.4f, -0.4f, -3.5f}, 
		{-1.7f,  3.0f, -7.5f}, 
		{1.3f, -2.0f, -2.5f}, 
		{1.5f,  2.0f, -2.5f}, 
		{1.5f,  0.2f, -1.5f}, 
		{-1.3f,  1.0f, -1.5f}
	};

	gfx::LightId light_id = gfx::LightManager::addLight();
	gfx::LightManager::setDirection(light_id, mth::Vec3(1, -1, 0));
	gfx::LightManager::setColor(light_id, mth::Vec3(1, 1, 1));

	/*gfx::Text text;
	text.setFontId(fontid);
	text.setString("fontid");
	text.setScale(mth::Vec3(0.02));*/

	gfx::CanvasItem ci;
	//ci.setColor(gfx::Color(255, 0, 0, 255));
	gfx::CanvasItem::Vertex verts[4] = {
		{{0, 0}, {0, 0}},
		{{1000, 0}, {1, 0}},
		{{1000, 300}, {1, 1}},
		{{0, 300}, {0, 1}}
	};
	std::cout << "CAN: " << ci.loadData(verts, 4) << std::endl;
	//ci.setTexture(tex[2]);
	ci.setTexture(*gfx::FontManager::getTexture(fontid));
	ci.setPosition(mth::Vec2(0, 300));

	gfx::Text2D info_texts[2];
	for (unsigned int i = 0; i < 2; i++)
	{
		info_texts[i].setFont(fontid);
		info_texts[i].setPosition(mth::Vec2(0, gfx::FontManager::getSize(fontid)*i));
	}

	float speed = 0.1;
	mth::Vec2 rot_angles;
	float delta_time = 0;
	while(window.isOpen())
	{
		float start_time = glfwGetTime();
		mth::Vec3 vel;
		gfx::EventManager::pull();
		if (gfx::EventManager::isPressed(GLFW_KEY_ESCAPE)) window.close();
		if (gfx::EventManager::isJustPressed(GLFW_KEY_L)) gfx::EventManager::setCursorLock(!gfx::EventManager::getCursorLock());
		if (gfx::EventManager::isPressed(GLFW_KEY_W))
			vel.z -= speed;
		if (gfx::EventManager::isPressed(GLFW_KEY_S))
			vel.z += speed;
		if (gfx::EventManager::isPressed(GLFW_KEY_A))
			vel.x -= speed;
		if (gfx::EventManager::isPressed(GLFW_KEY_D))
			vel.x += speed;
		if (gfx::EventManager::isPressed(GLFW_KEY_LEFT_CONTROL))
			vel.y -= speed;
		if (gfx::EventManager::isPressed(GLFW_KEY_SPACE))
			vel.y += speed;

		if (gfx::EventManager::Mouse::moved())
			rot_angles = rot_angles + gfx::EventManager::Mouse::getDelta();
		
		view3d.setRotation(mth::Quaternion(mth::Vec3(0, 1, 0), 0));
		view3d.rotate(mth::Quaternion(mth::Vec3(0, 1, 0), -rot_angles.x*0.01));
		view3d.rotate(mth::Quaternion(mth::Vec3(1, 0, 0), -rot_angles.y*0.01));

		if (vel.x || vel.y || vel.z)
			view3d.relativeMove(vel);

		float time = (GLfloat)glfwGetTime();
		obj.setRotation(mth::Quaternion(mth::Vec3(0.5, 1, 0), time*0.5));

		info_texts[0].setString("Frame time: " + std::to_string(delta_time) + "s");
		info_texts[1].setString("Position: " + std::to_string(view3d.getPosition().x) + " " + std::to_string(view3d.getPosition().y) + " " + std::to_string(view3d.getPosition().z));

		window.clear(gfx::Color(125));

		gfx::View::setActive(&view3d);
		gfx::Shader::setActive(&shader3d);
		for (unsigned int i = 0; i < positions.size(); i++)
		{
			obj.setPosition(positions[i]);
			window.draw(obj, states);
		}
		window.draw(floor, states);
		//window.draw(text, states);

		glClear(GL_DEPTH_BUFFER_BIT);
		gfx::View::setActive(&view2d);
		gfx::Shader::setActive(&shader2d);

		window.draw(ci, states);
		for (unsigned int i = 0; i < 2; i++)
		{
			//info_texts[i].setColor(gfx::Color(255, (sin(start_time) + 1)*0.5*255, 255, 255));
			window.draw(info_texts[i], states);
		}

		window.display();

		delta_time = glfwGetTime() - start_time;
	}

	gfx::FontManager::finalize();

	window.destroy();
	system("pause");
	return 0;
}