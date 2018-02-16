#version 150 core

uniform vec4 colour;

out vec4 gl_FragColor;

void main(void) {
	gl_FragColor = colour;
}