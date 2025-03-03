/*****************************************************
* TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * UserInterface class implementation
 *
 *****************************************************/
#include "UserInterface.h"

#include <imgui_internal.h>
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

    ofImage imgToolbarToggleCameras;
    imgToolbarToggleCameras.load("images/ui/toolbar_buttons/toggle_cameras.png");
    m_textureToolbarToggleCameras = imgToolbarToggleCameras.getTexture();

    ofImage imgToolbarToggleCamerasPressed;
    imgToolbarToggleCamerasPressed.load("images/ui/toolbar_buttons/toggle_cameras_pressed.png");
    m_textureToolbarToggleCamerasPressed = imgToolbarToggleCamerasPressed.getTexture();

    ofImage imgNotVisible;
    imgNotVisible.load("images/ui/not_visible.png");
    m_textureNotVisible = imgNotVisible.getTexture();

    currentCursor = "cursor";

	changeCursor();
    ofHideCursor();
}


/**
 * draw user interface
 */
void UserInterface::draw() {
    m_gui.begin();

    m_selectedWindow = "";
    m_hoveredWindow = "";

    drawMenu();
    drawToolbar();
    drawTree();
    drawTreeActions();
    drawProperties();
    drawViewports();
    drawStatus();

    if (m_initialDraw) {
        m_initialDraw = false;
        ImGui::SetWindowFocus("Main Camera");
    }
    drawCursor();
    m_gui.end();
}


/**
 * Draw the menu
 */
void UserInterface::drawMenu() {
    // Draw menu
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Level")) {
                onNewLevel();
            }
            if (ImGui::MenuItem("Load Level")) {
                onLoadLevel();
            }
            if (ImGui::MenuItem("Save Level")) {
                onSaveLevel();
            }

            if (ImGui::MenuItem("Generate Texture Atlas")) {
                onGenerateAtlas();
            }

            if (ImGui::MenuItem("Exit")) {
                ofExit(0);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo")) {
                onHistoryUndo();
            }
            if (ImGui::MenuItem("Redo")) {
                onHistoryRedo();
            }
            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Occlusion")) {
            bool noOcclusion = (Global::m_cameras[0].getOcclusion() == NO_OCCLUSION);

            if (ImGui::MenuItem("No Occlusion", nullptr, &noOcclusion)) {
                Global::m_cameras[0].setOcclusion(NO_OCCLUSION);
                Global::m_cameras[1].setOcclusion(NO_OCCLUSION);
                Global::m_cameras[2].setOcclusion(NO_OCCLUSION);
            }

            bool frustumCulling = (Global::m_cameras[0].getOcclusion() == FRUSTUM_CULLING);
            if (ImGui::MenuItem("Frustum Culling", nullptr, &frustumCulling)) {
                Global::m_cameras[0].setOcclusion(FRUSTUM_CULLING);
                Global::m_cameras[1].setOcclusion(FRUSTUM_CULLING);
                Global::m_cameras[2].setOcclusion(FRUSTUM_CULLING);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Key Bindings")) {
                onShowKeyBindings();
            }

            if (ImGui::MenuItem("About Knight Maker")) {
                onAboutProgram();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar(); // End the menu bar
    }
}


/**
 * Draw the toolbar_buttons
 */
void UserInterface::drawToolbar() {
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(ofGetWidth(), TOOLBAR_HEIGHT));

    ImGuiWindowFlags toolbarFlags = ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoScrollbar;

    if (ImGui::Begin("Toolbar", nullptr, toolbarFlags)) {
        if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_textureToolbarNewLevel.getTextureData().textureID),
                               ImVec2(48, 48))) {
            onNewLevel();
        }
        ImGui::SameLine();

        if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_textureToolbarLoadLevel.getTextureData().textureID),
                               ImVec2(48, 48))) {
            onLoadLevel();
        }
        ImGui::SameLine();

        if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_textureToolbarSaveLevel.getTextureData().textureID),
                               ImVec2(48, 48))) {
            onSaveLevel();
        }
        ImGui::SameLine();

        if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_textureToolbarGenerateAtlas.getTextureData().textureID),
                               ImVec2(48, 48))) {
            onGenerateAtlas();
        }
        ImGui::SameLine();

        if (ImGui::ImageButton(
            reinterpret_cast<ImTextureID>(m_onlyOneCamera
                                              ? m_textureToolbarToggleCamerasPressed.getTextureData().textureID
                                              : m_textureToolbarToggleCameras.getTextureData().textureID),
            ImVec2(48, 48))) {
            onToggleCameras();
        }

        ImGui::End();
    }
}


