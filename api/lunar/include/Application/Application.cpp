#include "Application.h"
#include "Window.h"
#include "State.h"
#include "../Render/Shader.h"
#include "../Render/Mesh/Mesh.h"

#include "../Helper/StringHelper.h"

#include <GLFW/glfw3.h>

#include "../Image/Texture.h"

using namespace osomi;
using namespace std;

vector<Application*> Application::apps = vector<Application*>();

Application::Application(Window *_w): w(_w), samples(0), time(0), st(nullptr), kh(nullptr), mouseX(0), mouseY(0), inScreen(false), eye(vec3(0, 0, -5)), fov(40.f), yaw(90), pitch(0){
	glGenFramebuffers(1, &pp);
	waterRes = 1024;

	//Generate reflection framebuffer
	glGenFramebuffers(1, &reflection);
	glBindFramebuffer(GL_FRAMEBUFFER, reflection);

	glGenTextures(1, &reflectionTexture);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, waterRes/2, waterRes/2, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenRenderbuffers(1, &reflectionDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, reflectionDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, waterRes/2, waterRes/2);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, reflectionDepth);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, reflectionTexture, 0);

	GLenum draw = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &draw);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("ERROR! Couldn't generate reflection texture! Aborting!\n");
		throw exception("Couldn't generate reflection texture!\n");
	}

	//Generate refraction framebuffer
	glGenFramebuffers(1, &refraction);
	glBindFramebuffer(GL_FRAMEBUFFER, refraction);

	glGenTextures(1, &refractionTexture);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, waterRes, waterRes, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenRenderbuffers(1, &refractionDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, refractionDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, waterRes, waterRes);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, refractionDepth);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, refractionTexture, 0);

	glDrawBuffers(1, &draw);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("ERROR! Couldn't generate refraction texture! Aborting!\n");
		throw exception("Couldn't generate refraction texture!\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
Application *Application::create(string windowTitle, string iconPath) {
	if (!StringHelper::endsWith(iconPath, ".png")) {
		printf("Couldn't create application! Error %u; invalid path (%s)!\n", 0, iconPath.c_str());
		return nullptr;
	}
	Window *w = Window::create(windowTitle, iconPath);
	if (w == nullptr) {
		printf("Couldn't create application! Error %u; window couldn't be created!\n", 1);
		return nullptr;
	}
	Application *app = new Application(w);
	app->kh = new KeyHandler();
	app->w = w;
	w->a = app;
	apps.push_back(app);
	if (Mesh::cube == nullptr) {
		GLfloat tri[] = {
			//Front
			-1, -1, 1,
			0, 0,
			0, 0, 1,

			1, -1, 1,
			1, 0,
			0, 0, 1,

			1, 1, 1,
			1, 1,
			0, 0, 1,

			-1, 1, 1,
			0, 1,
			0, 0, 1,

			//Back
			-1, -1, -1,
			0, 0,
			0, 0, -1,

			1, -1, -1,
			1, 0,
			0, 0, -1,

			1, 1, -1,
			1, 1,
			0, 0, -1,

			-1, 1, -1,
			0, 1,
			0, 0, -1,

			//Down
			-1, -1, -1,
			0, 0,
			0, -1, 0,

			1, -1, -1,
			1, 0,
			0, -1, 0,

			1, -1, 1,
			1, 1,
			0, -1, 0,

			-1, -1, 1,
			0, 1,
			0, -1, 0,

			//Up
			-1, 1, -1,
			0, 0,
			0, 1, 0,

			1, 1, -1,
			1, 0,
			0, 1, 0,

			1, 1, 1,
			1, 1,
			0, 1, 0,

			-1, 1, 1,
			0, 1,
			0, 1, 0,

			//Left
			-1, -1, -1,
			0, 0,
			-1, 0, 0,

			-1, 1, -1,
			1, 0,
			-1, 0, 0,

			-1, 1, 1,
			1, 1,
			-1, 0, 0,

			-1, -1, 1,
			0, 1,
			-1, 0, 0,

			//Right
			1, -1, -1,
			0, 0,
			1, 0, 0,

			1, 1, -1,
			1, 0,
			1, 0, 0,

			1, 1, 1,
			1, 1,
			1, 0, 0,

			1, -1, 1,
			0, 1,
			1, 0, 0
		};
		GLushort ind[] = {
			0,1,2, 2,3,0,			//F
			4,7,6, 6,5,4,			//B
			8,9,10, 10,11,8,		//U
			12,15,14, 14,13,12,		//D
			16,19,18, 18,17,16,		//L
			20,21,22, 22,23,20		//R
		};
		Mesh::cube = Mesh::create(tri, ind, nullptr);
	}
	return app;
}
void Application::quit() {
	for (unsigned int i = 0; i < apps.size(); i++)
		delete apps[i];
	apps.clear();
	Texture::clear();
}
void Application::setState(State *_st) {
	if (st != nullptr)delete st;
	st = _st;
	st->a = this;
}
Application::~Application() {
	if (w != nullptr)delete w;
	if (st != nullptr)delete st;
	glDeleteFramebuffers(1, &pp);
	glDeleteFramebuffers(1, &reflection);
	glDeleteFramebuffers(1, &refraction);
	glDeleteRenderbuffers(1, &reflectionDepth);
	glDeleteRenderbuffers(1, &refractionDepth);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteTextures(1, &refractionTexture);
}
void Application::run() {
	while (apps.size() != 0) {
		for (int i = apps.size() - 1; i >= 0; i--) {
			Application *a = apps[i];
			Window *win = a->w;
			GLFWwindow *w = win->w;
			if (glfwWindowShouldClose(w)) {
				delete win;
				apps.erase(apps.begin() + i);
				continue;
			}
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			double delta = glfwGetTime() - a->time;
			a->update(delta);
			a->time = glfwGetTime();
			int wi, he;
			glfwGetWindowSize(w, &wi, &he);
			glViewport(0, 0, wi, he);
			a->render();
			if(a->inScreen)
				win->setCursorPos(a->mouseX, a->mouseY);
			glfwSwapBuffers(w);
			if (a->kh != nullptr)
				a->kh->update();
		}
		glfwPollEvents();
	}
}
bool Application::mouseInScreen() {
	return inScreen;
}
void Application::setMouseInScreen(bool b){
	inScreen = b;
}
void Application::setMouse(float mx, float my) {
	mouseX = mx;
	mouseY = my;
}
void Application::getMouse(float &mx, float &my) {
	mx = mouseX;
	my = mouseY;
}
void Application::update(double delta) { if (st != nullptr)st->update(delta); }
void Application::render() {
	if (samples == 0) {
		glDisable(GL_MULTISAMPLE);
		renderScene();
	}
	else if(st != nullptr){
		glEnable(GL_MULTISAMPLE);
		glBindFramebuffer(GL_FRAMEBUFFER, pp);

		int width, height;
		w->getSize(width, height);

		//Generating multi sample texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture, 0);

		//Render buffer
		GLuint rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			printf("Couldn't create framebuffer! Disabling samples!\n");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			samples = 0;
			glDeleteTextures(1, &texture);
			glDeleteRenderbuffers(1, &rbo);
			render();
			return;
		}

		//Clear framebuffer
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderScene();

		glBindFramebuffer(GL_READ_FRAMEBUFFER, pp);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		//Get rid of temporary images
		glDeleteTextures(1, &texture);
		glDeleteRenderbuffers(1, &rbo);
	}
}

