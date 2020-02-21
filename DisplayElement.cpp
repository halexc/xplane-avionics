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
	if (func_update) func_update();
}

void DisplayElement::draw()
{
}

void DisplayElement::setUpdateFunction(void(*f)())
{
	func_update = f;
}

void DisplayElement::setBounds(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void DisplayElement::getBounds(int * x, int * y, int * width, int * height)
{
	if(x)
		*x = this->x;
	if(y)
		*y = this->y;
	if(width)
		*width = this->width;
	if(height)
		*height = this->height;
}
