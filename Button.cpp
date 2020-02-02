#include "Button.h"


Button::Button()
{
}

Button::Button(int x, int y, int width, int height)
{
	this->setBounds(x, y, width, height);
}

Button::~Button()
{
}

void Button::draw()
{
	if (hold) {
		XPLMBindTexture2d(texClick, 0);
		drawButtonQuad();
		return;
	}
	if (hover) {
		XPLMBindTexture2d(texHover, 0);
		drawButtonQuad();
		return;
	}
	if (texIdle) XPLMBindTexture2d(texIdle, 0);
	drawButtonQuad();
}

void Button::update()
{
	if (!hover) hold = false;
	hover = false;
}

void Button::onClick(int mouseX, int mouseY)
{
}

void Button::setTextureIdle(XPLMTextureID idle)
{
	texIdle = idle;
}

void Button::setTextureHover(XPLMTextureID hover)
{
	texHover = hover;
}

void Button::setTextureClick(XPLMTextureID click)
{
	texClick = click;
}

void Button::setLabel(const char * string, float scale)
{
	size = scale;
	strcpy(label, string);
	textWidth = Utils::GetTextWidth(label, *font, size);
}

void Button::setColor(float c[3])
{
	color[0] = c[0];
	color[1] = c[1];
	color[2] = c[2];
}

void Button::setColor(float r, float g, float b)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

void Button::setFont(std::map<GLchar, Character>* f)
{
	font = f;
}

void Button::drawButtonQuad()
{
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);		glVertex2f(x,			y);
		glTexCoord2f(0, 0);		glVertex2f(x,			y + height);
		glTexCoord2f(1, 0);		glVertex2f(x + width,	y + height);
		glTexCoord2f(1, 1);		glVertex2f(x + width,	y);
	glEnd();
	if (label) Utils::RenderText(label, *font, x + (width - textWidth) / 2, y + height / 2 - font->at('A').height / 2, size, color);
}