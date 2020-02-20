#pragma once
#include "DisplayElementFBO.h"
#include "Utils.h"

class PFDIndicatorAltitude :
	public DisplayElementFBO
{
public:
	PFDIndicatorAltitude();
	~PFDIndicatorAltitude();

	void drawTex();

	void setDataSources(float * alt, float * radioAlt, float * vs);
	void setFont(std::map<GLchar, Character> * f);

private:
	float * data_altitude;
	float * data_radioAlt;
	float * data_vs;

	GLint texAltNum;

	GLuint fboAltNum;

	void drawTexAltNumber(float alt, float width, float height);

	std::map<GLchar, Character> * font;
};

