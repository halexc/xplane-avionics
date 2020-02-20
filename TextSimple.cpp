#include "TextSimple.h"



TextSimple::TextSimple()
{
}

TextSimple::TextSimple(int n)
{
	label = new char[n];
}

TextSimple::~TextSimple()
{
}

void TextSimple::draw()
{
	if (alignCenter) {
		Utils::RenderText(label, *font, x - Utils::GetTextWidth(label, *font, size) / 2, y, size, color);
	}
	else if (alignRight) {
		Utils::RenderText(label, *font, x - Utils::GetTextWidth(label, *font, size), y, size, color);
	}
	else Utils::RenderText(label, *font, x, y, size, color);
}

void TextSimple::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void TextSimple::setText(const char * text)
{
	strcpy(label, text);
}

void TextSimple::setFont(std::map<GLchar, Character>* font)
{
	this->font = font;
}

void TextSimple::setSize(float size)
{
	this->size = size;
}

float TextSimple::getSize()
{
	return size;
}

float TextSimple::getTextWidth()
{
	return Utils::GetTextWidth(label, *font, size);
}

void TextSimple::setColor(float c[3])
{
	color[0] = c[0];
	color[1] = c[1];
	color[2] = c[2];
}