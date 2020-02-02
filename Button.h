#pragma once
#include "DisplayElement.h"

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "SOIL2/SOIL2.h"
#include "Utils.h"

#include <map>

class Button :
	public DisplayElement
{
public:

	Button();
	Button(int x, int y, int width, int height);
	~Button();

	void draw();
	void update();
	void onClick(int mouseX, int mouseY);

	void setTextureIdle(XPLMTextureID idle);
	void setTextureHover(XPLMTextureID hover);
	void setTextureClick(XPLMTextureID click);

	void setLabel(const char * string, float scale);
	void setColor(float c[3]);
	void setColor(float r, float g, float b);
	void setFont(std::map<GLchar, Character> * f);

protected:
	XPLMTextureID texIdle, texHover, texClick;
	void(*actionButton) (void);

private:

	char * label = new char[255];
	float textWidth = 0, size = 0;
	float color[3] = { 1, 1, 1 };

	std::map<GLchar, Character> * font;

	void drawButtonQuad();
	bool hover, hold;
};

