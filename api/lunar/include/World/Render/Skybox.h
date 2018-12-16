#pragma once
#include <GL/glew.h>
#include "../../Math/Math.h"
#include <vector>
#include <string>
namespace osomi {
	class Shader;
	class Cubemap;
	class Texture;
	class Timer;
	class Skybox {
	private:
		float size;
		Shader *shader;
		GLuint vao, vbo, indices;
		unsigned int vert, vertices;

		float rotationSpeed;

		std::vector<Texture*> cms;

		vec3 rotation;

		Skybox(std::vector<Texture*> _cms, Shader *_s, GLuint _vertex, GLuint _vbo, GLuint _indices, unsigned int _vert, unsigned int _vertices, float _size);
	public:
		static Skybox *create(std::vector<std::string> paths, float size);
		~Skybox();
		void render(mat4 v, mat4 p, vec3 eye, vec3 fogColor, float fogDensity, vec3 horizonColor, float horizon, Timer t);
		void update(double dt, Timer t);
		void setRotationSpeed(float rspeed) { rotationSpeed = rspeed; }
	};
}