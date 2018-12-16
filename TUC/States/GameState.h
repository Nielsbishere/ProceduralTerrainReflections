#pragma once
#include <Application/Window.h>
#include <Application/Application.h>
#include <Application/State.h>

#include <Render/Mesh/Mesh.h>
#include <Image/Texture.h>
#include <World/Mesh/WorldMesh.h>

#include <World/Render/Skybox.h>
#include <World/Render/Water/Water.h>

#include <Time/Timer.h>

#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

namespace osomi {
	class GameState : public State {
	private:
		Shader *s;
		Texture *t, *t1, *t2, *DuDv, *normals;
		WorldMesh *wm;
		Skybox *sb;
		Timer *timr;
		Water *w;
	protected:
		virtual ~GameState() {
			if (s != nullptr) delete s;
			if (wm != nullptr) delete wm;
			if (sb != nullptr) delete sb;
			if (w != nullptr) delete w;
			delete timr;
		}
	public:
		GameState() {
			srand(time(0));
			s = Shader::createShader("Shaders/terrainvert.glsl", "Shaders/terrainfrag.glsl");

			vector<string> cubemaps;
			cubemaps.push_back("Skyboxes/day");
			cubemaps.push_back("Skyboxes/night");
			sb = Skybox::create(cubemaps, 1);
			wm = WorldMesh::create(16, 32, 4, 128, 20, 0, 4, 8);

			w = Water::create(vec3(16 * 4 * 2, 1, 32 * 4 * 2), vec3(16 * 4, 0, 32 * 4));

			sb->setRotationSpeed(.05);
			timr = new Timer(60, 5 * 60 * 60);

			t = Texture::load("Grass.png");
			t1 = Texture::load("Stone.png");
			t2 = Texture::load("Snow.png");
			DuDv = Texture::load("Water DuDv.png");
			normals = Texture::load("Water NM.png");
		}

		bool init = false;
		void update(double delta) {
			if (!init) {
				a->setMSAA(4);
				init = true;
				a->getWindow()->disableCursor();
				return;
			}


			float forwardSpeed = 32, straifeSpeed = 16, backwardSpeed = 16, verticalSpeed = 16;

			if (a->getKeyHandler()->isDown(EKeyButton::W)) 
				a->moveCamera(vec3(0, 0, delta * forwardSpeed));
			if (a->getKeyHandler()->isDown(EKeyButton::S))
				a->moveCamera(vec3(0, 0, -delta * backwardSpeed));

			if (a->getKeyHandler()->isDown(EKeyButton::D))
				a->moveCamera(vec3(delta * straifeSpeed, 0, 0));
			if (a->getKeyHandler()->isDown(EKeyButton::A))
				a->moveCamera(vec3(-delta * straifeSpeed, 0, 0));

			if (a->getKeyHandler()->isDown(EKeyButton::SPACE))
				a->moveCamera(vec3(0, delta * verticalSpeed, 0));
			if (a->getKeyHandler()->isDown(EKeyButton::Q))
				a->moveCamera(vec3(0, -delta * verticalSpeed, 0));

			if (sb != nullptr)sb->update(delta, *timr);
			timr->update(delta);

			a->moveFocus(delta);
		}
		void render(mat4 v, mat4 p, vec3 eye, bool renderWater, bool renderParticles, vec4 plane) {
			int w, h;
			a->getWindow()->getSize(w, h);

			vec3 fogColor = vec3(.5, .5, .5);
			float fogDensity = 0.002f;
			float intensity = timr->sunIntensity();
			float moonLight = .2;
			vec3 sunCol = vec3(1, 1, 1);
			float startHorizon = 5, endHorizon = 7;
			bool hasHorizon = false;
			if (timr->getFGameHours() >= startHorizon && timr->getFGameHours() < endHorizon) {
				float perc = (timr->getFGameHours() - startHorizon) / (endHorizon - startHorizon);
				float val = sin(M_PI * perc);
				sunCol = vec3(1, 1, 1) * (1 - val) + vec3(.95, .5, .1) * val;
				hasHorizon = true;
			}
			vec3 sunDir = vec3(0.5, -1, sin(M_PI * intensity * (timr->getUGameHours() < 12 ? 1 : -1)));

			if (sb != nullptr && renderParticles)sb->render(v, p, eye, fogColor, fogDensity, sunCol, hasHorizon ? 1 - intensity : 0, *timr);


			if (this->w != nullptr && renderWater && renderParticles)
				this->w->render(v, p, eye, fogColor, fogDensity, *timr, DuDv, normals, 16, vec2(timr->getFGameHours() / 6.f, -timr->getFGameHours() / 6.f), 0.5, sunCol, sunDir, intensity);

			s->use();

			s->setAmbientLight(vec3(.1, .1, .1));
			s->setFog(fogColor, fogDensity);
			vector<Light> vl = vector<Light>();
			s->setLights(vl);
			s->setSun(sunDir, sunCol, intensity * (1 - moonLight) + moonLight);

			t->bind(GL_TEXTURE_2D);
			if (!renderWater)
				glDisable(GL_CULL_FACE);
			if (wm != nullptr)
				wm->render(s, v, p, eye, t, t1, t2, plane != vec4(9000.1f, 9000.1f, 9000.1f, 9000.1f) ? plane : (this->w->getHeight() != 9000.1f && renderWater ? vec4(0, 1, 0, -this->w->getHeight()) : vec4(9000.1f, 9000.1f, 9000.1f, 9000.1f)));
			if (!renderWater)
				glEnable(GL_CULL_FACE);
		}
		void mouseButton(MouseEvent me) {

		}
		void keyButton(KeyEvent ke) {

		}
	};
}