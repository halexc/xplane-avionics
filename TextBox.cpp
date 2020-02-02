#include "TextBox.h"
#include <string.h>

TextBox::TextBox()
{
}

TextBox::TextBox(int n)
{
	text = new char[n];
}


TextBox::~TextBox()
{
}

void TextBox::update()
{
}

void TextBox::drawTex()
{
	if (valid) return;
	if (!init) {
		if (!Utils::InitFBO(&fbo, &texFBO, width, height, GL_RGBA)) {
			XPLMDebugString("TextBox.cpp: Error when setting up TextBox framebuffer.");
		}
		else init = true;
	}
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

	//Draw background:
	if (colorBackground[3] > 0) {
		XPLMBindTexture2d(0, 0);
		glColor4f(colorBackground[0], colorBackground[1], colorBackground[2], colorBackground[3]);
		glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f(0, 1);
			glVertex2f(1, 1);
			glVertex2f(1, 0);
		glEnd();
	}

	//Draw text:
	if(font && text){

		int lines = 0;
		std::size_t remainderPos = 0;
		std::string str = std::string(text);
		while (remainderPos != std::string::npos) {
			lines++;
			remainderPos = str.find('\n', remainderPos + 1);
		}

		float yPos;
		if (alignCenterY) {
			yPos = height / 2 + (-1 + (lines-1) * lineSpacing)  * font->at('A').height * size / 2;
		}
		else if (alignBot) {
			yPos = (0.5f + (lines - 1) * lineSpacing) * font->at('A').height * size + borderWidth;
		}
		else {
			yPos = height - (1.5f * font->at('A').height * size + borderWidth);
		}
		// Draw all lines ending with a line break (\n):
		std::string textFull = std::string(text);
		int lineBreak = -1;
		for (int i = 0; i < lines - 1; i++) {
			int lineStart = lineBreak + 1;
			lineBreak = textFull.find('\n', lineStart);
			std::string line = textFull.substr(lineStart, lineBreak - lineStart);
			if (alignCenterX) {
				float textWidth = Utils::GetTextWidth(line.c_str(), *font, size);
				Utils::RenderText(line.c_str(), *font, width / 2 - textWidth / 2, yPos, size, colorText);
			}
			else if (alignRight) {
				float textWidth = Utils::GetTextWidth(line.c_str(), *font, size);
				Utils::RenderText(line.c_str(), *font, width - font->at('A').width * 0.25f - textWidth - borderWidth, yPos, size, colorText);
			}
			else {
				Utils::RenderText(line.c_str(), *font, 0 + font->at('A').width * 0.25f + borderWidth, yPos, size, colorText);
			}
			yPos -= lineSpacing * font->at('A').height * size;
		}
		// Draw final line:
		std::string line = textFull.substr(lineBreak + 1);
		if (alignCenterX) {
			float textWidth = Utils::GetTextWidth(line.c_str(), *font, size);
			Utils::RenderText(line.c_str(), *font, width / 2 - textWidth / 2, yPos, size, colorText);
		}
		else if (alignRight) {
			float textWidth = Utils::GetTextWidth(line.c_str(), *font, size);
			Utils::RenderText(line.c_str(), *font, width - font->at('A').width * 0.25f - textWidth - borderWidth, yPos, size, colorText);
		}
		else {
			Utils::RenderText(line.c_str(), *font, 0 + font->at('A').width * 0.25f + borderWidth, yPos, size, colorText);
		}
	}

	//Draw border:
	if (borderWidth > 0.0f) {
		float xs[] = { 0, 0, width, width, 0 };
		float ys[] = { 0, height, height, 0, 0 };
		Utils::DrawLinesConnected(xs, ys, 5, colorBorder, borderWidth);
	}

	//Reset State:
	glViewport(oldView[0], oldView[1], oldView[2], oldView[3]);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, fboOld);

	valid = true;
}

void TextBox::setFont(std::map<GLchar, Character>* f)
{
	font = f;
	valid = false;
}

void TextBox::setText(const char * text)
{
	std::strcpy(this->text, text);
	valid = false;
}

void TextBox::setTextSize(float size)
{
	this->size = size;
	valid = false;
}

void TextBox::setBorderSize(float width)
{
	borderWidth = width;
	valid = false;
}

void TextBox::setColorText3f(float r, float g, float b)
{
	colorText[0] = r;
	colorText[1] = g;
	colorText[2] = b;
	valid = false;
}

void TextBox::setColorText3fv(float rgb[3])
{
	colorBorder[0] = rgb[0];
	colorBorder[1] = rgb[1];
	colorBorder[2] = rgb[2];
	valid = false;
}

void TextBox::setColorBorder3f(float r, float g, float b)
{
	colorBorder[0] = r;
	colorBorder[1] = g;
	colorBorder[2] = b;
	valid = false;
}

void TextBox::setColorBorder3fv(float rgb[3])
{
	colorBorder[0] = rgb[0];
	colorBorder[1] = rgb[0];
	colorBorder[2] = rgb[0];
	valid = false;
}

void TextBox::setColorBackground3f(float r, float g, float b)
{
	colorBackground[0] = r;
	colorBackground[1] = g;
	colorBackground[2] = b;
	colorBackground[3] = 1.0f;
	valid = false;
}

void TextBox::setColorBackground3fv(float rgb[3])
{
	colorBackground[0] = rgb[0];
	colorBackground[1] = rgb[1];
	colorBackground[2] = rgb[2];
	colorBackground[3] = 1.0f;
	valid = false;
}

void TextBox::setColorBackground4f(float r, float g, float b, float a)
{
	colorBackground[0] = r;
	colorBackground[1] = g;
	colorBackground[2] = b;
	colorBackground[3] = a;
	valid = false;
}

void TextBox::setColorBackground4fv(float rgba[4])
{
	colorBackground[0] = rgba[0];
	colorBackground[1] = rgba[1];
	colorBackground[2] = rgba[2];
	colorBackground[3] = rgba[3];
	valid = false;
}
