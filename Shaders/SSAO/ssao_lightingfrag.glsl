#version 430

out vec4 FragColor;

uniform int numberOfLights;

uniform int numShadowCastingLights;
uniform float ambientLighting;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;

uniform sampler2DShadow shadows[5];
uniform mat4 texMatrices[5];
uniform mat4 camMatrix;
uniform mat4 viewMatrix;

uniform sampler2D ambientTextures[1];

uniform int numXTiles;
uniform int numYTiles;

in vec2 TexCoords;
in mat4 textureMat;
in vec2 screenPos;

struct LightData
{
	vec4 pos;
	vec4 lightColour;
	float lightRadius;
	float intensity;

	float fpadding[2];
};

layout (std430, binding = 1) buffer LightDataBuffer
{
	LightData lightData[];
};

void CalSpecularLight(int index, vec3 position, vec3 normal, vec4 albedo, inout vec4 finalColour)
{
	vec3 lightPosition = lightData[index].pos.xyz;
	vec3 lightPosView = vec3(camMatrix * vec4(lightPosition, 1.0));
	float dist = length(lightPosView - position);

	if (dist <= lightData[index].lightRadius)
	{
		//Diffuse
		vec3 viewDir = normalize(-position);
		vec3 lightDir = normalize(lightPosView - position);
		vec3 diffuse = max(dot(normal, lightDir), 0.0) *
			albedo.rgb * lightData[index].lightColour.rgb;

		//Specular
		vec3 halfDir = normalize(lightDir + viewDir);
		float specPower = pow(max(dot(normal, halfDir), 0.0), 500.0);
		vec3 specular = lightData[index].lightColour.rgb * specPower;

		//Attenuation
		float attenuation = 0;
		attenuation = 1.0 - clamp(dist / lightData[index].lightRadius, 0.0, 1.0);
		attenuation *= lightData[index].intensity;

		diffuse *= attenuation;
		specular *= attenuation;

		finalColour.rgb += diffuse.rgb + specular.rgb;
	}
}

void main(void){
	vec3 position = texture(gPosition, TexCoords).xyz;
	vec3 normal = texture(gNormal, TexCoords).xyz;
	vec4 albedo = texture(gAlbedo, TexCoords);

	if (position.z > 0.0f)
	{
		//Its the skybox, dont touch it...
		FragColor = albedo;
	}
	else 
	{
		vec4 finalColour = vec4(0, 0, 0, 1);

		for (int i = 0; i < numberOfLights; i++)
		{
			CalSpecularLight(i, position, normal, albedo, finalColour);
		}

		FragColor = finalColour;
	}
}