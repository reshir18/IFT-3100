/*****************************************************
 * TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * Application class implementation
 *
 *****************************************************/


#include "Application.h"
#include "MainWindow.h"
#include <ofAppRunner.h>
#include <glm/gtx/string_cast.hpp>

 /**
  * Start application
  */
void Application::start() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 3);
	settings.setSize(1440, 900);
	settings.windowMode = OF_WINDOW;

	// Disable rectangle textures
	ofDisableArbTex();
	ofCreateWindow(settings);
	ofRunApp(new MainWindow);

}


/**
 * Main entry point for the program
 */
int main() {

	Application app;
	app.start();

}