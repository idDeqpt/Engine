#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
};

in vec3 fFragPos;
in vec2 fTexCoord;
in vec3 fNormal;

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
	vec3 normal = normalize(fNormal);
	vec3 toViewDir = normalize(uViewPos - fFragPos);
	vec3 toLightDir = normalize(-light.direction);
	vec3 reflectDir = reflect(-toLightDir, normal);

	vec3 ambient = color*light.color;

	float diff = max(dot(normal, toLightDir), 0.0f);
	vec3 diffuse = diff*color*light.color;

	float spec = pow(max(dot(toViewDir, reflectDir), 0.0f), uMaterial.shininess);
	vec3 specular = spec*texture(uMaterial.specular, fTexCoord).rgb*light.color;
	
	oColor = vec4(ambient*0.5 + diffuse + specular, 1.0f);
	//oColor = vec4(color, 1.0f);
}