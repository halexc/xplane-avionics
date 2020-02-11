#include "TextSimpleData.h"

#include <string>
#include <type_traits>

TextSimpleData::TextSimpleData()
{
}

TextSimpleData::~TextSimpleData()
{
}


void TextSimpleData::draw()
{
	std::string s;
	if (decimals >= 0) {
		int pre = (int)(*data);
		int post = (int)((*data - pre) * pow(10, decimals));
		s.append(std::to_string(pre));
		if (decimals > 0) {
			s.append(".");
			s.append(std::to_string(post));
		}
	} 
	else s = std::to_string(*data);

	TextSimple::setText(s.c_str());
	TextSimple::draw();
}

void TextSimpleData::update()
{
}

void TextSimpleData::setDataSource(float * src)
{
	data = src;
}

void TextSimpleData::setDecimals(int decs)
{
	decimals = decs;
}
