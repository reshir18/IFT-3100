/*****************************************************
* TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * BaseNode class implementation
 *
 *****************************************************/
#include "BaseNode.h"

#include <Global.h>
#include <of3dGraphics.h>
#include <ofGraphics.h>


/**
  * Constructor
  */
BaseNode::BaseNode(const std::string& p_name) : m_name(p_name) {

	static int id_next = 1;
	m_id = id_next++;

	m_materialUnlit.setEmissiveColor(ofFloatColor(1.0, 1.0, 0.0));
	m_materialUnlit.setAmbientColor(ofFloatColor(0.0, 0.0, 0.0));
	m_materialUnlit.setDiffuseColor(ofFloatColor(0.0));
	m_materialUnlit.setSpecularColor(ofFloatColor(0.0));

	m_materialNode.setShininess(100);  // Controls specular reflection
	m_materialNode.setSpecularColor(ofColor(255, 255, 255));  // Highlights

}


/**
 * Destructor
 */
BaseNode::~BaseNode() {

	ofLog() << "Delete base node: " << m_name;
	for (BaseNode* child : m_children) {
		delete child;
	}

	m_children.clear();

}


/**
 * Draw node content
 */
int BaseNode::draw(bool p_objectPicking, Camera* p_camera) {

	if (!m_displayNode) return 0;

	beginDraw(p_objectPicking);
	// Nothing to render
	return endDraw(p_objectPicking, p_camera);

}


/**
* Retrieve unique ID associated to the node
*/
int BaseNode::getId() const {
	return m_id;
}


/**
* Toggle display of a red bounding box, drawing is honored in draw function
*/
void BaseNode::displayBoundingBox(bool display) {
	m_displayBoundingBox = display;
}


/**
 * Add child node
 */
void BaseNode::addChild(BaseNode* p_child) {
	p_child->m_transform.setParent(m_transform);
	p_child->setParent(this);
	m_children.push_back(p_child);
}


/**
* Swap child order
*/
void BaseNode::swapChildOrder(BaseNode* p_child1, BaseNode* p_child2) {

	std::vector<BaseNode *> temp_children;

	for (BaseNode* child : m_children) {
		if (child == p_child1) {
			temp_children.push_back(p_child2);
		} else if (child == p_child2) {
			temp_children.push_back(p_child1);
		} else {
			temp_children.push_back(child);
		}
	}

	m_children = temp_children;
}


/**
 * Reveal properties to the editor
 */
std::vector<NodeProperty> BaseNode::getProperties() const {

	std::vector<NodeProperty> properties;
	properties.emplace_back("Name", PROPERTY_TYPE::TEXT_FIELD, m_name);
	properties.emplace_back("Display",PROPERTY_TYPE::BOOLEAN_FIELD, m_displayNode);
	properties.emplace_back("Transform", PROPERTY_TYPE::LABEL, nullptr);
	properties.emplace_back("Position", PROPERTY_TYPE::VECTOR3, m_transform.getPosition());
	properties.emplace_back("Orientation", PROPERTY_TYPE::VECTOR3, m_transform.getOrientationEulerDeg());
	properties.emplace_back("Scale", PROPERTY_TYPE::VECTOR3, m_transform.getScale());
	properties.emplace_back("Material", PROPERTY_TYPE::LABEL, nullptr);
	properties.emplace_back("Diffuse Color", PROPERTY_TYPE::COLOR_PICKER, m_materialNode.getDiffuseColor());
	properties.emplace_back("Ambient Color", PROPERTY_TYPE::COLOR_PICKER, m_materialNode.getAmbientColor());
	properties.emplace_back("Emissive Color", PROPERTY_TYPE::COLOR_PICKER, m_materialNode.getEmissiveColor());
	properties.emplace_back("Specular Color", PROPERTY_TYPE::COLOR_PICKER, m_materialNode.getSpecularColor());

	return properties;

}


