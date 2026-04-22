#ifndef DEFAULT_3D_DEFERRED_FRAGMENT_SHADER_STRING_HPP
#define DEFAULT_3D_DEFERRED_FRAGMENT_SHADER_STRING_HPP

namespace eng::core::shader
{
	static const char* default3d_deferred_frag =R"(
#version 330 core

struct Material
{
    bool useNormal;
    bool useMetallic;
    bool useRoughness;
    bool useHeight;
    bool useAo;
    bool useEmission;
    sampler2D albedo;
    sampler2D normal;
    sampler2D metallic;
    sampler2D roughness;
    sampler2D height;
    sampler2D ao;
    sampler2D emission;
};

in vec3 fFragPos;
in vec2 fTexCoord;
in mat3 fTBN;

layout (location = 0) out vec4 oPosition;
layout (location = 1) out vec4 oNormal;
layout (location = 2) out vec4 oAlbedo;
layout (location = 3) out vec4 oMetRogHeiAo;
layout (location = 4) out vec4 oEmission;

uniform Material uMaterial;

void main()
{
    oPosition = vec4(fFragPos, 1);

    vec3 normal = uMaterial.useNormal ? (fTBN*(texture(uMaterial.normal, fTexCoord).rgb*2.0f - 1.0f)) : fTBN[2];
    oNormal = vec4(normalize(normal), 1);

    oAlbedo = vec4(texture(uMaterial.albedo, fTexCoord).rgb, 1);

    oMetRogHeiAo = vec4(
        uMaterial.useMetallic  ? texture(uMaterial.metallic,  fTexCoord).r : 0.0,
        uMaterial.useRoughness ? texture(uMaterial.roughness, fTexCoord).r : 0.5,
        uMaterial.useHeight    ? texture(uMaterial.height,    fTexCoord).r : 0.0,
        uMaterial.useAo        ? texture(uMaterial.ao,        fTexCoord).r : 1.0
    );

    oEmission = (uMaterial.useEmission) ?
        vec4(texture(uMaterial.emission, fTexCoord).rgb, 1.0) :
        vec4(vec3(0.0), 1.0);
}
)";
}

#endif //DEFAULT_3D_DEFERRED_FRAGMENT_SHADER_STRING_HPP