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

	virtual void	draw(Shader& shader, NCLMatrix4& worldTransform);
	static	LegacyMesh*	generateTriangle();
	static	LegacyMesh*	generateQuad();

	void	setTexture(GLuint tex) { texture = tex; }
	GLuint	getTexture() { return texture; }

	void	setBumpMap(GLuint tex) { bumpTexture = tex; }
	GLuint	getBumpMap() { return bumpTexture; }

	void	setColour(NCLVector4 newCol, LegacyMesh* m);

protected:
	void		bufferData();
	void		generateNormals();

	void		generateTangents();
	NCLVector3		generateTangent(const NCLVector3 &a, const NCLVector3 &b,
		const NCLVector3 &c, const NCLVector2 &ta,
		const NCLVector2 &tb, const NCLVector2 &tc);

	GLuint		arrayObject;
	GLuint		bufferObject[MAX_BUFFER];
	GLuint		numVertices;
	GLuint		type;
	GLuint		texture;
	GLuint		numIndices;
	NCLVector2*	textureCoords;

	NCLVector3*	vertices;
	NCLVector3*	normals;
	NCLVector3*	tangents;
	NCLVector4*	colours;

	unsigned int* indices;
	GLuint		bumpTexture;
};