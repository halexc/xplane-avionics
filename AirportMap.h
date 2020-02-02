#pragma once
#include "DisplayElementFBO.h"
#include "Airport.h"

class AirportMap :
	public DisplayElementFBO
{
public:
	AirportMap();
	~AirportMap();

	void drawTex();
	void update();

	//Map scale: meters mapped across the width of the map (on screen)
	void setScale(float scale);
	void setDataSource_GPS(float * lat, float * lon);
	void setDataSource_hdg(float * hdg);
	
	void setAirport(Airport * apt);

private:
	Airport * airport;

	void drawRunways(float w, float color[3], bool drawID = false);
	void drawTaxiways(float w, float color[3], bool drawID = false);
	void drawGates();

	bool valid = false;

	float * data_lat;
	float * data_lon;
	float * data_hdg;

	GLint texMap;
	GLuint fboMap;

	//1NM as default scale
	float scale = 1852.0f;
	//
	float minX, minY, maxX, maxY;

};

