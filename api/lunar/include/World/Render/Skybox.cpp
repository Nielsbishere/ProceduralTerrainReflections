#include "Skybox.h"

#include "../../Render/Shader.h"
#include "../../Image/Texture.h"
#include <vector>
#include <string>

#include "../../Time/Timer.h"

using namespace osomi;
using namespace std;

Skybox::Skybox(vector<Texture*> _cms, Shader *_s, GLuint _vertex, GLuint _vbo, GLuint _indices, unsigned int _vert, unsigned int _vertices, float _size) : size(_size), shader(_s), vao(_vertex), vbo(_vbo), indices(_indices), vert(_vert), vertices(_vertices), cms(_cms), rotationSpeed(1) {}

Skybox *Skybox::create(vector<string> paths, float size) {
	if (size <= 0) {
		printf("Invalid skybox size!\n");
		return nullptr;
	}

	string vert =
		"#version 450 core\r\n"
		"layout(location = 0) in vec3 position;"
		"out vec3 coord;"
		"out vec3 cpos;"
		"uniform mat4 MVP;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"void main() {"
			"gl_Position = MVP * vec4(position, 1);"
			"cpos = vec3(view * model * vec4(position, 1));"
			"coord = position;"
		"}";

	string frag =
		"#version 450 core\r\n"
		"in vec3 coord;"
		"in vec3 cpos;"
		"out vec4 color;"
		"uniform samplerCube skybox;"
		"uniform samplerCube skybox2;"
		"uniform float percentage;"
		"uniform vec3 fogColor;"
		"uniform float fogDensity;"
		"uniform vec3 horizonColor;"
		"uniform float horizon;"
		"void main() {"
			"float LOWER = -0.1;"
			"float UPPER = 0.1;"
			"vec3 c1 = texture(skybox, coord).rgb;"
			"vec3 c2 = texture(skybox2, coord).rgb;"
			"vec3 final = c1 * (1 - percentage) + c2 * percentage;"
			"float horifog = clamp((coord.y - LOWER) / (UPPER - LOWER), 0, 1);"
			"if(fogDensity > 0){"
				"float d = length(cpos);"
				"float fogFactor = clamp(1.0 / exp((d * fogDensity) * (d * fogDensity)), 0, 1);"
				"final = mix(fogColor, final, fogFactor);"
			"}"
			"if(horizon != 0)color = vec4(mix(horizonColor, final, horizon * horifog), 1);"
			"else color = vec4(final, 1);"
		"}";

	Shader *sh = Shader::stringToShader(vert, frag);
	if (sh == nullptr) {
		printf("Couldn't create skybox shader!\n");
		return nullptr;
	}

	vector<Texture*> cms;
	for (unsigned int i = 0; i < paths.size(); i++) {
		Texture *cm = Texture::loadCubemap(paths[i]);
		if (cm == nullptr)break;
		cms.push_back(cm);
	}
	if (cms.size() != paths.size()) {
		delete sh;
		for (unsigned int i = 0; i < cms.size(); i++)
			delete cms[i];
		printf("Couldn't load one of the cubemaps!\n");
		return nullptr;
	}

	GLfloat vertexData[] = {
		-1, -1, 1,
		1, -1, 1,
		1, 1, 1,
		-1, 1, 1,

		-1, -1, -1,
		1, -1, -1,
		1, 1, -1,
		-1, 1, -1
	};
	GLushort indices[] = {
		0,1,2, 2,3,0,	//Front
		4,7,6, 6,5,4,	//Back
		1,5,6, 6,2,1,	//Right
		0,3,7, 7,4,0,	//Left
		3,2,6, 6,7,3,	//Up
		0,4,5, 5,1,0	//Down
	};
	unsigned int M = 24, N = 36;

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, M * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (char*)0);

	GLuint indexId;
	glGenBuffers(1, &indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * N, indices, GL_STATIC_DRAW);

	return new Skybox(cms, sh, vao, vbo, indexId, M, N, size);
}

Skybox::~Skybox() {
	delete shader;
	glDeleteBuffers(vert * sizeof(GLfloat), &vbo);
	glDeleteBuffers(vertices * sizeof(GLushort), &indices);
	glDeleteVertexArrays(1, &vao);
	for (unsigned int i = 0; i < cms.size(); i++)
		delete cms[i];
}

void Skybox::render(mat4 v, mat4 p, vec3 eye, vec3 fogColor, float fogDensity, vec3 horizonColor, float horizon, Timer t) {
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	shader->use();
	v.setTranslate(vec3::zero());
	shader->setMVP(mat4::rotateDeg(rotation) * mat4::scale(vec3(size, size, size)), v, p, eye);
	shader->setFog(fogColor, fogDensity);
	shader->set("horizonColor", horizonColor);
	shader->set("horizon", horizon);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	int active = t.interval(cms.size(), 12);

	glActiveTexture(GL_TEXTURE0);
	cms[active]->bind(GL_TEXTURE_CUBE_MAP);
	shader->set("skybox", active);
	if (cms.size() > active+1) {
		glActiveTexture(GL_TEXTURE1);
		cms[active+1]->bind(GL_TEXTURE_CUBE_MAP);
		shader->set("skybox2", active+1);
	}

	shader->set("percentage", 1 - t.sunIntensity());

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);

	glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_SHORT, (char*)0);

	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void Skybox::update(double dt, Timer timr) {
	rotation += vec3(0, rotationSpeed*dt*timr.getGTPS()/7.5f, 0);
}