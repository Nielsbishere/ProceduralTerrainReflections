#pragma once
#include "KeyHandler.h"
namespace osomi {
	class mat4;
	class vec3;
	class State {
		friend class Application;
	protected:
		Application *a;
		virtual ~State() {}
	public:
		virtual void update(double delta) = 0;
		virtual void render(mat4 p, mat4 v, vec3 eye, bool renderWater, bool renderParticles, vec4 plane) = 0;
		virtual void mouseButton(MouseEvent me) {}
		virtual void keyButton(KeyEvent me) {}
	};
}