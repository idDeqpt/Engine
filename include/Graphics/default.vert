#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 direction;
	vec3 color;
};

in vec3 fFragPos;
in vec2 fTexCoord;
in vec3 fNormal;

out vec4 oColor;

uniform Material uMaterial;
uniform int uLightsCount;
uniform sampler2D uLightsTexture;
uniform vec3 uViewPos;

Light getLight(int index)
{
	Light light;
	float dir_u = (index*2 + 0.5)/float(uLightsCount*2);
	float col_u = (index*2 + 1.5)/float(uLightsCount*2);
	float v = 0.5;
	light.direction = texture(uLightsTexture, vec2(dir_u, v)).rgb;
	light.color = texture(uLightsTexture, vec2(col_u, v)).rgb;
	return light;
}

void main()
{
	vec3 color = texture(uMaterial.diffuse, fTexCoord).rgb;
	if (uLightsCount == 0)
	{
		oColor = vec4(color, 1.0f);
		return;
	}

	Light light = getLight(0);
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