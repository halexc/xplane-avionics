#pragma once
#include "DisplayElement.h"
#include "Utils.h"
class DisplayElementFBO :
	public DisplayElement
{
public:
	DisplayElementFBO();
	~DisplayElementFBO();

	void draw();
	virtual void drawTex();
	virtual void update();

	void setBounds(int x, int y, int width, int height);

protected:
	bool init = false;
	GLint texFBO;
	GLuint fbo;
};

