#pragma once

#include <GL/glew.h>

#include "../../Math/Math.h"
#include <vector>

namespace osomi {
	class Mesh;
	class Shader;
	class Texture;
	class WorldMesh {
	private:
		Mesh **meshes;
		float *heightMap;
		unsigned int chunkW, chunkH;
		vec3 size;
		static void init(float *hm, unsigned int tw, unsigned int th, unsigned int w, unsigned int h, unsigned int s, unsigned int e, SimplexNoise *sn, float *mi, float *ma, float noiseScale, unsigned int octaves, float persistance, float lacunarity);
		static void avg(float *hm, unsigned int tw, unsigned int th, unsigned int w, unsigned int h, unsigned int s, unsigned int e, float mi, float ma);
		static void mesh(float *hm, unsigned int tw, unsigned int th, unsigned int w, unsigned int h, unsigned int s, unsigned int e, vec3 size, float offy, float uvx, float uvy, GLfloat **dat, GLushort **tris);
		WorldMesh(float *heightMap, Mesh **meshes, vec3 sz, unsigned int cw, unsigned int ch);
	public:
		//Creates a WorldMesh; arguments
		//ChunksX		(Minimum 1)
		//ChunksY		(Minimum 1)
		//noise scale	(/32.f) default = 1 / 32.f (minimum 0.0001)
		//quality		(base2)	Nodes per chunk; minimum 32, maximum 128
		//height		(Minimum 1)
		//textureDetail	(dividable by 2, minimum 2) the times a texture loops around the mesh
		//octaves		(Minimum 1) quality of noise function; with hi-res meshes go for more octaves and lower quality less octaves.
		//persistance	(Minimum 0.0001) noise argument for changing amplitude
		//lacunarity	(Minimum 1) noise argument for changing frequency
		//
		//Limit: When (chunkY*quality+1)*(chunkX*quality+1) reaches beyond UINT_MAX
		static WorldMesh *create(unsigned int w, unsigned int h, float nscale=1, unsigned int quality=128, float height=10, float offy=0, unsigned int textureDetail=2, unsigned int octaves=8, float persistance=.5, float lacunarity=2);
		~WorldMesh();
		void render(Shader *s, mat4 v, mat4 p, vec3 eye, Texture *grass, Texture *stone, Texture *snow, vec4 plane, bool reverse=false);
	};
}