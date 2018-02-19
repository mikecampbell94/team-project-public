#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

uniform sampler2D texture_diffuse;
uniform int hasTexture;

uniform vec3 cameraPos;
uniform vec4 baseColour;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

void main(void) {
	gPosition = FragPos;
	gNormal = normalize(Normal);

	vec4 col = baseColour;
	float alpha = col.a;

	//if (hastexture == 1) 
	//{
	//	col = texture2d(texture_diffuse, texcoords);
	//}
	//
	//if (isreflective == 1) 
	//{
	//	vec3 i = normalize(reflectionpos - camerapos);
	//	vec3 r = reflect(i, normalize(reflectionnormal));
	//	vec4 reflectioncolour = vec4(texture(skybox, r).rgb, 1.0);
	//	col += reflectioncolour * reflectionstrength;
	//	col /= 2;
	//}

	gAlbedo = vec4(col.rgb, 1.0);
	//glAlbedo = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}