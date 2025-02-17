/*****************************************************
 * TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * KnightColorConverter class implementation
 *
 *****************************************************/
#include "KnightColor.h"
#include "KnightColorRGB.h"
#include <ofxImGui.h>
#include <ofColor.h>
#include <cmath>
#include <vector>

#pragma once
class KnightColorConverter
{
protected:
	std::string name;
public:
	KnightColorConverter();
	virtual KnightColorRGB TransformTo(KnightColor currentKnightColor) = 0;
	virtual KnightColor TransformFrom(KnightColorRGB currentKnightColor) = 0;
	virtual ofColor GetColor(KnightColor currentKnightColor) = 0;
	virtual std::vector<ofParameter<float>> getParameters(KnightColor currentKnightColor) = 0;
	std::string get_name();
};

