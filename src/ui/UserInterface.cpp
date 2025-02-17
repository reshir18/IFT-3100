/*****************************************************
* TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * UserInterface class implementation
 *
 *****************************************************/

#include "UserInterface.h"
#include <ofAppRunner.h>
#include <ofGraphics.h>
#include "Global.h"

/**
 * Setup user interface
 */
void UserInterface::setup() {
 m_gui.setup();

 // Load images needed by the toolbar
 ofImage imgToolbarNewLevel;
 imgToolbarNewLevel.load("images/ui/toolbar_buttons/new_level.png");
 m_textureToolbarNewLevel = imgToolbarNewLevel.getTexture();

 ofImage imgToolbarLoadLevel;
 imgToolbarLoadLevel.load("images/ui/toolbar_buttons/load_level.png");
 m_textureToolbarLoadLevel = imgToolbarLoadLevel.getTexture();

 ofImage imgToolbarSaveLevel;
 imgToolbarSaveLevel.load("images/ui/toolbar_buttons/save_level.png");
 m_textureToolbarSaveLevel = imgToolbarSaveLevel.getTexture();

 ofImage imgToolbarGenerateAtlas;
 imgToolbarGenerateAtlas.load("images/ui/toolbar_buttons/generate_atlas.png");
 m_textureToolbarGenerateAtlas = imgToolbarGenerateAtlas.getTexture();

 knight_color_setup();
}

void UserInterface::knight_color_setup(){
    converter = new KnightColorConverterRGB();
    KnightColor kc = KnightColor(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1, 0.0f, 255);
    currentColorRGB = converter->TransformTo(kc);
    colorParameters.clear();
    colorParameters = converter->getParameters(kc);
}


/**
 * draw user interface
 */
void UserInterface::draw() {

 m_gui.begin();

 draw_menu();
 draw_toolbar();
 draw_tree();
 draw_properties();
 draw_color_setup();
 draw_viewports();
 draw_status();


 m_gui.end();

}


/**
* Draw the menu
*/
void UserInterface::draw_menu() {

 // Draw menu
 if (ImGui::BeginMainMenuBar()) {

  if (ImGui::BeginMenu("File")) {
   if (ImGui::MenuItem("New Level")) {
    on_new_level();
   }
   if (ImGui::MenuItem("Load Level")) {
    on_load_level();
   }
   if (ImGui::MenuItem("Save Level")) {
    on_save_level();
   }

   if (ImGui::MenuItem("Generate Texture Atlas")) {
    on_generate_atlas();
   }

   if (ImGui::MenuItem("Exit")) {
    ofExit(0);
   }

   ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Edit")) {
   if (ImGui::MenuItem("Undo")) {
    on_history_undo();
   }
   if (ImGui::MenuItem("Redo")) {
    on_history_redo();
   }
   ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Help")) {
   if (ImGui::MenuItem("About Knight Maker")) {
    on_about_program();
   }
   ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Knight Color")) {
      if (ImGui::MenuItem("RGB")) {
		  change_color_converter(new KnightColorConverterRGB());
      }
      if (ImGui::MenuItem("CYMK")) {
          change_color_converter(new KnightColorConverterCYMK());
      }
      if (ImGui::MenuItem("HSV")) {
          change_color_converter(new KnightColorConverterHSV());
      }
      ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar(); // End the menu bar
 }
}


/**
 * Draw the toolbar_buttons
 */
void UserInterface::draw_toolbar() {

 ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()), ImGuiCond_Always);
 ImGui::SetNextWindowSize(ImVec2(ofGetWidth(), TOOLBAR_HEIGHT));

 ImGuiWindowFlags toolbarFlags = ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_NoScrollbar;

 if (ImGui::Begin("Toolbar", nullptr, toolbarFlags)) {

  if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_textureToolbarNewLevel.getTextureData().textureID), ImVec2(48, 48))) {
   on_new_level();
  }
  ImGui::SameLine();

  if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_textureToolbarLoadLevel.getTextureData().textureID), ImVec2(48, 48))) {
    on_load_level();
  }
  ImGui::SameLine();

  if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_textureToolbarSaveLevel.getTextureData().textureID), ImVec2(48, 48))) {
   on_save_level();
  }
  ImGui::SameLine();

  if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_textureToolbarGenerateAtlas.getTextureData().textureID), ImVec2(48, 48))) {
   on_generate_atlas();
  }

 }
 ImGui::End();
}


