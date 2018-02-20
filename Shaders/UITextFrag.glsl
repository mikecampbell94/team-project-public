#version 150
uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 FragColor;

void main(void){
	
	//FragColor = texture(diffuseTex, IN.texCoord);
	FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
	
}