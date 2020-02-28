#pragma once
#include "DisplayElement.h"
#include "Container.h"
#include "Clickable.h"

#include <vector>

class TabContainer :
	public DisplayElement, public Clickable
{
public:
	TabContainer();
	~TabContainer();

	virtual void draw();
	virtual void update();

	virtual void onClick(XPLMMouseStatus status, float mx, float my);
	virtual void onHover(float mx, float my);

	void addTab(const char * label, Container * c);

	void setTabFontColor(float c[3]);
	void setTabFontColor(float r, float g, float b);
	void setTabFont(std::map<GLchar, Character> * f);
	void setTabFontSize(float size);

	void setTabTextureIdle(XPLMTextureID idle);
	void setTabTextureHover(XPLMTextureID hover);
	void setTabTextureClick(XPLMTextureID click);

	void setTab(Container * c);

private:
	XPLMTextureID texIdle, texHover, texClick;

	float size = 1;
	float color[3] = { 1, 1, 1 };

	std::map<GLchar, Character> * font;

	Container * tabs;
	Container * content;
};