/**
 * Draw tree containing nodes
 */
void UserInterface::drawTree() {
    // Define position and size
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight() + TOOLBAR_HEIGHT + 6), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(LEFTPANEL_WIDTH, TREEVIEW_HEIGHT), ImGuiCond_Always);

    if (ImGui::Begin("Level", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Always);

        for (BaseNode *child: Global::m_level.getTree()->getChildren()) {
            drawTreeElement(child);
        }

        ImGui::End();
    }
}


/**
 * Draw tree actions
 */
void UserInterface::drawTreeActions() {
    int posY = ImGui::GetFrameHeight() + TOOLBAR_HEIGHT + TREEVIEW_HEIGHT + 8;
    ImGui::SetNextWindowPos(ImVec2(0, posY));
    ImGui::SetNextWindowSize(ImVec2(LEFTPANEL_WIDTH, TREEVIEW_ACTIONS_HEIGHT));

    ImGuiWindowFlags toolbarFlags = ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoScrollbar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(LEFTPANEL_WIDTH, TREEVIEW_ACTIONS_HEIGHT));

    if (ImGui::Begin("TreeActions", nullptr, toolbarFlags)) {
        if (ImGui::Button("Add Node")) {
            m_addNodeDialog.openDialog();
        }
        ImGui::SameLine();

        if (Global::m_selectedNode != -1) {
            if (ImGui::Button("Delete")) {
            m_deleteNodeDialog.openDialog();
            }

            ImGui::SameLine();

            if (ImGui::Button("Up")) {
                BaseNode* node = Global::m_level.getTree()->findNode(Global::m_selectedNode);
                BaseNode* prev = node->getPreviousNode();
                if (prev != nullptr) {
                    node->getParent()->swapChildOrder(node, prev);
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Down")) {
                BaseNode* node = Global::m_level.getTree()->findNode(Global::m_selectedNode);
                BaseNode* next = node->getNextNode();
                if (next != nullptr) {
                    node->getParent()->swapChildOrder(node, next);
                }
            }
        }


        if (m_addNodeDialog.isOpen()) {
            m_addNodeDialog.draw();
        }

        if (m_deleteNodeDialog.isOpen()) {
            m_deleteNodeDialog.draw();
        }

        ImGui::End();
    }

    ImGui::PopStyleVar();
}


/**
* Draw one tree element (node)
*/
void UserInterface::drawTreeElement(BaseNode *node) {
    ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Always);
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Bullet;
    if (node->getChildren().size() == 0) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (Global::m_selectedNode == node->getId()) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    if (ImGui::TreeNodeEx(node->getName().c_str(), flags)) {

        if (ImGui::IsItemClicked()) {
            if (Global::m_selectedNode != -1) {
                Global::m_level.getTree()->findNode(Global::m_selectedNode)->displayBoundingBox(false);
            }
            Global::m_selectedNode = node->getId();
            Global::m_selectedFromViewport = false;

            Global::m_level.getTree()->findNode(Global::m_selectedNode)->displayBoundingBox(true);
        }

        if (Global::m_selectedNode == node->getId()) {
            if ((Global::m_selectedNode != m_previousNodeSelection) && (Global::m_selectedFromViewport)) {
                ImGui::SetScrollHere();
                m_previousNodeSelection = Global::m_selectedNode;
            }
        }

        if (!node->getDisplayNode()) {
            ImGui::SameLine();
            ImGui::Image(ImTextureID(m_textureNotVisible.getTextureData().textureID), ImVec2(12, 12));
        }

        for (BaseNode *child: node->getChildren()) {
            drawTreeElement(child);
        }

        ImGui::TreePop();
    }
}


