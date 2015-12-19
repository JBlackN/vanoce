#version 140

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

out vec4 color;

uniform mat4 model;
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
	vec4 bgTexColor = texture(bgTex, fTexCoord);
	vec4 fgTexColor = texture(fgTex, fTexCoord);
	color = mix(bgTexColor, fgTexColor, 0.7f);
}