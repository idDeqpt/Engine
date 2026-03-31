#ifndef DEFAULT_3D_DEFERRED_FRAGMENT_SHADER_STRING_HPP
#define DEFAULT_3D_DEFERRED_FRAGMENT_SHADER_STRING_HPP

namespace eng::core::shader
{
	static const char* default3d_deferred_frag =R"(
#version 330 core

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
};

in vec2 fTexCoord;

out vec4 oColor;

uniform sampler2D uPosition;
uniform sampler2D uNormal;
uniform sampler2D uAlbedo;
uniform sampler2D uMetRogHeiAo;
uniform sampler2D uEmission;

uniform vec3 uViewPos;
uniform bool uUseDirectionalLight;
uniform DirectionalLight uDirectionalLight;

void main()
{
    vec2 tex_coord = fTexCoord;

    vec3 position = texture(uPosition, tex_coord).rgb;
    vec3 normal = texture(uNormal, tex_coord).rgb;
    vec3 albedo = texture(uAlbedo, tex_coord).rgb;

    vec3 color = albedo;

    if (!uUseDirectionalLight)
    {
        oColor = vec4(color, 1.0f);
        return;
    }

    DirectionalLight light = uDirectionalLight;
    vec3 toViewDir  = normalize(uViewPos - position);
    vec3 toLightDir = normalize(-light.direction);
    vec3 halfWayDir = normalize(toLightDir + toViewDir);

    vec3 ambient = color*light.color;

    float diff = max(dot(normal, toLightDir), 0.0f);
    vec3 diffuse = diff*color*light.color;
    
    oColor = vec4(ambient*0.2 + diffuse, 1.0f);
}
)";
}

#endif //DEFAULT_3D_DEFERRED_FRAGMENT_SHADER_STRING_HPP