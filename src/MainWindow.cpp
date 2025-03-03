/*****************************************************
 * TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * MainWindow class implementation
 *
 *****************************************************/
#include "MainWindow.h"
#include "Global.h"


 // Declare GUI elements

 /**
  * Main window setup (title, menu and various visual elements)
  */
void MainWindow::setup() {

	ofSetEscapeQuitsApp(false);  // Disable ESC quitting

	ofSetWindowTitle("Knight Maker");
	ofSetVerticalSync(false);

	Global::setup();
	ui.setup();

	// To be removed
	ofEnableDepthTest();  // Enable depth for 3D rendering
	light.setup();
	light.setPosition(0, 0, 2000);  // Set light position

}


/**
 * Main draw function
 */
void MainWindow::draw() {

	ofBackground(0);

	// Reset render stats
	for (int i=0;i<3;i++) {
		Global::m_countNodeRender[i] = 0;
	}

	// We render three times
	for (int i = 0; i < 3; i++) {

		cameraDraw(i);
		if (ui.onlyOneCamera()) break;
	}

	ui.draw();

}


/**
* Draw a specific camera to a FBO texture
*/
void MainWindow::cameraDraw(int index) {

	auto fbo = Global::m_cameras[index].getFbo();
	Global::m_cameras[index].update();
	auto camera = Global::m_cameras[index].getCamera();
	Global::m_cameras[index].resizeTextureIfNeeded();

	fbo->begin();
	camera->begin();
	ofBackground(0);
	ofEnableLighting();
	light.enable();
	ofSetColor(255);
	Global::m_countNodeRender[index] = Global::m_level.draw(false, &Global::m_cameras[index]);
	Global::m_transformTools.draw(false);
	light.disable();
	ofDisableLighting();
	camera->end();
	fbo->end();

	// Generate Object picking FBO only for hovered camera viewport
	if (getCurrentCameraIndex(true) == index) {
		auto fboPicking = Global::m_cameras[index].getPickingFbo();
		fboPicking->begin();
		camera->begin();
		ofBackground(0);
		Global::m_level.draw(true, &Global::m_cameras[index]);
		Global::m_transformTools.draw(true);
		camera->end();
		fboPicking->end();
	}

}


/**
 * Callback function for handling key pressed events
 */
void MainWindow::keyPressed(ofKeyEventArgs& key) {


	int index = getCurrentCameraIndex(false);
	if (index == -1) {
		cameraMovement.set(0, 0, 0);
		return;
	}

	if (!key.hasModifier(OF_KEY_CONTROL))
		handleCameraInput(true, key, index);

}


/**
 * Callback function for handling key released events
 */
void MainWindow::keyReleased(ofKeyEventArgs& key) {

	int index = getCurrentCameraIndex(false);
	if (index == -1) {
		resetCameraMovement();
		return;
	}

	if (!key.hasModifier(OF_KEY_CONTROL))
		handleCameraInput(false, key, index);

	// Handle history
	if (key.hasModifier(OF_KEY_CONTROL) && (key.key == 'z')) {
		Global::m_actions.undo();
	}

	if (key.hasModifier(OF_KEY_CONTROL) && (key.key == 'r')) {
		Global::m_actions.redo();
	}

}


/**
 * Reset camera movement
 */
void MainWindow::resetCameraMovement() {
	cameraMovement.set(0, 0, 0);
	cameraRotation = 0.0;
}

/**
 * Handle key presses for camera control
 */
void MainWindow::handleCameraInput(bool pressed, ofKeyEventArgs& key, int index) {


	if (!pressed) {

		// Handle reset
		if (key.key == OF_KEY_HOME) {
			Global::m_cameras[index].reset();
			resetCameraMovement();
			return;
		}

		// Print debug information
		if (key.key == 'p') {
			Global::m_cameras[index].debug();
		}

		// Toggle ortho projection
		if (key.key == 'o') {
			if (Global::m_cameras[index].getCamera()->getOrtho()) {
				Global::m_cameras[index].getCamera()->disableOrtho();
			}
			else {
				Global::m_cameras[index].getCamera()->enableOrtho();

			}

		}

		// Swap cameras
		if (key.key == OF_KEY_INSERT) {
			if (index != 0) {

				// Retrieve position and orientation of main camera
				glm::vec3 firstPosition = Global::m_cameras[0].getCamera()->getPosition();
				glm::vec3 firstDirection = Global::m_cameras[0].getCamera()->getLookAtDir();
				bool firstOrtho = Global::m_cameras[0].getCamera()->getOrtho();

				// Retrieve position of current camera
				glm::vec3 currentPosition = Global::m_cameras[index].getCamera()->getPosition();
				glm::vec3 currentDirection = Global::m_cameras[index].getCamera()->getLookAtDir();
				bool currentOrtho = Global::m_cameras[index].getCamera()->getOrtho();

				// Set main camera to current camera
				Global::m_cameras[index].getCamera()->setPosition(firstPosition);
				Global::m_cameras[index].getCamera()->lookAt(firstDirection);
				if (firstOrtho) {
					Global::m_cameras[index].getCamera()->enableOrtho();
				}
				else {
					Global::m_cameras[index].getCamera()->disableOrtho();
				}

				// Set current camera to main
				Global::m_cameras[0].getCamera()->setPosition(currentPosition);
				Global::m_cameras[0].getCamera()->lookAt(currentDirection);
				if (currentOrtho) {
					Global::m_cameras[0].getCamera()->enableOrtho();
				}
				else {
					Global::m_cameras[0].getCamera()->disableOrtho();
				}

				ImGui::SetWindowFocus("Main Camera");

			}
			return;
		}
	}

	// Handle rotation along forward vector
	if (key.key == 'q' || key.key == 'e') {
		if (!pressed) {
			cameraRotation = 0.0;
			return;
		}
		cameraRotation = (key.key == 'q' ? 1.0f : -1.0f);
	}

	// Handle longitudinal movements
	if (key.key == 'w' || key.key == 's') {

		if (!pressed) {
			cameraMovement.z = 0;
			return;
		}

		cameraMovement.z = (key.key == 'w' ? -1.0f : 1.0f);
		return;
	}

	// Handle lateral movements
	if (key.key == 'a' || key.key == 'd') {

		if (!pressed) {
			cameraMovement.x = 0;
			return;
		}

		cameraMovement.x = (key.key == 'a' ? -1.0f : 1.0f);
		return;
	}

	// Handle vertical movements
	if (key.key == ' ' || key.key == 'z') {

		if (!pressed) {
			cameraMovement.y = 0;
			return;
		}

		cameraMovement.y = (key.key == ' ' ? 1.0f : -1.0f);
	}

}


