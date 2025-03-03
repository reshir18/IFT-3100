/*****************************************************
 * TP IFT3100H25 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * ModelNode class implementation
 *
 *****************************************************/

#include "ModelNode.h"

ModelNode::ModelNode(const std::string& p_name, const std::string& p_filePath) : BaseNode(p_name)
{
	m_model.loadModel(p_filePath);
	initPosesList();
	m_model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
	m_model.setPausedForAllAnimations(true);

	debugPrintNames();
}


int ModelNode::draw(bool p_objectPicking, Camera* p_camera)
{
	int count = 0;
	beginDraw(p_objectPicking);

	if (p_camera->testVisibility(m_transform.getGlobalPosition(), getBoundingBox())) {
		m_transform.transformGL();
		m_model.update();
		m_model.drawFaces();
		m_transform.restoreTransformGL();
		count++;
	}

	count += endDraw(p_objectPicking, p_camera);
	return count;
}


std::vector<NodeProperty> ModelNode::getProperties() const
{

	std::vector<NodeProperty> properties;
	properties.emplace_back("Name", PROPERTY_TYPE::TEXT_FIELD, m_name);
	properties.emplace_back("Transform", PROPERTY_TYPE::LABEL, nullptr);
	properties.emplace_back("Position", PROPERTY_TYPE::VECTOR3, m_transform.getPosition());
	properties.emplace_back("Orientation", PROPERTY_TYPE::VECTOR3, m_transform.getOrientationEulerDeg());
	properties.emplace_back("Scale", PROPERTY_TYPE::VECTOR3, m_transform.getScale());
	//no material property

	properties.emplace_back("Animation", PROPERTY_TYPE::LABEL, nullptr);
	properties.emplace_back("Pose", PROPERTY_TYPE::TEXT_FIELD, m_poses.at(getCurrentAnimationID())->m_playerFacingName);

	//TODO dropdown list for animations and a tick box for the loop param

	return properties;
}


void ModelNode::setProperty(const std::string& p_name, std::any p_value)
{
	BaseNode::setProperty(p_name, p_value);
	if (p_name == "Pose") {
		for (auto pose : m_poses)
		{
			if ((std::any_cast<std::string>(p_value) == pose->m_playerFacingName) || (std::any_cast<std::string>(p_value) == pose->m_devName))
			{
				setCurrentAnimationID(pose->m_animationID);
				break;
			}
		}
	}
}


/**
 * Get the animation's name
 */
std::string ModelNode::getCurrentAnimationName() const
{
	return getCurrentAnimation()->m_playerFacingName;
}


/**
 * Get the model
 */
ofxAssimpModelLoader& ModelNode::getModel()
{
	return m_model;
}


ModelNode::~ModelNode()
{
}


void ModelNode::initPosesList()
{
	bool addAnimation = true;
	std::string animName;
	for (unsigned int i = 0; i < m_model.getAnimationCount(); i++, addAnimation = true)
	{
		// Duration of 0 means the animation is a pose.
		if (m_model.getAnimation(i).getDurationInSeconds() == 0)
		{
			if (m_poses.size() != 0)
			{
				// for some reason, the model lists hundreds of animation (several times the same ones)
				// do not add the animation if it's already in the list
				for (auto anim : m_poses)
				{
					if (anim->m_devName == m_model.getAnimation(i).getAnimation()->mName.data)
					{
						addAnimation = false;
						break;
					}
				}
			}
			if (addAnimation)
			{
				animName = m_model.getAnimation(i).getAnimation()->mName.data;
				ModelNodeAnimationData* animData = new ModelNodeAnimationData(i, animName, makePlayerFacingName(animName));
				m_poses.push_back(animData);
			}
		}
	}
	if (m_poses.size() != 0)
	{
		int randomID = rand() % m_poses.size();
		m_currentPoseID = randomID;
	}

}



/**
 * 
 */
void ModelNode::initAnimationsList()
{
	/*
		TODO - find a way to avoid listing non-looping animations, or to handle them elegantly
	*/
}


/**
 * Get pointer to the current animation
 */
ModelNodeAnimationData* ModelNode::getCurrentAnimation() const
{
	return m_poses.at(m_currentPoseID);
}


/**
 * Get the current animation ID
 */
int ModelNode::getCurrentAnimationID() const
{
	return m_currentPoseID;
}


/**
 * Set the current animation ID
 */
void ModelNode::setCurrentAnimationID(int ID)
{
	m_currentPoseID = ID;
}


/**
 * Remove some characters from string names
 */
std::string ModelNode::makePlayerFacingName(const std::string& p_name)
{
	std::string playerFacingName = p_name;
	if (playerFacingName.size() != 0)
	{
		size_t pos = playerFacingName.find("_Pose"); //TODO list of forbidden words
		if (pos != string::npos)
		{
			playerFacingName.erase(pos, 5);
		}
		std::replace(playerFacingName.begin(), playerFacingName.end(), '_', ' ');
	}
	return playerFacingName;
}

void ModelNode::debugPrintNames() const
{
	ofLog() << "----------------------------";
	ofLog() << "- Printing names for [" << m_name << "]";
	for (auto anim : m_poses)
	{
		ofLog() << " --- Anim: " << anim->m_animationID << " : " << anim->m_devName << " (Player facing name: " << anim->m_playerFacingName << ")" ;
	}
}
