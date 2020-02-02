#pragma once
#include "DisplayElement.h"
#include "Utils.h"

class CircularProgressGauge :
	public DisplayElement
{
public:
	CircularProgressGauge();
	~CircularProgressGauge();

	void draw();
	void update();

	void setMaximum(float max);
	void setTextColor(float c[3]);
	void setColorExceed(float c[3]);
	void setDataSource(float * src);
	void configureMarker(float angle, float openness, float c[3]);
	void setNumberFormat(int prePoint, int postPoint);

	void setFont(std::map<GLchar, Character> * f);

private:
	float* data;
	float max = 100.0f;
	float color_text[3] = {1,1,1};
	float color_circle[3] = { 0.5f, 0.5f, 0.5f };
	float color_exceed[3] = { 1 , 0 , 0 };
	float color_marker[3] = { Utils::COLOR_FUCHSIA[0], Utils::COLOR_FUCHSIA[1] , Utils::COLOR_FUCHSIA[2] };
	float markerAngle = -M_PI;
	float markerOpen = M_PI / 20;
	int decimalsPre = 3;
	int decimalsPost = 1;

	std::map<GLchar, Character> * font;
};

