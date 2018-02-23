#version 150 core

out vec4 gl_FragColor;

uniform vec4 baseColour;

void main(void){
	gl_FragColor = baseColour;// vec4(1.0);
}