/*****************************************************
 * TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * BaseNode class definition
 *
 *****************************************************/
#pragma once
#include <Camera.h>
#include <ofMaterial.h>
#include <ofNode.h>
#include "NodeProperty.h"

class BaseNode {
protected:
    std::vector<BaseNode *> m_children;
    BaseNode* m_parentNode;

    ofNode m_transform;
    std::string m_name = "Unnamed";
    int m_id = 0;
    bool m_displayBoundingBox = false;
    bool m_displayNode = true;

    ofMaterial m_materialNode;
    ofMaterial m_materialUnlit;

    void beginDraw(bool p_objectPicking);

    int endDraw(bool p_objectPicking, Camera* p_camera);

    void drawBoundingBox();

public:
    virtual ~BaseNode();

    explicit BaseNode(const std::string &p_name);

    virtual int draw(bool p_objectPicking, Camera* p_camera);

    virtual ofVec3f getBoundingBox() const;

    void setParent(BaseNode* p_parentNode);
    BaseNode* getParent();

    void addChild(BaseNode *p_child);
    void swapChildOrder(BaseNode* p_child1, BaseNode* p_child2);
    void removeChild(int p_index);

    ofNode &getTransform() { return m_transform; };
    void setName(const std::string &p_name) { m_name = p_name; };
    const std::string &getName() { return m_name; }

    int getId() const;
    bool getDisplayNode() const;

    void displayNode(bool p_display);
    void displayBoundingBox(bool p_display);

    std::vector<BaseNode *> &getChildren() { return m_children; };

    virtual std::vector<NodeProperty> getProperties() const;
    virtual void setProperty(const std::string &p_name, std::any p_value);

    BaseNode* findNode(int p_id);
    BaseNode* getPreviousNode();
    BaseNode* getNextNode();
};
