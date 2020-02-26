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
	delete label;
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
	if (func_update) func_update();
}

void Button::onClick(XPLMMouseStatus status, float mouseX, float mouseY)
{
	hold = true;
	if(actionButton)
		actionButton(status, mouseX, mouseY);
}

void Button::onHover(float mouseX, float mouseY)
{
	hover = true;
	if(actionHover)
		actionHover(mouseX, mouseY);
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

void Button::setActionClick(void(*f)(XPLMMouseStatus, float, float))
{
	actionButton = f;
}
void Button::setActionHover(void(*f)(float, float))
{
	actionHover = f;
}

void Button::setLabel(const char * string)
{
	strcpy(label, string);
	if(font) textWidth = Utils::GetTextWidth(label, *font, size);
}

void Button::setLabel(const char * string, float scale)
{
	size = scale;
	strcpy(label, string);
	if (font) textWidth = Utils::GetTextWidth(label, *font, size);
}

void Button::setFontColor(float c[3])
{
	color[0] = c[0];
	color[1] = c[1];
	color[2] = c[2];
}

void Button::setFontColor(float r, float g, float b)
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
	if (label)
		if(font)
			Utils::RenderText(label, *font, x + (width - textWidth) / 2, y + height / 2 - font->at('A').height / 2, size, color);
}