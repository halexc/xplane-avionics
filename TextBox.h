#pragma once
#include "DisplayElementFBO.h"
#include "Utils.h"

class TextBox :
	public DisplayElementFBO
{
public:
	TextBox();
	TextBox(int n);
	~TextBox();
	
	void drawTex();

	void setFont(std::map<GLchar, Character> * f);
	void setText(const char * text);
	void setTextSize(float size);
	void setBorderSize(float width);

	void setColorText3f(float r, float g, float b);
	void setColorText3fv(float rgb[3]);
	void setColorBorder3f(float r, float g, float b);
	void setColorBorder3fv(float rgb[3]);
	void setColorBackground3f(float r, float g, float b);
	void setColorBackground3fv(float rgb[3]);
	void setColorBackground4f(float r, float g, float b, float a);
	void setColorBackground4fv(float rgba[4]);

	//Text properties:
	bool alignCenterX = false;
	bool alignRight = false;
	bool alignCenterY = true;
	bool alignBot = false;
	bool autoLinebreak = true;
	float lineSpacing = 1.5f;

private:
	std::map<GLchar, Character> * font;

	bool valid = false;

	char * text = new char[255];
	float size = 1.0f;
	float borderWidth = 0.0f;

	float colorText[3] = { 1.0f, 1.0f, 1.0f };
	float colorBorder[3] = { 1.0f, 1.0f, 1.0f };
	float colorBackground[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

};

