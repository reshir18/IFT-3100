/*****************************************************
* TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * UserInterface class definition
 *
 *****************************************************/
#pragma once
#include "colors/KnightColorConverterRGB.h"
#include "colors/KnightColorConverterCYMK.h"
#include "colors/KnightColorConverterHSV.h"

#include <ofxImGui.h>



class UserInterface {

public:
    void setup();
    void draw();

    // Some useful definitions
    static const int        TOOLBAR_HEIGHT        = 60;
    static const int        STATUSBAR_HEIGHT      = 30;
    static const int        LEFTPANEL_WIDTH       = 256;
    static const int        TREEVIEW_HEIGHT       = 240;
    static constexpr float  CAMERA_ASPECT_RATIO   = 16.0f / 9.0f;



private:
    ofxImGui::Gui m_gui;
    
    ofTexture m_textureToolbarNewLevel;
    ofTexture m_textureToolbarLoadLevel;
    ofTexture m_textureToolbarSaveLevel;
    ofTexture m_textureToolbarGenerateAtlas;

    // Draw functions

    void draw_menu();
    void draw_toolbar();
    void draw_tree();
    void draw_status();
    void draw_properties();
    void draw_viewports();
	void draw_color_setup();

    // Callback functions

    void on_about_program();
    void on_new_level();
    void on_load_level();
    void on_save_level();
    void on_generate_atlas();
    void on_history_undo();
    void on_history_redo();

	// Knight color setup
    
	KnightColorConverter* converter = new KnightColorConverterRGB();
	KnightColorRGB currentColorRGB;
    std::vector<ofParameter<float>> colorParameters;
    void knight_color_setup();
    void change_color_converter(KnightColorConverter* knightColorConverter);
    std::vector<float> getColorParameterValues() const;


};