/**
* Set property from editor
*/
void BaseNode::setProperty(const std::string& p_name, std::any p_value) {
	if (p_name == "Name") {
		m_name = std::any_cast<std::string>(p_value);
	}

	if (p_name == "Display") {
		m_displayNode = std::any_cast<bool>(p_value);
	}

	if (p_name == "Position") {
		m_transform.setPosition(std::any_cast<glm::vec3>(p_value));
	}

	if (p_name == "Orientation") {
		m_transform.setOrientation(std::any_cast<glm::vec3>(p_value));
	}

	if (p_name == "Scale") {
		ofLog() << "Scale:" << std::any_cast<glm::vec3>(p_value).x;
		m_transform.setScale(std::any_cast<glm::vec3>(p_value));
	}

	if (p_name == "Diffuse Color") {
		m_materialNode.setDiffuseColor(std::any_cast<ofFloatColor>(p_value));
	}

	if (p_name == "Ambient Color") {
		m_materialNode.setAmbientColor(std::any_cast<ofFloatColor>(p_value));
	}

	if (p_name == "Emissive Color") {
		m_materialNode.setEmissiveColor(std::any_cast<ofFloatColor>(p_value));
	}

	if (p_name == "Specular Color") {
		m_materialNode.setSpecularColor(std::any_cast<ofFloatColor>(p_value));
	}
}


/**
* Find node (recursive search) by ID
*/
BaseNode* BaseNode::findNode(int p_id) {
	if (p_id == m_id) return this;
	for (BaseNode* child : m_children) {
		BaseNode* result = child->findNode(p_id);
		if (result != nullptr) return result;
	}

	return nullptr;
}


/**
 * Set parent node
 */
void BaseNode::setParent(BaseNode* p_parentNode) {
	m_parentNode = p_parentNode;
}


/**
* Get parent node
*/
BaseNode* BaseNode::getParent() {
	return m_parentNode;
}


/**
 * Remove child
 */
void BaseNode::removeChild(int p_index) {

	int index = 0;
	for (BaseNode* child : m_children) {
		if (child->getId() == p_index) {
			delete child;
			m_children.erase(m_children.begin() + index);
			return;
		}
		index++;
	}


}


/**
 * Begin draw context
 */
void BaseNode::beginDraw(bool p_objectPicking) {

	if (!p_objectPicking) {
		m_materialNode.begin();
	}
	else {
		ofSetColor(Global::idToColor(m_id));
	}

}


/**
 * End draw context
 */
int BaseNode::endDraw(bool p_objectPicking, Camera* p_camera) {
	int count = 0;
	if (!p_objectPicking) {
		m_materialNode.end();

		if (m_displayBoundingBox) {
			m_materialUnlit.begin();
			drawBoundingBox();
			m_materialUnlit.end();
		}
	}

	for (BaseNode* child : m_children) {
		count += child->draw(p_objectPicking, p_camera);
	}
	return count;
}


/**
 * Get display node
 */
bool BaseNode::getDisplayNode() const {
	return m_displayNode;
}


/**
 * Set display node
 */
void BaseNode::displayNode(bool p_display) {
	m_displayNode = p_display;
}


/**
 * Get bounding box for current node
 */
ofVec3f BaseNode::getBoundingBox() const {
	return {10.0, 10.0, 10.0};
}


/**
 * Draw bounding box
 */
void BaseNode::drawBoundingBox() {
	ofVec3f boundingBox = getBoundingBox();
	ofNoFill();
	m_transform.transformGL();
	ofDrawBox(glm::vec3(0, 0, 0), boundingBox.x, boundingBox.y, boundingBox.z);
	m_transform.restoreTransformGL();

}


/**
* Get previous node
*/
BaseNode* BaseNode::getPreviousNode() {

	BaseNode* parent = getParent();
	for  (int i=0;i<parent->m_children.size();i++) {
		if (parent->m_children[i] == this) {
			if (i > 0) return parent->m_children[i-1];
		}
	}

	return nullptr;
}


/**
* Get next node
*/
BaseNode* BaseNode::getNextNode() {
	BaseNode* parent = getParent();
	for  (int i=0;i<parent->m_children.size();i++) {
		if (parent->m_children[i] == this) {
			if (i+1 < parent->m_children.size()) return parent->m_children[i+1];
		}
	}

	return nullptr;
}
