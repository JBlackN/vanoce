#version 140

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

out vec4 color;

uniform mat4 model;
uniform mat4 view;
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
	int type;
	bool enabled;

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
} lights[10];
uniform int lights_count;

uniform vec3 camera_position;

uniform struct Fog
{
	float density;
	vec4 color;
	bool enabled;
} fog;

vec3 useLight(Light light, vec3 normal, vec3 worldPosition)
{
	if (!light.enabled) return 0;

	vec3 directionToLight = light.type == 2 ? normalize(light.position.xyz) : light.position - worldPosition; // Directional or not
	vec3 directionFromLight = -directionToLight;
	float distanceToLight = length(directionToLight);

	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = max(dot(normalize(directionToLight), normal), 0) * light.diffuse * material.diffuse;

	vec3 camera_direction = camera_position - worldPosition;
	vec3 s = normalize((camera_direction + directionToLight) / length(camera_direction + directionToLight));
	vec3 specular = pow(max(dot(s, normal), 0), material.shininess) * light.specular * material.specular;

	if (light.type == 0) // Point
	{
		float attenuation = 1.0 / (light.constant_attenuation + (length(distanceToLight) * light.linear_attenuation)
			+ (pow(length(distanceToLight), 2) * light.quadratic_attenuation));

		return attenuation * clamp(ambient + diffuse + specular, 0, 1);
	}
	else if (light.type == 1) // Spot
	{
		float attenuation = 1.0 / (light.constant_attenuation + (length(distanceToLight) * light.linear_attenuation)
			+ (pow(length(distanceToLight), 2) * light.quadratic_attenuation));

		float cos_alpha_positive = max(dot(directionFromLight, light.spot_direction), 0);
		float spotlight_effect = cos_alpha_positive < light.spot_cutoff ? 0 : pow(cos_alpha_positive, light.spot_exponent);
		return spotlight_effect * attenuation * clamp(ambient + diffuse + specular, 0, 1);
	}
	else // Dir
	{
		return clamp(ambient + diffuse + specular, 0, 1);
	}
}

vec4 useFog(vec4 color, vec3 worldPosition)
{
	if (!fog.enabled) return color;

	float z = length(view * model * vec4(fPosition, 1));//normalize(length(worldPosition - camera_position));
	float f = clamp(exp(-pow((fog.density * z), 2)), 0, 1);
	return vec4(f * color.rgb + (1 - f) * fog.color.rgb, color.a);
}

void main()
{
	vec3 normal = normalize(transpose(inverse(mat3(model))) * fNormal);
	vec3 worldPosition = vec3(model * vec4(fPosition, 1));

	vec3 global_ambient = vec3(0, 0, 0.05) * material.ambient;

	vec3 computed_color = material.emission + global_ambient;
	for (int i = 0; i < lights_count; i++)
		computed_color += useLight(lights[i], normal, worldPosition);

	vec4 texture = texture(tex, fTexCoord);
	color = vec4(computed_color, 1) * texture;
	color = useFog(color, worldPosition);
}