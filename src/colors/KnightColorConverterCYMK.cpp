#include "KnightColorConverterCYMK.h"

KnightColorConverterCYMK::KnightColorConverterCYMK() {
	name = "CYMK";
}

KnightColorRGB KnightColorConverterCYMK::TransformTo(KnightColor currentKnightColor) {
	float black = (1 - currentKnightColor.get_value4() / 100);
	int r = (int)(255 * (1 - currentKnightColor.get_value1() / 100) * black);
	int g = (int)(255 * (1 - currentKnightColor.get_value2() / 100) * black);
	int b = (int)(255 * (1 - currentKnightColor.get_value3() / 100) * black);
	KnightColorRGB arc(r, g , b , currentKnightColor.get_alpha());
	return arc;
}

KnightColor KnightColorConverterCYMK::TransformFrom(KnightColorRGB currentKnightColor) {
	float black = 1 - (std::max(std::max(currentKnightColor.get_red(), currentKnightColor.get_green()), currentKnightColor.get_blue()) / static_cast<float>(255));
	float cyan = (1 - currentKnightColor.get_red() / static_cast<float>(255) - black) / (1 - black);
	float magenta = (1 - currentKnightColor.get_green() / static_cast<float>(255) - black) / (1 - black);
	float yellow = (1 - currentKnightColor.get_blue() / static_cast<float>(255) - black) / (1 - black);
	black = round(black * 100);
	cyan = round(cyan * 100);
	magenta = round(magenta * 100);
	yellow = round(yellow * 100);
	KnightColor ac(cyan, magenta, yellow, black, currentKnightColor.get_alpha());
	return ac;
}

ofColor KnightColorConverterCYMK::GetColor(KnightColor currentKnightColor) {
	KnightColorRGB rgb = TransformTo(currentKnightColor);
	return ofColor(rgb.get_red(), rgb.get_green(), rgb.get_blue(), rgb.get_alpha());
}

std::vector<ofParameter<float>> KnightColorConverterCYMK::getParameters(KnightColor currentKnightColor) {
	std::vector<ofParameter<float>> params;
	params.push_back(ofParameter<float>("Cyan", currentKnightColor.get_value1(), 0.0f, 100.0f));
	params.push_back(ofParameter<float>("Yellow", currentKnightColor.get_value2(), 0.0f, 100.0f));
	params.push_back(ofParameter<float>("Magenta", currentKnightColor.get_value3(), 0.0f, 100.0f));
	params.push_back(ofParameter<float>("Black", currentKnightColor.get_value4(), 0.0f, 100.0f));
	params.push_back(ofParameter<float>("Alpha", currentKnightColor.get_alpha(), 0.0f, 255.0f));
	return params;
}