/**
 * Draw properties for a selected node
 */
void UserInterface::drawProperties() {

    int posY = ImGui::GetFrameHeight() + TOOLBAR_HEIGHT + TREEVIEW_ACTIONS_HEIGHT + TREEVIEW_HEIGHT + 8;
    ImGui::SetNextWindowPos(ImVec2(0, posY));
    ImGui::SetNextWindowSize(ImVec2(LEFTPANEL_WIDTH, ofGetHeight() - STATUSBAR_HEIGHT - posY - 2));

    ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (Global::m_selectedNode == -1) {
        ImGui::End();
        return;
    }

    BaseNode *selectedNode = Global::m_level.getTree()->findNode(Global::m_selectedNode);
    if (selectedNode == nullptr) {
        ImGui::End();
        return;
    }

    auto properties = selectedNode->getProperties();
    int count = 0;
    for (auto property: properties) {
        switch (property.getType()) {
            case PROPERTY_TYPE::TEXT_FIELD: {
                ImGui::Text(property.getName().c_str());
                ImGui::SameLine(110);

                char buffer[255];
                auto value = std::any_cast<std::string>(property.getValue());
                std::strncpy(buffer, value.c_str(), sizeof(buffer) - 1); // Copy with limit
                buffer[sizeof(buffer) - 1] = '\0';

                if (ImGui::InputText(("##" + std::to_string(count)).c_str(), buffer, IM_ARRAYSIZE(buffer))) {
                    selectedNode->setProperty(property.getName(), std::string(buffer));
                }
            }
            break;

            case PROPERTY_TYPE::COLOR_PICKER: {
                ImGui::Text(property.getName().c_str());
                auto color = std::any_cast<ofFloatColor>(property.getValue());
                ImGui::SameLine(110);

                ImVec4 imColor = color;
                if (ImGui::ColorButton(("...##" + std::to_string(count)).c_str(), imColor, 0, ImVec2(272, 16))) {
                    m_colorDialog.setTitle("Change " + property.getName());
                    m_colorDialog.useProperty(selectedNode, property.getName(), color);
                    m_colorDialog.openDialog();
                }
            }
            break;

            case PROPERTY_TYPE::LABEL: {
                ImGui::Dummy(ImVec2(0, 5));
                ImGui::TextColored(ImVec4(1, 0.5, 0.5, 1), property.getName().c_str());
            }
            break;

            case PROPERTY_TYPE::VECTOR3: {
                ImGui::Text(property.getName().c_str());
                ImGui::SameLine(110);
                glm::vec3 value = std::any_cast<glm::vec3>(property.getValue());
                ImGui::Text("%.02f, %.02f, %.02f", value.x, value.y, value.z);
                ImGui::SameLine(272);

                if (ImGui::Button(("...##" + std::to_string(count)).c_str())) {
                    m_vec3Dialog.setTitle("Change " + property.getName());
                    m_vec3Dialog.useProperty(selectedNode, property.getName(), value);
                    m_vec3Dialog.openDialog();
                }
            }
            break;

            case PROPERTY_TYPE::FLOAT_FIELD:
            {

                ImGui::Text(property.getName().c_str());
                ImGui::SameLine(110);
                auto value = std::any_cast<float>(property.getValue());
                float initialValue = value;

                ImGui::PushItemWidth(186.0f);
                if (ImGui::InputFloat(("##Float_" + std::to_string(count)).c_str(), &value, 0.5f, 1.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)) {
                    Global::m_actions.addAction(selectedNode, property.getName(), initialValue, value);
                }
                ImGui::PopItemWidth();


            }
            break;

            case PROPERTY_TYPE::INT_FIELD:
            {

                ImGui::Text(property.getName().c_str());
                ImGui::SameLine(110);
                auto value = std::any_cast<int>(property.getValue());
                int initialValue = value;

                ImGui::PushItemWidth(186.0f);
                if (ImGui::InputInt(("##Integer_" + std::to_string(count)).c_str(), &value, 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
                    Global::m_actions.addAction(selectedNode, property.getName(), initialValue, value);
                }
                ImGui::PopItemWidth();


            }
            break;

            case PROPERTY_TYPE::BOOLEAN_FIELD:
            {

                ImGui::Text(property.getName().c_str());
                ImGui::SameLine(110);
                auto value = std::any_cast<bool>(property.getValue());
                int initialValue = value;

                ImGui::PushItemWidth(186.0f);
                if (ImGui::Checkbox(("##Boolean_" + std::to_string(count)).c_str(), &value)) {
                    Global::m_actions.addAction(selectedNode, property.getName(), initialValue, value);
                }
                ImGui::PopItemWidth();


            }
            break;
        }

        count++;
    }


    // Render dialogs

    if (m_vec3Dialog.isOpen()) {
        m_vec3Dialog.draw();
    }

    if (m_colorDialog.isOpen()) {
        m_colorDialog.draw();
    }

    ImGui::End();
}


/**
* Draw the status bar
*/
void UserInterface::drawStatus() {
    ImGui::SetNextWindowPos(ImVec2(0, ofGetHeight() - STATUSBAR_HEIGHT)); // Position the status bar at the bottom
    ImGui::SetNextWindowSize(ImVec2(ofGetWidth(), STATUSBAR_HEIGHT)); // Set the height of the status bar

    ImGui::Begin("Status Bar", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    int countTotalRender = Global::m_countNodeRender[0] + Global::m_countNodeRender[1] + Global::m_countNodeRender[2];

    ImGui::Text("");
    std::string line = "Rendering " + std::to_string(countTotalRender) + " nodes, FPS: " + std::to_string(static_cast<int>(ofGetFrameRate()));
    ImVec2 textSize = ImGui::CalcTextSize(line.c_str(), nullptr, true);

    ImGui::SameLine(ImGui::GetWindowWidth() - textSize.x - 10);
    ImGui::Text(line.c_str());
    ImGui::End();
}

void UserInterface::drawCursor() {
    ImVec2 mousePos = ImGui::GetMousePos();
    ImGui::GetForegroundDrawList()->AddImage(
        (void*)(intptr_t)cursorImage.getTexture().getTextureData().textureID,
        mousePos,
        ImVec2(mousePos.x + cursorImage.getWidth(), mousePos.y + cursorImage.getHeight())
    );

}

void UserInterface::changeCursor()
{
    ofImage img;
    img.load("images/ui/cursors/" + currentCursor + ".png");

    // Retrieve piexels
    ofPixels& pixels = img.getPixels();

    // Go through all pixels
    for (int y = 0; y < img.getHeight(); ++y) {
        for (int x = 0; x < img.getWidth(); ++x) {
            ofColor color = pixels.getColor(x, y);
            if (color.a > 0) { // Si le pixel n'est pas transparent
                color = ofColor(255, 255, 255, color.a); // Swtich color to white while keeping original alpha value
                pixels.setColor(x, y, color);
            }
        }
    }

    // Update image
    img.update();
    cursorImage = img;
}


/**
 * Draw viewports
 */
void UserInterface::drawViewports() {
    if (m_onlyOneCamera) {
        float cameraSizeFirstWidth = (float) (ofGetWidth() - LEFTPANEL_WIDTH - 4);
        float cameraSizeFirstHeight = (float) ofGetHeight() - ImGui::GetFrameHeight() - STATUSBAR_HEIGHT -
                                      TOOLBAR_HEIGHT - 8;
        drawViewport("Main Camera", 0, ImVec2(LEFTPANEL_WIDTH + 2, ImGui::GetFrameHeight() + TOOLBAR_HEIGHT + 6),
                     ImVec2(cameraSizeFirstWidth, cameraSizeFirstHeight));
    } else {
        float cameraSizeFirstHeight = (float) ofGetHeight() - ImGui::GetFrameHeight() - STATUSBAR_HEIGHT -
                                      TOOLBAR_HEIGHT - 8;
        float cameraSizeSecond = ((float) ofGetHeight() - ImGui::GetFrameHeight() - STATUSBAR_HEIGHT - TOOLBAR_HEIGHT -
                                  8) / 2.0;
        float cameraSizeThird = cameraSizeSecond;
        float cameraSizeFirstWidth = (float) (ofGetWidth() - cameraSizeSecond - LEFTPANEL_WIDTH - 4);

        drawViewport("Main Camera", 0, ImVec2(LEFTPANEL_WIDTH + 2, ImGui::GetFrameHeight() + TOOLBAR_HEIGHT + 6),
                     ImVec2(cameraSizeFirstWidth, cameraSizeFirstHeight));
        drawViewport("Second Camera", 1,
                     ImVec2(LEFTPANEL_WIDTH + 4 + cameraSizeFirstWidth, ImGui::GetFrameHeight() + TOOLBAR_HEIGHT + 6),
                     ImVec2(cameraSizeSecond, cameraSizeSecond));
        drawViewport("Third Camera", 2,
                     ImVec2(LEFTPANEL_WIDTH + 4 + cameraSizeFirstWidth,
                            ImGui::GetFrameHeight() + TOOLBAR_HEIGHT + 7 + cameraSizeSecond),
                     ImVec2(cameraSizeThird, cameraSizeThird));
    }
}


/**
 * Draw a specific viewport (each viewport have a camera associated with)
 */
void UserInterface::drawViewport(const std::string &name, int index, const ImVec2 &position, const ImVec2 &size) {
    // Convert from float to integers
    std::string oldCursor = currentCursor;
    int size_x = static_cast<int>(size.x);
    int size_y = static_cast<int>(size.y);

    auto fbo = Global::m_cameras[index].getFbo();
    auto pickingFbo = Global::m_cameras[index].getPickingFbo();
    auto camera = Global::m_cameras[index].getCamera();

    Global::m_cameras[index].setViewportSize(size_x, size_y);

    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);

    ImGui::Begin(name.c_str(), nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing);

    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    auto textureID = reinterpret_cast<ImTextureID>(fbo->getTexture().getTextureData().textureID);
    ImVec2 imagePos = ImGui::GetCursorScreenPos();
    ImGui::Image(textureID, windowSize);
    int overlayItemHeight = 21;


    if (ImGui::IsWindowHovered()) {
        m_hoveredWindow = name;

        ImVec2 mousePos = ImGui::GetMousePos();

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            Global::m_transformTools.onMouseButtonReleased(mousePos);
        }
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            ImVec2 realSize = ImGui::GetItemRectSize();
            float localX = mousePos.x - imagePos.x;
            float localY = mousePos.y - imagePos.y;

            // Adjust for DPI scaling if needed
            float scaleFactor = ImGui::GetIO().DisplayFramebufferScale.x;
            localX /= scaleFactor;
            localY /= scaleFactor;

            // Also adjust if the image in the viewport is not the same size as the texture size
            float scaleX = realSize.x / size.x;
            float scaleY = realSize.y / size.y;
            localX /= scaleX;
            localY /= scaleY;

            auto pixelX = static_cast<int>(localX);
            auto pixelY = static_cast<int>(localY);

            // Ignore clicks on translate buttons area
            if (pixelY > overlayItemHeight) {
                ofPixels pixels;
                pickingFbo->readToPixels(pixels); // Read the entire FBO content to pixels

                // Get the color of the clicked pixel
                int pickedObjectId = Global::colorToId(pixels.getColor(pixelX, pixelY));
                //ofLog() << "picked:" << pixels.getColor(pixelX, pixelY) << " Object:" << pickedObjectId;
                if (pickedObjectId != 0) {
                    if (pickedObjectId >= TRANSLATE_X) {
                        Global::m_transformTools.onMouseButtonPressed(pickedObjectId, mousePos);
                    } else {
                        // Verify if the object exists (just in case)
                        if (Global::m_level.getTree()->findNode(pickedObjectId) != nullptr) {
                            if (Global::m_selectedNode != -1) {
                                Global::m_level.getTree()->findNode(Global::m_selectedNode)->displayBoundingBox(false);
                            }

                            Global::m_selectedNode = pickedObjectId;
                            Global::m_selectedFromViewport = true;
                            Global::m_level.getTree()->findNode(Global::m_selectedNode)->displayBoundingBox(true);
                        }
                    }
                } else {
                    if (Global::m_selectedNode != -1) {
                        Global::m_level.getTree()->findNode(Global::m_selectedNode)->displayBoundingBox(false);
                    }
                    Global::m_selectedNode = -1;
                    Global::m_selectedFromViewport = true;
                }
            }
        }
        if (Global::m_selectedNode != -1)
        {
            currentCursor = "hand";
        }
        else {
            currentCursor = "cursor";
        }
    }
    else{
        currentCursor = "cursor";
    }


    if (ImGui::IsWindowFocused()) {
        ImVec2 mousePos = ImGui::GetMousePos();

        // Check if an object is clicked
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            Global::m_transformTools.onMouseDrag(mousePos);
            if (Global::m_transformTools.getTransformMode() == TRANSFORM_MODE::TRANSLATE) {
                currentCursor = "move";
            }
            else if (Global::m_transformTools.getTransformMode() == TRANSFORM_MODE::ROTATE) {
                currentCursor = "rotate";
            }
            else if (Global::m_transformTools.getTransformMode() == TRANSFORM_MODE::SCALE) {
				currentCursor = "scale";
            }

        }


        m_selectedWindow = name;

        // Draw axis arrows
        float axisLength = 40.0f;

        ofVec3f rightDirection = camera->getXAxis();
        ofVec3f upDirection = camera->getYAxis();
        ofVec3f forwardDirection = camera->getZAxis();

        rightDirection *= axisLength;
        upDirection *= axisLength;
        forwardDirection *= axisLength;

        ImVec2 startPos = position + ImVec2(50, 70);
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        drawList->AddLine(startPos, startPos + ImVec2(rightDirection.x, rightDirection.y), IM_COL32(255, 0, 0, 255),
                          2.0f);
        drawList->AddLine(startPos, startPos + ImVec2(upDirection.x, upDirection.y), IM_COL32(0, 255, 0, 255), 2.0f);
        drawList->AddLine(startPos, startPos + ImVec2(forwardDirection.x, forwardDirection.y), IM_COL32(0, 0, 255, 255),
                          2.0f);
    }

    drawViewportOverlay(index, position, size_x, overlayItemHeight);

	if (oldCursor != currentCursor) {
        changeCursor();
	}
    ImGui::End();
}


