/*****************************************************
 * TP IFT3100H25 - Adventure Party Maker
 * by Team 12
 *****************************************************
 *
 * SplineNode class definition
 *
 *****************************************************/
#pragma once
#include "BaseNode.h"
#include "SplineControlPoint.h"
#include <ofPath.h>
#include <vector>

class SplineNode : public BaseNode
{
public:
    explicit SplineNode(const std::string& p_nodeName);
    std::string getClassName() override { return "SplineNode"; }
    void init(int p_controlPointsCount = 6);

private:
    ofPath m_path;
    const float m_DEFAULT_SPACING = 200.0;
    const float m_DEFAULT_STROKE_WIDTH = 100.0f;
    float m_strokeWidth;
    std::vector<SplineControlPoint*> m_controlPoints;
    bool m_dirty = false;
    int m_toDelete = -1;

public:
    void updatePath();
    int draw(bool p_objectPicking, Camera* p_camera);

    void nodeChanged(const std::string& p_name = "", std::any p_value = nullptr) override;

    std::vector<NodeProperty> getProperties() const override;
    void removeAllChildren() override {}; // This class uses his own housekeeping functions
    void setProperty(const std::string& p_name, std::any p_value);

private:
    void setDisplayNodeOnControlPoints(bool p_value);
};

