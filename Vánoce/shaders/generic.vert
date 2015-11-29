#version 140

in vec3 position;
in vec3 norm;
in vec2 vTexCoord;
out vec2 fTexCoord;

uniform mat4 model;
uniform mat4 camera;

void main()
{
	fTexCoord = vTexCoord;
	gl_Position = camera * model * vec4(position, 1.0);
}