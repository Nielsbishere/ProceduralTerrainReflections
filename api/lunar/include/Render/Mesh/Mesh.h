#pragma once
#include <GL/glew.h>
#include "../../Image/Texture.h"

#include "../Shader.h"

namespace osomi {
	class vec4;
	class Texture;
	class Mesh {
		friend class ObjLoader;
		friend class WorldMesh;
		friend class Application;
	private:
		GLuint vertex, vbo, indices;
		unsigned int vert, vertices;
		float miX, miY, miZ, maX, maY, maZ;
		Texture *t;

		Mesh(GLuint _vertex, GLuint _vbo, GLuint _indices, unsigned int _vert, unsigned int _vertices, float _miX, float _miY, float _miZ, float _maX, float _maY, float _maZ, Texture *t);
	protected:
		static Mesh *create(GLfloat *triangle, unsigned int M, GLushort *indices, unsigned int N, float miX, float miY, float miZ, float maX, float maY, float maZ, Texture *t, bool recalculateNormals=false);
		static Mesh *cube;
	public:
		~Mesh();
		vec4 getMin();
		vec4 getMax();

		template<unsigned int M, unsigned int N> static Mesh *create(GLfloat(&triangle)[M], GLushort(&indices)[N], Texture *t) {
			return create(triangle, M, indices, N, 0, 0, 0, 0, 0, 0, t);
		}

		static Mesh *getCube() { return cube; }
		void render(Shader *s, GLenum target = GL_TEXTURE_2D, Texture *rep = nullptr);
	};
}