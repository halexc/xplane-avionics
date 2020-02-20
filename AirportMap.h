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

	/** void setScale(float scale)
	*
	*	Parameters:
	*	- float scale: Distance to be mapped across the width
	*
	*	Use:
	*	Set the scale of the map.
	*/
	void setScale(float scale);

	/** void setDataSource_GPS(float * lat, float * lon)
	*
	*	Parameters:
	*	- float * lat: Adress of where to get latitude data
	*	- float * lon: Adress of where to get longitude data
	*	
	*	Use:
	*	Set the data sources for positional data.
	*/
	void setDataSource_GPS(float * lat, float * lon);

	/** void setDataSource_hdg(float * hdg)
	*
	*	Parameters:
	*	- float * hdg: Adress of where to get HDG data
	*
	*	Use:
	*	Set the data sources for heading data.
	*/
	void setDataSource_hdg(float * hdg);

	/** void setAirport(Airport * apt)
	*
	*	Parameters:
	*	- Airport * apt: Airport data from which to draw
	*
	*	Use:
	*	Set the airport from which the data for drawing is taken.
	*/
	void setAirport(Airport * apt);

private:
	Airport * airport;

	void drawRunways(float w, float color[3], bool drawID = false);
	void drawTaxiways(float w, float color[3], bool drawID = false);
	void drawGates();
	void drawPavement(float color[3]);

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

