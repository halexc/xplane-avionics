#include "Airport.h"

#include "UTM.h"

#include <fstream>
#include <string>
#include <cmath>

#include <stdio.h>
#include <iostream>
#include <sstream>

Airport::Airport()
{
}


Airport::~Airport()
{
}

bool Airport::loadAirport(char * path, char * icao)
{
	strcpy(icaoCode, icao);
	std::ifstream infile(path);
	std::string line;
	bool detected = false;
	while (infile.good() && std::getline(infile, line))
	{
		//Find the data to the corresponding airport:
		if (!detected) {
			if (!(line.at(0) == '1' && line.at(1) == ' ')) {
				continue;
			}
			else if (line.find(icao) != std::string::npos) {
				XPLMDebugString("d\n");
				detected = true;
			}
			continue;
		}
		//Once the airport header is found, parse the data
		else {
			if (line.at(0) != '1') continue;
			if (line.at(1) == ' ') {
				XPLMDebugString("e1\n");
				infile.close();
				return true;
			}
			//Parse runways:
			if (line.at(1) == '0' && line.at(2) == '0' && line.at(3) == ' ') {
				//XPLMDebugString(line.c_str());
				//XPLMDebugString("\n");

				std::stringstream ssin(line);
				std::string rw_data[26];
				int i = 0;
				while (ssin.good() && i < 26) {
					ssin >> rw_data[i];
					++i;
				}
				Runway * rw = new Runway;

				//Runway width:
				rw->width = std::stof(rw_data[1]);
				//Runway pavement:
				rw->surface = SurfaceType(std::stoi(rw_data[2]));
				//Skip shoulder pavement:
					//SurfaceType(std::stoi(rw_data[3]));
				//Skip runway smoothness:
					//std::stof(rw_data[4]);
				//Extract Runway information for each direction:

				//DIR1:
				strcpy(rw->id1, rw_data[8].c_str());
				//Latitude
				rw->lat1 = std::stof(rw_data[9]);
				//Longitude:
				rw->lon1 = std::stof(rw_data[10]);

				//DIR2:
				strcpy(rw->id2, rw_data[17].c_str());
				//Latitude
				rw->lat2 = std::stof(rw_data[18]);
				//Longitude:
				rw->lon2 = std::stof(rw_data[19]);
				float x, y;
				UTMZone = LatLonToUTMXY(rw->lat1, rw->lon1, UTMZone, x, y);
				rw->E1 = x;
				rw->N1 = y;

				maxX = fmaxf(rw->E1, maxX);
				maxY = fmaxf(rw->N1, maxY);
				minX = fminf(rw->E1, minX);
				minY = fminf(rw->N1, minY);

				UTMZone = LatLonToUTMXY(rw->lat2, rw->lon2, UTMZone, x, y);
				rw->E2 = x;
				rw->N2 = y;

				maxX = fmaxf(rw->E2, maxX);
				maxY = fmaxf(rw->N2, maxY);
				minX = fminf(rw->E2, minX);
				minY = fminf(rw->N2, minY);

				runways.push_back(rw);
				continue;
			}
			//Parse Pavement bounds:
			if (line.at(1) == '1' && line.at(2) == '0' && line.at(3) == ' ') {

				std::vector<Vector2f*> pvmt_vs;
				while (infile.good() && std::getline(infile, line)) {
					std::stringstream ssin(line);
					std::string pvmt_data[3];
					int i = 0;
					while (ssin.good() && i < 3) {
						ssin >> pvmt_data[i];
						++i;
					}

					if (line.at(1) == '1' && line.at(3) == ' ') {
						if (line.at(2) == '1' || line.at(2) == '2') {
							Vector2f * pos = new Vector2f;
							float lat = std::stof(pvmt_data[1]);
							float lon = std::stof(pvmt_data[2]);
							UTMZone = LatLonToUTMXY(lat, lon, UTMZone, pos->x, pos->y);
							/*
							XPLMDebugString("Airport.cpp: Adding vertex to pavement polygon:\n");
							XPLMDebugString("  x: ");
							XPLMDebugString(std::to_string(pos->x).c_str());
							XPLMDebugString(",  y: ");
							XPLMDebugString(std::to_string(pos->y).c_str());
							XPLMDebugString(".\n");
							*/

							pvmt_vs.push_back(pos);

							continue;
						}
						if (line.at(2) == '3' || line.at(2) == '4') {
							//Parse ending node
							Vector2f * pos = new Vector2f;
							float lat = std::stof(pvmt_data[1]);
							float lon = std::stof(pvmt_data[2]);
							UTMZone = LatLonToUTMXY(lat, lon, UTMZone, pos->x, pos->y);

							pvmt_vs.push_back(pos);
							//pvmt_vs.push_back(pvmt_vs.at(0));

							/*
							XPLMDebugString("Airport.cpp: Adding final vertex to pavement polygon:\n");
							XPLMDebugString("  x: ");
							XPLMDebugString(std::to_string(pos->x).c_str());
							XPLMDebugString(",  y: ");
							XPLMDebugString(std::to_string(pos->y).c_str());
							XPLMDebugString(".\n");

							XPLMDebugString("\n");
							XPLMDebugString("Splitting polygon with ");
							XPLMDebugString(std::to_string(pvmt_vs.size()).c_str());
							XPLMDebugString(" polygons.\n");
							*/
							for (std::vector<Vector2f*> polygon : splitPolygon(pvmt_vs)) {
								pavements.push_back(polygon);
							}
							XPLMDebugString("\n");

							break;
						}
					}
				}
			}
			//Parse Taxiway Nodes:
			if (line.at(1) == '2' && line.at(2) == '0' && line.at(3) == '1') {
				//XPLMDebugString(line.c_str());
				//XPLMDebugString("\n");
				TaxiwayNode * node = new TaxiwayNode;

				std::stringstream ssin(line);
				std::string twn_data[5];
				int i = 0;
				while (ssin.good() && i < 5) {
					ssin >> twn_data[i];
					++i;
				}

				//Latitude:
				node->lat = std::stof(twn_data[1]);
				//Longitude:
				node->lon = std::stof(twn_data[2]);
				UTMZone = LatLonToUTMXY(node->lat, node->lon, UTMZone, node->E, node->N);

				maxX = fmaxf(node->E, maxX);
				maxY = fmaxf(node->N, maxY);
				minX = fminf(node->E, minX);
				minY = fminf(node->N, minY);
				
				//TaxiwayNode use:
				NodeUse use = NodeUse::BOTH;
				if (twn_data[3].find("both") != std::string::npos) {
					NodeUse use = NodeUse::BOTH;
				}
				else if (twn_data[3].find("dest") != std::string::npos) {
					NodeUse use = NodeUse::END;
				}
				else if (twn_data[3].find("init") != std::string::npos) {
					NodeUse use = NodeUse::START;
				}
				else if (twn_data[3].find("junc") != std::string::npos) {
					NodeUse use = NodeUse::JUNC;
				}
				node->use = use;
				//TaxiwayNode ID:
				int id = std::stoi(twn_data[4]);
				
				nodes.insert(std::pair<int, TaxiwayNode>(id, *node));
				continue;
			}
			//Parse Taxiway edges:
			if (line.at(1) == '2' && line.at(2) == '0' && line.at(3) == '2') {
				//XPLMDebugString(line.c_str());
				//XPLMDebugString("\n");
				Taxiway * tw = new Taxiway;

				std::stringstream ssin(line);
				std::string tw_data[6];
				int i = 0;
				while (ssin.good() && i < 6) {
					ssin >> tw_data[i];
					++i;
				}

				//Node (start):
				tw->startNode = std::stoi(tw_data[1]);
				//Node (end):
				tw->endNode = std::stoi(tw_data[2]);
				//Taxiway direction:
				bool twoway = false;
				if (tw_data[3].find("twoway") != std::string::npos) {
					twoway = true;
				}
				tw->twoway = twoway;
				//Taxiway Type:
				Category cat = Category::A;
				if (tw_data[4].find("taxiway_F") != std::string::npos) {
					cat = Category::F;
				}
				else if (tw_data[4].find("taxiway_E") != std::string::npos) {
					cat = Category::E;
				}
				else if (tw_data[4].find("taxiway_D") != std::string::npos) {
					cat = Category::D;
				}
				else if (tw_data[4].find("taxiway_C") != std::string::npos) {
					cat = Category::C;
				}
				else if (tw_data[4].find("taxiway_B") != std::string::npos) {
					cat = Category::B;
				}
				else if (tw_data[4].find("taxiway_A") != std::string::npos) {
					cat = Category::A;
				}
				else if (tw_data[4].find("runway") != std::string::npos) {
					cat = Category::RW;
				}
				tw->cat = cat;

				//Taxiway identifier:
				strcpy(tw->id, tw_data[5].c_str());

				edges.push_back(tw);
				continue;
			}
			//Parse Gate data:
			if (line.at(1) == '3' && line.at(2) == '0' && line.at(3) == '0') {
				if (line.find("gate") == std::string::npos) continue; 
				//XPLMDebugString(line.c_str());
				//XPLMDebugString("\n");
				Gate g;

				std::stringstream ssin(line);
				std::string gate_data[7];
				int i = 0;
				while (ssin.good() && i < 7) {
					ssin >> gate_data[i];
					++i;
				}
				//XPLMDebugString("Gate Data loaded into string array.\n");

				//Latitude:
				g.lat = std::stof(gate_data[1]);
				//Longitude:
				g.lon = std::stof(gate_data[2]);
				//Hdg:
				g.hdg = std::stof(gate_data[3]);
				//Type:
				size_t sz;
				if (sz = line.find("Gate") != std::string::npos) {
					g.type = GateType::GATE;
				}
				else if (sz = line.find("Apron") != std::string::npos) {
					g.type = GateType::APRON;
				}
				else if (sz = line.find("Cargo") != std::string::npos) {
					g.type = GateType::CARGO;
				}
				line = line.substr(sz);
				//Gate ID:
					//TODO

				gates.push_back(&g);
				continue;
			}
		}
	}
	XPLMDebugString("Airport minX: ");
	XPLMDebugString(std::to_string(minX).c_str());
	XPLMDebugString(", minY: ");
	XPLMDebugString(std::to_string(minY).c_str());
	XPLMDebugString("\nAirport maxX: ");
	XPLMDebugString(std::to_string(maxX).c_str());
	XPLMDebugString(", maxY: ");
	XPLMDebugString(std::to_string(maxY).c_str());
	infile.close();
	return detected;
}

