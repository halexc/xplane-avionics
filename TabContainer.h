#pragma once
#include "DisplayElement.h"
#include "Container.h"
#include "Clickable.h"
#include "Button.h"

#include <vector>

class TabContainer :
	public DisplayElementFBO, public Clickable
{
public:
	TabContainer();
	~TabContainer();

	virtual void drawTex();
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
	void setTabTextureActive(XPLMTextureID active);

	void setTab(Container * c, Button * b);
	Container * getTabButtons();

	void setTabBarBounds(int x, int y, int width, int height);
	void setTabsPerLine(int n);
	void setTabSpacing(float sp);

private:
	void reorganizeTabs();

	XPLMTextureID texIdle, texHover, texClick, texActive;

	float size = 1;
	float color[3] = { 1, 1, 1 };
	int tabsPerLine = 4;
	float spacing = 2.0f;

	std::map<GLchar, Character> * font;

	Container * tabs;
	Container * content;
	Button * activeTab;
};

