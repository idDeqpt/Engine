#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Graphics/Color.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/Shape.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/Drawable.hpp>
#include <Graphics/Vertex.hpp>
#include <Graphics/RenderStates.hpp>
#include <Graphics/PrimitiveType.hpp>
#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>
#include <Math/Transform3.hpp>
#include <Math/Transformable3.hpp>
#include <cmath>

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

	gfx::Vertex vertices_obj[] = {
		{mth::Vec3(0.5, 0.5, 0), gfx::Color(255, 0, 0), mth::Vec2(1, 0)},
		{mth::Vec3(0.5, -0.5, 0), gfx::Color(0, 255, 0), mth::Vec2(1, 1)},
		{mth::Vec3(-0.5, -0.5, 0), gfx::Color(0, 0, 255), mth::Vec2(0, 1)},
		{mth::Vec3(-0.5, 0.5, 0), gfx::Color(255, 255, 0), mth::Vec2(0, 0)},
		{mth::Vec3(0, 1, 0), gfx::Color(255, 255, 255), mth::Vec2(0.5, 0)},
	};

	gfx::Texture tex1("C:/Projects/C++/libraries/Engine/Graphics/tests/image1.png");
	gfx::Texture tex2("C:/Projects/C++/libraries/Engine/Graphics/tests/image2.png");

	std::cout << "Tex1 error: " << tex1.getLastError() << std::endl;
	std::cout << "Tex2 error: " << tex2.getLastError() << std::endl;

	mth::Transformable3 tr;
	tr.setPosition(mth::Vec3(0, 0, 0));
	//tr.setOrigin(mth::Vec3(0, -0.5, 0));
	//tr.setScale(mth::Vec3(0.5));
	//tr.setRotation(mth::Vec3(0, 0, 1), 3.14*0.25);

	gfx::VertexBuffer vb;
	std::cout << "VertexBuffer create: " << vb.create() << std::endl;
	std::cout << "VertexBuffer update: " << vb.update(vertices_obj, 5) << std::endl;
	vb.setPrimitiveType(gfx::PrimitiveType::TRIANGLE_FAN);

	gfx::Shape shape;
	shape.setColor(gfx::Color(200, 50, 100));
	shape.addPoint(mth::Vec3(0), mth::Vec2(0));
	shape.addPoint(mth::Vec3(0, 1, 0), mth::Vec2(0, 1));
	shape.addPoint(mth::Vec3(1, 1, 0), mth::Vec2(1, 1));
	shape.addPoint(mth::Vec3(1, 0, 0), mth::Vec2(1, 0));
	shape.update();
	shape.setTexture(tex1);

	gfx::RenderStates states;
	states.m_transform = tr.getGlobalTransform();
	states.m_shader = &shader;
	//states.m_texture = &tex1;
	states.m_view.setOrtho(-10, 10, 10, -10, -30, 30);
	//states.m_view.setPosition(mth::Vec3(10, 10, 0));
	//states.m_view.setScale(mth::Vec3(2, 2, 1));
	//states.m_view.setOrigin(mth::Vec3(5, 5, 0));

	print(tr.getGlobalTransform().getMatrix());
	print(states.m_view.getGlobalTransform().getMatrix());
	print(states.m_view.getProjectionMatrix());

	print(states.m_view.getGlobalTransform().getMatrix()*tr.getGlobalTransform().getMatrix());
	print(states.m_view.getProjectionMatrix()*states.m_view.getGlobalTransform().getMatrix()*tr.getGlobalTransform().getMatrix());

	while(window.isOpen())
	{
	    glfwPollEvents();

	    float time = (GLfloat)glfwGetTime();
	    //tr.setRotation(mth::Vec3(1, 0, 0), time);
	    //states.m_transform = tr.getGlobalTransform();
		states.m_view.setRotation(mth::Vec3(0, 0, 1), time);

	    window.clear(gfx::Color(125));

	    //window.draw(vb, states);
	    window.draw(shape, states);

	    window.display();
	}

	window.destroy();
	system("pause");
	return 0;
}