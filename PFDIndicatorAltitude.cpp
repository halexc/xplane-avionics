#include "PFDIndicatorAltitude.h"
#include <cmath>
#include <string>


PFDIndicatorAltitude::PFDIndicatorAltitude()
{
}


PFDIndicatorAltitude::~PFDIndicatorAltitude()
{
}

void PFDIndicatorAltitude::update()
{
}


void PFDIndicatorAltitude::setDataSources(float * alt, float * radioAlt, float * vs)
{
	data_altitude = alt;
	data_radioAlt = radioAlt;
	data_vs = vs;
}

void PFDIndicatorAltitude::setFont(std::map<GLchar, Character>* f)
{
	font = f;
}

void PFDIndicatorAltitude::drawTex()
{
	float alt = *data_altitude;
	float radioAlt = *data_radioAlt;
	float xs[] = { 0.2f * width, 0.2f * width, 0.15f * width, 0.2 * width, 0.2 * width, 1.0f * width, 1.0f * width, 0.2f * width };
	float ys[] = { 0.4125f * height, 0.475f * height, 0.5f * height, 0.525f * height, 0.5875f * height, 0.5875f * height, 0.4125f * height, 0.4125f * height };
	drawTexAltNumber(alt, 0.8f * width, 0.175f * height);
	if (!init) {
		if (!Utils::InitFBO(&fbo, &texFBO, width, height, GL_RGBA)) {
			XPLMDebugString("PFDIndicatorAltitude.cpp: Error when setting up altitude framebuffer.");
		}
		else init = true;
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
	glViewport(0, 0, width, height);

	//Draw:
	XPLMBindTexture2d(0, 0);
	glColor4f(0.25f, 0.25f, 0.25f, 0.75f);
	glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(0, height);
		glVertex2f(width, height);
		glVertex2f(width, 0);
	glEnd();

	float interval = height / 1500.0f;
	alt = fmaxf(alt, 0);
	float offsetMSL = height / 2.0f - alt * interval;
	float offsetGL = (alt - radioAlt) * interval;
	Utils::DrawLine(0, height, 0, fmaxf(offsetMSL + offsetGL, 0), Utils::COLOR_WHITE, width / 16.0f);
	
	for (int i = 100; i < 55000; i += 100) {
		if (i < alt - radioAlt) continue;
		if (alt - i > 1000) continue;
		if (i - alt > 1000) continue;
		if (i % 500 == 0) {
			Utils::DrawLine(0, i * interval + offsetMSL, 0.15f * width, i * interval + offsetMSL, Utils::COLOR_WHITE, width / 32.0f);
			Utils::RenderText(std::to_string(i).c_str(), *font, 0.175f * width, i * interval + offsetMSL - font->at('0').height * width / 256.0f, width / 128.0f);
		}
		else
			Utils::DrawLine(0, i * interval + offsetMSL, 0.15f * width, i * interval + offsetMSL, Utils::COLOR_WHITE, width / 64.0f);
	}
	if (radioAlt < 1000)
		for (int i = 0; i < 20; i++)
			Utils::DrawLine((i - 12) * width / 8, offsetMSL + offsetGL, (i - 12) * width / 8 + height / 2, offsetMSL + offsetGL - height / 2, Utils::COLOR_YELLOW, width / 24.0f);
	Utils::DrawLine(0, offsetMSL + offsetGL, width, offsetMSL + offsetGL, Utils::COLOR_WHITE, width / 24.0f);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	XPLMBindTexture2d(0, 0);
	glBegin(GL_TRIANGLES);
		glVertex2f(xs[1], ys[1]);
		glVertex2f(xs[2], ys[2]);
		glVertex2f(xs[3], ys[3]);
	glEnd();
	glBegin(GL_QUADS);
		glVertex2f(xs[0], ys[0]);
		glVertex2f(xs[4], ys[4]);
		glVertex2f(xs[5], ys[5]);
		glVertex2f(xs[6], ys[6]);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	XPLMBindTexture2d(texAltNum, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);		glVertex2f(xs[0], ys[0]);
		glTexCoord2f(0, 1);		glVertex2f(xs[4], ys[4]);
		glTexCoord2f(1, 1);		glVertex2f(xs[5], ys[5]);
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

void PFDIndicatorAltitude::drawTexAltNumber(float alt, float width, float height)
{
	if (!init) {
		if (!Utils::InitFBO(&fboAltNum, &texAltNum, (int)width, (int)height, GL_RGBA)) {
			XPLMDebugString("PFDIndicatorAltitude.cpp: Error when setting up altitude framebuffer.");
		}
	}
	//Bind framebuffer & adjust projection
	GLint framebuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, fboAltNum);

	glClear(GL_COLOR_BUFFER_BIT);

	GLint oldView[4];
	glGetIntegerv(GL_VIEWPORT, oldView);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glViewport(0, 0, (int)width, (int)height);

	//Draw:
	float textSize = width / 90;
	int ttsds = (int)(fmodf(alt, 100000) - fmodf(alt, 10000)) / 10000;
	int tsds = (int)(fmodf(alt, 10000) - fmodf(alt, 1000)) / 1000;
	int huns = (int)(fmodf(alt, 1000) - fmodf(alt, 100)) / 100;
	int tens = (int)(fmodf(alt, 100) - fmodf(alt, 10)) / 10;
	float decimals = fmodf(alt, 20);

	glColor4f(1, 1, 1, 1);
	if (ttsds || (tsds == 9 && huns == 9 && tens >= 8)) {
		if (tsds == 9 && huns == 9 && tens >= 8) {
			Utils::RenderText(std::to_string((ttsds + 2) % 10).c_str(), *font, 0.03f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals / 20 * 0.75f - 1.5f), textSize);
			Utils::RenderText(std::to_string((ttsds + 1) % 10).c_str(), *font, 0.03f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals / 20 * 0.75f - 0.75f), textSize);
			if (ttsds)
				Utils::RenderText(std::to_string(ttsds).c_str(), *font, 0.03f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals / 20 * 0.75f), textSize);
		}
		else
			Utils::RenderText(std::to_string(ttsds).c_str(), *font, 0.03f * width, 0.5f * height - font->at('0').height * textSize / 2, textSize);
	}
	if (ttsds || tsds || (huns == 9 && tens >= 8)) {
		if (huns == 9 && tens >= 8) {
			Utils::RenderText(std::to_string((tsds + 2) % 10).c_str(), *font, 0.24f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals / 20 * 0.75f - 1.5f), textSize);
			Utils::RenderText(std::to_string((tsds + 1) % 10).c_str(), *font, 0.24f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals / 20 * 0.75f - 0.75f), textSize);
			if (ttsds || tsds)
				Utils::RenderText(std::to_string(tsds).c_str(), *font, 0.24f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals / 20 * 0.75f), textSize);
		}
		else
			Utils::RenderText(std::to_string(tsds).c_str(), *font, 0.24f * width, 0.5f * height - font->at('0').height * textSize / 2, textSize);
	}
	if (ttsds || tsds || huns || tens >= 8) {
		if (tens >= 8) {
			Utils::RenderText(std::to_string((huns + 2) % 10).c_str(), *font, 0.45f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals / 20 * 0.75f - 1.5f), textSize);
			Utils::RenderText(std::to_string((huns + 1) % 10).c_str(), *font, 0.45f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals / 20 * 0.75f - 0.75f), textSize);
			if (ttsds || tsds || huns)
				Utils::RenderText(std::to_string(huns).c_str(), *font, 0.45f * width, 0.5f * height - font->at('0').height * textSize / 2 - height * (decimals / 20 * 0.75f), textSize);
		}
		else
			Utils::RenderText(std::to_string(huns).c_str(), *font, 0.45f * width, 0.5f * height - font->at('0').height * textSize / 2, textSize);
	}
	tens = (int)((int)(tens / 2.0f) * 2) % 10;
	Utils::RenderText(std::to_string(((tens + 4) % 10)).c_str(), *font, 0.66f * width, 0.5f * height - font->at('0').height * textSize * 3 / 8 - height * (decimals / 20 * 0.4f - 0.8f), textSize * 3 / 4);
	Utils::RenderText("0", *font, 0.8f * width, 0.5f * height - font->at('0').height * textSize * 3 / 8 - height * (decimals / 20 * 0.4f - 0.8f), textSize * 3 / 4);
	Utils::RenderText(std::to_string(((tens + 2) % 10)).c_str(), *font, 0.66f * width, 0.5f * height - font->at('0').height * textSize * 3 / 8 - height * (decimals / 20 * 0.4f - 0.4f), textSize * 3 / 4);
	Utils::RenderText("0", *font, 0.8f * width, 0.5f * height - font->at('0').height * textSize * 3 / 8 - height * (decimals / 20 * 0.4f - 0.4f), textSize * 3 / 4);
	Utils::RenderText(std::to_string(tens).c_str(), *font, 0.66f * width, 0.5f * height - font->at('0').height * textSize * 3 / 8 - height * (decimals / 20 * 0.4f), textSize * 3 / 4);
	Utils::RenderText("0", *font, 0.8f * width, 0.5f * height - font->at('0').height * textSize * 3 / 8 - height * (decimals / 20 * 0.4f), textSize * 3 / 4);
	if (alt > 0)
	{
		Utils::RenderText(std::to_string(((tens + 8) % 10)).c_str(), *font, 0.66f * width, 0.5f * height - font->at('0').height * textSize * 3 / 8 - height * (decimals / 20 * 0.4f + 0.4f), textSize * 3 / 4);
		Utils::RenderText("0", *font, 0.78f * width, 0.5f * height - font->at('0').height * textSize * 3 / 8 - height * (decimals / 20 * 0.4f + 0.4f), textSize * 3 / 4);
	}

	//Reset projection & framebuffer:
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(oldView[0], oldView[1], oldView[2], oldView[3]);
}
