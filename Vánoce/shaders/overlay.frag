#version 140

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

out vec4 color;

uniform sampler2D bgTex;
uniform sampler2D fgTex;

void main()
{
	vec4 bgTexColor = texture(bgTex, fTexCoord);
	vec4 fgTexColor = texture(fgTex, fTexCoord);
	color = mix(bgTexColor, fgTexColor, 0.7f);
}