/**
* Draw tree containing nodes
*/
void UserInterface::draw_tree() {

// Define position and size
 ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight() + TOOLBAR_HEIGHT + 6), ImGuiCond_Always);
 ImGui::SetNextWindowSize(ImVec2(LEFTPANEL_WIDTH, TREEVIEW_HEIGHT), ImGuiCond_Always);

 if (ImGui::Begin("Level", nullptr,
     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {

  ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Always);

  // Root node
  if (ImGui::TreeNode("Root Node")) {

   // All tree nodes opens

   // Child nodes under Root Node
   if (ImGui::TreeNode("Child 1")) {
    ImGui::Selectable(" Sub-Child 1");
    ImGui::TreePop();
   }

   if (ImGui::TreeNode("Child 2")) {
    ImGui::Selectable(" Sub-Child 2");
    ImGui::TreePop();
   }

   // Nested tree nodes
   if (ImGui::TreeNode("Child 3")) {
    ImGui::Selectable(" Sub-Child 3");
    ImGui::TreePop();
   }

   ImGui::TreePop(); // Close the root node
  }
  ImGui::End();

 }
}


/**
* Draw properties for a selected node
*/


void UserInterface::draw_properties() {

 int posY = ImGui::GetFrameHeight() + TOOLBAR_HEIGHT + TREEVIEW_HEIGHT + 8;
 ImGui::SetNextWindowPos(ImVec2(0, posY));
 ImGui::SetNextWindowSize(ImVec2(LEFTPANEL_WIDTH, ofGetHeight() - STATUSBAR_HEIGHT - posY - 230));

 ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

 // Add some properties with input fields
 static char name[128] = "Sample Node";
 ImGui::Text("Name");
 ImGui::SameLine(100);
 ImGui::InputText("", name, IM_ARRAYSIZE(name));

 static int age = 30;
 ImGui::Text("Render Order");
 ImGui::SameLine(100);
 ImGui::InputInt("", &age);

 static float height = 1.75f;
 ImGui::Text("Alpha Value");
 ImGui::SameLine(100);
 ImGui::InputFloat("", &height, 0.01f, 0.1f, 3);

 static bool isActive = true;
 ImGui::Text("Active");
 ImGui::SameLine(100);
 ImGui::Checkbox("", &isActive);


 ImGui::End();

}


