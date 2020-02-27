#pragma once
#include "DisplayElementFBO.h"
#include "Clickable.h"

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "Utils.h"

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

#include <vector>

class Container :
	public DisplayElementFBO, public Clickable
{
public:
	Container();
	Container(int resX, int resY);
	~Container();

	virtual void drawTex();
	virtual void update();

	void addElement(DisplayElement * element);
	std::vector<DisplayElement*> getElements();

	void onClick(XPLMMouseStatus status, float mx, float my);
	void onHover(float mx, float my);

	void setBounds(int x, int y, int width, int height);
	void setBounds(int x, int y, int width, int height, int resX, int resY);

private:
	int resX, resY;

	std::vector<DisplayElement*> elements;
};

