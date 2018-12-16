#include "Water.h"

#include "../../../Render/Shader.h"
#include "../../../Image/Texture.h"

#include "../../../Time/Timer.h"

using namespace osomi;
using namespace std;

Water::Water(Shader *_s, GLuint _vertex, GLuint _vbo, GLuint _indices, unsigned int _vert, unsigned int _vertices, vec3 _size, vec3 _off) : size(_size), shader(_s), vao(_vertex), vbo(_vbo), indices(_indices), vert(_vert), vertices(_vertices), off(_off) {}

Water *Water::create(vec3 size, vec3 pos) {
	if (size.x <= 0 || size.y <= 0 || size.z <= 0) {
		printf("Invalid water size!\n");
		return nullptr;
	}

	string vert =
		"#version 450 core\r\n"
		"layout(location = 0) in vec2 position;"
		"out vec4 clipSpace;"
		"out vec2 uv;"
		"out vec3 camVec;"
		"out vec3 wpos;"
		"uniform mat4 MVP;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"uniform vec3 eye;"
		"void main() {"
			"vec4 wopos = model * vec4(position.x, 0, position.y, 1);"
			"wpos = vec3(wopos);"
			"clipSpace = projection * view * wopos;"
			"gl_Position = clipSpace;"
			"camVec = eye - wopos.xyz;"
			"float tiling = 8;"
			"uv = (position + vec2(1,1))/2 * tiling;"
		"}";

	string frag =
		"#version 450 core\r\n"
		"out vec4 color;"
		"in vec4 clipSpace;"
		"in vec2 uv;"
		"in vec3 camVec;"
		"in vec3 wpos;"
		"uniform sampler2D DuDv;"
		"uniform sampler2D normals;"
		"uniform sampler2D reflection;"
		"uniform sampler2D refraction;"
		"uniform float waveLength;"
		"uniform vec2 offset;"
		"uniform float reflectiveness;"
		"uniform vec3 sunColor=vec3(1,1,1);"
		"uniform vec3 sunLight=vec3(0,-1,0);"
		"uniform float sunIntensity=1;"
		"uniform vec3 eye;"
		"void main() {"
			"vec2 uv2 = clipSpace.xy / clipSpace.w;"
			"uv2 = (uv2 + vec2(1,1)) / 2;"
			"vec2 dudv = texture(DuDv, vec2(uv.x + offset.x, uv.y)).rg*0.1;"
			"vec2 distorted = uv + vec2(dudv.x, dudv.y + offset.y);"
			"dudv = (texture(DuDv, distorted).rg * 2.0 - 1.0) / waveLength;"
			"vec2 reflec = vec2(uv2.x, -uv2.y);"
			"vec2 refrac = vec2(uv2.x, uv2.y);"
			"reflec += dudv / waveLength;"
			"refrac += dudv / waveLength;"
			"refrac = clamp(refrac, 0.001, 0.999);"
			"reflec.x = clamp(reflec.x, 0.001, 0.999);"
			"reflec.y = clamp(reflec.y, -0.999, -0.001);"
			"vec4 refl = texture(reflection, reflec);"
			"vec4 refr = texture(refraction, refrac);"
			"float refractive = pow(clamp(dot(normalize(camVec), vec3(0, 1, 0)), 0, 1), reflectiveness);"
			"vec4 final = refl * (1 - refractive) + refr * refractive;"
			"vec3 normal = texture(normals, distorted).rgb;"
			"normal = normalize(vec3(normal.r, 0, normal.g) * 2 - vec3(1, -normal.b, 1));"
			"vec3 reflected = .5f * reflectiveness * sunIntensity * sunColor * pow(clamp(dot(reflect(normalize(sunLight), normal), normalize(eye - wpos)), 0, 1), 10);"
			"color = final + vec4(reflected, 0);"
		"}";

	Shader *sh = Shader::stringToShader(vert, frag);
	if (sh == nullptr) {
		printf("Couldn't create skybox shader!\n");
		return nullptr;
	}
	GLfloat vertexData[] = {
		-1, -1,
		1, -1,
		1, 1,
		-1, 1
	};
	GLushort indices[] = {
		0,1,2, 2,3,0
	};
	unsigned int M = 8, N = 6;

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, M * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (char*)0);

	GLuint indexId;
	glGenBuffers(1, &indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * N, indices, GL_STATIC_DRAW);

	return new Water(sh, vao, vbo, indexId, M, N, size, pos);
}

Water::~Water() {
	delete shader;
	glDeleteBuffers(vert * sizeof(GLfloat), &vbo);
	glDeleteBuffers(vertices * sizeof(GLushort), &indices);
	glDeleteVertexArrays(1, &vao);
}

void Water::render(mat4 v, mat4 p, vec3 eye, vec3 fogColor, float fogDensity, Timer t, Texture *DuDv, Texture *nmap, float waveLength, vec2 offset, float reflectiveness, vec3 sunCol, vec3 sunDir, float intensity) {
	glDisable(GL_CULL_FACE);
	shader->use();
	glActiveTexture(GL_TEXTURE0);
	DuDv->bind(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	nmap->bind(GL_TEXTURE_2D);
	shader->set("DuDv", 0);
	shader->set("normals", 1);
	shader->set("reflection", 30);
	shader->set("refraction", 31);
	shader->set("waveLength", waveLength);
	shader->set("offset", offset);
	shader->set("reflectiveness", reflectiveness);
	shader->set("sunColor", sunCol);
	shader->set("sunLight", sunDir);
	shader->set("sunIntensity", intensity);
	shader->setMVP(mat4::translate(off) * mat4::scale(size), v, p, eye);
	shader->setFog(fogColor, fogDensity);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);

	glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_SHORT, (char*)0);
	glEnable(GL_CULL_FACE);
}

void Water::update(double dt, Timer timr) {

}