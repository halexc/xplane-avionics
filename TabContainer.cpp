#include "TabContainer.h"
#include "Button.h"

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

void TabContainer::draw()
{
}

void TabContainer::update()
{
}

void TabContainer::onClick(XPLMMouseStatus status, float mx, float my)
{
}

void TabContainer::onHover(float mx, float my)
{
}

void TabContainer::addTab(const char * label, Container * c)
{
	Button * b = new Button();
	b->setActionClick( [](XPLMMouseStatus status, float mx, float my) 
		{
			
		} );
}
void TabContainer::setTabFontColor(float c[3])
{
	color[0] = c[0];
	color[1] = c[1];
	color[2] = c[2];
}

void TabContainer::setTabFontColor(float r, float g, float b)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
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

void TabContainer::setTab(Container * c)
{
	content = c;
}
