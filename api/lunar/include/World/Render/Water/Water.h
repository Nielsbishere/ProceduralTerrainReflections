#pragma once
#include <GL/glew.h>
#include "../../../Math/Math.h"

namespace osomi {
	class Shader;
	class Timer;
	class Texture;
	class Water {
	private:
		vec3 size, off;
		Shader *shader;
		GLuint vao, vbo, indices;
		unsigned int vert, vertices;

		Water(Shader *_s, GLuint _vertex, GLuint _vbo, GLuint _indices, unsigned int _vert, unsigned int _vertices, vec3 _size, vec3 _off);
	public:
		static Water *create(vec3 size, vec3 off);
		~Water();
		void render(mat4 v, mat4 p, vec3 eye, vec3 fogColor, float fogDensity, Timer t, Texture *DuDv, Texture *nmap, float waveLength, vec2 offset, float reflectiveness, vec3 sunCol, vec3 sunDir, float intensity);
		void update(double dt, Timer t);
		float getHeight() { return off.y; }
	};
}