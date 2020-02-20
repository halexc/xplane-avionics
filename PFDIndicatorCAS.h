#pragma once
#include "DisplayElementFBO.h"
#include "Utils.h"

class PFDIndicatorCAS :
	public DisplayElementFBO
{
public:
	PFDIndicatorCAS();
	~PFDIndicatorCAS();
	
	void drawTex();

	void setDataSources(float * cas, float * acceleration);
	void setFont(std::map<GLchar, Character> * f);

private:
	float * data_cas;
	float * data_acceleration;

	bool initAS = false;

	GLint texASnum;

	GLuint fboASnum;

	void drawTexIASNumber(float speed, float width, float height);

	std::map<GLchar, Character> * font;
};

