#include "CircularProgressGauge.h"

#include <string>

CircularProgressGauge::CircularProgressGauge()
{
}


CircularProgressGauge::~CircularProgressGauge()
{
}

void CircularProgressGauge::draw()
{
	//Draw filled area (progress):
	float p = 0;
	if (data) {
		p = (*data) / max;
	}
	float r = max(width * ((1 - 0.125f) / 2), width / 2 - 10);
	if (p <= 1) {
		Utils::DrawPartCircleFilled(0.9325f * r, x + width / 2, y + height / 2, p * markerAngle, 0, color_circle);
	}
	else {
		Utils::DrawPartCircleFilled(0.9325f * r, x + width / 2, y + height / 2, min(p, (float) (M_PI * 1.125f / abs(markerAngle))) * markerAngle, 0, color_exceed);
	}
	Utils::DrawLine(x + width / 2, y + height / 2, x + width / 2 + r * cosf(min(p, (float)(M_PI * 1.125f / abs(markerAngle))) * markerAngle),
										y + height / 2 + r * sinf(min(p, (float)(M_PI * 1.125f / abs(markerAngle))) * markerAngle), Utils::COLOR_WHITE, min(8, max(3, width / 40)));
	//Utils::DrawLine(x + width / 2, y + height / 2, x + width / 2 + r, y + height / 2, Utils::COLOR_WHITE, min(8, max(3, width / 40)));

	//Draw scale (Circle + Max-Indicator):
		//Circle
	Utils::DrawPartCircle(r, x + width / 2, y + height / 2, -(float)M_PI * 1.125f, 0, Utils::COLOR_WHITE, min(8, max(3, width / 60)));
		//Indicator
	float xs[3] = { x + width / 2 + width / 2 * cosf(markerAngle),
					x + width / 2 + (width / 2 - min(width / 2 * 0.25f, 20)) * cosf(markerAngle),
					x + width / 2 + width / 2 * cosf(markerAngle - markerOpen) };
	float ys[3] = { y + height / 2 + width / 2 * sinf(markerAngle), 
					y + height / 2 + (width / 2 - min(width / 2 * 0.25f, 20)) * sinf(markerAngle),
					y + height / 2 + width / 2 * sinf(markerAngle - markerOpen) };
	Utils::DrawLinesConnected(xs, ys, 3, color_marker, min(6, max(2, width / 60)));

	//Display numerical value (ONLY IF font IS ASSIGNED AND PREPOINT > 0:
		//Border:
	if (font && decimalsPre > 0) {
		float textScale = (0.5f) * width / (max(3, decimalsPre + decimalsPost + 1) * font->at('0').advance / 64.0f);
		float h = font->at('0').height * textScale;
		float xs2[5] = { x + 0.5125f * width,	x + width,				x + width,						x + 0.5125f * width,			x + 0.5125f * width, };
		float ys2[5] = { y + 0.525f * height,	y + 0.525f * height,	y + 0.525f * height + 2 * h,	y + 0.525f * height + 2 * h,	y + 0.525f * height };
		Utils::DrawLinesConnected(xs2, ys2, 5, Utils::COLOR_WHITE, min(8, max(3, width / 40)));

		int pre = (int)(*data);
		int post = (int)((*data - pre) * pow(10, decimalsPost));
		char* text;
		if (decimalsPost)
			text = new char[decimalsPre + 1 + decimalsPost];
		else text = new char[decimalsPre];
		strcpy(text, std::to_string(pre).c_str());
		if (decimalsPost) {
			strcat(text, ".");
			strcat(text, std::to_string(post).c_str());
		}
		float textX = x + 0.975f * width - Utils::GetTextWidth(text, *font, textScale);
		if(p <= 1)
			Utils::RenderText(text, *font, textX, y + 0.5125f * height + 0.5f * h, textScale, Utils::COLOR_WHITE);
		else 
			Utils::RenderText(text, *font, textX, y + 0.5125f * height + 0.5f * h, textScale, color_exceed);
	}
}

void CircularProgressGauge::setMaximum(float max)
{
	this->max = max;
}

void CircularProgressGauge::setTextColor(float c[3])
{
	color_text[0] = c[0];
	color_text[1] = c[1];
	color_text[2] = c[2];
}

void CircularProgressGauge::setColorCircle(float c[3])
{
	color_circle[0] = c[0];
	color_circle[1] = c[1];
	color_circle[2] = c[2];
}

void CircularProgressGauge::setColorExceed(float c[3])
{
	color_exceed[0] = c[0];
	color_exceed[1] = c[1];
	color_exceed[2] = c[2];
}

void CircularProgressGauge::setDataSource(float * src)
{
	data = src;
}

void CircularProgressGauge::configureMarker(float angle, float openness, float c[3])
{
	markerAngle = angle;
	markerOpen = openness;
	color_marker[0] = c[0];
	color_marker[1] = c[1];
	color_marker[2] = c[2];

	while (markerAngle > 0) {
		markerAngle -= (float) M_2_PI;
	}
}

void CircularProgressGauge::setNumberFormat(int prePoint, int postPoint)
{
	decimalsPre = prePoint;
	decimalsPost = postPoint;
}

void CircularProgressGauge::setFont(std::map<GLchar, Character>* f)
{
	font = f;
}