void UserInterface::drawViewportOverlay(int index, const ImVec2& position, int availableWidth, int verticalOffset) {
// Hovering buttons and fields at the top of the camera viewport


    ofCustomCamera* camera = Global::m_cameras[index].getCamera();

    float overlayWidth = 378.0f;
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(position + ImVec2(10, verticalOffset - 2.0f), position + ImVec2(availableWidth - 10, verticalOffset + 21), IM_COL32(35, 35, 35, 255));

    ImGui::SetCursorPos(ImVec2(availableWidth - overlayWidth, verticalOffset));
    ImGui::BeginGroup();

    if (camera->getOrtho()) {
        if (ImGui::Button("Camera: Ortho", ImVec2(140, 16))) {
            camera->disableOrtho();
        }
    } else {
        ImGui::SetNextItemWidth(200);
        if (ImGui::Button("Camera: Perspective", ImVec2(140, 16))) {
            camera->enableOrtho();
        }
    }
    ImGui::SameLine();

    if (camera->getOrtho()) {
        float zoomFactor = camera->getOrthoZoom();
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputFloat("##CameraOrthoZoom", &zoomFactor, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue)) {
            camera->setOrthoZoom(zoomFactor);
        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Orthographic zoom factor");
        }

    } else {
        float fov = camera->getFov();
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputFloat("##CameraFOV", &fov,0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue)) {
            ofLog() << "Camera FOV changed to: " << fov;
            camera->setFov(fov);

        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Field of view (in degrees)");
        }

    }
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    float zNear = camera->getNearClip();
    if (ImGui::InputFloat("##CameraZNear", &zNear,0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue)) {
        ofLog() << "Camera ZNear changed to: " << zNear;
        camera->setNearClip(zNear);

    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Near clipping plane");
    }

    ImGui::SameLine();

    ImGui::SetNextItemWidth(60);
    float zFar = camera->getFarClip();
    if (ImGui::InputFloat("##CameraZFar", &zFar,0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue)) {
        ofLog() << "Camera ZFor changed to: " << zFar;
        camera->setFarClip(zFar);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Far clipping plane");
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        Global::m_cameras[index].reset();
    }
    ImGui::SameLine();

    // Draw transform buttons
    if ((index == 0)  && (Global::m_selectedNode != -1)) {

        ImGui::SetCursorPos(ImVec2(12, verticalOffset));
        ImGui::Text("Transform:");
        ImGui::SameLine();

        if (Global::m_transformTools.getTransformMode() == TRANSFORM_MODE::TRANSLATE) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
            if (ImGui::Button("Translate")) {
                Global::m_transformTools.setTransformMode(TRANSFORM_MODE::TRANSLATE);

            }
            ImGui::PopStyleColor(2);
        } else {
            if (ImGui::Button("Translate")) {
                Global::m_transformTools.setTransformMode(TRANSFORM_MODE::TRANSLATE);
            }
        }


        ImGui::SameLine();

        if (Global::m_transformTools.getTransformMode() == TRANSFORM_MODE::ROTATE) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);

            if (ImGui::Button("Rotate")) {
                Global::m_transformTools.setTransformMode(TRANSFORM_MODE::ROTATE);
            }
            ImGui::PopStyleColor(2);
        } else {
            if (ImGui::Button("Rotate")) {
                Global::m_transformTools.setTransformMode(TRANSFORM_MODE::ROTATE);
            }
        }
        ImGui::SameLine();

        if (Global::m_transformTools.getTransformMode() == TRANSFORM_MODE::SCALE) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);

            if (ImGui::Button("Scale")) {
                Global::m_transformTools.setTransformMode(TRANSFORM_MODE::SCALE);
            }
            ImGui::PopStyleColor(2);
        } else {
            if (ImGui::Button("Scale")) {
                Global::m_transformTools.setTransformMode(TRANSFORM_MODE::SCALE);
            }
        }

    }

    ImGui::EndGroup();


}


