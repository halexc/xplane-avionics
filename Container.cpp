#include "Container.h"
#include "Button.h"


Container::Container()
{
}

Container::Container(int resX, int resY)
{
	this->resX = resX;
	this->resY = resY;
}


Container::~Container()
{
}

void Container::drawTex()
{
	if (!init) {
		if (!Utils::InitFBO(&fbo, &texFBO, resX, resY, GL_RGBA)) {
			XPLMDebugString("Container.cpp: Error when setting up container framebuffer.");
		}
		else init = true;
	}
	for (DisplayElement * de : elements)
	{
		if (DisplayElementFBO * deFBO = dynamic_cast<DisplayElementFBO *>(de)) {
			glColor4f(1, 1, 1, 1);
			deFBO->drawTex();
		}
	}
	GLint fboOld;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fboOld);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, resX, 0, resY, -1, 1);

	GLint oldView[4];
	glGetIntegerv(GL_VIEWPORT, oldView);

	glViewport(0, 0, resX, resY);

	for (DisplayElement * de : elements)
	{
		glColor4f(1, 1, 1, 1);
		de->draw();
	}

	glViewport(oldView[0], oldView[1], oldView[2], oldView[3]);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, fboOld);
}

void Container::update()
{
	if (func_update) func_update();
	for (DisplayElement * de : elements)
	{
		de->update();
	}
}

void Container::addElement(DisplayElement * element)
{
	elements.push_back(element);
}

std::vector<DisplayElement*> Container::getElements()
{
	return elements;
}

void Container::onClick(XPLMMouseStatus status, float mx, float my)
{
	for (std::vector<DisplayElement*>::reverse_iterator it = elements.rbegin(); it != elements.rend(); it++) {
		DisplayElement * de = *it;
		if (Clickable * deButton = dynamic_cast<Clickable * >(de)) {
			int x, y, width, height;
			de->getBounds(&x, &y, &width, &height);

			if (mx >= (float)x / resX  &&  mx <= (float)(x + width) / resX  &&  my >= (float)y / resY  &&  my <= (float)(y + height) / resY) {
				deButton->onClick(status, mx * (float)resX / width - (float)x / width, my * (float)resY / height - (float)y / height);
			}
		}
	}
}

void Container::onHover(float mx, float my)
{
	for (std::vector<DisplayElement*>::reverse_iterator it = elements.rbegin(); it != elements.rend(); it++) {
		DisplayElement * de = *it;
		if (Clickable * deButton = dynamic_cast<Clickable * >(de)) {
			int x, y, width, height;
			de->getBounds(&x, &y, &width, &height);

			if (mx >= (float)x / resX  &&  mx <= (float)(x + width) / resX  &&  my >= (float)y / resY  &&  my <= (float)(y + height) / resY) {
				deButton->onHover(mx * (float)resX / width - (float)x / width, my * (float)resY / height - (float)y / height);
			}
		}
	}
}

void Container::setBounds(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	//if (resX == 0)
		this->resX = width;
	//if(resY == 0)
		this->resY = height; 
	init = false;
}

void Container::setBounds(int x, int y, int width, int height, int resX, int resY)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->resX = resX;
	this->resY = resY;
	init = false;
}
