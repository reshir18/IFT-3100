/*****************************************************
* TP IFT3100H24 - Knight Maker
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
	TEXT_FIELD,
	VECTOR3,
	COLOR_PICKER,
	FLOAT_FIELD,
	LABEL,
	BOOLEAN_FIELD,
	INT_FIELD
};


class NodeProperty {

private:

	std::string m_name;
	int m_type;
	std::any m_value;

public:

	NodeProperty(std::string p_name, int p_type, std::any p_value);
	const std::string getName();
	const int getType();
	const std::any getValue();

};

