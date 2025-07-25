#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Graphics/Color.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/TextureManager.hpp>
#include <Graphics/EventManager.hpp>
#include <Graphics/Object.hpp>
#include <Graphics/GeometricObject.hpp>
#include <Graphics/Vertex.hpp>
#include <Graphics/RenderStates.hpp>
#include <Graphics/PrimitiveType.hpp>
#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>
#include <Math/Vec4.hpp>
#include <Math/Transform3.hpp>
#include <Math/Transformable3.hpp>
#include <cmath>
#include <vector>

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


void print(const mth::Vec4& vec)
{
	std::cout << vec.x << " ";
	std::cout << vec.y << " ";
	std::cout << vec.z << " ";
	std::cout << vec.w << " ";
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
	gfx::Window window(800, 800, "LearnOpenGL");

	window.setViewport(0, 0, 800, 800);

	gfx::Shader shader(
		"#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec4 color;\n"
		"layout (location = 2) in vec2 texCoord;\n"
		"out vec4 vertexColor;\n"
		"out vec2 vertexTexCoord;\n"
		"uniform mat4 projection;\n"
		"uniform mat4 view;\n"
		"uniform mat4 model;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = projection*view*model*vec4(position, 1.0f);\n"
		"    vertexColor = color;\n"
		"    vertexTexCoord = texCoord;\n"
		"}",

		"#version 330 core\n"
		"in vec4 vertexColor;\n"
		"in vec2 vertexTexCoord;\n"
		"out vec4 color;\n"
		"uniform bool use_texture;\n"
		"uniform sampler2D u_texture;\n"
		"void main()\n"
		"{\n"
		"	if (use_texture)\n"
		"		color = texture(u_texture, vertexTexCoord);\n"
		"	else\n"
		"		color = vertexColor;\n"
		"}");
	std::cout << "Shader error: " << shader.getLastError() << std::endl;
	if (shader.getLastError() != gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader error log: " << shader.getLastErrorLog() << std::endl;

	mth::Vec3 obj_pos[] = {
		{-0.5, -0.5, -0.5},
		{0.5, -0.5, -0.5},
		{-0.5, 0.5, -0.5},
		{0.5, 0.5, -0.5},
		{-0.5, -0.5, 0.5},
		{0.5, -0.5, 0.5},
		{-0.5, 0.5, 0.5},
		{0.5, 0.5, 0.5},
	};
	gfx::Color obj_col[] = {
		{255, 0, 0},
		{0, 255, 0},
		{0, 0, 255},
		{255, 255, 0},
		{255, 0, 0},
		{0, 255, 0},
		{0, 0, 255},
		{255, 255, 0},
	};
	mth::Vec2 obj_texc[] = {
		{0.25, 0.25},
		{0.75, 0.25},
		{0.25, 0.75},
		{0.75, 0.75},
		{0, 0},
		{1, 0},
		{0, 1},
		{1, 1},
	};

	unsigned int indexes_obj[] = {
		0, 1, 2, 1, 2, 3, //низ
		4, 5, 6, 5, 6, 7, //верх
		0, 1, 5, 0, 5, 4, //перед
		2, 3, 7, 2, 7, 6, //зад
		0, 2, 6, 0, 6, 4, //лево
		1, 3, 7, 1, 7, 5, //право
	};

	gfx::TextureManager::initialize();
	gfx::EventManager::initialize(window.getHandler());

	gfx::TextureId tex[] = {
		gfx::TextureManager::loadFromFile("C:/Projects/C++/libraries/Engine/Graphics/tests/image1.png"),
		gfx::TextureManager::loadFromFile("C:/Projects/C++/libraries/Engine/Graphics/tests/image2.png")
	};

	std::cout << "Tex1: " << tex[0] << std::endl;
	std::cout << "Tex2: " << tex[1] << std::endl;

	const unsigned int floor_size = 2;
	const unsigned int floor_accuracy = 4;
	const unsigned int floor_points_count = floor_accuracy*floor_accuracy;
	const unsigned int floor_indexes_count = (floor_accuracy - 1)*(floor_accuracy - 1)*6;
	mth::Vec3 floor_points[floor_points_count];
	gfx::Color floor_colors[floor_points_count];
	mth::Vec2 floor_tex_coords[floor_points_count];
	unsigned int floor_indexes[floor_indexes_count];
	unsigned int index = 0;
	for (unsigned int i = 0; i < floor_accuracy; i++)
		for (unsigned int j = 0; j < floor_accuracy; j++)
		{
			floor_points[index] = mth::Vec3(floor_size*float(i)/(floor_accuracy-1), 0, floor_size*float(j)/(floor_accuracy-1));
			floor_colors[index] = gfx::Color(125, 0, 125);
			floor_tex_coords[index++] = mth::Vec2(float(i)/(floor_accuracy-1), float(j)/(floor_accuracy-1));
		}
	index = 0;
	for (unsigned int i = 0; i < (floor_accuracy - 1); i++)
		for (unsigned int j = 0; j < (floor_accuracy - 1); j++)
		{
			unsigned int first = i*floor_accuracy + j;
			unsigned int second = first + floor_accuracy;

			floor_indexes[index++] = first;
			floor_indexes[index++] = second;
			floor_indexes[index++] = first + 1;

			floor_indexes[index++] = second;
			floor_indexes[index++] = second + 1;
			floor_indexes[index++] = first + 1;
		}

	gfx::Object floor;
	//floor.setScale(mth::Vec3(5));
	std::cout << "SUCCESS: " << floor.loadData({floor_points, floor_colors, floor_tex_coords, floor_indexes, floor_points_count, floor_indexes_count}) << std::endl;
	floor.setTexture(tex[0]);
/*
	gfx::Object obj;
	obj.create();
	obj.loadData({obj_pos, obj_col, obj_texc, indexes_obj, 8, 36});
	obj.setTexture(tex[0]);
	//obj.setOrigin(mth::Vec3(0.5));
	//obj.setScale(mth::Vec3(0.1));
	obj.setPosition(mth::Vec3(0, 0, -1));
	//obj.setRotation(mth::Vec3(0, 1, 0), 3.14*0.2);
*/
	gfx::GeometricObject obj(gfx::GeometricObject::Type::ELLIPSOID);
	//obj.setAccuracy();

	gfx::RenderStates states;
	states.m_shader = &shader;
	//states.m_texture = &tex1;
	//states.m_view.setOrtho(-5, 5, 5, -5, -30, 30);
	states.m_view.setPerspective(3.14*0.25, 1, 1, 100);
	states.m_view.setPosition(mth::Vec3(0, 1, 2));
	//states.m_view.setRotation(mth::Vec3(1, 0, 0), -3.14*0.25);

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

	float speed = 0.1;
	while(window.isOpen())
	{
		mth::Vec3 vel;
		gfx::EventManager::pull();
		if (gfx::EventManager::isPressed(GLFW_KEY_ESCAPE)) window.close();
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

		states.m_view.move(vel);
		float time = (GLfloat)glfwGetTime();
		obj.setRotation(mth::Vec3(0.5, 1, 0), time*0.5);
		//states.m_view.setPosition(mth::Vec3(0, 0, 5 + sin(time)));
		float near = (sin(time/2) + 1)*2;
		//states.m_view.setPerspective(3.14*0.25, 1, near, 100);
		//std::cout << near << std::endl;
		//states.m_view.setPosition(mth::Vec3(0, cos(time), 5 + sin(time)));
		//states.m_view.setRotation(mth::Vec3(1, 0, 0), sin(time));

		window.clear(gfx::Color(125));

		for (unsigned int i = 0; i < positions.size(); i++)
		{
			obj.setPosition(positions[i]);
			obj.setTexture(tex[i % 2]);
			//window.draw(obj, states);
		}
		window.draw(floor, states);

		window.display();
	}

	gfx::TextureManager::finalize();

	window.destroy();
	system("pause");
	return 0;
}