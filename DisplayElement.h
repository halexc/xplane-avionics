#pragma once
class DisplayElement
{
public:
	DisplayElement();
	~DisplayElement();

	virtual void update();
	virtual void draw();
	void onClick(int mouseX, int mouseY);
	void setBounds(int x, int y, int width, int height);

	int x, y;
	int width, height;
};

