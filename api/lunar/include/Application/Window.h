#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class GLFWwindow;
namespace osomi {
	class Window {
		friend class Application;
	protected:
		GLFWwindow *w;
		Application *a;

		~Window();
		void setCursorPos(float x, float y);
		void setCursor(GLFWcursor *_c);

		static Window *create(std::string title, std::string icon);
	private:
		GLFWcursor *c;

		static std::vector<Window*> windows;
		static void err(int error, const char *description);
		static void cursorEnter(GLFWwindow* window, int entered); 
		static void mouseButton(GLFWwindow* window, int button, int action, int mods);
		static void cursorPos(GLFWwindow* window, double xpos, double ypos);
		static void key(GLFWwindow* window, int key, int scancode, int action, int mods);
		Window(GLFWwindow *_w);
	public:
		void getSize(int &w, int &h);
		void disableCursor();
		void enableCursor();
	};
}