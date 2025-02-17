#include "KnightColorConverter.h"
#pragma once
class KnightColorConverterCYMK : public KnightColorConverter
{
public:
	KnightColorConverterCYMK();
	KnightColorRGB TransformTo(KnightColor currentKnightColor) override;
	KnightColor TransformFrom(KnightColorRGB currentKnightColor) override;
	ofColor GetColor(KnightColor currentKnightColor) override;
	std::vector<ofParameter<float>> getParameters(KnightColor currentKnightColor) override;
};