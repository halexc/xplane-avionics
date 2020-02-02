#include "PFD.h"



PFD::PFD()
{
}


PFD::~PFD()
{
}

void PFD::drawTex()
{

	if (!init) {
		if (!Utils::InitFBO(&fbo, &texFBO, (int)width, (int)height, GL_RGB)) {
			XPLMDebugString("PFD.cpp: Error when setting up gyro framebuffer.");
		}
		else init = true;
	}
	//Predraw textures:
	if (gyro) gyro->drawTex();
	if (altIndicator) altIndicator->drawTex();
	if (speedIndicator) speedIndicator->drawTex();

	GLint fb_draw;
	GLint fb_read;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &fb_draw);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &fb_read);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClear(GL_COLOR_BUFFER_BIT);

	GLint prevView[4];
	glGetIntegerv(GL_VIEWPORT, prevView);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glViewport(0, 0, (int)width, (int)height);

	//Draw:
	if (gyro) gyro->draw();
	if (altIndicator) altIndicator->draw();
	if (speedIndicator) speedIndicator->draw();

	//Reset state:
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb_draw);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fb_read);
	glViewport(prevView[0], prevView[1], prevView[2], prevView[3]);
}

void PFD::update()
{
	if (gyro) gyro->update();
	if (altIndicator) altIndicator->update();
	if (speedIndicator) speedIndicator->update();
}
