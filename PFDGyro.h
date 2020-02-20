#pragma once
#include "DisplayElementFBO.h"
#include "Utils.h"

class PFDGyro :
	public DisplayElementFBO
{
public:
	PFDGyro();
	~PFDGyro();

	void draw();

	void drawTex();
	void drawGyro(float width, float height);

	void setBounds(int x, int y, int width, int height);
	
	void setDataSources(float * pitch, float * roll, float * hdg);

	void setFont(std::map<GLchar, Character> * f);

private:
	float * data_pitch;
	float * data_roll;
	float * data_hdg;

	bool initPitch = false;
	bool initHdg = false;

	GLint texPitch;
	GLint texHdg;

	GLuint fboPitch;
	GLuint fboHdg;

	void drawTexPitchIndicator(float pitch, float width, float height);
	void drawTexHdgIndicator(float pitch, float width, float height);

	std::map<GLchar, Character> * font;
};

