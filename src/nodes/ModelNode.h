/*****************************************************
 * TP IFT3100H25 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * ModelNode class definition
 *
 *****************************************************/
#pragma once

#include "BaseNode.h"
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include <vector>


struct ModelNodeAnimationData
{
	ModelNodeAnimationData(int p_id, const std::string& p_dName, const std::string& p_pName) : m_animationID(p_id), m_devName(p_dName), m_playerFacingName(p_pName)	{}
	int m_animationID;
	std::string m_devName;
	std::string m_playerFacingName;
};


class ModelNode : public BaseNode {

private:
	ofxAssimpModelLoader m_model;
	bool m_playAnimation = false;
	std::vector<ModelNodeAnimationData*> m_poses;
	int m_currentPoseID;

public:
	explicit ModelNode(const std::string& p_name, const std::string& p_filePath);
	int draw(bool objectPicking, Camera* p_camera) override;
	std::vector<NodeProperty> getProperties() const override;
	void setProperty(const std::string& p_name, std::any p_value) override;
	std::string getCurrentAnimationName() const;
	ofxAssimpModelLoader& getModel();
	~ModelNode();

private:
	void initPosesList();
	void initAnimationsList();
	ModelNodeAnimationData* getCurrentAnimation() const;
	int getCurrentAnimationID() const;
	void setCurrentAnimationID(int ID);
	std::string makePlayerFacingName(const std::string& p_name);
	void debugPrintNames() const;

};
