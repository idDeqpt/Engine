#version 330 core

in vec2 fTexCoord;
in vec4 fColor;

out vec4 oColor;

uniform bool uUseTexture;
uniform sampler2D uTexture;

void main()
{
	if (uUseTexture)
		oColor = texture(uTexture, fTexCoord)*fColor;
	else
		oColor = fColor;
}