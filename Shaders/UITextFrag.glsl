#version 150 core

uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void){
	gl_FragColor = texture(diffuseTex, IN.texCoord);
	//gl_FragColor = vec4(1,IN.texCoord.y,IN.texCoord.x,1);
	//gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}