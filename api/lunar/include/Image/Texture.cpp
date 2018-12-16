#include "Texture.h"
#include "../Math/Math.h"

#include "../Helper/StringHelper.h"

#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
	#include "../API/stb/stb_image.h"
#endif

#include <iostream>

using namespace osomi;
using namespace std;

vector<Texture*> Texture::textures = vector<Texture*>();

Texture::Texture(string p, int w, int h, int t, GLuint i, unsigned char *data, bool dat) : path(p), width(w), height(h), type(t), id(i), image(data), keepsData(dat) {}
Texture::~Texture() {
	printf("Deleting texture: ");
	print();
	glDeleteTextures(1, &id);
	if(keepsData)stbi_image_free(image);
	for (unsigned int i = 0; i < textures.size(); i++)
		if (textures[i]->path == path) {
			textures.erase(textures.begin() + i);
			break;
		}
}
void Texture::bind(GLenum target) {
	glBindTexture(target, id);
}
void Texture::print() {
	printf("%s %i %i %i %u %p %i\n", path.c_str(), width, height, type, id, image, keepsData);
}
Texture *Texture::load(string path, bool keepsData) {
	for (unsigned int i = 0; i < textures.size(); i++) 
		if (textures[i]->path == path)
			return textures[i];

	int width, height, type;
	unsigned char *image = stbi_load(path.c_str(), &width, &height, &type, STBI_rgb_alpha);

	if (image == nullptr) {
		printf("Couldn't load image!\n");
		return nullptr;
	}

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	if (type == 3 || type == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	else {
		glDeleteTextures(1, &id);
		stbi_image_free(image);
		printf("Image had invalid format! (Only supporting RGBA and RGB)\n");
		return nullptr;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 

	float ani = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &ani);
	if (ani > 1) {
		ani = Math::max(4, ani);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, ani);
	}

	Texture *t = new Texture(path, width, height, type, id, keepsData ? image : nullptr, keepsData);
	textures.push_back(t);

	if(!keepsData)
		stbi_image_free(image);

	printf("Loaded texture: ");
	t->print();

	return t;
}
Texture *Texture::loadCubemap(string path) {
	string str[] = {path + "_r.png", path + "_l.png", path + "_u.png", path + "_d.png", path + "_f.png", path + "_b.png"};

	GLuint id;
	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	for (unsigned int i = 0; i < 6; i++) {
		int width, height, type;
		unsigned char *image = stbi_load(str[i].c_str(), &width, &height, &type, STBI_rgb);

		if (image == nullptr) {
			printf("Couldn't load image!\n");
			return nullptr;
		}

		GLenum pos = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;

		if (type == 3) glTexImage2D(pos, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else {
			glDeleteTextures(1, &id);
			stbi_image_free(image);
			printf("Image had invalid format! (Only supporting RGB)\n");
			return nullptr;
		}
		stbi_image_free(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return new Texture(path, -1, -1, 3, id, nullptr);
}
void Texture::clear() {
	for (unsigned int i = 0; i < textures.size(); i++)
		delete textures[i];
	textures.clear();
}