//----------------------------------------------------------------------------------------
/**
* \file       animatedTexture.vert
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains vertex shader for animated texture.
*
*  File contains vertex shader for animated texture.
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
uniform mat4 textureAdjustmentMatrix;

void main()
{
	fPosition = vPosition;
	fNormal = vNormal;

	fTexCoord = vec2(textureAdjustmentMatrix * vec4(vTexCoord.x, vTexCoord.y, 0, 1));

	gl_Position = projection * view * model * vec4(vPosition, 1.0);
}