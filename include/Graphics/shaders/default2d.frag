#version 330 core

in vec2 fTexCoord;

out vec4 oColor;

uniform sampler2D uTexture;

void main()
{
	oColor = texture(uTexture, fTexCoord);
}