void Application::renderScene(){
	if (st == nullptr)return;
	int width, height;
	w->getSize(width, height);

	float near = 0.1;
	float far = 100;
	float fov = 40;
	vec3 up(0, 1, 0);

	float waterHeight = 0;

	//Render reflection
	vec3 teye = vec3(eye.x, eye.y - 2 * (eye.y - waterHeight), eye.z);

	vec3 center = teye + calculateDirection(yaw, -pitch);

	mat4 p = mat4::perspective(fov, 1, near, far);
	mat4 v = mat4::lookat(teye, center, up);
	glViewport(0, 0, waterRes/2, waterRes/2);
	glBindFramebuffer(GL_FRAMEBUFFER, reflection);
	st->render(v, p, eye, false, true, vec4(0, 1, 0, -waterHeight));

	center = eye + calculateDirection(yaw, pitch);


	//Plane testing
	float aspect = (float)width / height;

	vec3 d = (center - eye).normalize();
	vec3 npos = eye + d * near;
	vec3 fpos = eye + d * far;
	float Hnear = 2 * near * tan(40.f / 2);
	float Hfar = 2 * far * tan(40.f / 2);
	float Wnear = Hnear * aspect;
	float Wfar = Hfar * aspect;

	vec3 right = up.cross(d).normalize();

	vec3 ftl = fpos - right * Wfar / 2 + up * Hfar / 2;
	vec3 ftr = fpos + right * Wfar / 2 + up * Hfar / 2;
	vec3 fbl = fpos - right * Wfar / 2 - up * Hfar / 2;
	vec3 fbr = fpos + right * Wfar / 2 - up * Hfar / 2;

	vec3 ntl = npos - right * Wnear / 2 + up * Hnear / 2;
	vec3 ntr = npos + right * Wnear / 2 + up * Hnear / 2;
	vec3 nbl = npos - right * Wnear / 2 - up * Hnear / 2;
	vec3 nbr = npos + right * Wnear / 2 - up * Hnear / 2;

	vec3 fn = (ftr - ftl).cross(fbl - ftl).normalize();
	vec4 fplane = vec4(-fn, (fn).dot(ftl));
	vec4 nplane = vec4(fn, (-fn).dot(ntl));

	//TODO:

	fplane.print();
	nplane.print();
	printf("\n\n");

	//Render refraction
	v = mat4::lookat(eye, center, up);
	p = mat4::perspective(fov, aspect, near, far);

	glViewport(0, 0, waterRes, waterRes);
	glBindFramebuffer(GL_FRAMEBUFFER, refraction);
	st->render(v, p, eye, false, true, vec4(0, -1, 0, waterHeight));

	//Render scene
	glViewport(0, 0, width, height);
	if(samples == 0)glBindFramebuffer(GL_FRAMEBUFFER, 0);
	else glBindFramebuffer(GL_FRAMEBUFFER, pp);

	glActiveTexture(GL_TEXTURE30);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glActiveTexture(GL_TEXTURE31);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);

	st->render(v, p, eye, true, true, vec4(9000.1f, 9000.1f, 9000.1f, 9000.1f));
}
vec3 Application::calculateDirection(float yaw, float pitch) {
	yaw = yaw / 180 * M_PI;
	pitch = pitch / 180 * M_PI;
	float xzLen = cos(pitch);
	float x = xzLen * cos(yaw);
	float y = sin(pitch);
	float z = xzLen * sin(-yaw);
	return vec3(x, y, z).normalize();
}
void Application::moveCamera(vec3 d) {
	vec3 center = eye + calculateDirection(yaw, pitch);
	mat4 v = mat4::lookat(eye, center, vec3(0, 1, 0));

	vec3 fd = v.getForward().normalize();
	vec3 rd = v.getRight().normalize();
	vec3 ud = v.getUp().normalize();

	vec3 m = -fd * d.z + rd * d.x + ud * d.y;

	eye += m;
	center += m;
}
void Application::moveFocus(vec2 yawPitch) {
	yaw += yawPitch.x;
	pitch += yawPitch.y;
	if (pitch < -70)pitch = -70;
	else if (pitch > 35)pitch = 35;
}
void Application::moveFocus(double delta) {
	if (mouseInScreen()) {
		vec2 d = vec2(-mouseX, mouseY);
		setMouse(0, 0);
		d *= delta * 720;
		d.x *= 2.5;
		d.y = -d.y;
		moveFocus(d);
	}
}

void Application::mouseButton(int button, int action, int mods) {
	if (kh != nullptr) {
		MouseEvent me = kh->mouseButton(button, action, mods);
		if (st != nullptr && me.isValid())
			st->mouseButton(me);
	}
}
void Application::keyButton(int key, int scancode, int action, int mods) {
	if (kh != nullptr) {
		KeyEvent me = kh->keyButton(key, scancode, action, mods);
		if (st != nullptr && me.isValid())
			st->keyButton(me);
	}
}