#include "LegacyMesh.h"

LegacyMesh::LegacyMesh(void)
{
	for (int i = 0; i < MAX_BUFFER; ++i) {
		bufferObject[i] = 0;
	}
	glGenVertexArrays(1, &arrayObject);

	numVertices = 0;
	texture = 0;
	numIndices = 0;
	bumpTexture = 0;

	vertices = NULL;
	colours = NULL;
	normals = NULL;
	textureCoords = NULL;
	indices = NULL;
	tangents = NULL;
	type = GL_TRIANGLES;
}


LegacyMesh::~LegacyMesh()
{
	glDeleteVertexArrays(1, &arrayObject);
	glDeleteBuffers(MAX_BUFFER, bufferObject);
	glDeleteTextures(1, &bumpTexture);
	glDeleteTextures(1, &texture);
	delete[] vertices;
	delete[] colours;
	delete[] normals;
	delete[] textureCoords;
	delete[] indices;
	delete[] tangents;
}

LegacyMesh * LegacyMesh::GenerateTriangle() {
	LegacyMesh* m = new LegacyMesh();
	m->numVertices = 3;

	m->vertices = new Vector3[m->numVertices];
	m->vertices[0] = Vector3(0.0f, 0.5f, 0.0f);
	m->vertices[1] = Vector3(0.5f, -0.5f, 0.0f);
	m->vertices[2] = Vector3(-0.5f, -0.5f, 0.0f);

	m->textureCoords = new Vector2[m->numVertices];
	m->textureCoords[0] = Vector2(0.5f, 0.0f);
	m->textureCoords[1] = Vector2(1.0f, 1.0f);
	m->textureCoords[2] = Vector2(0.0f, 1.0f);

	m->colours = new Vector4[m->numVertices];

	m->colours[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	m->colours[1] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	m->colours[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	m->BufferData();
	return m;
}

LegacyMesh * LegacyMesh::GenerateQuad() {
	LegacyMesh* m = new LegacyMesh();
	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->normals = new Vector3[m->numVertices];
	m->tangents = new Vector3[m->numVertices];

	m->vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
	m->vertices[1] = Vector3(-1.0f, 1.0f, 0.0f);
	m->vertices[2] = Vector3(1.0f, -1.0f, 0.0f);
	m->vertices[3] = Vector3(1.0f, 1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f, 1.0f);
	m->textureCoords[1] = Vector2(0.0f, 0.0f);
	m->textureCoords[2] = Vector2(1.0f, 1.0f);
	m->textureCoords[3] = Vector2(1.0f, 0.0f);

	for (int i = 0; i < 4; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		m->normals[i] = Vector3(0.0f, 0.0f, -1.0f);
		m->tangents[i] = Vector3(1.0f, 0.0f, 1.0f);
	}

	m->BufferData();
	return m;
}

void LegacyMesh::SetColour(Vector4 newCol, LegacyMesh* m) {
	m->colours = new Vector4[m->numVertices];
	for (unsigned i = 0; i < m->numVertices; i++) {
		m->colours[i] = newCol;
	}

	m->BufferData();
}

void LegacyMesh::BufferData() {
	//bind this meshes array object, now any vertex array functionality will be performed on the newly bund array object
	glBindVertexArray(arrayObject);

	//generate new VBO, store its name in first index of our bufferObject array
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	//bind it, so all vertex buffer functions will be applied to our new VBO,  and also assigns new VBO to to currently bound VAO, which will tie all VBOs into our VAO
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	//now buffer the vertex position data, which copies data into graphics memory, second param tells how much memory we need in graphics memory to store this
	//and the pointer to the data to pass, finally inform OpenGL how we expect data to be used, either dynamically updated, or loaded once as static data
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), vertices, GL_STATIC_DRAW);
	//now VBO data copied into memory, set how to access it, by modifying VAO.
	//tell OpenGL vertex attrib has 3 float components per vertex
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//next lines enables this
	glEnableVertexAttribArray(VERTEX_BUFFER);

	//now do same for textureCoords
	if (textureCoords) {
		glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2), textureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}
	//now do the same for colours
	if (colours) {
		glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector4), colours, GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}
	if (normals) {
		glGenBuffers(1, &bufferObject[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(NORMAL_BUFFER);
	}
	if (tangents) {
		glGenBuffers(1, &bufferObject[TANGENT_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TANGENT_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), tangents, GL_STATIC_DRAW);
		glVertexAttribPointer(TANGENT_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TANGENT_BUFFER);
	}
	if (indices) {
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);

	}


	glBindVertexArray(0);
}

