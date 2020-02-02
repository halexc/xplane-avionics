#pragma once
#include "DisplayElementFBO.h"

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "Utils.h"

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

#include <list>

class Container :
	public DisplayElementFBO
{
public:
	Container();
	Container(int resX, int resY);
	~Container();

	void drawTex();
	void update();

	void addElement(DisplayElement * element);

	void setBounds(int x, int y, int width, int height);
	void setBounds(int x, int y, int width, int height, int resX, int resY);

private:
	int resX, resY;

	//GLint tex;
	//GLuint fbo;

	std::list<DisplayElement*> elements;
};

