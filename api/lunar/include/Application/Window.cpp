#include "Window.h"
#include "Application.h"

#include "../Image/Texture.h"

using namespace osomi;
using namespace std;

std::vector<Window*> Window::windows = std::vector<Window*>();

void Window::err(int error, const char *description) {
	printf("GLFW error %i: %s\n", error, description);
}
Window::Window(GLFWwindow *_w) : w(_w) {}
Window::~Window() {
	glfwDestroyCursor(c);
	glfwDestroyWindow(w);
	for (unsigned int i = 0; i < windows.size(); i++)
		if (windows[i] == this) {
			windows.erase(windows.begin() + i);
			break;
		}
}
void Window::getSize(int &w, int &h) {
	glfwGetWindowSize(this->w, &w, &h);
}
void Window::setCursorPos(float x, float y) {
	if (x < -1 || x > 1 || y < -1 || y > 1)return;
	x = (x + 1) / 2;
	y = (y + 1) / 2;
	int width, height;
	glfwGetWindowSize(w, &width, &height);
	glfwSetCursorPos(w, x * width, y * height);
}
void Window::setCursor(GLFWcursor *_c) {
	if (_c == nullptr)return;
	glfwDestroyCursor(c);
	c = _c;
}
void Window::disableCursor() {
	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void Window::enableCursor() {
	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

Window *Window::create(string title, string path) {
	glfwSetErrorCallback(err);
	if (!glfwInit()) {
		printf("Couldn't create window! Error %u, couldn't init glfw!\n", 0);
		return nullptr;
	}
	GLFWmonitor *mon = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(mon);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

	GLFWwindow *w = glfwCreateWindow(mode->width, mode->height - 30, title.c_str(), NULL, NULL);
	if (w == nullptr) {
		printf("Couldn't create window! Error %u, glfwCreateWindow failed!\n", 1);
		return nullptr;
	}
	glfwSetWindowPos(w, 0, 30);
	glfwMakeContextCurrent(w);
	glfwSetMouseButtonCallback(w, mouseButton);
	glfwSetCursorPosCallback(w, cursorPos);
	glfwSetKeyCallback(w, key);
	/*glfwSetScrollCallback*/
	glfwSetCursorEnterCallback(w, cursorEnter);

	GLFWcursor *c = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwSetCursor(w, c);

	if (glewInit()) {
		printf("Couldn't create window! Error %u, couldn't create glew!\n", 2);
		glfwDestroyCursor(c);
		return nullptr;
	}

	Texture *t = Texture::load(path, true);
	if (t == nullptr) {
		printf("Couldn't create window! Error %i, couldn't find the window icon!\n", 3);
		glfwSetWindowShouldClose(w, true);
		return nullptr;
	}
	GLFWimage img;
	img.width = t->getWidth();
	img.height = t->getHeight();
	img.pixels = t->getData();
	glfwSetWindowIcon(w, 1, &img);

	delete t;

	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));
	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glDepthFunc(GL_LESS);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	glEnable(GL_TEXTURE_2D);

	Window *win = new Window(w);
	win->c = c;
	windows.push_back(win);
	return win;
}
void Window::cursorEnter(GLFWwindow* window, int entered) {
	for (unsigned int i = 0; i < windows.size(); i++)
		if (windows[i]->w == window) {
			windows[i]->a->setMouseInScreen(entered == 1);
			break;
		}
}
void Window::mouseButton(GLFWwindow* window, int button, int action, int mods){
	for (unsigned int i = 0; i < windows.size(); i++)
		if (windows[i]->w == window) {
			windows[i]->a->mouseButton(button, action, mods);
			break;
		}
}
void Window::cursorPos(GLFWwindow* window, double xpos, double ypos) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	xpos = xpos / width * 2 - 1;
	ypos = ypos / height * 2 - 1;
	for (unsigned int i = 0; i < windows.size(); i++)
		if (windows[i]->w == window) {
			windows[i]->a->setMouse(xpos, ypos);
			break;
		}
}
void Window::key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	for (unsigned int i = 0; i < windows.size(); i++)
		if (windows[i]->w == window) {
			windows[i]->a->keyButton(key, scancode, action, mods);
			break;
		}
}