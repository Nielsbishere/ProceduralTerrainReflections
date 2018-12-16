#pragma once
#include <string>
#include <vector>
#include "KeyHandler.h"
#include "../Math/Math.h"
namespace osomi {
	class State;
	class Mesh;
	class Shader;
	class Application {
		friend class Window;
	protected:
		void setMouseInScreen(bool b);
		void setMouse(float mx, float my);
		void getMouse(float &mx, float &my);
	private:
		static std::vector<Application*> apps;

		double time;
		bool inScreen;
		float mouseX, mouseY;

		unsigned int samples;
		GLuint pp;

		GLuint reflection, refraction;
		unsigned int waterRes;
		GLuint reflectionTexture, refractionTexture;
		GLuint reflectionDepth, refractionDepth;

		Window *w;
		State *st;
		KeyHandler *kh;

		vec3 eye;
		float fov;
		float yaw, pitch;

		Application(Window *w);
		~Application();

		void update(double delta);
		void render();
		void mouseButton(int button, int action, int mods);
		void keyButton(int key, int scancode, int action, int mods);
	public:
		static Application *create(std::string windowTitle, std::string iconPath);
		static void quit();
		static void run();

		void setState(State *_st);
		bool mouseInScreen();

		void setMSAA(unsigned int _samples) { samples = _samples; }

		//Moves the camera vec3(0, 0, 1) is forward, vec3(1, 0, 0) is right, etc.
		void moveCamera(vec3 movement);
		void moveFocus(vec2 yawPitch);
		void moveFocus(double delta);

		KeyHandler *getKeyHandler() { return kh; }
		Window *getWindow() { return w; }
	private:
		void renderScene();
		vec3 calculateDirection(float yaw, float pitch);
	};
}