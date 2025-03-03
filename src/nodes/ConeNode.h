/*****************************************************
* TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * ConeNode class definition
 *
 *****************************************************/
#pragma once
#include <of3dPrimitives.h>
#include "BaseNode.h"


class ConeNode : public BaseNode {

private:

    ofConePrimitive m_primitive;


public:
    explicit ConeNode(const std::string& p_name);
    int draw(bool p_objectPicking, Camera* p_camera) override;
    ofVec3f getBoundingBox() const override;
    void setRadius(float p_radius);
    void setHeight(float p_radius);
    void setResolutionRadius(int p_resolution);
    void setResolutionHeight(int p_resolution);
    std::vector<NodeProperty> getProperties() const override;
    void setProperty(const std::string &p_name, std::any p_value) override;
};
