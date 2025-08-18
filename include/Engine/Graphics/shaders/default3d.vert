#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in mat4 aInstanceTransform;

out vec3 fFragPos;
out vec2 fTexCoord;
out mat3 fTBN;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
    mat4 result_model = uModel*transpose(aInstanceTransform);
    fFragPos = vec3(result_model*vec4(aPosition, 1.0f));
    fTexCoord = aTexCoord;
    mat3 normal_matrix = transpose(inverse(mat3(result_model)));
    vec3 normal  = normalize(normal_matrix*aNormal);
    vec3 tangent = normalize(normal_matrix*aTangent);
    fTBN = mat3(
        normal_matrix*normalize(tangent),
        normal_matrix*cross(tangent, normal),
        normal_matrix*normalize(normal)
    );
    
    gl_Position = uProjection*uView*vec4(fFragPos, 1.0f);
}