#version 330 core

struct Material
{
	bool useNormal;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	float shininess;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
};

in vec3 fFragPos;
in vec2 fTexCoord;
in mat3 fTBN;

out vec4 oColor;

uniform Material uMaterial;
uniform vec3 uViewPos;
uniform bool uUseDirectionalLight;
uniform DirectionalLight uDirectionalLight;

void main()
{
	vec3 color = texture(uMaterial.diffuse, fTexCoord).rgb;
	if (!uUseDirectionalLight)
	{
		oColor = vec4(color, 1.0f);
		return;
	}

	DirectionalLight light = uDirectionalLight;
	vec3 normal     = uMaterial.useNormal ? (fTBN*(texture(uMaterial.normal, fTexCoord).rgb*2.0f - 1.0f)) : fTBN[2];
	vec3 toViewDir  = normalize(uViewPos - fFragPos);
	vec3 toLightDir = normalize(-light.direction);
	vec3 halfWayDir = normalize(toLightDir + toViewDir);

	vec3 ambient = color*light.color;

	float diff = max(dot(normal, toLightDir), 0.0f);
	vec3 diffuse = diff*color*light.color;

	float spec = pow(max(dot(normal, halfWayDir), 0.0f), uMaterial.shininess);
	vec3 specular = spec*texture(uMaterial.specular, fTexCoord).rgb*light.color;
	
	oColor = vec4(ambient*0.2 + diffuse + specular, 1.0f);
}