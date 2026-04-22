#ifndef DEFAULT_2D_FRAGMENT_SHADER_STRING_HPP
#define DEFAULT_2D_FRAGMENT_SHADER_STRING_HPP

namespace eng::core::shader
{
	static const char* default2d_frag =R"(
#version 330 core

in vec2 fTexCoord;

out vec4 oColor;

uniform bool uUseTexture;
uniform bool uTextureFlipX;
uniform bool uTextureFlipY;
uniform sampler2D uTexture;
uniform vec4 uColor;

void main()
{
    if (uUseTexture)
    {
        vec2 tex_coords = vec2(
            uTextureFlipX ? (1.0f - fTexCoord.x) : fTexCoord.x,
            uTextureFlipY ? (1.0f - fTexCoord.y) : fTexCoord.y
        );
        oColor = texture(uTexture, tex_coords)*uColor;
    }
    else
        oColor = uColor;
}
)";
}

#endif //DEFAULT_2D_FRAGMENT_SHADER_STRING_HPP