#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

in mat4 fView;
in vec2 fTexCoord;
in vec3 fViewPos;
in vec3 fFragPos;
in vec3 fNormal;

out vec4 oColor;

uniform bool uUseTexture;
uniform sampler2D uTexture;
uniform Material uMaterial;

void main()
{
	vec3 normal = normalize(fNormal);
	vec3 lightPos = (fView*vec4(vec3(10.0f), 1.0f)).xyz;
	vec3 viewDir = normalize(vec3(0.0f) - fFragPos);
	vec3 toLightDir = normalize(lightPos - fFragPos);
	vec3 reflectDir = reflect(-toLightDir, normal);

	vec3 ambient = uMaterial.ambient * uMaterial.diffuse;

	float diff = max(dot(normal, toLightDir), 0.0f);
	vec3 diffuse = diff * uMaterial.diffuse;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), uMaterial.shininess);
	vec3 specular = spec*uMaterial.specular;
	
	if (uUseTexture)
		oColor = vec4((ambient + diffuse + specular)*texture(uTexture, fTexCoord).rgb, 1.0f);
	else
		oColor = vec4(ambient + diffuse + specular, 1.0f);
}