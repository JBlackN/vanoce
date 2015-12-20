#version 140

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform sampler2D tex;

uniform vec3 cameraPosition;
uniform int lightCount;

uniform struct Light
{
	int type;
	bool enabled;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;

	vec3 spotDirection;
	float spotExponent;
	float spotCutoff;

	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
} lights[10];

uniform struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	float shininess;
} material;

uniform struct Fog
{
	bool enabled;
	float density;
	vec4 color;
} fog;

vec3 applyLightModifiers(Light light, vec3 directionToLight, int lightType, vec3 baseColor)
{
	float attenuation = 1.0 / (light.constantAttenuation + (length(directionToLight) * light.linearAttenuation)
		+ (pow(length(directionToLight), 2) * light.quadraticAttenuation));

	switch (lightType)
	{
	case 0: // Point light
		return attenuation * baseColor;
	case 1: // Spot light
		float cosAlphaPositive = max(dot(-directionToLight, light.spotDirection), 0);
		float spotlightEffect = cosAlphaPositive < light.spotCutoff ? 0 : pow(cosAlphaPositive, light.spotExponent);
		return spotlightEffect * attenuation * baseColor;
	}
}

vec3 useLight(Light light, vec3 normal, vec3 worldPosition)
{
	if (!light.enabled) return 0;

	vec3 directionToLight = light.type == 2 ? normalize(light.position.xyz) : light.position - worldPosition; // Directional or not
	vec3 cameraDirection = cameraPosition - worldPosition;
	vec3 camLightHalfAngle = normalize((cameraDirection + directionToLight) / length(cameraDirection + directionToLight));

	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = max(dot(normalize(directionToLight), normal), 0) * light.diffuse * material.diffuse;
	vec3 specular = pow(max(dot(camLightHalfAngle, normal), 0), material.shininess) * light.specular * material.specular;

	vec3 baseColor = clamp(ambient + diffuse + specular, 0, 1);
	return light.type == 2 ? baseColor : applyLightModifiers(light, directionToLight, light.type, baseColor);
}

vec4 useFog(vec4 color, vec3 worldPosition)
{
	if (!fog.enabled) return color;

	float zDistance = length(view * model * vec4(fPosition, 1));
	float blendFactor = clamp(exp(-pow((fog.density * zDistance), 2)), 0, 1);
	return vec4(blendFactor * color.rgb + (1 - blendFactor) * fog.color.rgb, color.a);
}

void main()
{
	vec3 worldPosition = vec3(model * vec4(fPosition, 1));
	vec3 normal = normalize(transpose(inverse(mat3(model))) * fNormal);

	vec3 globalAmbient = vec3(0, 0, 0.05) * material.ambient;

	vec3 computedColor = material.emission + globalAmbient;
	for (int i = 0; i < lightCount; i++)
		computedColor += useLight(lights[i], normal, worldPosition);

	vec4 texture = texture(tex, fTexCoord);
	color = vec4(computedColor, 1) * texture;
	color = useFog(color, worldPosition);
}