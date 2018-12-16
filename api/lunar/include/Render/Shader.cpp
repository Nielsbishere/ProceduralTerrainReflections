#include "Shader.h"

#include "../Helper/StringHelper.h"

using namespace osomi;
using namespace std;

void Shader::printErr(GLuint shader) {
	GLint maxLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	vector<GLchar> errorLog(maxLength);
	glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

	printf("Error: ");
	for (unsigned int i = 0; i < errorLog.size(); i++)printf("%c", errorLog[i]);
	printf("\n");
}
Shader::~Shader() {
	glDeleteProgram(shader);
}
void Shader::use() {
	glUseProgram(shader);
}
void Shader::set(string s, mat4 m) {
	GLuint loc = glGetUniformLocation(shader, s.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}
void Shader::set(string s, vec4 v) {
	GLuint loc = glGetUniformLocation(shader, s.c_str());
	glUniform4fv(loc, 1, &v[0]);
}
void Shader::set(string s, vec3 v) {
	GLuint loc = glGetUniformLocation(shader, s.c_str());
	glUniform3fv(loc, 1, &v[0]);
}
void Shader::set(string s, vec2 v) {
	GLuint loc = glGetUniformLocation(shader, s.c_str());
	glUniform2fv(loc, 1, &v[0]);
}
void Shader::set(string s, float f) {
	GLuint loc = glGetUniformLocation(shader, s.c_str());
	glUniform1f(loc, f);
}
void Shader::set(string s, int i) {
	GLuint loc = glGetUniformLocation(shader, s.c_str());
	glUniform1i(loc, i);
}
void Shader::set(string s, bool b) {
	GLuint loc = glGetUniformLocation(shader, s.c_str());
	glUniform1i(loc, b);
}
void Shader::setMVP(mat4 m, mat4 v, mat4 p, vec3 eye) {
	set("model", m);
	set("view", v);
	set("projection", p);
	set("eye", eye);
	mat4 mvp = p * v * m;
	set("MVP", mvp);
}
void Shader::setAmbientLight(vec3 v) {
	set("ambient", v);
}
void Shader::setLights(vector<Light> &lights) {
	unsigned int MAX = 16;
	set("actualLights", Math::clamp(lights.size(), 0, MAX));
	for (unsigned int i = 0; i < MAX && i < lights.size(); i++) {
		Light &l = lights[i];
		string position = StringHelper::numString(i, "lights[", "].position");
		string color = StringHelper::numString(i, "lights[", "].color");
		string power = StringHelper::numString(i, "lights[", "].power");
		set(position, l.getPosition());
		set(color, l.getColor());
		set(power, l.getPower());
	}
}
void Shader::setFog(vec3 col, float density) {
	if (density > 0) {
		set("fogColor", col);
		set("fogDensity", density);
	}
}
void Shader::setSun(vec3 dir, vec3 col, float intensity) {
	set("sunLight", dir);
	set("sunColor", col);
	set("sunIntensity", intensity);
}
Shader *Shader::createShader(string vertPath, string fragPath) {
	if (vertPath == "" || fragPath == "" || !StringHelper::endsWith(vertPath, ".glsl") || !StringHelper::endsWith(fragPath, ".glsl")) {
		printf("The file wasn't a .glsl file!\n");
		return nullptr;
	}
	string vert = StringHelper::fromFile(vertPath), frag = StringHelper::fromFile(fragPath);
	if (vert == "" || frag == "") {
		printf("Couldn't read the file!\n");
		return nullptr;
	}
	return stringToShader(vert, frag);
}
Shader *Shader::stringToShader(string vert, string frag) {
	const char *vertexShader = vert.c_str(), *fragmentShader = frag.c_str();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShader, NULL);
	glCompileShader(vs);
	GLint vsCompile;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &vsCompile);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShader, NULL);
	glCompileShader(fs);
	GLint fsCompile;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &fsCompile);

	if (vsCompile != GL_TRUE || fsCompile != GL_TRUE) {
		printf("Couldn't compile the %s shader!\n", vsCompile != GL_TRUE && fsCompile != GL_TRUE ? "vertex and fragment" : (vsCompile != GL_TRUE ? "vertex" : "fragment"));
		if (vsCompile != GL_TRUE) {
			printf("Vert info:\n%s\n", vertexShader);
			printErr(vs);
		}
		if (fsCompile != GL_TRUE) {
			printf("Frag info:\n%s\n", fragmentShader);
			printErr(fs);
		}
		glDeleteShader(vs);
		glDeleteShader(fs);
		return nullptr;
	}
	GLuint shader = glCreateProgram();
	glAttachShader(shader, fs);
	glAttachShader(shader, vs);
	glLinkProgram(shader);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(shader);
	printf("Successfully added a shader!\n");

	return new Shader(shader);
}