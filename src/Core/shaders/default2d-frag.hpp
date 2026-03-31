#ifndef DEFAULT_2D_FRAGMENT_SHADER_STRING_HPP
#define DEFAULT_2D_FRAGMENT_SHADER_STRING_HPP

namespace eng::core::shader
{
	static const char* default2d_frag =R"(
#version 330 core

in vec2 fTexCoord;

out vec4 oColor;

uniform bool uUseTexture;
uniform sampler2D uTexture;
uniform vec4 uColor;

void main()
{
    if (uUseTexture)
        oColor = texture(uTexture, fTexCoord)*uColor;
    else
        oColor = uColor;
}
)";
}

#endif //DEFAULT_2D_FRAGMENT_SHADER_STRING_HPP