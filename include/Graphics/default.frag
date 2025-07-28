#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out mat4 fView;
out vec2 fTexCoord;
out vec3 fViewPos;
out vec3 fFragPos;
out vec3 fNormal;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
    fFragPos = vec3(uView*uModel*vec4(aPosition, 1.0));
    fViewPos = vec3(uView*uModel*vec4(aPosition, 1.0f));
    vec3 worldNormal = transpose(inverse(mat3(uModel)))*aNormal;
    vec3 viewNormal = mat3(uView)*worldNormal;
    fNormal = normalize(viewNormal);
    fView = uView;
    fTexCoord = aTexCoord;
    
    gl_Position = uProjection*vec4(fFragPos, 1.0f);
}