void LegacyMesh::GenerateNormals() {
	if (!normals) {
		normals = new Vector3[numVertices];
	}

	for (GLuint i = 0; i < numVertices; ++i) {
		normals[i] = Vector3();
	}

	if (indices) { //Generate per-vertex normals
		for (GLuint i = 0; i < numIndices; i += 3) {
			unsigned int a = indices[i];
			unsigned int b = indices[i + 1];
			unsigned int c = indices[i + 2];

			Vector3 normal = Vector3::cross(
				(vertices[b] - vertices[a]),
				(vertices[c] - vertices[a]));

			normals[a] += normal;
			normals[b] += normal;
			normals[c] += normal;
		}
	}
	else { //Simply list of triangles, so generate face normals
		for (GLuint i = 0; i < numVertices; i += 3) {
			Vector3 &a = vertices[i];
			Vector3 &b = vertices[i + 1];
			Vector3 &c = vertices[i + 2];

			Vector3 normal = Vector3::cross(b - a, c - a);

			normals[i] = normal;
			normals[i = 1] = normal;
			normals[i + 2] = normal;
		}
	}

	for (GLuint i = 0; i < numVertices; ++i) {
		normals[i].normalise();
	}
}

void LegacyMesh::GenerateTangents() {
	if (!tangents) {
		tangents = new Vector3[numVertices];
	}

	if (!textureCoords) {
		textureCoords = new Vector2[numVertices];
	}


	for (GLuint i = 0; i < numVertices; ++i) {
		tangents[i] = Vector3();
	}

	if (indices) {
		for (GLuint i = 0; i < numIndices; i += 3) {
			int a = indices[i];
			int b = indices[i + 1];
			int c = indices[i + 2];

			Vector3 tangent = GenerateTangent(vertices[a], vertices[b],
				vertices[c], textureCoords[a],
				textureCoords[b], textureCoords[c]);

			tangents[a] += tangent;
			tangents[b] += tangent;
			tangents[c] += tangent;
		}
	}
	else { //Simply list of triangles, so generate face normals
		for (GLuint i = 0; i < numVertices; i += 3) {
			Vector3 tangent = GenerateTangent(vertices[i], vertices[i + 1],
				vertices[i + 2], textureCoords[i],
				textureCoords[i + 1], textureCoords[i + 2]);

			tangents[i] += tangent;
			tangents[i + 1] += tangent;
			tangents[i + 2] += tangent;
		}
	}

	for (GLuint i = 0; i < numVertices; ++i) {
		tangents[i].normalise();
	}
}

Vector3 LegacyMesh::GenerateTangent(const Vector3 &a, const Vector3 &b,
	const Vector3 &c, const Vector2 &ta,
	const Vector2 &tb, const Vector2 &tc) {
	Vector2 coord1 = tb - ta;
	Vector2 coord2 = tc - ta;

	Vector3 vertex1 = b - a;
	Vector3 vertex2 = c - a;

	Vector3 axis = Vector3(vertex1 * coord2.y - vertex2 * coord1.y);

	float factor = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);

	return axis * factor;
}

void LegacyMesh::Draw(Shader& shader, Matrix4& worldTransform) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);




	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, (float*)&worldTransform);


	glBindVertexArray(arrayObject);
	if (bufferObject[INDEX_BUFFER]) {
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(type, 0, numVertices);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}