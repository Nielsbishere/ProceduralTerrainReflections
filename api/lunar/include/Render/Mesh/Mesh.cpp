#include "Mesh.h"
#include "../../Image/Texture.h"
#include "../../Math/Math.h"

#include "../../Helper/StringHelper.h"

#include <GL/glew.h>

using namespace osomi;
using namespace std;

Mesh *Mesh::cube = nullptr;

Mesh::Mesh(GLuint _vertex, GLuint _vbo, GLuint _indices, unsigned int _vert, unsigned int _vertices, float _miX, float _miY, float _miZ, float _maX, float _maY, float _maZ, Texture *_t) :
	vertex(_vertex), vbo(_vbo), indices(_indices), vertices(_vertices), vert(_vert), miX(_miX), miY(_miY), miZ(_miZ), maX(_maX), maY(_maY), maZ(_maZ), t(_t){}

Mesh *Mesh::create(GLfloat *triangle, unsigned int M, GLushort *indices, unsigned int N, float miX, float miY, float miZ, float maX, float maY, float maZ, Texture *t, bool recalculateNormals) {
	if (N % 3 != 0) {
		printf("Mesh loading error: indices should use triangles!\n");
		return nullptr;
	}
	if (M % 8 != 0) {
		printf("Mesh loading error: elements supposed to have 8 floats!\n");
		return nullptr;
	}

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, M * sizeof(GLfloat), triangle, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLsizei posl = sizeof(GLfloat) * 3;
	GLsizei coll = sizeof(GLfloat) * 2;
	GLsizei normall = sizeof(GLfloat) * 3;
	GLsizei stride = posl + coll + normall;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (char*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (char*)posl);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (char*)(posl + coll));

	GLuint indexId;
	glGenBuffers(1, &indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * N, indices, GL_STATIC_DRAW);

	return new Mesh(vao, vbo, indexId, M, N, miX, miY, miZ, maX, maY, maZ, t);
}
Mesh::~Mesh() {
	glDeleteBuffers(vert * sizeof(GLfloat), &vbo);
	glDeleteBuffers(vertices * sizeof(GLushort), &indices);
	glDeleteVertexArrays(1, &vertex);
}
void Mesh::render(Shader *s, GLenum target, Texture *rep) {
	s->use();
	if(rep != nullptr)rep->bind(target);
	else if(t != nullptr)t->bind(target);
	if (target >= GL_TEXTURE0 && target <= GL_TEXTURE31) 
		s->set(StringHelper::numString(target - GL_TEXTURE0, "t", ""), (int)(target - GL_TEXTURE0));
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_SHORT, 0);
}

vec4 Mesh::getMin() { return vec4(miX, miY, miZ, 1); }
vec4 Mesh::getMax() { return vec4(maX, maY, maZ, 1); }