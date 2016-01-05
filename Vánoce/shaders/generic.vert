//----------------------------------------------------------------------------------------
/**
* \file       generic.vert
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains generic vertex shader.
*
*  File contains generic vertex shader.
*
*/
//----------------------------------------------------------------------------------------
#version 140

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fPosition = vPosition;
	fNormal = vNormal;
	fTexCoord = vTexCoord;

	gl_Position = projection * view * model * vec4(vPosition, 1.0);
}