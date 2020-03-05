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
	XPLMBindTexture2d(texIdle, 0);
	if (hold) {
		if (texClick) XPLMBindTexture2d(texClick, 0);
		drawButtonQuad();
		return;
	}
	if (hover) {
		if (texHover) XPLMBindTexture2d(texHover, 0);
		drawButtonQuad();
		return;
	}
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
	if (status == xplm_MouseDown) hold = true;
	if (status == xplm_MouseUp) hold = false;
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

void Button::setActionClick(std::function<void(XPLMMouseStatus, float, float)> f)
{
	actionButton = f;
}
void Button::setActionHover(std::function<void(float, float)> f)
{
	actionHover = f;
}

void Button::setLabel(const char * string)
{
	label = std::string(string);
	if(font) textWidth = Utils::GetTextWidth(label.c_str(), *font, size);
}

void Button::setLabel(const char * string, float scale)
{
	size = scale;
	label = std::string(string);
	if (font) textWidth = Utils::GetTextWidth(label.c_str(), *font, size);
}

void Button::setFontColor3fv(float c[3])
{
	color[0] = c[0];
	color[1] = c[1];
	color[2] = c[2];
}

void Button::setFontColor3f(float r, float g, float b)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

void Button::setFont(std::map<GLchar, Character>* f)
{
	font = f;
}

void Button::setFontSize(float s)
{
	size = s;
	if (font) textWidth = Utils::GetTextWidth(label.c_str(), *font, size);
}

void Button::setButtonColor4fv(float c[4])
{
	colorButton[0] = c[0];
	colorButton[1] = c[1];
	colorButton[2] = c[2];
	colorButton[3] = c[3];
}

void Button::setButtonColor4f(float r, float g, float b, float a)
{
	colorButton[0] = r;
	colorButton[1] = g;
	colorButton[2] = b;
	colorButton[3] = a;
}

void Button::setButtonColor3fv(float c[3])
{
	setButtonColor4f(c[0], c[1], c[2], 1);
}

void Button::setButtonColor3f(float r, float g, float b)
{
	setButtonColor4f(r, g, b, 1);
}

void Button::drawButtonQuad()
{
	glColor4fv(colorButton);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);		glVertex2f(x,			y);
		glTexCoord2f(0, 0);		glVertex2f(x,			y + height);
		glTexCoord2f(1, 0);		glVertex2f(x + width,	y + height);
		glTexCoord2f(1, 1);		glVertex2f(x + width,	y);
	glEnd();
	if (label.length() > 0)
		if(font)
			Utils::RenderText(label.c_str(), *font, x + (width - textWidth) / 2, y + height / 2 - font->at('A').height * 0.5f * size, size, color);
}