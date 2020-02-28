#pragma once
#include "DisplayElement.h"
#include "Clickable.h"

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "SOIL2/SOIL2.h"
#include "Utils.h"

#include <map>
#include <string>

class Button :
	public DisplayElement, public Clickable
{
public:

	Button();
	Button(int x, int y, int width, int height);
	~Button();

	virtual void draw();
	virtual void update();
	virtual void onClick(XPLMMouseStatus status, float mouseX, float mouseY);
	virtual void onHover(float mouseX, float mouseY);

	void setTextureIdle(XPLMTextureID idle);
	void setTextureHover(XPLMTextureID hover);
	void setTextureClick(XPLMTextureID click);

	void setActionClick(void(*f)(XPLMMouseStatus, float, float));
	void setActionHover(void(*f)(float, float));

	void setLabel(const char * string);
	void setLabel(const char * string, float scale);
	void setFontColor(float c[3]);
	void setFontColor(float r, float g, float b);
	void setFont(std::map<GLchar, Character> * f);
	void setFontSize(float s);

protected:
	XPLMTextureID texIdle, texHover, texClick;
	void(*actionButton) (XPLMMouseStatus, float, float);
	void(*actionHover) (float, float);

private:

	std::string label;
	float textWidth = 0, size = 1;
	float color[3] = { 1, 1, 1 };

	std::map<GLchar, Character> * font;

	void drawButtonQuad();
	bool hover, hold;
};

