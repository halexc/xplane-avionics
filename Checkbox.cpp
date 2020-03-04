#include "Checkbox.h"

Checkbox::Checkbox()
{
	box = new Rect();
	box->setLineWidth(2.0f);
	box->setColor3f(0, 0, 0);
	box->setLineColor3f(1, 1, 1);
}

Checkbox::~Checkbox()
{
	delete box;
}

void Checkbox::draw()
{
	box->draw();
	if(checked)
	{
		XPLMBindTexture2d(texCheck, 0);
		glColor3fv(colorCheckmark);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 1);		glVertex2f(x, y);
			glTexCoord2f(0, 0);		glVertex2f(x, y + height);
			glTexCoord2f(1, 0);		glVertex2f(x + width, y + height);
			glTexCoord2f(1, 1);		glVertex2f(x + width, y);
		glEnd();
	}
}

void Checkbox::onClick(XPLMMouseStatus status, float mx, float my)
{
	if (status == xplm_MouseDown) checked = !checked;
	if (func_click) func_click(status, mx, my);
}

void Checkbox::onHover(float mx, float my)
{
	if (func_hover) func_hover(mx, my);
}

void Checkbox::setActionClick(std::function<void(XPLMMouseStatus, float, float)> f)
{
	func_click = f;
}

void Checkbox::setActionHover(std::function<void(float, float)> f)
{
	func_hover = f;
}

void Checkbox::setBounds(int x, int y, int width, int height)
{
	DisplayElement::setBounds(x, y, width, height);
	box->setBounds(x, y, width, height);
}

void Checkbox::setBoxBackgroundColor3f(float r, float g, float b)
{
	box->setColor3f(r, g, b);
}

void Checkbox::setBoxBackgroundColor3fv(float color[3])
{
	box->setColor3fv(color);
}

void Checkbox::setBoxColor3f(float r, float g, float b)
{
	box->setLineColor3f(r, g, b);
}

void Checkbox::setBoxColor3fv(float color[3])
{
	box->setLineColor3fv(color);
}

void Checkbox::setBoxTexture(GLint tex)
{
	box->setTexture(tex);
}

void Checkbox::setBoxOutlineWidth(float w)
{
	box->setLineWidth(w);
}

void Checkbox::setCheckmarkColor3f(float r, float g, float b)
{
	colorCheckmark[0] = r;
	colorCheckmark[1] = g;
	colorCheckmark[2] = b;
}

void Checkbox::setCheckmarkColor3fv(float color[3])
{
	colorCheckmark[0] = color[0];
	colorCheckmark[1] = color[1];
	colorCheckmark[2] = color[2];
}

void Checkbox::setCheckmarkTexture(GLint tex)
{
	texCheck = tex;
}

void Checkbox::setChecked(bool check)
{
	checked = check;
}

bool Checkbox::isChecked()
{
	return checked;
}