/**
 * Update function, called as often as draw
 */
void MainWindow::update() {

	// Calculate delta
	float currentTime = ofGetElapsedTimef();
	float deltaTime = currentTime - lastUpdateTime;
	lastUpdateTime = currentTime;

	int index = getCurrentCameraIndex(false);
	if (index == -1) return;
	updateCamera(index, deltaTime);
	Global::m_transformTools.setCameraIndex(index);

}


/**
 * Update camera (movements)
 */
void MainWindow::updateCamera(int index, float deltaTime) {

	float cameraMoveSpeed = 800.0f;
	float cameraRotationSpeed = 100.0f;
	float cameraOrthoZoomSpeed = 1.0f;

	auto camera = Global::m_cameras[index].getCamera();

	// Allow only if not in ortho mode
	if (!camera->getOrtho()) {
		camera->dolly(cameraMovement.z * cameraMoveSpeed * deltaTime);  // Move forward/backward
	} else {
		// Set zoom
		camera->setOrthoZoom(camera->getOrthoZoom() - (cameraMovement.z * cameraOrthoZoomSpeed * deltaTime));
	}

	camera->truck(cameraMovement.x * cameraMoveSpeed * deltaTime);  // Move left/right
	camera->boom(cameraMovement.y * cameraMoveSpeed * deltaTime);   // Move up/down
	camera->rollDeg(cameraRotation * cameraRotationSpeed * deltaTime);  // Rotate along forward vector

}


/**
 * Get the currenty selected viewport camera
 */
int MainWindow::getCurrentCameraIndex(bool hovered) {

	std::string windowName;

	if (hovered) {
		windowName = ui.getHoveredWindow();
	}
	else {
		windowName = ui.getSelectedWindow();
	}

	int index = -1;

	if (windowName == "Main Camera") {
		index = 0;
	}
	else if (windowName == "Second Camera") {
		index = 1;
	}
	else if (windowName == "Third Camera") {
		index = 2;
	}
	return index;
}


/**
 * Called when the application is about to exit
 */
void MainWindow::exit() {
	// Do nothing for now
}



/**
 * Callback function for handling mouse pressed events
 */
void MainWindow::mousePressed(int x, int y, int button) {
	if (button == OF_MOUSE_BUTTON_MIDDLE) {
		isMiddleMousePressed = true;
		lastMousePosition.set(x, y);
	}
}


/**
 * Callback function for handling mouse released events
 */
void MainWindow::mouseReleased(int x, int y, int button) {

	if (button == OF_MOUSE_BUTTON_MIDDLE) {
		isMiddleMousePressed = false;
	}
}


/**
 * Callback function for handling mouse dragged events
 */
void MainWindow::mouseDragged(int x, int y, int button) {

	int index = getCurrentCameraIndex(true);
	if (index == -1) return;
	if (isMiddleMousePressed && button == OF_MOUSE_BUTTON_MIDDLE) {

		auto mx = static_cast<float>(x);
		auto my = static_cast<float>(y);

		float sensitivity = 0.2f; // Rotation speed

		float deltaX = mx - lastMousePosition.x;
		float deltaY = my - lastMousePosition.y;

		ofCamera* camera = Global::m_cameras[index].getCamera();
		camera->panDeg(-deltaX * sensitivity);  // Rotate horizontally
		camera->tiltDeg(-deltaY * sensitivity); // Rotate vertically

		lastMousePosition.set(mx, my);
	}
}


/**
 * Callback function for handling mouse scrolled events
 */
void MainWindow::mouseScrolled(ofMouseEventArgs& args) {

	int index = getCurrentCameraIndex(true);
	if (index == -1) return;

	float zoomSpeed = 50.0f;
	float cameraOrthoZoomSpeed = 1.0f;


	auto camera = Global::m_cameras[index].getCamera();

	// Allow only if not in ortho mode
	if (!camera->getOrtho()) {
		camera->dolly(-args.scrollY * zoomSpeed);
	} else {
		// Set zoom
		camera->setOrthoZoom(camera->getOrthoZoom() - (args.scrollY * cameraOrthoZoomSpeed));
	}


}