#include "DisplayElement.h"

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

DisplayElement::DisplayElement()
{
}


DisplayElement::~DisplayElement()
{
}

void DisplayElement::update()
{
}

void DisplayElement::draw()
{
}

void DisplayElement::onClick(int mouseX, int mouseY)
{
}

void DisplayElement::setBounds(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}
