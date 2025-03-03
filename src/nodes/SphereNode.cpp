/*****************************************************
* TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * SphereNode class implementation
 *
 *****************************************************/
#include "SphereNode.h"

#include <Global.h>
#include <of3dGraphics.h>
#include <ofGraphics.h>
#include <ofLight.h>


/**
 * Constructor
 */
SphereNode::SphereNode(const std::string& p_name) : BaseNode(p_name) {
	m_primitive.setRadius(100.0);
	m_primitive.setResolution(16.0);
	m_primitive.setPosition(0, 0, 0);
}


/**
 * Draw node content
 */
int SphereNode::draw(bool p_objectPicking, Camera* p_camera) {
    if (!m_displayNode) return 0;
    int count = 0;
    beginDraw(p_objectPicking);

    if (p_camera->testVisibility(m_transform.getGlobalPosition(), getBoundingBox())) {
        m_transform.transformGL();
        m_primitive.draw();
        m_transform.restoreTransformGL();
        count++;
    }
	count += endDraw(p_objectPicking, p_camera);
    return count;
}


/**
 * Set sphere radius
 */
void SphereNode::setRadius(float p_radius) {
	m_primitive.setRadius(p_radius);
}


/**
 * Get bounding box
 */
ofVec3f SphereNode::getBoundingBox() const {
	const float size = m_primitive.getRadius() * 2.0f;
	return {size, size, size};
}

/**
 * Get properties
 */
std::vector<NodeProperty> SphereNode::getProperties() const {
    auto properties = BaseNode::getProperties();
    properties.emplace_back("Radius", PROPERTY_TYPE::FLOAT_FIELD, m_primitive.getRadius());
    properties.emplace_back("Resolution", PROPERTY_TYPE::INT_FIELD, m_primitive.getResolution());
    return properties;
}

/**
 * Set property
 */
void SphereNode::setProperty(const std::string &p_name, std::any p_value) {
    if (p_name == "Radius") {
        m_primitive.setRadius(std::any_cast<float>(p_value));
        return;
    }

    if (p_name == "Resolution") {
        m_primitive.setResolution(std::any_cast<int>(p_value));
        return;
    }

    BaseNode::setProperty(p_name, std::any(p_value));
}

/**
 * Set resolution
 */
void SphereNode::setResolution(int p_resolution) {
    m_primitive.setResolution(p_resolution);
}
