#include "TabContainer.h"
#include <cmath>

TabContainer::TabContainer()
{
	tabs = new Container();
}

TabContainer::~TabContainer()
{
	for (DisplayElement * de : tabs->getElements()) {
		delete de;
	}
	delete tabs;
}

void TabContainer::drawTex()
{
	if (!init) {
		if (!Utils::InitFBO(&fbo, &texFBO, width, height, GL_RGBA)) {
			XPLMDebugString("TabContainer.cpp: Error when setting up container framebuffer.");
		}
		else init = true;
	}

	if (content) content->drawTex();
	tabs->drawTex();

	GLint fboOld;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fboOld);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);

	GLint oldView[4];
	glGetIntegerv(GL_VIEWPORT, oldView);

	glViewport(0, 0, width, height);

	if (content) content->draw();
	tabs->draw();

	glViewport(oldView[0], oldView[1], oldView[2], oldView[3]);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, fboOld);
}

void TabContainer::update()
{
	tabs->update();
	if (content) content->update();
}

void TabContainer::onClick(XPLMMouseStatus status, float mx, float my)
{
	// Call click on the tab bar (the button container):
	int x, y, width, height;
	tabs->getBounds(&x, &y, &width, &height);
	if (mx >= (float)x / this->width  &&  mx <= (float)(x + width) / this->width  &&  my >= (float)y / this->height  &&  my <= (float)(y + height) / this->height)
		tabs->onClick(status, mx * (float)this->width / width - (float)x / width, my * (float)this->height / height - (float)y / height);

	// Call click on the current content:
	if (content) {
		content->getBounds(&x, &y, &width, &height);
		if (mx >= (float)x / this->width  &&  mx <= (float)(x + width) / this->width  &&  my >= (float)y / this->height  &&  my <= (float)(y + height) / this->height)
			content->onClick(status, mx * (float)this->width / width - (float)x / width, my * (float)this->height / height - (float)y / height);
	}
}

void TabContainer::onHover(float mx, float my)
{
	// Call hover on the tab bar:
	int x, y, width, height;
	tabs->getBounds(&x, &y, &width, &height);
	if (mx >= (float)x / this->width  &&  mx <= (float)(x + width) / this->width  &&  my >= (float)y / this->height  &&  my <= (float)(y + height) / this->height)
		tabs->onHover(mx * (float)this->width / width - (float)x / width, my * (float)this->height / height - (float)y / height);

	// Call hover on the current content:
	if (content) {
		content->getBounds(&x, &y, &width, &height);
		if (mx >= (float)x / this->width  &&  mx <= (float)(x + width) / this->width  &&  my >= (float)y / this->height  &&  my <= (float)(y + height) / this->height)
			content->onHover(mx * (float)this->width / width - (float)x / width, my * (float)this->height / height - (float)y / height);
	}
}

void TabContainer::addTab(const char * label, Container * c)
{
	Button * b = new Button();

	b->setActionClick([this, c, b](XPLMMouseStatus status, float mx, float my) 
	{
	if(status == xplm_MouseDown)this->setTab(c, b);
	});
	b->setTextureIdle(texIdle);
	b->setTextureHover(texHover);
	b->setTextureClick(texClick);
	b->setFont(font);
	b->setFontSize(size);
	b->setFontColor(color);
	b->setLabel(label);

	tabs->addElement(b);
	reorganizeTabs();
}
void TabContainer::setTabFontColor(float c[3])
{
	color[0] = c[0];
	color[1] = c[1];
	color[2] = c[2];
	for (DisplayElement * de : tabs->getElements()) {
		if (Button * b = dynamic_cast<Button*>(de)) {
			b->setFontColor(color);
		}
	}
}

void TabContainer::setTabFontColor(float r, float g, float b)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	for (DisplayElement * de : tabs->getElements()) {
		if (Button * b = dynamic_cast<Button*>(de)) {
			b->setFontColor(color);
		}
	}
}

void TabContainer::setTabFont(std::map<GLchar, Character>* f)
{
	font = f;
	for (DisplayElement * de : tabs->getElements()) {
		if (Button * b = dynamic_cast<Button*>(de)) {
			b->setFont(f);
		}
	}
}

void TabContainer::setTabFontSize(float size)
{
	this->size = size;
	for (DisplayElement * de : tabs->getElements()) {
		if (Button * b = dynamic_cast<Button*>(de)) {
			b->setFontSize(size);
		}
	}
}

void TabContainer::setTabTextureIdle(XPLMTextureID idle)
{
	texIdle = idle;
	for (DisplayElement * de : tabs->getElements()) {
		if (Button * b = dynamic_cast<Button*>(de)) {
			b->setTextureIdle(idle);
		}
	}
}

void TabContainer::setTabTextureHover(XPLMTextureID hover)
{
	texHover = hover;
	for (DisplayElement * de : tabs->getElements()) {
		if (Button * b = dynamic_cast<Button*>(de)) {
			b->setTextureHover(hover);
		}
	}
}

void TabContainer::setTabTextureClick(XPLMTextureID click)
{
	texClick = click;
	for (DisplayElement * de : tabs->getElements()) {
		if (Button * b = dynamic_cast<Button*>(de)) {
			b->setTextureClick(click);
		}
	}
}

void TabContainer::setTabTextureActive(XPLMTextureID active)
{
	texActive = active;
}

void TabContainer::setTab(Container * c, Button * b)
{
	if (activeTab) {
		activeTab->setTextureHover(texHover);
		activeTab->setTextureIdle(texIdle);
	}
	content = c;

	activeTab = b;
	activeTab->setTextureHover(texActive);
	activeTab->setTextureIdle(texActive);
}

Container * TabContainer::getTabButtons()
{
	return tabs;
}

void TabContainer::setTabBarBounds(int x, int y, int width, int height)
{
	// Sets the bounds of the tab bar
	tabs->setBounds(x, y, width, height);
	reorganizeTabs();
}

void TabContainer::setTabsPerLine(int n)
{
	// Sets the number of tabs that will fit in one line.
	tabsPerLine = n;
	reorganizeTabs();
}

void TabContainer::setTabSpacing(float sp)
{
	spacing = sp;
	reorganizeTabs();
}

void TabContainer::reorganizeTabs()
{
	// This function changes the bounds of the tab buttons according to
	// the specified number of tabs per line and the tab bar bounds.
	// The tabs are organized from left to right, then top to bottom.
	int lines = ceil((float)tabs->getElements().size() / (float)tabsPerLine);
	int x, y, width, height;
	tabs->getBounds(&x, &y, &width, &height);

	float btWidth = float(width) / tabsPerLine - spacing / 2;
	float btHeight = float(height) / lines - spacing / 2;
	// Set the bounds for each tab button:
	for (int i = 0; i < tabs->getElements().size(); ++i)
	{
		int xPos = (int) (spacing / 2 + (i % tabsPerLine) * (btWidth + spacing));
		int yPos = (int) (height - spacing / 2 - (i / tabsPerLine) * (btHeight + spacing));
		tabs->getElements().at(i)->setBounds(xPos, (int)(yPos - btHeight), (int) btWidth, (int)btHeight);
	}
}