void Airport::getData(std::map<int, TaxiwayNode> & n, std::list<Taxiway*> & e, std::list<Gate*> & g, std::list<Runway*> & rw)
{
	n = nodes;
	e = edges;
	g = gates;
	rw = runways;
}

void Airport::getBounds(float &minX, float &minY, float &maxX, float &maxY)
{
	minX = this->minX;
	minY = this->minY;
	maxX = this->maxX;
	maxY = this->maxY;
}

int Airport::getUTMZone()
{
	return UTMZone;
}

std::map<int, TaxiwayNode> & Airport::getNodes()
{
	return nodes;
}

std::list<Taxiway*> & Airport::getEdges()
{
	return edges;
}

std::list<Gate*> & Airport::getGates()
{
	return gates;
}

std::list<Runway*> & Airport::getRunways()
{
	return runways;
}

std::list<std::vector<Vector2f*>>& Airport::getPavementPolygons()
{
	return pavements;
}

std::list<std::vector<Vector2f*>> Airport::splitPolygon(std::vector<Vector2f*> polygon)
{
	//TODO: Fix the polygon triangulation.

	std::list<std::vector<Vector2f*>> list = std::list<std::vector<Vector2f*>>();
	if (polygon.size() < 3) { 
		XPLMDebugString("Airport.cpp: No further split possible.\n");
		return list; 
	}
	
	std::vector<Vector2f*> tmp = std::vector<Vector2f*>();
	XPLMDebugString("Airport.cpp: Current polygon consists of ");
	XPLMDebugString(std::to_string(polygon.size()).c_str());
	XPLMDebugString(" vertices.\n");
	
	//My "Convexication" Algorithm: 
	//1.	Add Vertices as long as the connecting edge forms a convex polygon.
	//		Since by xplane apt.dat definition all vertices are ordered in CCW
	//		direction, this means that the polygon is still convex if the suc-
	//		ceeding vertex is located to the left of the vertex.
	//2.	If the successor is not on the left, we need to cut through that
	//		vertex. To find a second vertex to which we cut from here, we go
	//		find the first vertex to the left of the edge which would make
	//		the polygon concave. Here, 2 cuts are made, one from each end of
	//		aforementioned edge, resulting in a triangle and up to two new
	//		polygons.
	//3.	Recursively apply this algorithm to the new polygons.
	
	// (1)
	for (int i = 0; i < polygon.size(); i++) {
		Vector2f u, v;
		XPLMDebugString("A\n");
		u.x = polygon.at((i + 1) % polygon.size())->x - polygon.at(i)->x;
		u.y = polygon.at((i + 1) % polygon.size())->y - polygon.at(i)->y;
		v.x = polygon.at((i + 2) % polygon.size())->x - polygon.at((i + 1) % polygon.size())->x;
		v.y = polygon.at((i + 2) % polygon.size())->y - polygon.at((i + 1) % polygon.size())->y;

		float phi_u = atan2f(u.x, u.y);
		float phi_v = atan2f(v.x, v.y);
		float delta = (phi_v - phi_u) * 180 / M_PI;
		delta = fmodf(delta + 720, 360);

		if (delta < 180) {
			tmp.push_back(polygon.at(i));
		}
		else {
			XPLMDebugString("B\n");
			tmp.clear();
			tmp.push_back(polygon.at((i + 1) % polygon.size()));
			tmp.push_back(polygon.at((i + 2) % polygon.size()));
			for (int j = i + 3; j <= i + polygon.size(); j++) {
				Vector2f w;
				w.x = polygon.at(j % polygon.size())->x - polygon.at((i + 2) % polygon.size())->x;
				w.y = polygon.at(j % polygon.size())->y - polygon.at((i + 2) % polygon.size())->y;

				float phi_w = atan2f(w.x, w.y);
				float delta = (phi_w - phi_v) * 180 / M_PI;
				delta = fmodf(delta + 720, 360);
				if (delta < 180) {
					XPLMDebugString("Airport.cpp: Splitting polygon up...\n");
					tmp.push_back(polygon.at(j % polygon.size()));
					//Split:
					std::vector<Vector2f*> fwd = std::vector<Vector2f*>();
					std::vector<Vector2f*> bwd = std::vector<Vector2f*>();
					for (int k = i + 2; k <= j; k++) {
						fwd.push_back(polygon.at(k % polygon.size()));
					}
					for (int k = j % polygon.size() - polygon.size(); k <= (i + 1) % polygon.size(); k++) {
						bwd.push_back(polygon.at((k + 2 * polygon.size()) % polygon.size()));
					}

					XPLMDebugString("Airport.cpp: Polygon split up into additional polygons.\n");
					//Recursively add convex polygons:
					for (std::vector<Vector2f*> poly : splitPolygon(fwd)) {
						list.push_back(poly);
					}
					for (std::vector<Vector2f*> poly : splitPolygon(bwd)) {
						list.push_back(poly);
					}

					break;
				}
			}
		}
	}

	list.push_back(tmp);
	return list;
}
