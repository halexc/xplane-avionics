#include "AirportMap.h"
#include "Utils.h"
#include "UTM.h"

#include <string>

AirportMap::AirportMap()
{
}


AirportMap::~AirportMap()
{
}

void AirportMap::drawTex()
{
	if (!init) {
		if (!Utils::InitFBO(&fbo, &texFBO, width, height, GL_RGB)) {
			XPLMDebugString("AirportMap.cpp: Error when setting up AirportMap framebuffer.\n");
		}
		else init = true;
	}

	if (data_lon == NULL || data_lat == NULL) return;

	if (maxX == 0) {
		airport->getBounds(minX, minY, maxX, maxY);
	}

	int puffer = 100;
	int sizeX = (int)(maxX - minX + 2 * puffer);
	int sizeY = (int)(maxY - minY + 2 * puffer);

	if (!valid) {

		//XPLMDebugString("AirportMap.cpp: Redrawing Airport Map.\n");
		if (!Utils::InitFBO(&fboMap, &texMap, (int) (sizeX * 1024 / scale), (int) (sizeY * 1024 / scale), GL_RGBA)) {
			XPLMDebugString("AirportMap.cpp: Error when setting up Map framebuffer.\n");
		}
		GLint fboPrev;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fboPrev);
		glBindFramebuffer(GL_FRAMEBUFFER, fboMap);

		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(minX - puffer, maxX + puffer, minY - puffer, maxY + puffer, -1, 1);
		/*
		XPLMDebugString("Airport bounds:\nHorizontal: ");
		XPLMDebugString(std::to_string(minX).c_str());
		XPLMDebugString(", ");
		XPLMDebugString(std::to_string(maxX).c_str());
		XPLMDebugString(".\nVertical: ");
		XPLMDebugString(std::to_string(minY).c_str());
		XPLMDebugString(", ");
		XPLMDebugString(std::to_string(maxY).c_str());
		XPLMDebugString(".\n");
		*/
		GLint oldView[4];
		glGetIntegerv(GL_VIEWPORT, oldView);

		glViewport(0, 0, (int)(sizeX * 1024 / scale), (int)(sizeY * 1024 / scale));

		//Draw:
		drawPavement(Utils::COLOR_GRAY_DARK);
		//drawTaxiways(15, Utils::COLOR_GRAY);
		//drawRunways(60, Utils::COLOR_TURQUOISE_BRIGHT);

		glViewport(oldView[0], oldView[1], oldView[2], oldView[3]);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glBindFramebuffer(GL_FRAMEBUFFER, fboPrev);

		valid = true;
	}

	GLint fboOld;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fboOld);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);

	GLint oldView[4];
	glGetIntegerv(GL_VIEWPORT, oldView);

	glViewport(0, 0, width, height);

	//XPLMDebugString("AirportMap.cpp: Drawing map at correct position and rotation.\n");
	//Draw:
	float gps_x, gps_y;
	LatLonToUTMXY(*data_lat, *data_lon, airport->getUTMZone(), gps_x, gps_y);
	float offsetCX = ((maxX + minX) / 2 - gps_x) * width / scale;
	float offsetCY = ((maxY + minY) / 2 - gps_y) * height / scale;

	float hdg = 0;

	if(data_hdg) hdg = *data_hdg;

	float addX = sizeX / scale * width / 2;
	float addY = sizeY / scale * height / 2;
	
	XPLMBindTexture2d(texMap, 0);

	glBegin(GL_QUADS);
		float px, py;
		Utils::calcRotatedPoint(width / 2 - addX + offsetCX, height / 2 - addY + offsetCY, hdg * (float) M_PI / 180, &px, &py, width / 2, height / 2);
		glTexCoord2f(0, 0);		glVertex2f(px, py);
		Utils::calcRotatedPoint(width / 2 - addX + offsetCX, height / 2 + addY + offsetCY, hdg * (float) M_PI / 180, &px, &py, width / 2, height / 2);
		glTexCoord2f(0, 1);		glVertex2f(px, py);
		Utils::calcRotatedPoint(width / 2 + addX + offsetCX, height / 2 + addY + offsetCY, hdg * (float) M_PI / 180, &px, &py, width / 2, height / 2);
		glTexCoord2f(1, 1);		glVertex2f(px, py);
		Utils::calcRotatedPoint(width / 2 + addX + offsetCX, height / 2 - addY + offsetCY, hdg * (float) M_PI / 180, &px, &py, width / 2, height / 2);
		glTexCoord2f(1, 0);		glVertex2f(px, py);
	glEnd();
	
	glViewport(oldView[0], oldView[1], oldView[2], oldView[3]);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, fboOld);
}

