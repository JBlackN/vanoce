#version 140

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

out vec4 color;

uniform mat4 model;
uniform sampler2D tex;
uniform struct Light
{
	vec3 position;
	vec3 color;
} light;

void main()
{
	vec3 normal = normalize(transpose(inverse(mat3(model))) * fNormal);
	vec3 worldPosition = vec3(model * vec4(fPosition, 1));
	vec3 directionToLight = light.position - worldPosition;
	float brightness = clamp(dot(normal, directionToLight) / (length(directionToLight) * length(normal)), 0, 1);

	vec4 texture = texture(tex, fTexCoord);
	color = vec4(brightness * light.color, 1) * texture;
}