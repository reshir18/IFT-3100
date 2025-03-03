/*****************************************************
* TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * Level class definition
 *
 *****************************************************/
#pragma once

#include "nodes/BaseNode.h"

class Level {

private:
	BaseNode* m_tree;

public:
	Level();

	BaseNode* getTree();

	void reset();
	bool loadFromFile(const std::string& p_filename);
	bool saveToFile(const std::string& p_filename);
	int draw(bool p_objectPicking, Camera* p_camera);

};

