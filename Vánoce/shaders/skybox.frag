//----------------------------------------------------------------------------------------
/**
* \file       skybox.frag
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains skybox fragment shader.
*
*  File contains skybox fragment shader without lights, materials and fog.
*
*/
//----------------------------------------------------------------------------------------
#version 140

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

out vec4 color;

uniform sampler2D tex;

void main()
{
	color = texture(tex, fTexCoord);
}