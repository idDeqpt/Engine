#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec2 fTexCoord;
out vec4 fColor;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat3 uModel;
uniform vec4 uColor;

void main()
{
    fTexCoord = aTexCoord;
    fColor = uColor;
    
    vec3 model_pos = uModel*vec3(aPosition, -1.0f);
    gl_Position = uProjection*uView*vec4(model_pos, 1.0f);
}