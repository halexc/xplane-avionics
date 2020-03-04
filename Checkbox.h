#pragma once
#include "DisplayElement.h"
#include "Clickable.h"

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "Rect.h"

#include <functional>

class Checkbox :
	public DisplayElement, public Clickable
{
public:
	Checkbox();
	~Checkbox();

	virtual void draw();

	virtual void onClick(XPLMMouseStatus status, float mx, float my);
	virtual void onHover(float mx, float my);
	void setActionClick(std::function<void(XPLMMouseStatus, float, float)> f);
	void setActionHover(std::function<void(float, float)> f);

	void setBounds(int x, int y, int width, int height);

	void setBoxBackgroundColor3f(float r, float g, float b);
	void setBoxBackgroundColor3fv(float color[3]);

	void setBoxColor3f(float r, float g, float b);
	void setBoxColor3fv(float color[3]);
	void setBoxTexture(GLint tex);
	void setBoxOutlineWidth(float w);

	void setCheckmarkColor3f(float r, float g, float b);
	void setCheckmarkColor3fv(float color[3]);

	void setCheckmarkTexture(GLint tex);
	void setChecked(bool check);

	bool isChecked();

private:
	bool checked = false;
	float colorCheckmark[3] = {1.0f, 1.0f, 1.0f};
	Rect * box;
	GLint texCheck;
	std::function<void(XPLMMouseStatus, float, float)> func_click;
	std::function<void(float, float)> func_hover;
};

