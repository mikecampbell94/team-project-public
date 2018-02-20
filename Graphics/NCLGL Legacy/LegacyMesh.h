#pragma once

#include <GL/glew.h>

#include "Maths/Vector2.h"
#include "Maths/Vector3.h"
#include "Maths/Vector4.h"
#include "../Shaders/Shader.h"
#include "../../Utilities/Maths/Matrix4.h"

enum MeshBuffer
{
	VERTEX_BUFFER, COLOUR_BUFFER,
	TEXTURE_BUFFER, TANGENT_BUFFER,
	INDEX_BUFFER, NORMAL_BUFFER,
	MAX_BUFFER
};

class LegacyMesh
{
public:
	LegacyMesh(void);
	virtual ~LegacyMesh(void);

	virtual void	Draw(Shader& shader, Matrix4& worldTransform);
	static	LegacyMesh*	GenerateTriangle();
	static	LegacyMesh*	GenerateQuad();

	void	SetTexture(GLuint tex) { texture = tex; }
	GLuint	GetTexture() { return texture; }

	void	SetBumpMap(GLuint tex) { bumpTexture = tex; }
	GLuint	GetBumpMap() { return bumpTexture; }

	void	SetColour(Vector4 newCol, LegacyMesh* m);

protected:
	void		BufferData();
	void		GenerateNormals();

	void		GenerateTangents();
	Vector3		GenerateTangent(const Vector3 &a, const Vector3 &b,
		const Vector3 &c, const Vector2 &ta,
		const Vector2 &tb, const Vector2 &tc);

	GLuint		arrayObject;
	GLuint		bufferObject[MAX_BUFFER];
	GLuint		numVertices;
	GLuint		type;
	GLuint		texture;
	GLuint		numIndices;
	Vector2*	textureCoords;

	Vector3*	vertices;
	Vector3*	normals;
	Vector3*	tangents;
	Vector4*	colours;

	unsigned int* indices;
	GLuint		bumpTexture;
};