/**
 * Callback function : Show about dialog
 */
void UserInterface::onAboutProgram() {
    // TODO
}


/**
 * Callback function : New level
 */
void UserInterface::onNewLevel() {
    // TODO
}


/**
 * Callback function : load level
 */
void UserInterface::onLoadLevel() {
    // TODO
}


/**
 * Callback function : Save level
 */
void UserInterface::onSaveLevel() {
    // TODO
}


/**
 * Callback function : Generate atlas
 */
void UserInterface::onGenerateAtlas() {
    // TODO
}


/**
 * Display key bindings
 */
void UserInterface::onShowKeyBindings() {
}


/**
 * Callback function : History undo
 */
void UserInterface::onHistoryUndo() {
    Global::m_actions.undo();
}


/**
 * Callback function : History redo
 */
void UserInterface::onHistoryRedo() {
    Global::m_actions.redo();
}


/**
 * Toggle 3 or 1 camera
 */
void UserInterface::onToggleCameras() {
    m_onlyOneCamera = !m_onlyOneCamera;
}


/**
 * Get selected window
 */
const std::string &UserInterface::getSelectedWindow() const {
    return m_selectedWindow;
}


/**
 * Get hovered window
 */
const std::string &UserInterface::getHoveredWindow() const {
    return m_hoveredWindow;
}


/**
 * Only one camera need to be shown
 */
bool UserInterface::onlyOneCamera() const {
    return m_onlyOneCamera;
}


