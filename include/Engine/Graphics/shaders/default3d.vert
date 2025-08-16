#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 aInstanceTransform;

out vec3 fFragPos;
out vec2 fTexCoord;
out vec3 fNormal;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
    mat4 result_model = transpose(aInstanceTransform)*uModel;
    fFragPos = vec3(result_model*vec4(aPosition, 1.0f));
    fTexCoord = aTexCoord;
    vec3 worldNormal = transpose(inverse(mat3(result_model)))*aNormal;
    fNormal = normalize(worldNormal);
    
    gl_Position = uProjection*uView*vec4(fFragPos, 1.0f);
}