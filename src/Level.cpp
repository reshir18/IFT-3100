/*****************************************************
* TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * Level class definition
 *
 *****************************************************/
#include "Level.h"

#include <GroupNode.h>
#include <imgui.h>

#include "SphereNode.h"
#include "ModelNode.h"

 /**
  * Constructor
  */
Level::Level() {

	m_tree = new BaseNode("Level");

}


/**
 * Draw node tree
 */
int Level::draw(bool objectPicking, Camera* p_camera) {

	m_tree->getTransform().transformGL();
	int count = m_tree->draw(objectPicking, p_camera);
	m_tree->getTransform().restoreTransformGL();
	return count;
}


/**
* Retrieve node tree (first node)
*/
BaseNode* Level::getTree() {
	return m_tree;
}


/**
 * Load level from file
 */
bool Level::loadFromFile(const std::string& filename)
{
	return false;
}


/**
 * Save level to file
 */
bool Level::saveToFile(const std::string& filename)
{
	return false;
}


/**
 * Reset to a level with default values
 */
void test_spawn_sphere(BaseNode* tree, int sphere_count, float sphere_spacing, float z, int depth = 0) {

	for (int y = 0; y < sphere_count; y++) {
		for (int x = 0; x < sphere_count; x++) {
			SphereNode* node = new SphereNode("Sphere_" + std::to_string(x) + "_" + std::to_string(y));
			node->setProperty("Diffuse Color", ofFloatColor(1, 0, 0));
			float ox = -((float)sphere_count * sphere_spacing) / 4.0f;
			float oy = -((float)sphere_count * sphere_spacing) / 4.0f;
			tree->addChild(node);
			float xx = ox + ((float)x * sphere_spacing);
			float yy = oy + ((float)y * sphere_spacing);
			node->getTransform().setPosition(xx, yy, z);

			if (depth == 0) {
				node->setRadius(100.0f);
				test_spawn_sphere(node, 2, 150.0, 0, 1);
			}
			else {
				node->setRadius(50.0f);
			}
		}
	}
}


/**
 * Reset level
 */
void Level::reset() {


	GroupNode* node = new GroupNode("Group");
	node->getTransform().setPosition(0, 0, 0);
	m_tree->addChild(node);

	int sphere_count = 10;
	float sphere_spacing = 320.0f;
	for (int z = 0; z < 1; z++) {
	  	test_spawn_sphere(node, sphere_count, sphere_spacing, z * sphere_spacing, 0);
	 }

	// ModelNode* node_skeleton = new ModelNode("Skeleton", "Skeletons/characters/gltf/Skeleton_Warrior.glb");
	// node_skeleton->getModel().setAnimation(67);
	// node_skeleton->getModel().setRotation(0, 180, 1, 0, 0);
	// node_skeleton->getTransform().setPosition(0, 0, 0);
	// m_tree->addChild(node_skeleton);



	// ModelNode* node_rogue = new ModelNode("Rogue", "Kaykit/Characters/gltf/Rogue.glb");
	// m_tree->addChild(node_rogue);
	// node_rogue->getModel().setRotation(0, 180, 1, 0, 0);
	//
	//
	// ModelNode* node_barbarian = new ModelNode("Barbarian", "Kaykit/Characters/gltf/Barbarian.glb");
	// m_tree->addChild(node_barbarian);
	// node_barbarian->getModel().setPosition(350,0,400);
	// node_barbarian->getModel().setRotation(0, 180, 1, 0, 0);
	//
	// ModelNode* node_mage = new ModelNode("Mage", "Kaykit/Characters/gltf/Mage.glb");
	// m_tree->addChild(node_mage);
	// node_mage->getModel().setPosition(-300, 0, 0);
	// node_mage->getModel().setRotation(0, 180, 1, 0, 0);
	//
	// ModelNode* node_engineer = new ModelNode("Engineer", "Kaykit/Characters/gltf/Engineer.glb");
	// m_tree->addChild(node_engineer);
	// node_engineer->getModel().setPosition(-700, 0, -250);
	// node_engineer->getModel().setRotation(0, 180, 1, 0, 0);
	//
	// ModelNode* node_druid = new ModelNode("Druid", "Kaykit/Characters/gltf/Druid.glb");
	// m_tree->addChild(node_druid);
	// node_druid->getModel().setPosition(600, 0, -400);
	// node_druid->getModel().setRotation(0, 180, 1, 0, 0);

}
