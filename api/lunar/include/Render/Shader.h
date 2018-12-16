#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "../Math/Math.h"
#include "../World/Render/Light/Light.h"

namespace osomi {
	class Shader {
		friend class Application;
		friend class Skybox;
		friend class Water;
	private:
		GLuint shader;
		Shader(GLuint _shader) : shader(_shader) {};
		static void printErr(GLuint shader);
	public:
		~Shader();
		static Shader *createShader(std::string vertPath, std::string fragPath);
		void use();
		void setMVP(mat4 m, mat4 v, mat4 p, vec3 eye);
		void setAmbientLight(vec3 v);
		void setLights(std::vector<Light> &lights);
		void setFog(vec3 col, float density);
		void setSun(vec3 dir, vec3 col, float intensity);
		void set(std::string var, mat4 m);
		void set(std::string var, vec4 v);
		void set(std::string var, vec3 v);
		void set(std::string var, vec2 v);
		void set(std::string var, float f);
		void set(std::string var, int i);
		void set(std::string var, bool i);
	protected:
		static Shader *stringToShader(std::string vert, std::string frag);
	};
};