#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Graphics/Color.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Texture.hpp>
#include <Math/Transform3.hpp>
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

	gfx::Window window(800, 600, "LearnOpenGL");

	window.setViewport(0, 0, 800, 600);

	gfx::Shader shader;
	shader.loadFromBuffer(
		"#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 color;\n"
		"layout (location = 2) in vec2 texCoord;\n"
		"out vec3 vertexColor;\n"
		"out vec2 vertexTexCoord;\n"
		"uniform mat4 transform;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transform*vec4(position, 1.0f);\n"
		"    vertexColor = color;\n"
		"    vertexTexCoord = texCoord;\n"
		"}",

		"#version 330 core\n"
		"in vec3 vertexColor;\n"
		"in vec2 vertexTexCoord;\n"
		"out vec4 color;\n"
		"uniform sampler2D ourTexture1;\n"
		"uniform sampler2D ourTexture2;\n"
		"void main()\n"
		"{\n"
		"	color = mix(texture(ourTexture1, vertexTexCoord), texture(ourTexture2, vertexTexCoord), 0.5);\n"
		"}");
	std::cout << "Shader error: " << shader.getLastError() << std::endl;
	if (shader.getLastError() != gfx::Shader::Error::NO_ERROR)
		std::cout << "Shader error log: " << shader.getLastErrorLog() << std::endl;

	GLfloat texCoords[] = {
	    0.0f, 0.0f,  // Нижний левый угол 
	    1.0f, 0.0f,  // Нижний правый угол
	    0.5f, 1.0f   // Верхняя центральная сторона
	};

	GLfloat vertices[] = {
	    // Позиции          // Цвета             // Текстурные координаты
	     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // Верхний правый
	     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // Нижний правый
	    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // Нижний левый
	    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // Верхний левый
	};

	GLuint indexes[] = {  // Помните, что мы начинаем с 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
	};

	gfx::Texture tex1("C:/Projects/C++/libraries/Engine/Graphics/tests/image1.png");
	gfx::Texture tex2("C:/Projects/C++/libraries/Engine/Graphics/tests/image2.png");

	std::cout << "Tex1 error: " << tex1.getLastError() << std::endl;
	std::cout << "Tex2 error: " << tex2.getLastError() << std::endl;

	GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);  

	glBindVertexArray(0);

	while(window.isOpen())
	{
	    glfwPollEvents();

	    float time = (GLfloat)glfwGetTime();
	    mth::Transform3 trans;
		trans.translate(mth::Vec3(cos(time)*0.4, sin(time)*0.4, 0));
		trans.rotate(mth::Vec3(0, 0, 1), time);
	    trans.scale(mth::Vec3(0.7 + cos(time*4)*0.2, 0.7 - cos(time*4)*0.2, 1));

	    window.clear(gfx::Color(125));

		shader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex1.getId());
		shader.setUniform1i("ourTexture1", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex2.getId());
		shader.setUniform1i("ourTexture2", 1);
		shader.setUniformMatrix4fv("transform", trans.getMatrix().getValuesPtr());

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	    window.display();
	}

	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

	window.destroy();
	system("pause");
	return 0;
}