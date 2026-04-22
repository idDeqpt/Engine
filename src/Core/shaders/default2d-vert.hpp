#ifndef DEFAULT_2D_VERTEX_SHADER_STRING_HPP
#define DEFAULT_2D_VERTEX_SHADER_STRING_HPP

namespace eng::core::shader
{
	static const char* default2d_vert =R"(
#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec2 fTexCoord;

uniform uint uLayer;
uniform mat3 uProjection;
uniform mat3 uView;
uniform mat3 uModel;

void main()
{
    fTexCoord = aTexCoord;
    
    vec3 model_pos = uModel*vec3(aPosition, 1.0f);
    vec3 view_pos = uView*model_pos;
    vec3 clip_pos = uProjection*view_pos;

    gl_Position = vec4(clip_pos.xy, (float(uLayer)/255.0)*2.0 - 1.0, 1.0f);
}
)";
}

#endif //DEFAULT_2D_VERTEX_SHADER_STRING_HPP