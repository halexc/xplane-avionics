#pragma once
#include "TextSimple.h"

class TextSimpleData :
	public TextSimple
{
public:
	TextSimpleData();
	~TextSimpleData();

	void draw();

	void setDataSource(float * src);

	void setDecimals(int decs);
private:
	float * data;
	int decimals = 0;
};


