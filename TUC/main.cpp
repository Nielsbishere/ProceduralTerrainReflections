#include <Application/Application.h>

#include "States/GameState.h"

using namespace osomi;

int main() {
	Application *a = Application::create("Osomi Lunar's the undead castle v0.0.1", "Logo/TUC.png");
	a->setState(new GameState());
	Application::run();
	Application::quit();
}