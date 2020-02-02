#pragma once

#include "XPLMUtilities.h"
#include <map>
#include <list>

enum NodeUse {
	START = 1, END = 2, BOTH = 3, JUNC = 4
}; 
enum Category {
	A = 1, B = 3, C = 7, D = 15, E = 31, F = 63, RW = 0
};
enum SurfaceType {
	NONE = 0, ASPHALT = 1, CONCRETE = 2
};
enum GateType {
	APRON = 0, GATE = 1, CARGO = 2
};

struct Runway {
	char * id1 = new char[4];
	float lon1;
	float lat1;
	float E1;
	float N1;
	char * id2 = new char[4];
	float lon2;
	float lat2;
	float E2;
	float N2;

	SurfaceType surface;
	float width;
};
struct TaxiwayNode {
	float lon;
	float E;
	float lat;
	float N;
	NodeUse use;
};
struct Taxiway {
	char * id = new char[16];;
	int startNode;
	int endNode;
	Category cat;
	bool twoway;
};
struct Gate {
	char * id = new char[16];
	float lon;
	float lat;
	float hdg;
	GateType type;
};

class Airport
{
public:
	Airport();
	~Airport();

	bool loadAirport(char * path, char * icao);

	void getData(std::map<int, TaxiwayNode> & n, std::list<Taxiway*> & e, std::list<Gate*> & g, std::list<Runway*> & rw);
	void getBounds(float &minX, float &minY, float &maxX, float &maxY);
	int getUTMZone();

	std::map<int, TaxiwayNode> & getNodes();
	std::list<Taxiway*> & getEdges();
	std::list<Gate*> & getGates();
	std::list<Runway*> & getRunways();


private:
	char * icaoCode = new char[5];
	std::map<int, TaxiwayNode> nodes;
	std::list<Taxiway*> edges;
	std::list<Gate*> gates;
	std::list<Runway*> runways;
	
	int UTMZone = 0;
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float maxX = -FLT_MAX;
	float maxY = -FLT_MAX;
};

