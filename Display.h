#include "DisplayElementFBO.h"

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "XPLMDefs.h"
#include "XPLMDisplay.h"

#include <vector>

#pragma once
class Display
{
public:

	Display();
	Display(float x, float y, float width, float height);
	Display(float x, float y, float width, float height, int pixelsX, int pixelsY);
	~Display();

	void setBounds(float x, float y, float width, float height);
	void getBounds(float * x, float * y, float * width, float * height);

	void setResolution(int pixelsX, int pixelsY);
	void getResolution(int * pixelsX, int * pixelsY);

	void addElement(DisplayElement* de);
	std::vector<DisplayElement*> getElements();

	void onClick(XPLMMouseStatus status, float mx, float my);
	void onHover(float mx, float my);

	void draw();
	void update();

private:

	float texX, texY, texW, texH;
	int resX, resY;
	
	bool initialized = false;

	GLint oldFramebuffer;
	GLuint framebuffer;
	GLint texture;

	std::vector<DisplayElement*> elements;

	void init();

};

