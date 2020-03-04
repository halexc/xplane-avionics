#include "DisplayElementFBO.h"



DisplayElementFBO::DisplayElementFBO()
{
}


DisplayElementFBO::~DisplayElementFBO()
{
}

void DisplayElementFBO::draw()
{
	XPLMBindTexture2d(texFBO, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);		glVertex2f(x, y);
		glTexCoord2f(0, 1);		glVertex2f(x, y + height);
		glTexCoord2f(1, 1);		glVertex2f(x + width, y + height);
		glTexCoord2f(1, 0);		glVertex2f(x + width, y);
	glEnd();
}

void DisplayElementFBO::draw(float offsetX, float offsetY)
{
	XPLMBindTexture2d(texFBO, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);		glVertex2f(offsetX + x, offsetY + y);
	glTexCoord2f(0, 1);		glVertex2f(offsetX + x, offsetY + y + height);
	glTexCoord2f(1, 1);		glVertex2f(offsetX + x + width, offsetY + y + height);
	glTexCoord2f(1, 0);		glVertex2f(offsetX + x + width, offsetY + y);
	glEnd();
}

void DisplayElementFBO::drawTex()
{
	if (!init) {
		if (!Utils::InitFBO(&fbo, &texFBO, width, height, GL_RGBA)) {
			XPLMDebugString("DisplayElementFBO.cpp: Error when setting up altitude framebuffer.");
		}
		else init = true;
	}
}

void DisplayElementFBO::setBounds(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	init = false;
}
