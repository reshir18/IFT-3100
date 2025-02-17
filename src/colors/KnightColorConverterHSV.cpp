/*****************************************************
 * TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * KnightColorConverterHSV class implementation
 *
 *****************************************************/
#include "KnightColorConverterHSV.h"

KnightColorConverterHSV::KnightColorConverterHSV() {
	name = "HSV";
}

KnightColorRGB KnightColorConverterHSV::TransformTo(KnightColor currentKnightColor) {
	float hue = currentKnightColor.get_value1();
	float saturation = currentKnightColor.get_value2() / 100.00f;
	float value = currentKnightColor.get_value3() / 100.00f;
	float ligthness = abs(2 * value - 1);
	float chroma = value * saturation;
	float x = chroma * (1 - abs(fmod(hue / 60, 2) - 1));
	float m = value - chroma;
	float r = 0.00f;
	float g = 0.00f;
	float b = 0.00f;
	if (hue < 60.00f)
	{
		r = chroma + m;
		g = x + m;
		b = m;
	}
	else if (hue < 120.00f)
	{
		r = x + m;
		g = chroma + m;
		b = m;
	}
	else if (hue < 180.00f)
	{
		r = m;
		g = chroma + m;
		b = x + m;
	}
	else if (hue < 240.00f)
	{
		r = m;
		g = x + m;
		b = chroma + m;
	}
	else if (hue < 300.00f)
	{
		r = x + m;
		g = m;
		b = chroma + m;
	}
	else if (hue < 360.00f)
	{
		r = chroma + m;
		g = m;
		b = x + m;
	}

	KnightColorRGB arc(r * 255.00f, g * 255.00f, b * 255.00f, currentKnightColor.get_alpha());
	return arc;
}

KnightColor KnightColorConverterHSV::TransformFrom(KnightColorRGB currentKnightColor) {
	int red = currentKnightColor.get_red();
	int green = currentKnightColor.get_green();
	int blue = currentKnightColor.get_blue();
	int alpha = currentKnightColor.get_alpha();

	float maxValue = std::max({ red, green, blue });
	float minValue = std::min({ red, green, blue });
	float maxMin = maxValue - minValue;
	
	minValue = maxMin / maxValue;
	float value = maxValue / static_cast<float>(255) * 100;
	minValue = minValue * 100;
	
	float hueValue = 0;
	int maxValueInt = (int)maxValue;

	if (maxValueInt == red)
	{
		hueValue = (green - blue) / maxMin;
	}
	else if (maxValueInt == green) {
		hueValue = (blue - red) / maxMin + 2;
	}
	else if (maxValueInt == blue) {
		hueValue = (red - green) / maxMin + 4;
	}
	hueValue = 60 * hueValue;
	KnightColor ac(hueValue, minValue, value,0.0f, alpha);

	return ac;
}

ofColor KnightColorConverterHSV::GetColor(KnightColor currentKnightColor) {
	return ofColor::fromHsb(currentKnightColor.get_value1(), currentKnightColor.get_value2(), currentKnightColor.get_value3(), currentKnightColor.get_alpha());
}

std::vector<ofParameter<float>> KnightColorConverterHSV::getParameters(KnightColor currentKnightColor) {
	std::vector<ofParameter<float>> params;
	params.push_back(ofParameter<float>("Hue", currentKnightColor.get_value1(), 0.0f, 100.0f));
	params.push_back(ofParameter<float>("Saturation", currentKnightColor.get_value2(), 0.0f, 100.0f));
	params.push_back(ofParameter<float>("Value", currentKnightColor.get_value3(), 0.0f, 100.0f));
	params.push_back(ofParameter<float>("Alpha", currentKnightColor.get_alpha(), 0.0f, 255.0f));
	return params;
}
