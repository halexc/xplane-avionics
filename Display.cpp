#include "Display.h"

#include "Utils.h"

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"


Display::Display()
{
	init();
}

Display::Display(float x, float y, float width, float height)
{
	texX = x;
	texY = y;
	texW = width;
	texH = height;
	this->resX = (int) (width * 2048);
	this->resY = (int)(height * 2048);
	init();
}

Display::Display(float x, float y, float width, float height, int pixelsX, int pixelsY)
{
	texX = x;
	texY = y;
	texW = width;
	texH = height;
	this->resX = pixelsX;
	this->resY = pixelsY;
	init();
}


Display::~Display()
{
}

void Display::addElement(DisplayElement * de)
{
	elements.push_back(de);
}

void Display::draw()
{
	for (DisplayElement * de : elements)
	{
		if (DisplayElementFBO * deFBO = dynamic_cast<DisplayElementFBO *>(de)) {
			glColor4f(1, 1, 1, 1);
			deFBO->drawTex();
		}
	}
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, resX, 0, resY, -1, 1);

	GLint oldView[4];
	glGetIntegerv(GL_VIEWPORT, oldView);
	glViewport(0, 0, resX, resY);

	for(DisplayElement * de : elements)
	{
		glColor4f(1, 1, 1, 1);
		de->draw();
	}

	glViewport(oldView[0], oldView[1], oldView[2], oldView[3]);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, oldFramebuffer);

	XPLMBindTexture2d(texture, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);		glVertex2f(texX, texY);
		glTexCoord2f(0, 1);		glVertex2f(texX, texY + texH);
		glTexCoord2f(1, 1);		glVertex2f(texX + texW, texY + texH);
		glTexCoord2f(1, 0);		glVertex2f(texX + texW, texY);
	glEnd();
}

void Display::update()
{
	for (DisplayElement * de : elements)
	{
		de->update();
	}
}

void Display::init() {
	if (!Utils::InitFBO(&framebuffer, &texture, resX, resY, GL_RGB)) {
		XPLMDebugString("Display.cpp: Error when setting up framebuffer.");
	}
}