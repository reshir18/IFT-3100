/*****************************************************
* TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * SphereNode class definition
 *
 *****************************************************/
#pragma once
#include <of3dPrimitives.h>

#include "BaseNode.h"


class SphereNode : public BaseNode {

private:

	ofSpherePrimitive m_primitive;


public:
	explicit SphereNode(const std::string& p_name);
	int draw(bool p_objectPicking, Camera* p_camera) override;
	ofVec3f getBoundingBox() const override;
	void setRadius(float p_radius);
	void setResolution(int p_resolution);
	std::vector<NodeProperty> getProperties() const override;
	void setProperty(const std::string &p_name, std::any p_value) override;
};
