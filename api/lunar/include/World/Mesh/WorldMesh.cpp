#include "WorldMesh.h"
#include "../../Render/Mesh/Mesh.h"

#include <GL/glew.h>

#include <thread>

using namespace osomi;
using namespace std;
WorldMesh::WorldMesh(float *_heightMap, Mesh **_meshes, vec3 sz, unsigned int cw, unsigned int ch): heightMap(_heightMap), meshes(_meshes), size(sz), chunkW(cw), chunkH(ch) {}

void WorldMesh::init(float *hm, const unsigned int tw, unsigned int th, unsigned int w, unsigned int h, unsigned int s, unsigned int e, SimplexNoise *sn, float *mi, float *ma, float noiseScale, unsigned int octaves, float persistance, float lacunarity) {
	for (unsigned int i = s; i < e; i++) {
		unsigned int cx = i % w;
		unsigned int cy = (i / w) % h;

		unsigned int xmax = tw + (cx == w - 1 ? 1 : 0);
		unsigned int ymax = th + (cy == h - 1 ? 1 : 0);

		for (unsigned int jx = 0; jx < xmax; jx++)
			for (unsigned int jy = 0; jy < ymax; jy++) {
				unsigned int x = cx * tw + jx;
				unsigned int y = cy * th + jy;

				unsigned int k = y * (w * tw + 1) + x;

				float f = hm[k] = sn->tnoise((float)x / tw * noiseScale, (float)y / th * 2 * noiseScale, 1, octaves, persistance, lacunarity);
				if (f < *mi)*mi = f;
				if (f > *ma)*ma = f;
			}
	}
}
void WorldMesh::avg(float *hm, const unsigned int tw, unsigned int th, unsigned int w, unsigned int h, unsigned int s, unsigned int e, float mi, float ma) {
	for (unsigned int i = s; i < e; i++) {
		unsigned int cx = i % w;
		unsigned int cy = (i / w) % h;

		unsigned int xmax = tw + (cx == w - 1 ? 1 : 0);
		unsigned int ymax = th + (cy == h - 1 ? 1 : 0);

		for (unsigned int jx = 0; jx < xmax; jx++)
			for (unsigned int jy = 0; jy < ymax; jy++) {
				unsigned int x = cx * tw + jx;
				unsigned int y = cy * th + jy;

				unsigned int k = y * (w * tw + 1) + x;

				hm[k] = (hm[k] - mi) / (ma - mi);
			}
	}
}
void WorldMesh::mesh(float *hm, const unsigned int tw, unsigned int th, unsigned int w, unsigned int h, unsigned int s, unsigned int e, vec3 size, float offy, float uvx, float uvy, GLfloat **dat, GLushort **tris) {
	unsigned int m = 8 * (tw + 1) * (th + 1);
	unsigned int n = 6 * tw * th;
	for (unsigned int i = s; i < e; i++) {
		unsigned int cx = i % w;
		unsigned int cy = (i / w) % h;

		for (unsigned int jx = 0; jx < tw+1; jx++)
			for (unsigned int jy = 0; jy < th+1; jy++) {
				unsigned int x = cx * tw + jx;
				unsigned int y = cy * th + jy;

				unsigned int k = y * (w * tw + 1) + x;

				float xperc = (float)jx / (tw + 1);
				float yperc = (float)jy / (th + 1);
				
				dat[i][8 * (jy * (tw + 1) + jx)] = ((float)jx / tw * 2 - 1) * size.x;
				dat[i][8 * (jy * (tw + 1) + jx) + 1] = (hm[k] * 2 - 1) * size.y + offy;
				dat[i][8 * (jy * (tw + 1) + jx) + 2] = ((float)jy / th * 2 - 1) * size.z;
				dat[i][8 * (jy * (tw + 1) + jx) + 3] = xperc * uvx;
				dat[i][8 * (jy * (tw + 1) + jx) + 4] = yperc * uvy;
				dat[i][8 * (jy * (tw + 1) + jx) + 5] = 0;
				dat[i][8 * (jy * (tw + 1) + jx) + 6] = 0;
				dat[i][8 * (jy * (tw + 1) + jx) + 7] = 0;

				if (jx < tw && jy < th) {
					unsigned int trindex = jy * tw + jx;

					tris[i][6 * trindex] = jy * (tw + 1) + jx;
					tris[i][6 * trindex + 1] = jy * (tw + 1) + jx + 1;
					tris[i][6 * trindex + 2] = (jy + 1) * (tw + 1) + jx + 1;
					tris[i][6 * trindex + 3] = (jy + 1) * (tw + 1) + jx + 1;
					tris[i][6 * trindex + 4] = (jy + 1) * (tw + 1) + jx;
					tris[i][6 * trindex + 5] = jy * (tw + 1) + jx;
				}
			}

		for (unsigned int x = 0; x < tw; x++)
			for (unsigned int y = 0; y < th; y++) {
				unsigned int k = y * tw + x;
				vec3 p0 = *(vec3*)(&dat[i][8 * tris[i][k * 6]]);
				vec3 p1 = *(vec3*)(&dat[i][8 * tris[i][k * 6 + 1]]);
				vec3 p2 = *(vec3*)(&dat[i][8 * tris[i][k * 6 + 2]]);
				vec3 p3 = *(vec3*)(&dat[i][8 * tris[i][k * 6 + 4]]);

				vec3 t0 = (p0 - p1).cross(p2 - p1).normalize();
				vec3 t1 = (p2 - p3).cross(p0 - p3).normalize();

				t0 *= (p0 - p1).magnitude() * (p2 - p1).magnitude() / 2;
				t1 *= (p2 - p3).magnitude() * (p0 - p3).magnitude() / 2;

				vec3 &n0 = *(vec3*)(&dat[i][8 * tris[i][k * 6] + 5]);
				vec3 &n1 = *(vec3*)(&dat[i][8 * tris[i][k * 6 + 1] + 5]);
				vec3 &n2 = *(vec3*)(&dat[i][8 * tris[i][k * 6 + 2] + 5]);
				vec3 &n3 = *(vec3*)(&dat[i][8 * tris[i][k * 6 + 4] + 5]);

				n0 += t0;
				n1 += t0;
				n2 += t0;
				n2 += t1;
				n3 += t1;
				n0 += t1;
			}

		//Normalizing normals (foreach vertex)
		for (unsigned int x = 0; x < tw+1; x++)
			for (unsigned int y = 0; y < th+1; y++) {
				unsigned int k = y * (tw+1) + x;
				vec3 &n = *(vec3*)(&dat[i][8 * k + 5]);
				n = n.normalize();
			}
	}
}

