#version 430 core
#extension NV_shader_atomic_float : enable

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uniform sampler2D paintTrailTexture;

struct PlayerScores
{
	float red;
};

layout(std430, binding = 4) buffer PlayerScoresBuffer
{
	PlayerScores scores;
};

//layout(binding = 0) uniform atomic_uint count;

void main()
{
	int xCoord = int(gl_GlobalInvocationID.x);
	int yCoord = int(gl_GlobalInvocationID.y);

	float textureCoordX = float(xCoord) / 1280.0f;
	float textureCoordY = float(yCoord) / 720.0f;

	vec4 colour = texture2D(paintTrailTexture, vec2(textureCoordX, textureCoordY));

	if (colour.rgb == vec3(1.0f, 0.0f, 0.0f))
	{
		//scores.red = int(atomicCounterIncrement(count));
		atomicAdd(scores.red, 0.1f);
	}

	//uint index = uint(tile);

	//int intersections = 0;

	//uint lightsOnScreen = atomicCounter(count);
	//for (int i = 0; i < lightsOnScreen; i++)
	//{
	//	int lightIndex = int(indexes[i]);

	//	tileLights[index][intersections] = lightIndex;
	//	intersections++;
	//}

	//lightIndexes[index] = intersections;


}