void UserInterface::draw_color_setup() {
    int posY = ImGui::GetFrameHeight() + TOOLBAR_HEIGHT + TREEVIEW_HEIGHT + TREEVIEW_HEIGHT + 100;
    ImGui::SetNextWindowPos(ImVec2(0, posY));
    ImGui::SetNextWindowSize(ImVec2(LEFTPANEL_WIDTH, ofGetHeight() - STATUSBAR_HEIGHT - posY - 2), ImGuiCond_Always);

    ImGui::Begin("Colors", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
  
   
   for (auto& param : colorParameters) {
        ImGui::Text("%s", param.getName().c_str());
        ImGui::SameLine(80);
        float value = param.get();
        ImGui::SliderFloat("", &value, param.getMin(), param.getMax());
        param.set(value);
    }
    std::vector<float> colorValues = getColorParameterValues();
    currentColorRGB = converter->TransformTo(KnightColor(colorValues[0], colorValues[1], colorValues[2], colorValues[3], colorValues[4]));

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec2 center = ImVec2(p.x + 50, p.y + 50); // Center of the circle
    float radius = 50.0f; // Radius of the circle
    ImU32 color = IM_COL32(currentColorRGB.get_red(), currentColorRGB.get_green(), currentColorRGB.get_blue(), currentColorRGB.get_alpha()); // Red color
    draw_list->AddCircleFilled(center, radius, color);
    ImGui::End();  // End of the property list window

}

/**
* Draw the status bar
*/
void UserInterface::draw_status() {

 ImGui::SetNextWindowPos(ImVec2(0, ofGetHeight() - STATUSBAR_HEIGHT)); // Position the status bar at the bottom
 ImGui::SetNextWindowSize(ImVec2(ofGetWidth(), STATUSBAR_HEIGHT)); // Set the height of the status bar

 ImGui::Begin("Status Bar", nullptr,
  ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

 ImGui::Text("");
 ImGui::SameLine(ImGui::GetWindowWidth() - 100);
 ImGui::Text("FPS: %.1f", ofGetFrameRate());
 ImGui::End();

}


/**
* Draw viewports
*/
void UserInterface::draw_viewports() {

 float camera_size_first = (float) ofGetHeight() - ImGui::GetFrameHeight() - STATUSBAR_HEIGHT - TOOLBAR_HEIGHT - 8;
 float camera_size_second = ((float) ofGetHeight() - ImGui::GetFrameHeight() - STATUSBAR_HEIGHT - TOOLBAR_HEIGHT - 8) / 2.0;
 float camera_size_third = camera_size_second;

 // First camera
 {
  ImGui::SetNextWindowPos(ImVec2(LEFTPANEL_WIDTH + 2, ImGui::GetFrameHeight() + TOOLBAR_HEIGHT + 6)); // Position the status bar at the bottom
  ImGui::SetNextWindowSize(ImVec2(camera_size_first, camera_size_first)); // Set the height of the status bar

  ImGui::Begin("Main Camera", nullptr,
   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

  ImVec2 windowSize = ImGui::GetContentRegionAvail();

  auto cameraTextureMain = reinterpret_cast<ImTextureID>(Global::m_fboCameraMain.getTexture().getTextureData().textureID);
  ImGui::Image(cameraTextureMain, windowSize);
  ImGui::End();
 }

 // Second camera
 {
  ImGui::SetNextWindowPos(ImVec2(LEFTPANEL_WIDTH + 4 + camera_size_first, ImGui::GetFrameHeight() + TOOLBAR_HEIGHT + 6)); // Position the status bar at the bottom
  ImGui::SetNextWindowSize(ImVec2(camera_size_second, camera_size_second)); // Set the height of the status bar

  ImGui::Begin("Second Camera", nullptr,
   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

  ImVec2 windowSize = ImGui::GetContentRegionAvail();

  auto cameraTextureMain = reinterpret_cast<ImTextureID>(Global::m_fboCameraSecond.getTexture().getTextureData().textureID);
  ImGui::Image(cameraTextureMain, windowSize);
  ImGui::End();
 }

 // Third camera
 {
  ImGui::SetNextWindowPos(ImVec2(LEFTPANEL_WIDTH + 4 + camera_size_first, ImGui::GetFrameHeight() + TOOLBAR_HEIGHT + 7 + camera_size_second)); // Position the status bar at the bottom
  ImGui::SetNextWindowSize(ImVec2(camera_size_third, camera_size_third)); // Set the height of the status bar

  ImGui::Begin("Third Camera", nullptr,
   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

  ImVec2 windowSize = ImGui::GetContentRegionAvail();

  auto cameraTextureMain = reinterpret_cast<ImTextureID>(Global::m_fboCameraThird.getTexture().getTextureData().textureID);
  ImGui::Image(cameraTextureMain, windowSize);
  ImGui::End();
 }

}

std::vector<float> UserInterface::getColorParameterValues() const {
    std::vector<float> values;
    for (const auto& param : colorParameters) {
        values.push_back(param.get());
    }
	if (values.size() == 4) {
		values.push_back(values[3]);
	}
    return values;
}
void UserInterface::change_color_converter(KnightColorConverter* knightColorConverter) {
    converter = knightColorConverter;
    KnightColor kc = converter->TransformFrom(currentColorRGB);
    colorParameters.clear();
    colorParameters = converter->getParameters(kc);
}


/**
 * Callback function : Show about dialog
 */
void UserInterface::on_about_program() {
 // TODO
}


/**
 * Callback function : New level
 */
void UserInterface::on_new_level() {
 // TODO
}


/**
 * Callback function : load level
 */
void UserInterface::on_load_level() {
 // TODO
}


/**
 * Callback function : Save level
 */
void UserInterface::on_save_level() {
 // TODO
}


/**
 * Callback function : Generate atlas
 */
void UserInterface::on_generate_atlas() {
 // TODO
}


/**
 * Callback function : History undo
 */
void UserInterface::on_history_undo() {
 // TODO
}


/**
 * Callback function : History redo
 */
void UserInterface::on_history_redo() {
 // TODO
}