WorldMesh *WorldMesh::create(unsigned int w, unsigned int h, float nscale, unsigned int quality, float height, float offy, unsigned int textureDetail, unsigned int octaves, float persistance, float lacunarity) {
	if (w == 0 || h == 0) {
		printf("Couldn't create an empty world!\n");
		return nullptr;
	}
	if (quality != 32 && quality != 64 && quality != 128 && quality != 256 && quality != 512 && quality != 1024) {
		printf("Couldn't create world with quality %u! Supported: 32,64,128,256,512,1024.\n", quality);
		return nullptr;
	}
	if (nscale < 0.0001) {
		printf("Warning; world mesh requested scale of < 0.0001; %f: Clamping...\n", nscale);
		nscale = 0.0001;
	}
	if (height < 1) {
		printf("Warning; world mesh requested height of < 1; %f: Clamping...\n", height);
		height = 1;
	}
	if (textureDetail < 2 || textureDetail % 2 != 0) {
		printf("Warning; world mesh requested texture detail of %!= 2 or < 2; %u: Clamping...\n", textureDetail);
		if (textureDetail > 2)textureDetail = (textureDetail / 2) * 2;
		else textureDetail = 2;
	}
	if (octaves < 1) {
		printf("Warning; world mesh requested < 1 octaves! Clamping...\n");
		octaves = 1;
	}
	if (persistance < .0001) {
		printf("Warning; world mesh requested persistance of < .0001; %f: Clamping...\n");
		persistance = .0001;
	}
	if (lacunarity < 1) {
		printf("Warning; world mesh requested lacunarity of < 1; %f: Clamping...\n");
		lacunarity = 1;
	}
	unsigned int tw = quality, th = quality;
	if (tw > 128)
		tw = th = 128;
	vec3 size = vec3(4, height, 4);
	if (quality > 128) {
		size.x = size.z /= (quality / 128);
		nscale /= (quality / 128);
	}
	const float uvx = size.x * textureDetail, uvy = size.z * textureDetail;

	if (((double)0xFFFFFFFF + 1) / tw / th / w / h <= 1) {
		printf("Couldn't create a world! Out of bounds exception!\n");
		return nullptr;
	}

	float *hm = new float[(tw*w + 1)*(th*h + 1)];
	SimplexNoise *sn = new SimplexNoise();

	int currency = thread::hardware_concurrency();
	if (currency == 1)currency = 0;
	//currency = 0;
	int thrnum = currency < 1 ? 1 : currency;
	vector<float> mima(thrnum * 2);

	for (int i = 0; i < thrnum; i++) {
		mima[2 * i] = numeric_limits<float>::max();
		mima[2 * i + 1] = numeric_limits<float>::min();
	}

	int tclock = clock();

	vector<thread*> thrs(currency);
	unsigned int cpthr, cleft;

	if (currency < 1) {
		init(hm, tw, th, w, h, 0, w*h, sn, &mima[0], &mima[1], 1 / 32.f * nscale, octaves, persistance, lacunarity);
		printf("Finished generating heightmap within %ums with 1 core.\n", clock() - tclock);
		tclock = clock();
	}
	else {
		cpthr = (w * h) / currency;
		cleft = (w * h) - cpthr * currency;
		for (unsigned int i = 0; i < currency; i++) {
			unsigned int s = cpthr * i;
			unsigned int e = cpthr * (i + 1) + (i == currency - 1 ? cleft : 0);
			float *mi = &mima[2 * i];
			float *ma = &mima[2 * i + 1];
			float noise = nscale / 32.f;
			thrs[i] = new thread(&WorldMesh::init, hm, tw, th, w, h, s, e, sn, mi, ma, noise, octaves, persistance, lacunarity);
		}
		for (unsigned int i = 0; i < currency; i++) {
			thrs[i]->join();
			delete thrs[i];
		}
		printf("Finished generating heightmap within %ums with %i core(s).\n", clock() - tclock, currency);
		tclock = clock();
	}
	delete sn;

	float mi = numeric_limits<float>::max();
	float ma = numeric_limits<float>::min();
	for (unsigned int i = 0; i < thrnum; i++) {
		if (mima[2 * i] < mi)mi = mima[2 * i];
		if (mima[2 * i + 1] > ma)ma = mima[2 * i + 1];
	}

	if (currency < 1) {
		avg(hm, tw, th, w, h, 0, w*h, mi, ma);
		printf("Finished averaging heightmap within %ums with 1 core.\n", clock() - tclock);
		tclock = clock();
	}else{
		for (unsigned int i = 0; i < currency; i++) {
			unsigned int s = cpthr * i;
			unsigned int e = cpthr * (i + 1) + (i == currency - 1 ? cleft : 0);
			thrs[i] = new thread(&WorldMesh::avg, hm, tw, th, w, h, s, e, mi, ma);
		}
		for (unsigned int i = 0; i < currency; i++) {
			thrs[i]->join();
			delete thrs[i];
		}
		printf("Finished averaging heightmap within %ums with %i core(s).\n", clock() - tclock, currency);
		tclock = clock();
	}

	unsigned int m = 8 * (tw + 1) * (th + 1);
	unsigned int n = 6 * tw * th;
	GLfloat **dat = new GLfloat*[w*h];
	unsigned short **tri = new unsigned short*[w*h];
	for (unsigned int i = 0; i < w*h; i++) {
		dat[i] = new GLfloat[m];
		tri[i] = new unsigned short[n];
	}
	if (currency < 1) {
		mesh(hm, tw, th, w, h, 0, w*h, size, offy, uvx, uvy, dat, tri);
		printf("Finished generating mesh data within %ums with 1 core.\n", clock() - tclock);
		tclock = clock();
	}else {
		for (unsigned int i = 0; i < currency; i++) {
			unsigned int s = cpthr * i;
			unsigned int e = cpthr * (i + 1) + (i == currency - 1 ? cleft : 0);
			thrs[i] = new thread(&WorldMesh::mesh, hm, tw, th, w, h, s, e, size, offy, uvx, uvy, dat, tri);
		}
		for (unsigned int i = 0; i < currency; i++) {
			thrs[i]->join();
			delete thrs[i];
		}
		printf("Finished generating mesh data within %ums with %i core(s).\n", clock() - tclock, currency);
		tclock = clock();
	}

	//Not everyone has 1 TB of memory, please hold the memory only when necessary

	Mesh **mshs = new Mesh*[w*h];
	for (unsigned int i = 0; i < w*h; i++) {
		mshs[i] = Mesh::create(dat[i], m, tri[i], n, -size.x, -size.y, -size.z, size.x, size.y, size.z, nullptr);
		delete[] dat[i];
		delete[] tri[i];
	}
	delete[] dat;
	delete[] tri;

	return new WorldMesh(hm, mshs, size, w, h);
}
WorldMesh::~WorldMesh() {
	delete[] heightMap;
	for (unsigned int i = 0; i < chunkW*chunkH; i++)
		delete meshes[i];
	delete[] meshes;
}
void WorldMesh::render(Shader *s, mat4 v, mat4 p, vec3 eye, Texture *grass, Texture *stone, Texture *snow, vec4 plane, bool reverse) {
	if (grass == nullptr || stone == nullptr || snow == nullptr)return;
	if (plane != vec4(9000.1f, 9000.1f, 9000.1f, 9000.1f)) {
		glEnable(GL_CLIP_DISTANCE0);
		s->set("plane", plane);
	}
	s->set("t", 0);
	glActiveTexture(GL_TEXTURE0);
	grass->bind(GL_TEXTURE_2D);
	s->set("t1", 1);
	glActiveTexture(GL_TEXTURE1);
	stone->bind(GL_TEXTURE_2D);
	s->set("t2", 2);
	glActiveTexture(GL_TEXTURE2);
	snow->bind(GL_TEXTURE_2D);
	if(!reverse)glCullFace(GL_FRONT);
	else glCullFace(GL_BACK);
	for (unsigned int i = 0; i < chunkH*chunkW; i++) {
		unsigned int cx = i % chunkW;
		unsigned int cy = (i / chunkW) % chunkH;
		vec3 vc = vec3();
		vc.x = 2 * size.x * cx;
		vc.z = 2 * size.z * cy;
		
		s->setMVP(mat4::translate(vc), v, p, eye);
		meshes[i]->render(s);
	}
}