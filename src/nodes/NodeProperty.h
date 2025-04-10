/*****************************************************
 * TP IFT3100H25 - Adventure Party Maker
 * by Team 12
 *****************************************************
 *
 * NodeProperty class definition
 *
 *****************************************************/
#pragma once
#include <any>
#include <string>

enum PROPERTY_TYPE {
	TEXT_FIELD, //An editable field in which the user can write text
	VECTOR3,
	COLOR_PICKER,
	FLOAT_FIELD,
	PRECISE_FLOAT_FIELD,
	LABEL, //Use to organise properties into categories; a label is the title of a category
	BOOLEAN_FIELD, //A checkbox that the user can tick
	INT_FIELD,
	ITEM_LIST, //Pass a std::vector<std::string> with the currentItem placed at the back
	ITEM_CLIST, //Pass a std::pair<int, std::vector<const char*>>: first is the currenItem and second is the list of options
	TEXT_INFO, //Use to display information about the node, for example the value of a variable that the user can't directly modify (i.e.: the index of a spline control point)
	DUMB_BUTTON, //A simple button that does something when clicked. It takes a boolean value, but you don't have to use it (for a toggle, use BOOLEAN_FIELD instead)
};


class NodeProperty {

private:

	std::string m_name;
	int m_type;
	std::any m_value;
	std::string m_tooltip;

public:

	NodeProperty(std::string p_name, int p_type, std::any p_value, std::string p_tooltip = "");
	const std::string getName();
	const int getType();
	const std::any getValue();
	const std::string getTooltip();

};

