#pragma once
#include "DisplayElement.h"
#include "Utils.h"

class TextSimple :
	public DisplayElement
{
public:
	TextSimple();
	TextSimple(int n);
	~TextSimple();

	void draw();
	void update();

	void setPosition(float x, float y);

	void setText(const char * text);
	void setFont(std::map<GLchar, Character> * font);
	void setSize(float size);
	float getSize();
	float getTextWidth();

	void setColor(float c[3]);

	bool alignCenter = false;
	bool alignRight = false;

private:
	std::map<GLchar, Character> * font;
	char * label = new char[255];

	float size = 1.0f;
	float color[3] = { 1.0f, 1.0f, 1.0f };
};

