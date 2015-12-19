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
	vec4 adjustedVTexCoords = textureAdjustmentMatrix * vec4(vTexCoord.x, vTexCoord.y, 0, 1);
	fTexCoord = vec2(adjustedVTexCoords.x, adjustedVTexCoords.y);

	gl_Position = projection * view * model * vec4(vPosition, 1.0);
}