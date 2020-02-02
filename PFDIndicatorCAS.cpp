#include "PFDIndicatorCAS.h"
#include <cmath>
#include <string>


PFDIndicatorCAS::PFDIndicatorCAS()
{
}


PFDIndicatorCAS::~PFDIndicatorCAS()
{
}

void PFDIndicatorCAS::update()
{
}

void PFDIndicatorCAS::setDataSources(float * cas, float * acceleration)
{
	data_cas = cas;
	data_acceleration = acceleration;
}

void PFDIndicatorCAS::setFont(std::map<GLchar, Character>* f)
{
	font = f;
}

void PFDIndicatorCAS::drawTex()
{
	float speed = *data_cas;
	float xs[] = { 0.2f * width, 0.2f * width, 0.8f * width, 0.8f * width, 0.85f * width, 0.8f * width, 0.8f * width, 0.2f * width };
	float ys[] = { 0.425f * height, 0.575f * height, 0.575f * height, 0.525f * height, 0.5f * height, 0.475f * height, 0.425f * height, 0.425f * height };
	drawTexIASNumber(speed, 0.6f * width, 0.15f * height);
	if (!initAS) {
		if (!Utils::InitFBO(&fbo, &texFBO, (int)width, (int)height, GL_RGBA)) {
			XPLMDebugString("PFDIndicatorCAS.cpp: Error when setting up airspeed framebuffer.");
		}
		else initAS = true;
	}
	//Bind framebuffer & adjust projection
	GLint framebuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClear(GL_COLOR_BUFFER_BIT);

	GLint oldView[4];
	glGetIntegerv(GL_VIEWPORT, oldView);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glViewport(0, 0, (int)width, (int)height);

	//Draw:
	XPLMBindTexture2d(0, 0);
	glColor4f(0.25f, 0.25f, 0.25f, 0.75f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(0, height);
	glVertex2f(width, height);
	glVertex2f(width, 0);
	glEnd();

	float interval = height / 80.0f;
	speed = fmaxf(speed, 30.0f);
	float offset30 = height / 2.0f - (speed - 30) * interval;

	Utils::DrawLine(width, offset30, width, height, Utils::COLOR_WHITE, width / 16.0f);
	for (int i = 30; i < 450; i += 10) {
		if (fabsf(speed - i) > 50) continue;
		Utils::DrawLine(0.85f * width, (i - 30) * interval + offset30, width, (i - 30) * interval + offset30, Utils::COLOR_WHITE, width / 32.0f);
		if (i > 80 && i % 20 != 0) continue;
		float textWidth = Utils::GetTextWidth(std::to_string(i).c_str(), *font, interval / 6.0f);
		Utils::RenderText(std::to_string(i).c_str(), *font, 0.825f * width - textWidth, (i - 30) * interval + offset30 - font->at('0').height * interval / 12, interval / 6.0f);
	}
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	XPLMBindTexture2d(0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(xs[3], ys[3]);
	glVertex2f(xs[4], ys[4]);
	glVertex2f(xs[5], ys[5]);
	glEnd();
	glBegin(GL_QUADS);
	glVertex2f(xs[0], ys[0]);
	glVertex2f(xs[1], ys[1]);
	glVertex2f(xs[2], ys[2]);
	glVertex2f(xs[6], ys[6]);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	XPLMBindTexture2d(texASnum, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);		glVertex2f(xs[0], ys[0]);
	glTexCoord2f(0, 1);		glVertex2f(xs[1], ys[1]);
	glTexCoord2f(1, 1);		glVertex2f(xs[2], ys[2]);
	glTexCoord2f(1, 0);		glVertex2f(xs[6], ys[6]);
	glEnd();


	Utils::DrawLinesConnected(xs, ys, 8, Utils::COLOR_WHITE, width / 32);

	//Reset projection & framebuffer:
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(oldView[0], oldView[1], oldView[2], oldView[3]);
}

void PFDIndicatorCAS::drawTexIASNumber(float speed, float width, float height)
{
	if (!initAS) {
		if (!Utils::InitFBO(&fboASnum, &texASnum, (int)width, (int)height, GL_RGBA)) {
			XPLMDebugString("PFDIndicatorCAS.cpp: Error when setting up airspeed framebuffer.");
		}
	}
	//Bind framebuffer & adjust projection
	GLint framebuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, fboASnum);

	glClear(GL_COLOR_BUFFER_BIT);

	GLint oldView[4];
	glGetIntegerv(GL_VIEWPORT, oldView);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glViewport(0, 0, (int)width, (int)height);

	speed = fmaxf(speed, 30.0f);
	//Draw:
	float textSize = width / 68;
	int huns = (int)(fmodf(speed, 1000) - fmodf(speed, 100)) / 100;
	int tens = (int)(fmodf(speed, 100) - fmodf(speed, 10)) / 10;
	int ones = (int)(fmodf(speed, 10) - fmodf(speed, 1)) / 1;
	float decimals = fmodf(speed, 1);

	glColor4f(1, 1, 1, 1);

	if (huns || (tens == 9 && ones == 9)) {
		if (tens == 9 && ones == 9) {
			Utils::RenderText(std::to_string((huns + 2) % 10).c_str(), *font, 0.1f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals * 0.6f - 1.2f), textSize);
			Utils::RenderText(std::to_string((huns + 1) % 10).c_str(), *font, 0.1f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals * 0.6f - 0.6f), textSize);
			if (huns)
				Utils::RenderText(std::to_string(huns).c_str(), *font, 0.1f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals * 0.6f), textSize);
		}
		else
			Utils::RenderText(std::to_string(huns).c_str(), *font, 0.1f * width, 0.5f * height - font->at('0').height * textSize / 2, textSize);
	}
	if (tens || huns || ones == 9) {
		if (ones == 9) {
			Utils::RenderText(std::to_string((tens + 2) % 10).c_str(), *font, 0.35f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals * 0.6f - 1.2f), textSize);
			Utils::RenderText(std::to_string((tens + 1) % 10).c_str(), *font, 0.35f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals * 0.6f - 0.6f), textSize);
			if (tens || huns)
				Utils::RenderText(std::to_string(tens).c_str(), *font, 0.35f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals * 0.6f), textSize);
		}
		else
			Utils::RenderText(std::to_string(tens).c_str(), *font, 0.35f * width, 0.5f * height - font->at('0').height * textSize / 2, textSize);
	}
	Utils::RenderText(std::to_string((ones + 2) % 10).c_str(), *font, 0.6f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals * 0.6f - 1.2f), textSize);
	Utils::RenderText(std::to_string((ones + 1) % 10).c_str(), *font, 0.6f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals * 0.6f - 0.6f), textSize);
	Utils::RenderText(std::to_string(ones).c_str(), *font, 0.6f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals* 0.6f), textSize);
	if (speed >= 31)
		Utils::RenderText(std::to_string((ones + 9) % 10).c_str(), *font, 0.6f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals* 0.6f + 0.6f), textSize);

	//Reset projection & framebuffer:
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(oldView[0], oldView[1], oldView[2], oldView[3]);
}
