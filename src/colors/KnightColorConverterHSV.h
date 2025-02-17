/*****************************************************
 * TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * KnightColorConverterHSV class implementation
 *
 *****************************************************/
#include "KnightColorConverter.h"
#include <math.h>
#pragma once

class KnightColorConverterHSV : public KnightColorConverter
{
public:
	KnightColorConverterHSV();
	KnightColorRGB TransformTo(KnightColor currentKnightColor) override;
	KnightColor TransformFrom(KnightColorRGB currentKnightColor) override;
	ofColor GetColor(KnightColor currentKnightColor) override;
	std::vector<ofParameter<float>> getParameters(KnightColor currentKnightColor) override;
};


