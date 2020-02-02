#pragma once
#include "DisplayElement.h"
#include "Utils.h"

class NavRose :
	public DisplayElement
{
public:
	NavRose();
	~NavRose();

	void draw();
	void update();

	void setHdgData(float * hdg_source);
	void setFont(std::map<GLchar, Character> * font);

private:
	float * hdg_data;
	bool faceNorth = false;

	std::map<GLchar, Character> * font;
};

