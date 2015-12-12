#version 140

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

out vec4 color;

uniform mat4 model;
uniform sampler2D tex;

uniform struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	float shininess;
} material;

uniform struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;

	vec3 spot_direction;
	float spot_exponent;
	float spot_cutoff;

	float constant_attenuation;
	float linear_attenuation;
	float quadratic_attenuation;
} light;

uniform vec3 camera_position;

void main()
{
	vec3 normal = normalize(transpose(inverse(mat3(model))) * fNormal);
	vec3 worldPosition = vec3(model * vec4(fPosition, 1));
	vec3 directionToLight = light.position - worldPosition;

	vec3 global_ambient = vec3(0, 0, 0.05) * material.ambient;

	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = max(dot(normalize(directionToLight), normal), 0) * light.diffuse * material.diffuse;

	vec3 camera_direction = camera_position - worldPosition;
	vec3 s = normalize((camera_direction + directionToLight) / length(camera_direction + directionToLight));
	vec3 specular = pow(max(dot(s, normal), 0), material.shininess) * light.specular * material.specular;

	//light.quadratic_attenuation = 0.2;
	float distanceToLight = length(light.position - worldPosition);
	float attenuation = 1.0 / (light.constant_attenuation + (length(distanceToLight) * light.linear_attenuation)
		+ (pow(length(distanceToLight), 2) * light.quadratic_attenuation));

	vec4 texture = texture(tex, fTexCoord);
	vec3 computed_color = material.emission + global_ambient + /*100 * attenuation * */clamp((ambient + diffuse + specular), 0, 1);
	color = vec4(computed_color, 1) * texture;
}