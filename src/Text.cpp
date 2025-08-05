#include <Graphics/Text.hpp>

#include <Graphics/Shader.hpp>
#include <Graphics/FontManager.hpp>
#include <Graphics/RenderStates.hpp>
#include <Graphics/LightManager.hpp>
#include <Graphics/TextureManager.hpp>
#include <Math/Transformable3.hpp>
#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>
#include <Math/Vec4.hpp>
#include <Math/Mat4.hpp>
#include <glad/glad.h>
#include <string>


gfx::Text::Text() : font_id(0), str(""), mth::Transformable3()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 8, NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, (GLvoid*)(sizeof(GLfloat)*3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, (GLvoid*)(sizeof(GLfloat)*5));
	glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

gfx::Text::~Text()
{
	if (m_VAO)
		glDeleteVertexArrays(1, &m_VAO);
	if (m_VBO)
		glDeleteBuffers(1, &m_VBO);
}


void gfx::Text::setString(std::string str)
{
	this->str = str;
}

void gfx::Text::setFontId(FontId font_id)
{
	this->font_id = font_id;
}


void gfx::Text::draw(Window* window, RenderStates& states)
{
	if (font_id == 0) return;

	View* active_view = gfx::View::getActive();
	Shader* active_shader = gfx::Shader::getActive();
	mth::Vec3 view_loc_pos = active_view->getPosition();
	mth::Mat4 view_transform_mat = active_view->getGlobalTransform().getMatrix();
	mth::Vec4 view_glob_pos = view_transform_mat*mth::Vec4(view_loc_pos.x, view_loc_pos.y, view_loc_pos.z, 1);

	active_shader->use();
	active_shader->setUniformMatrix4fv("uProjection", active_view->getProjectionMatrix().getValuesPtr());
	active_shader->setUniformMatrix4fv("uView", active_view->getViewMatrix().getValuesPtr());
	active_shader->setUniformMatrix4fv("uModel", getGlobalTransform().getMatrix().getValuesPtr());

	active_shader->setUniform3fv("uViewPos", 1, &view_glob_pos.x);

	active_shader->setUniform1i("uLightsCount", LightManager::getLightsCount());
	if (LightManager::getLightsCount())
	{
		glActiveTexture(GL_TEXTURE2);
		TextureManager::bind(LightManager::getLightsTexture());
		active_shader->setUniform1i("uLightsTexture", 2);
	}

	glBindVertexArray(m_VAO);
	GLfloat x = 0;
	GLfloat y = 0;
	std::string::const_iterator c;
    for (c = str.begin(); c != str.end(); c++) 
    {
        Font::Character ch = FontManager::getCharacter(font_id, *c);

        GLfloat w = ch.width;
        GLfloat h = ch.height;
        GLfloat xpos = x + ch.bearingX;
        GLfloat ypos = y - (h - ch.bearingY);

        GLfloat vertices[6][8] = {
            { xpos,     ypos + h, 0,   0.0, 0.0,  0.0, 0.0, 1.0 },            
            { xpos,     ypos,     0,   0.0, 1.0,  0.0, 0.0, 1.0 },
            { xpos + w, ypos,     0,   1.0, 1.0,  0.0, 0.0, 1.0 },

            { xpos,     ypos + h, 0,   0.0, 0.0,  0.0, 0.0, 1.0 },
            { xpos + w, ypos,     0,   1.0, 1.0,  0.0, 0.0, 1.0 },
            { xpos + w, ypos + h, 0,   1.0, 0.0,  0.0, 0.0, 1.0 }           
        };
        // Render glyph texture over quad
        glActiveTexture(GL_TEXTURE0);
        TextureManager::bind(ch.id);
        active_shader->setUniform1i("uMaterial.diffuse", 0);
        glActiveTexture(GL_TEXTURE1);
        TextureManager::bind(ch.id);
        active_shader->setUniform1i("uMaterial.specular", 1);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
}