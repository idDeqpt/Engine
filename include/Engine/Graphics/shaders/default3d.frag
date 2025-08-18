#version 330 core

struct Material
{
	bool useNormal;
	bool useParallax;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	sampler2D parallax;
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

vec2 getParallaxShift(vec3 view_dir, vec2 tex_coord)
{
	float height =  texture(uMaterial.parallax, tex_coord).r;    
    vec2 shift = view_dir.xy / view_dir.z * (height * 0.1);
    return shift; 
}

void main()
{
	vec2 tex_coord = fTexCoord;
	if (uMaterial.useParallax)
	{
		vec3 coord_view_dir = normalize(fTBN*(uViewPos - fFragPos));
		tex_coord = tex_coord - getParallaxShift(coord_view_dir, fTexCoord);
		if(tex_coord.x > 1.0 || tex_coord.y > 1.0 || tex_coord.x < 0.0 || tex_coord.y < 0.0)
			discard;
	}
	vec3 color = texture(uMaterial.diffuse, tex_coord).rgb;

	if (!uUseDirectionalLight)
	{
		oColor = vec4(color, 1.0f);
		return;
	}

	DirectionalLight light = uDirectionalLight;
	vec3 normal     = uMaterial.useNormal ? (fTBN*(texture(uMaterial.normal, tex_coord).rgb*2.0f - 1.0f)) : fTBN[2];
	vec3 toViewDir  = normalize(uViewPos - fFragPos);
	vec3 toLightDir = normalize(-light.direction);
	vec3 halfWayDir = normalize(toLightDir + toViewDir);

	vec3 ambient = color*light.color;

	float diff = max(dot(normal, toLightDir), 0.0f);
	vec3 diffuse = diff*color*light.color;

	float spec = pow(max(dot(normal, halfWayDir), 0.0f), uMaterial.shininess);
	vec3 specular = spec*light.color;
	//vec3 specular = spec*texture(uMaterial.specular, tex_coord).rgb*light.color;
	
	oColor = vec4(ambient*0.2 + diffuse + specular, 1.0f);
}