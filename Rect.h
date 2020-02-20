#pragma once
#include "DisplayElement.h"
#include "Utils.h"

class Rect :
	public DisplayElement
{
public:
	Rect();
	~Rect();

	void draw();

	void setTexture(int texID);
	void setTextureCoordOffset(int i);


	void setColor3f(float r, float g, float b);
	void setColor3fv(float color[3]);
	void setColor4f(float r, float g, float b, float a);
	void setColor4fv(float color[4]);

	void setLineWidth(float w);
	void setLineColor3f(float r, float g, float b);
	void setLineColor3fv(float color[3]);

protected:
	int texID = 0;
	float color[4] = { 1, 1, 1, 1 };

	int offset = 0;

	float lineWidth = 0;
	float lineColor[3] = { 1, 1, 1 };
};

