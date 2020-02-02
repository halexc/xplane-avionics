#include "PFDGyro.h"
#include "Utils.h"

#include <string>
#include <cmath>


PFDGyro::PFDGyro()
{
}


PFDGyro::~PFDGyro()
{
}

void PFDGyro::draw()
{
	drawGyro(width, height);
}

void PFDGyro::update()
{
}

void PFDGyro::drawTex()
{
	drawTexPitchIndicator(*data_pitch, width, height);
	drawTexHdgIndicator(*data_hdg, width, height);
}

void PFDGyro::drawTexPitchIndicator(float pitch, float width, float height) {
	if (!initPitch) {
		if (!Utils::InitFBO(&fboPitch, &texPitch, (int) width, (int) height, GL_RGBA)) {
			XPLMDebugString("PFDGyro.cpp: Error when setting up pitch framebuffer.");
		}
		else initPitch = true;
	}
	GLint framebuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, fboPitch);

	glClear(GL_COLOR_BUFFER_BIT);

	GLint oldView[4];
	glGetIntegerv(GL_VIEWPORT, oldView);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glViewport(0, 0, (int) width, (int) height);
	
	//Draw:
	float interval = height / 60.0f;
	float offset10 = fmodf(pitch, 10);
	int highestNum = (int) roundf(pitch - offset10) + 30;

	//Draw large indicators at 10° and then every 10°:	
	for (int i = 0; i < 6; i++) {
		if (abs(highestNum - i * 10) > 90) continue;
		float textSize = interval / 10.0f;
		Utils::DrawLine(0.45f * width, height - (i * 10 + offset10) * interval, 0.55f * width, height - (i * 10 + offset10) * interval, Utils::COLOR_WHITE, 0.375f * interval);
		float textWidth = Utils::GetTextWidth(std::to_string((int) abs(highestNum - i * 10)).c_str(), *font, textSize);
		if (highestNum - i * 10 == 0) continue;
		Utils::RenderText(std::to_string((int) abs(highestNum - i * 10)).c_str(), *font, 0.44f * width - textWidth, height - (i * 10 + offset10) * interval - font->at('0').height * textSize/2, textSize);
		Utils::RenderText(std::to_string((int) abs(highestNum - i * 10)).c_str(), *font, 0.56f * width, height - (i * 10 + offset10) * interval - font->at('0').height * textSize/2, textSize);
	}
	//Draw medium indicators at 5° and then every 10°:
	for (int i = -1; i < 7; i++) {
		if (abs(highestNum - i * 10 - 5) > 90) continue;
		Utils::DrawLine(0.475f * width, height - ((i * 10 + offset10) + 5) * interval, 0.525f * width, height - ((i * 10 + offset10) + 5) * interval, Utils::COLOR_WHITE, 0.375f * interval);
	}
	//Draw small indicators at 2.5° and then every 5°:
	for (int i = -2; i < 14; i++) {
		if (abs(highestNum - i * 5 - 2.5f) > 90) continue;
		Utils::DrawLine(0.4875f * width, height - ((i * 5 + offset10) + 2.5f) * interval, 0.5125f * width, height - ((i * 5 + offset10) + 2.5f) * interval, Utils::COLOR_WHITE, 0.25f * interval);
	}

	//Reset Projection & Framebuffer:
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(oldView[0], oldView[1], oldView[2], oldView[3]);
	
}
void PFDGyro::drawTexHdgIndicator(float hdg, float width, float height) {
	float diag = sqrtf(width*width + height*height);
	if (!initHdg) {
		if (!Utils::InitFBO(&fboHdg, &texHdg, (int) diag, (int) height, GL_RGBA)) {
			XPLMDebugString("PFDGyro.cpp: Error when setting up heading (horizon) framebuffer.");
		}
		else initHdg = true;
	}
	//Bind framebuffer & adjust projection
	GLint framebuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHdg);

	glClear(GL_COLOR_BUFFER_BIT);

	GLint oldView[4];
	glGetIntegerv(GL_VIEWPORT, oldView);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, diag, 0, height, -1, 1);
	glViewport(0, 0, (int)diag, (int)height);

	//Draw:
	float interval = width / 90.0f;
	float textSize = height / (64*12);
	//Horizon line:
	Utils::DrawLine(0, height / 2, diag, height / 2, Utils::COLOR_WHITE, 6.0f * textSize);
	
	//Draw HDG indicators:
	float offset10 = fmodf(hdg, 10);
	int left = (int) roundf(hdg - offset10) - 40;
	float offsetWidth = (diag - width) / 2 + interval * 5;
	for (int i = -3; i < 12; i++) {
		float x = offsetWidth + (i * 10 - offset10) * interval;
		Utils::DrawLine(x, height / 2, x, height / 2 - textSize * 16.0f, Utils::COLOR_WHITE, 6.0f * textSize);
		if ((360 + left + i * 10) % 20 == 0) {
			if ((360 + left + i * 10) % 360 == 0) {
				float textWidth = Utils::GetTextWidth("36", *font, textSize);
				Utils::RenderText("36", *font, x - textWidth / 2, height / 2 - textSize * 16.0f - 1.25f* font->at('0').height * textSize, textSize);
				continue;
			}
			float textWidth = Utils::GetTextWidth(std::to_string(((360 + left + i * 10) % 360) / 10).c_str(), *font, textSize);
			Utils::RenderText(std::to_string(((360 + left + i * 10) % 360)/10).c_str(), *font, x - textWidth/2, height / 2 - textSize * 16.0f - 1.25f* font->at('0').height * textSize, textSize);
		}
	}
	//Utils::RenderText(std::to_string(hdg).c_str(), *font, diag * 0.48125f, height * 0.375f, 2 * textSize);

	//Reset projection & framebuffer:
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(oldView[0], oldView[1], oldView[2], oldView[3]);
}

