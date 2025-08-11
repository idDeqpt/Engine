#version 330 core

in vec2 fTexCoord;

out vec4 oColor;

uniform bool uUseTexture;
uniform sampler2D uTexture;
uniform vec4 uColor;

void main()
{
	if (uUseTexture)
		oColor = texture(uTexture, fTexCoord)*fColor;
	else
		oColor = uColor;
}