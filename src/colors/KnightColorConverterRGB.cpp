/*****************************************************
 * TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * KnightColorConverterRGB class implementation
 *
 *****************************************************/

#include "KnightColorConverterRGB.h"
KnightColorConverterRGB::KnightColorConverterRGB() {
	name = "RGB";
}

KnightColorRGB KnightColorConverterRGB::TransformTo(KnightColor currentKnightColor) {
	KnightColorRGB arc(currentKnightColor.get_value1(), currentKnightColor.get_value2(), currentKnightColor.get_value3(), currentKnightColor.get_alpha());
	return arc;
}

KnightColor KnightColorConverterRGB::TransformFrom(KnightColorRGB currentKnightColor) {
	float red = currentKnightColor.get_red();
	float green = currentKnightColor.get_green();
	float blue = currentKnightColor.get_blue();
	int alpha = currentKnightColor.get_alpha();
	KnightColor ac(red, green, blue,0.0f, currentKnightColor.get_alpha());
	return ac;
}

ofColor KnightColorConverterRGB::GetColor(KnightColor currentKnightColor) {
	return ofColor(currentKnightColor.get_value1(), currentKnightColor.get_value2(), currentKnightColor.get_value3(), currentKnightColor.get_alpha());
}

std::vector<ofParameter<float>> KnightColorConverterRGB::getParameters(KnightColor currentKnightColor)  {
	std::vector<ofParameter<float>> params;
	params.push_back(ofParameter<float>("Red", currentKnightColor.get_value1(), 0.0f, 255.0f));
	params.push_back(ofParameter<float>("Green", currentKnightColor.get_value2(), 0.0f, 255.0f));
	params.push_back(ofParameter<float>("Blue", currentKnightColor.get_value3(), 0.0f, 255.0f));
	params.push_back(ofParameter<float>("Alpha", currentKnightColor.get_alpha(), 0.0f, 255.0f));
	return params;
}