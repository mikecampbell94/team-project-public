#pragma once

#include "../Utilities/Maths/Vector3.h"
#include "../Utilities/Maths/Vector2.h"
#include "../Shaders/Shader.h"
#include "../Utilities/Maths/Matrix4.h"

#include <sstream>
#include <string>
#include <vector>
#include <matrix4x4.h>

const int NUM_BONES_PER_VEREX = 10;

struct Vertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexCoords;
	Vector3 Tangent;
	Vector3 Bitangent;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;  // we store the path of the texture to compare with other textures
};

struct BoundingBox
{
	Vector3 max;
	Vector3 min;
};

class SubMesh
{
public:
	SubMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::vector<Texture> textures, std::vector<Texture> heights,
		BoundingBox AABB, int numTransforms);
	SubMesh();
	~SubMesh();

	void Draw(Shader& shader, Matrix4& transform);
	void Draw(Shader& shader);
	void DrawShadow(Shader& shader, Matrix4& worldTransform); //Dont bother binding textures.

	float previousradius = 0;

	void SetTransform(Matrix4 newTransform)
	{
		if (newTransform.getScalingVector() != transform.getScalingVector())
		{
			previousradius = boundingRadius;

			Vector3 scale = newTransform.getScalingVector();
			box.max = box.max * (scale);
			box.min = box.min * (scale);
			CalculateBoundingRadius();
		}

		this->transform = newTransform;
	}

	Matrix4 GetTransform()
	{
		return transform;
	}

	float GetBoundingRadius() const
	{
		return boundingRadius;
	}

	void SetBoundingRadius(float radius)
	{
		boundingRadius = radius;
	}

	void SetCameraDistance(float dist)
	{
		distanceFromCamera = dist;
	}

	static bool CompareByCameraDistance(const SubMesh* a, const SubMesh* b)
	{
		return (a->distanceFromCamera < b->distanceFromCamera)
			? true : false;
	}

	void CalculateBoundingRadius()
	{
		//float firstMax = max(box.max.x, box.max.y);
		//float finalMax = max(firstMax, box.max.z);
		//boundingRadius = finalMax;

		float minLength = abs(box.min.length());
		float maxLength = abs(box.max.length());

		boundingRadius = max(minLength, maxLength);//(box.max - box.min).Length() / 2;//
	}

	void addTexture(unsigned int texture);

	void BufferData();
	void DrawOnly();

	//Mesh Data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	std::vector<Texture> heights;

	BoundingBox box;

	int hasTexture = 0;
	int isReflective = 0;
	float reflectionStrength = 1.0f;
	Vector4 baseColour;

	GLuint modelMatricesSSBO;

protected:
	Matrix4 transform;

	void SetupMesh();

	unsigned int VAO, VBO, EBO; //Render data

	float boundingRadius;
	float distanceFromCamera;
};