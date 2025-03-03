/*****************************************************
* TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * GroupNode class implementation
 *
 *****************************************************/
#include "GroupNode.h"


/**
  * Constructor
  */
GroupNode::GroupNode(const std::string& p_name) : BaseNode(p_name) {
}


/**
 * Get properties
 */
std::vector<NodeProperty> GroupNode::getProperties() const {
    std::vector<NodeProperty> properties;
    properties.emplace_back("Name", PROPERTY_TYPE::TEXT_FIELD, m_name);
    properties.emplace_back("Display",PROPERTY_TYPE::BOOLEAN_FIELD, m_displayNode);
    properties.emplace_back("Transform", PROPERTY_TYPE::LABEL, nullptr);
    properties.emplace_back("Position", PROPERTY_TYPE::VECTOR3, m_transform.getPosition());
    properties.emplace_back("Orientation", PROPERTY_TYPE::VECTOR3, m_transform.getOrientationEulerDeg());
    properties.emplace_back("Scale", PROPERTY_TYPE::VECTOR3, m_transform.getScale());
    return properties;
}


/**
 * Get bounding box for current node
 */
ofVec3f GroupNode::getBoundingBox() const {
    return {10.0, 10.0, 10.0};
}