void AirportMap::update()
{
}

void AirportMap::setScale(float scale)
{
	valid = false;
	this->scale = scale;
}

void AirportMap::setDataSource_GPS(float * lat, float * lon)
{
	data_lat = lat;
	data_lon = lon;
}

void AirportMap::setDataSource_hdg(float * hdg)
{
	data_hdg = hdg;
}

void AirportMap::setAirport(Airport * apt)
{
	valid = false;
	airport = apt;
	airport->getBounds(minX, minY, maxX, maxY);
}

void AirportMap::drawRunways(float w, float color[3], bool drawID)
{
	for (Runway * r : airport->getRunways()) {
		float w_r = w;
		if (w == 0) w_r = r->width * 1024 / scale;
		float dx = (r->E2 - r->E1);
		float dy = (r->N2 - r->N1);
		float endX = w_r / 2 * dy / sqrtf(dx * dx + dy * dy);
		float endY = w_r / 2 * dx / sqrtf(dx * dx + dy * dy);
		XPLMBindTexture2d(0, 0);
		glColor3fv(color);
		glBegin(GL_QUADS);
			glVertex2f(r->E1 - endX, r->N1 + endY);
			glVertex2f(r->E2 - endX, r->N2 + endY);
			glVertex2f(r->E2 + endX, r->N2 - endY);
			glVertex2f(r->E1 + endX, r->N1 - endY);
		glEnd();
	}
}

void AirportMap::drawTaxiways(float w, float color[3], bool drawID)
{
	for (Taxiway * tw : airport->getEdges()) {
		TaxiwayNode start = airport->getNodes().at(tw->startNode);
		TaxiwayNode end = airport->getNodes().at(tw->endNode);

		if (tw->cat == Category::RW) continue;

		float w_t = w;
		if (w == 0) {
			w_t = 10 * 1024 / scale;
		}
		switch (tw->cat) {
			case Category::A:
				w_t *= 1;
				break;
			case Category::B:
				w_t *= 1.5f;
				break;
			case Category::C:
				w_t *= 2;
				break;
			case Category::D:
				w_t *= 2.5f;
				break;
			case Category::E:
				w_t *= 3;
				break;
			case Category::F:
				w_t *= 3.25f;
				break;
			default:
				break;
		}
		float dx = (end.E - start.E);
		float dy = (end.N - start.N);
		float endX = w_t / 2 * dy / sqrtf(dx * dx + dy * dy);
		float endY = w_t / 2 * dx / sqrtf(dx * dx + dy * dy);
		XPLMBindTexture2d(0, 0);
		glColor3fv(color);
		glBegin(GL_QUADS);
		glVertex2f(start.E - endX, start.N + endY);
		glVertex2f(end.E - endX, end.N + endY);
		glVertex2f(end.E + endX, end.N - endY);
		glVertex2f(start.E + endX, start.N - endY);
		glEnd();
		Utils::DrawCircleFilled(w_t / 2, start.E, start.N, color, 8);
		Utils::DrawCircleFilled(w_t / 2, end.E, end.N, color, 8);
	}
}

void AirportMap::drawGates()
{
	
}

void AirportMap::drawPavement(float color[3])
{
	for (std::vector<Vector2f*> polygon : airport->getPavementPolygons()) {
		XPLMBindTexture2d(0, 0);
		glColor3fv(color);
		glBegin(GL_TRIANGLE_FAN);
		for (Vector2f * vertex : polygon) {
			glVertex2f(vertex->x, vertex->y);
		}
		glEnd();
	}
	for (std::vector<Vector2f*> polygon : airport->getHolePolygons()) {
		glColor3fv(Utils::COLOR_BLACK);
		glBegin(GL_TRIANGLE_FAN);
		for (Vector2f * vertex : polygon) {
			glVertex2f(vertex->x, vertex->y);
		}
		glEnd();
	}
	glLineWidth(4.0f);
	for (std::vector<Vector2f*> polygon : airport->getPavementPolygons_raw()) {
		//DEBUG:
		glColor3fv(Utils::COLOR_GREEN);
		glBegin(GL_LINE_LOOP);
		for (Vector2f * vertex : polygon) {
			glVertex2f(vertex->x, vertex->y);
		}
		glEnd();
	}
	for (std::vector<Vector2f*> polygon : airport->getHolePolygons_raw()) {
		//DEBUG:
		glColor3fv(Utils::COLOR_RED);
		glBegin(GL_LINE_LOOP);
		for (Vector2f * vertex : polygon) {
			glVertex2f(vertex->x, vertex->y);
		}
		glEnd();
	}
}
