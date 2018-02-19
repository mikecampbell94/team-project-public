#version 330 core

uniform mat4 projMatrix;
uniform vec3 samples[32];

int kernelSize = 32;
uniform float radius;
uniform float bias;

uniform float resolutionX;
uniform float resolutionY;

in vec2 TexCoords;

out float gl_FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

void main(void){
	//Generate noise scale based on screen resolution
	vec2 noiseScale = vec2(1280 / 4.0f, 720 / 4.0f);

	float b = 0.01f;
	float r = 50.0f;

	//Get input for SSAO algorithm
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz;

    //Create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    //Iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0f;
    for(int i = 0; i < kernelSize; ++i)
    {
        //Get sample position
        vec3 samplevec = TBN * samples[i]; // from tangent to view-space
		samplevec = fragPos + samplevec * r;
        
        //Project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(samplevec, 1.0);
        offset = projMatrix * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        //Get sample depth
        float sampleDepth = texture(gPosition, offset.xy).z; // get depth value of kernel sample
        
        //Range check & accumulate
        float rangeCheck = smoothstep(0.0f, 1.0f, r / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplevec.z + b ? 1.0f : 0.0f) * rangeCheck;
    }

    occlusion = 1.0f - (occlusion / kernelSize);
    
    gl_FragColor = pow(occlusion, 1.4f);
}