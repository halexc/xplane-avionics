#include "Rect.h"

Rect::Rect()
{
}

Rect::~Rect()
{
}

void Rect::draw()
{
	XPLMBindTexture2d(texID, 0);
	glColor4fv(color);
	glBegin(GL_QUADS);
		if (texID) {
			glTexCoord2f((0 + offset) % 2, (0 + offset) % 2);		glVertex2f(x, y);
			glTexCoord2f((0 + offset) % 2, (1 + offset) % 2);		glVertex2f(x, y + height);
			glTexCoord2f((1 + offset) % 2, (1 + offset) % 2);		glVertex2f(x + width, y + height);
			glTexCoord2f((1 + offset) % 2, (0 + offset) % 2);		glVertex2f(x + width, y);
		}
		else {
			glVertex2f(x, y);
			glVertex2f(x, y + height);
			glVertex2f(x + width, y + height);
			glVertex2f(x + width, y);
		}
	glEnd();

	if (lineWidth > 0) {
		GLboolean smooth_old;
		glGetBooleanv(GL_LINE_SMOOTH, &smooth_old);
		XPLMBindTexture2d(0, 0);
		glColor3fv(lineColor);
		glLineWidth(lineWidth);
		glEnable(GL_LINE_SMOOTH);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x, y + height);
		glVertex2f(x + width, y + height);
		glVertex2f(x + width, y);
		glEnd();
		if (!smooth_old) {
			glDisable(GL_LINE_SMOOTH);
		}
	}
}

void Rect::setTexture(int texID)
{
	this->texID = texID;
}

void Rect::setTextureCoordOffset(int i)
{
	offset = i;
}

void Rect::setColor3f(float r, float g, float b)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = 1;
}

void Rect::setColor3fv(float color[3])
{
	setColor3f(color[0], color[1], color[2]);
}

void Rect::setColor4f(float r, float g, float b, float a)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}

void Rect::setColor4fv(float color[4])
{
	setColor4f(color[0], color[1], color[2], color[3]);
}

void Rect::setLineWidth(float w)
{
	lineWidth = w;
}

void Rect::setLineColor3f(float r, float g, float b)
{
	lineColor[0] = r;
	lineColor[1] = g;
	lineColor[2] = b;
}

void Rect::setLineColor3fv(float color[3])
{
	setLineColor3f(color[0], color[1], color[2]);
}
