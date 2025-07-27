#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Graphics/Color.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/TextureManager.hpp>
#include <Graphics/EventManager.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/GeometricMesh.hpp>
#include <Graphics/Vertex.hpp>
#include <Graphics/RenderStates.hpp>
#include <Graphics/PrimitiveType.hpp>
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
	srand(0);
	gfx::Window window(800, 800, "LearnOpenGL");

	window.setViewport(0, 0, 800, 800);

	gfx::Shader shader(
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPosition;\n"
		"layout (location = 1) in vec4 aColor;\n"
		"layout (location = 2) in vec2 aTexCoord;\n"
		"layout (location = 3) in vec3 aNormal;\n"
		"out vec4 fColor;\n"
		"out vec2 fTexCoord;\n"
		"out vec3 fViewPos;\n"
		"out vec3 fNormal;\n"
		"uniform mat4 uProjection;\n"
		"uniform mat4 uView;\n"
		"uniform mat4 uModel;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = uProjection*uView*uModel*vec4(aPosition, 1.0f);\n"
		"    fColor = aColor;\n"
		"    fTexCoord = aTexCoord;\n"
		"    fViewPos = vec3(uView*uModel*vec4(aPosition, 1.0f));\n"
		"    vec3 worldNormal = transpose(inverse(mat3(uModel)))*aNormal;\n"
		"    vec3 viewNormal = mat3(uView)*worldNormal;\n"
		"    fNormal = normalize(viewNormal);\n"
		"}",

		"#version 330 core\n"
		"in vec4 fColor;\n"
		"in vec2 fTexCoord;\n"
		"in vec3 fViewPos;\n"
		"in vec3 fNormal;\n"
		"out vec4 oColor;\n"
		"uniform bool uUseTexture;\n"
		"uniform sampler2D uTexture;\n"
		"void main()\n"
		"{\n"
		"	if (uUseTexture)\n"
		"		oColor = texture(uTexture, fTexCoord);\n"
		"	else\n"
		"		oColor = fColor;\n"
		"	float coef = max(dot(fNormal, normalize(-fViewPos)), 0.0f);\n"
		"	oColor = vec4(oColor.rgb*coef, oColor.a);\n"
		"}");
	std::cout << "Shader error: " << shader.getLastError() << std::endl;
	if (shader.getLastError() != gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader error log: " << shader.getLastErrorLog() << std::endl;

	gfx::TextureManager::initialize();
	gfx::EventManager::initialize(window.getHandler());

	gfx::TextureId tex[] = {
		gfx::TextureManager::loadFromFile("C:/Projects/C++/libraries/Engine/Graphics/tests/image1.png"),
		gfx::TextureManager::loadFromFile("C:/Projects/C++/libraries/Engine/Graphics/tests/image2.png")
	};

	std::cout << "Tex1: " << tex[0] << std::endl;
	std::cout << "Tex2: " << tex[1] << std::endl;

	const unsigned int floor_size = 4;
	const unsigned int floor_accuracy = 10;
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
			floor_points[index] = mth::Vec3(floor_size*float(i)/(floor_accuracy-1), (rand()%10)*0.05, floor_size*float(j)/(floor_accuracy-1));
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

	gfx::Mesh floor;
	//floor.setScale(mth::Vec3(5));
	std::cout << "SUCCESS: " << floor.loadData({floor_points, floor_points_count, floor_indexes,
												floor_colors, floor_points_count, floor_indexes,
												floor_tex_coords, floor_points_count, floor_indexes,
												nullptr, 0, nullptr, floor_indexes_count}) << std::endl;
	floor.setTexture(tex[0]);

	gfx::GeometricMesh obj(gfx::GeometricMesh::Type::PARALLELEPIPED);
	//gfx::GeometricMesh obj(gfx::GeometricMesh::Type::ELLIPSOID);
	obj.setColor(gfx::Color(128, 64, 0));
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
	mth::Vec2 rot_angles;
	while(window.isOpen())
	{
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
		
		states.m_view.setRotation(mth::Quaternion(mth::Vec3(0, 1, 0), 0));
		states.m_view.rotate(mth::Quaternion(mth::Vec3(0, 1, 0), -rot_angles.x*0.01));
		states.m_view.rotate(mth::Quaternion(mth::Vec3(1, 0, 0), -rot_angles.y*0.01));

		if (vel.x || vel.y || vel.z)
			states.m_view.relativeMove(vel);

		if (gfx::EventManager::Mouse::moved())
		{
			std::cout << "MOVED: " << gfx::EventManager::Mouse::moved() << std::endl;
			std::cout << "MOUSE POS: " << gfx::EventManager::Mouse::getPosition().x << " " << gfx::EventManager::Mouse::getPosition().y << std::endl;
			std::cout << "MOUSE DEL: " << gfx::EventManager::Mouse::getDelta().x << " " << gfx::EventManager::Mouse::getDelta().y << std::endl;
		}

		float time = (GLfloat)glfwGetTime();
		obj.setRotation(mth::Quaternion(mth::Vec3(0.5, 0, 0), time*0.5));

		window.clear(gfx::Color(125));

		for (unsigned int i = 0; i < positions.size(); i++)
		{
			obj.setPosition(positions[i]);
			obj.setTexture(tex[i % 2]);
			window.draw(obj, states);
		}
		window.draw(floor, states);

		window.display();
	}

	gfx::TextureManager::finalize();

	window.destroy();
	system("pause");
	return 0;
}