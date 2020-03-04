#pragma once
#include "DisplayElement.h"
#include "Utils.h"
class DisplayElementFBO :
	public DisplayElement
{
public:
	DisplayElementFBO();
	~DisplayElementFBO();

	virtual void draw();
	virtual void draw(float offsetX, float offsetY);
	virtual void drawTex();

	void setBounds(int x, int y, int width, int height);

protected:
	bool init = false;
	GLint texFBO;
	GLuint fbo;
};

