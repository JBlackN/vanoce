#version 140

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

out vec4 color;

uniform sampler2D bgTex;
uniform sampler2D fgTex;

uniform struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	float shininess;
} material;

void main()
{
	vec4 ornamentTexColor = texture(bgTex, fTexCoord);
	if (ornamentTexColor.a < 0.5f) discard;

	vec4 numberTexColor = texture(fgTex, fTexCoord);
	color = mix(ornamentTexColor, numberTexColor, 0.3f);

	color = vec4(material.diffuse, 1) * vec4(color.xyz, 0.75f);
}