void PFDGyro::drawGyro(float width, float height)
{
	if (data_roll == 0) return;
	if (data_pitch == 0) return;
	if (data_hdg == 0) return;

	float cx = 0.5 * width;
	float cy = 0.5 * height;
	float roll_rad = *data_roll * M_PI / 180;

	//Draw:
	float diag = sqrtf(width*width + height*height);
	XPLMBindTexture2d(0, 0);
	glBegin(GL_QUADS);
		glColor3f(1.0f, 0.5f, 0.125f);
		float px = cx - diag / 2;
		float py = cy - 4 * height - *data_pitch * height / 60;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glVertex2f(px, py);
		glColor3f(0.6875f, 0.375f, 0.125f);
		px = cx - diag / 2;
		py = cy - *data_pitch * height / 60;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glVertex2f(px, py);

		px = cx + diag / 2;
		py = cy - *data_pitch * height / 60;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glVertex2f(px, py);

		glColor3f(1.0f, 0.5f, 0.125f);
		px = cx + diag / 2;
		py = cy - 4 * height - *data_pitch * height / 60;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glVertex2f(px, py);
	glEnd();
	glBegin(GL_QUADS);
		glColor3f(.25f, .25f, 1.0f);
		px = cx - diag / 2;
		py = cy - *data_pitch * height / 60;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glVertex2f(px, py);

		glColor3f(.1875f, .1875f, 0.875f);
		px = cx - diag / 2;
		py = cy + 4 * height - *data_pitch * height / 60;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glVertex2f(px, py);

		px = cx + diag / 2; 
		py = cy + 4 * height - *data_pitch * height / 60;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glVertex2f(px, py);

		glColor3f(.25f, .25f, 1.0f);
		px = cx + diag / 2;
		py = cy - *data_pitch * height / 60;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glVertex2f(px, py);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	XPLMBindTexture2d(texHdg, 0);
	glBegin(GL_QUADS);
		px = cx - diag / 2;
		py = -*data_pitch * height / 60;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glTexCoord2f(0, 0);		glVertex2f(px, py);

		px = cx - diag / 2;
		py = height - *data_pitch * height / 60;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glTexCoord2f(0, 1);		glVertex2f(px, py);

		px = cx + diag / 2;
		py = height - *data_pitch * height / 60;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glTexCoord2f(1, 1);		glVertex2f(px, py);

		px = cx + diag / 2;
		py = -*data_pitch * height / 60;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glTexCoord2f(1, 0);		glVertex2f(px, py);
	glEnd();
	XPLMBindTexture2d(texPitch, 0);
	glBegin(GL_QUADS);
		px = 0.4f * width;
		py = 0.15f * height;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glTexCoord2f(0.4f, 0.15f);		glVertex2f(px, py);

		px = 0.4f * width;
		py = 0.8f * height;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glTexCoord2f(0.4f, 0.8f);		glVertex2f(px, py);

		px = 0.6f * width;
		py = 0.8f * height;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glTexCoord2f(0.6f, 0.8f);		glVertex2f(px, py);

		px = 0.6f * width;
		py = 0.15f * height;
		Utils::calcRotatedPoint(px, py, roll_rad, &px, &py, cx, cy);
		glTexCoord2f(0.6f, 0.15f);		glVertex2f(px, py);
	glEnd();

	//Draw centering indicators:
	float xsL[] = { 0.375f * width, 0.375f * width, 0.45f * width, 0.45f * width, 0.45f * width - 0.015f * height, 0.45f * width - 0.015f * height, 0.375f * width };
	float xsR[] = { 0.55f * width, 0.55f * width, 0.625f * width, 0.625f * width, 0.55f * width + 0.015f * height, 0.55f * width + 0.015f * height, 0.55f * width };

	float ysL[] = { cy - 0.0075f * height, cy + 0.0075f * height, cy + 0.0075f * height, 0.45f * height, 0.45f * height, cy - 0.0075f * height, cy - 0.0075f * height };
	float ysR[] = { 0.45f * height, cy + 0.0075f * height, cy + 0.0075f * height, cy - 0.0075f * height, cy - 0.0075f * height, 0.45f * height, 0.45f * height };

	float xsC[] = { cx - 0.0075f * height, cx - 0.0075f * height, cx + 0.0075f * height, cx + 0.0075f * height, cx - 0.0075f * height };
	float ysC[] = { cy - 0.0075f * height, cy + 0.0075f * height, cy + 0.0075f * height, cy - 0.0075f * height, cy - 0.0075f * height };

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	XPLMBindTexture2d(0, 0);
	glBegin(GL_QUADS);
		glVertex2f(xsL[0], ysL[0]);
		glVertex2f(xsL[1], ysL[1]);
		glVertex2f(xsL[2], ysL[2]);
		glVertex2f(xsL[5], ysL[5]);

		glVertex2f(xsL[4], ysL[4]);
		glVertex2f(xsL[5], ysL[5]);
		glVertex2f(xsL[2], ysL[2]);
		glVertex2f(xsL[3], ysL[3]);

		glVertex2f(xsR[0], ysR[0]);
		glVertex2f(xsR[1], ysR[1]);
		glVertex2f(xsR[4], ysR[4]);
		glVertex2f(xsR[5], ysR[5]);

		glVertex2f(xsR[4], ysR[4]);
		glVertex2f(xsR[1], ysR[1]);
		glVertex2f(xsR[2], ysR[2]);
		glVertex2f(xsR[3], ysR[3]);

		glVertex2f(xsC[0], ysC[0]);
		glVertex2f(xsC[1], ysC[1]);
		glVertex2f(xsC[2], ysC[2]);
		glVertex2f(xsC[3], ysC[3]);
	glEnd();

	Utils::DrawLinesConnected(xsL, ysL, 7, Utils::COLOR_WHITE, height / 256);
	Utils::DrawLinesConnected(xsR, ysR, 7, Utils::COLOR_WHITE, height / 256);
	Utils::DrawLinesConnected(xsC, ysC, 5, Utils::COLOR_WHITE, height / 256);
}

void PFDGyro::setBounds(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	initPitch = false;
	initHdg = false;
}

void PFDGyro::setDataSources(float * pitch, float * roll, float * hdg)
{
	data_pitch = pitch;
	data_roll = roll;
	data_hdg = hdg;
}

void PFDGyro::setFont(std::map<GLchar, Character>* f)
{
	font = f;
}
