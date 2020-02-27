#pragma once
#include <XPLMDefs.h>
class DisplayElement
{
public:
	DisplayElement();
	~DisplayElement();

	virtual void update();
	virtual void draw();

	void setUpdateFunction(void(*f)());

	void setBounds(int x, int y, int width, int height);
	void getBounds(int *x, int *y, int *width, int *height);

protected:
	void(*func_update)() = NULL;
	int x, y;
	int width, height;
};

