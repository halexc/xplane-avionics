#include "NavRose.h"

#include <string>


NavRose::NavRose()
{
}


NavRose::~NavRose()
{
}

void NavRose::draw()
{
	float radius = fminf(width, height) / 2;
	float cx = x + width / 2;
	float cy = y + height / 2;
	float textSize = 0.05f * radius / 16;
	float hdg = 0;
	if (hdg_data && !faceNorth) hdg = *hdg_data;
	//Inner circle;
	Utils::DrawCircle(0.4f * radius, cx, cy, Utils::COLOR_WHITE, 0.005f * radius, 36);
	for (int i = 0; i < 360; i += 30) {
		Utils::DrawLine(cx + 0.375f * radius * sinf(-(i + hdg) * M_PI / 180), cy + 0.375f * radius * cosf(-(i + hdg) * M_PI / 180), 
							cx + 0.425f * radius * sinf(-(i + hdg) * M_PI / 180), cy + 0.425f * radius * cosf(-(i + hdg) * M_PI / 180),
								Utils::COLOR_WHITE, 0.005f * radius);
	}
	//Outer circle;
	Utils::DrawCircle(0.8f * radius, cx, cy, Utils::COLOR_WHITE, 0.01f * radius, 72);
	for (int i = 0; i < 360; i += 5) {
		if (i % 30 == 0) {
			Utils::DrawLine(cx + 0.8f * radius * sinf(-(i + hdg) * M_PI / 180), cy + 0.8f * radius * cosf(-(i + hdg) * M_PI / 180), 
								cx + 0.825f * radius * sinf(-(i + hdg) * M_PI / 180), cy + 0.825f * radius * cosf(-(i + hdg) * M_PI / 180),
									Utils::COLOR_WHITE, 0.01f * radius);
			float tx, ty;
			float tw = Utils::GetTextWidth(std::to_string((360 - i) / 10).c_str(), *font, textSize);
			Utils::calcRotatedPoint(width / 2 - tw / 2, height / 2 + 0.85f * radius, (i + hdg) * M_PI / 180, &tx, &ty, width / 2, height / 2);
			Utils::RenderTextAngle(std::to_string((360 - i) / 10).c_str(), *font, x + tx, y + ty, textSize, (i + hdg) * M_PI / 180);
		}
		else if (i % 10 == 0) {
			Utils::DrawLine(cx + 0.8f * radius * sinf(-(i + hdg) * M_PI / 180), cy + 0.8f * radius * cosf(-(i + hdg) * M_PI / 180), 
								cx + 0.83f * radius * sinf(-(i + hdg) * M_PI / 180), cy + 0.83f * radius * cosf(-(i + hdg) * M_PI / 180),
									Utils::COLOR_WHITE, 0.01f * radius);
		}
		else
		Utils::DrawLine(cx + 0.8f * radius * sinf(-(i + hdg) * M_PI / 180), cy + 0.8f * radius * cosf(-(i + hdg) * M_PI / 180), 
							cx + 0.8125f * radius * sinf(-(i + hdg) * M_PI / 180), cy + 0.8125f * radius * cosf(-(i + hdg) * M_PI / 180),
								Utils::COLOR_WHITE, 0.01f * radius);
	}
	//Draw Hdg Indicator:
	if (!faceNorth) {
		float xs[] = { x + 0.43f * width,	x + 0.43f * width,		x + 0.57f * width,		x + 0.57f * width,		x + 0.535f * width,		x + 0.5f * width,		x + 0.465f * width, 	x + 0.43f * width };
		float ys[] = { y + 0.92f * height,	y + 0.99f * height,		y + 0.99f * height,		y + 0.92f * height,		y + 0.92f * height,		y + 0.9f * height,		y + 0.92f * height,		y + 0.92f * height };
		XPLMBindTexture2d(0, 0);
		glColor3fv(Utils::COLOR_BLACK);
		glBegin(GL_QUADS);
			glVertex2f(xs[0], ys[0]);
			glVertex2f(xs[1], ys[1]);
			glVertex2f(xs[2], ys[2]);
			glVertex2f(xs[3], ys[3]);
		glEnd();
		glBegin(GL_TRIANGLES);
			glVertex2f(xs[4], ys[4]);
			glVertex2f(xs[5], ys[5]);
			glVertex2f(xs[6], ys[6]);
		glEnd();
		int hdgNum = (int)roundf(hdg);
		if (hdgNum == 0) hdgNum = 360;
		Utils::DrawLinesConnected(xs, ys, 8, Utils::COLOR_WHITE, 0.0125f * radius);
		float tw = Utils::GetTextWidth(std::to_string(hdgNum).c_str(), *font, 1.375 * textSize);
		Utils::RenderText(std::to_string(hdgNum).c_str(), *font, x + width / 2 - tw / 2, y + 0.93f * height, 1.375f * textSize);
	}
	//Draw "plane":
	if (faceNorth && hdg_data) hdg = *hdg_data;
	else hdg = 0;
	Utils::DrawLine(x + 0.5f * width, y + 0.475f * height, x + 0.5f * width, y + 0.535f * height, Utils::COLOR_FUCHSIA, 0.0175f * radius);
	Utils::DrawLine(x + 0.455f * width, y + 0.48f * height, x + 0.5f * width, y + 0.50f * height, Utils::COLOR_FUCHSIA, 0.0175f * radius);
	Utils::DrawLine(x + 0.545f * width, y + 0.48f * height, x + 0.5f * width, y + 0.50f * height, Utils::COLOR_FUCHSIA, 0.0175f * radius);
	Utils::DrawLine(x + 0.455f * width, y + 0.48f * height, x + 0.5f * width, y + 0.51f * height, Utils::COLOR_FUCHSIA, 0.0175f * radius);
	Utils::DrawLine(x + 0.545f * width, y + 0.48f * height, x + 0.5f * width, y + 0.51f * height, Utils::COLOR_FUCHSIA, 0.0175f * radius);
	Utils::DrawLine(x + 0.4825f * width, y + 0.46f * height, x + 0.5f * width, y + 0.47f * height, Utils::COLOR_FUCHSIA, 0.0175f * radius);
	Utils::DrawLine(x + 0.5175f * width, y + 0.46f * height, x + 0.5f * width, y + 0.47f * height, Utils::COLOR_FUCHSIA, 0.0175f * radius);
	Utils::DrawLine(x + 0.4825f * width, y + 0.46f * height, x + 0.5f * width, y + 0.48f * height, Utils::COLOR_FUCHSIA, 0.0175f * radius);
	Utils::DrawLine(x + 0.5175f * width, y + 0.46f * height, x + 0.5f * width, y + 0.48f * height, Utils::COLOR_FUCHSIA, 0.0175f * radius);
}

void NavRose::update()
{
}

void NavRose::setHdgData(float * hdg_source)
{
	hdg_data = hdg_source;
}

void NavRose::setFont(std::map<GLchar, Character>* font)
{
	this->font = font;
}
