#version 140

in vec3 position;
in vec2 vTexCoord;
out vec2 fTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fTexCoord = vTexCoord;
	gl_Position = projection * view * model * vec4(position, 1.0);
}