/*****************************************************
 * TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * KnightColorConverterRGB class implementation
 *
 *****************************************************/
#include "KnightColorConverter.h"
#pragma once
class KnightColorConverterRGB : public KnightColorConverter
{
public:
	KnightColorConverterRGB();
	KnightColorRGB TransformTo(KnightColor currentKnightColor) override;
	KnightColor TransformFrom(KnightColorRGB currentKnightColor) override;
	ofColor GetColor(KnightColor currentKnightColor) override;
	std::vector<ofParameter<float>> getParameters(KnightColor currentKnightColor) override;
};