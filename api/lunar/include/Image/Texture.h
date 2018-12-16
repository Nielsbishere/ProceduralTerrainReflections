#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace osomi {
	class Texture {
		friend class Window;
	private:
		static std::vector<Texture*> textures;
		std::string path;
		int width, height, type;
		unsigned char* image;
		GLuint id;
		bool keepsData;
		Texture(std::string p, int w, int h, int t, GLuint i, unsigned char *data, bool keepsData=false);
	public:
		~Texture();
		int getWidth() { return width; }
		int getHeight() { return height; }
		int getType() { return type; }
		void bind(GLenum target);
		void print();
		static Texture *load(std::string path, bool keepsData=false);
		static Texture *loadCubemap(std::string path);
		static void clear();
	protected:
		unsigned char *getData() { return image; }
	};
}