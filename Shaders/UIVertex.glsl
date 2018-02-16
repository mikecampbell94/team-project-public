#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix;

void main(void) {
	//gl_Position   = vec4(position, 1.0);
	//OUT.worldPos = (modelMatrix * vec4(aPos, 1)).xyz;
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(aPos, 1.0);
	//OUT.colour = vec4(1.0, 0, 0, 1.0); //colour;
}