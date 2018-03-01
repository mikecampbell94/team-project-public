#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uniform sampler2D paintTrailTexture;
uniform int arraySize;

layout(std430, binding = 4) buffer PlayerScoresBuffer
{
	int scores[];
};

layout(std430, binding = 5) buffer ColourBuffer
{
	vec4 colours[];
};

void main()
{
	int xCoord = int(gl_GlobalInvocationID.x);
	int yCoord = int(gl_GlobalInvocationID.y);

	float textureCoordX = float(xCoord) / 1280.0f;
	float textureCoordY = float(yCoord) / 720.0f;

	vec4 colour = texture2D(paintTrailTexture, vec2(textureCoordX, textureCoordY));

	for (int i = 0; i < arraySize; i++)
	{
		if (colour.rgb == colours[i].rgb)
		{
			atomicAdd(scores[i], 1);
		}
	}
}

