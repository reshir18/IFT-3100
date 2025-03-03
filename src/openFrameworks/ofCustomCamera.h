/*****************************************************
* TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * ofCustomCamera class definition
 *
 *****************************************************/
#pragma once
#include <ofCamera.h>


class ofCustomCamera : public ofCamera {

private:

    float m_orthoZoom = 1.0f;

public:
    float getOrthoZoom();
    void setOrthoZoom(float p_zoom);
    void enableOrtho();
    void begin();
};

