#include "XPLMDefs.h"
#include "XPLMDisplay.h"
#include "XPLMDataAccess.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"
#include "XPLMPlanes.h"
#include "XPLMPlugin.h"
#include "XPLMProcessing.h"

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include <string>

#include "Display.h"
#include "Button.h"
#include "CircularProgressGauge.h"
#include "PFD.h"
#include "Container.h"
#include "TextBox.h"
#include "TextSimple.h"
#include "TextSimpleData.h"
#include "AirportMap.h"
#include "NavRose.h"
#include "Rect.h"

static int render(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon);
static Display * displays[4];
static Container * pages[10];

std::map<GLchar, Character> * font_AirbusMCDUa = new std::map<GLchar, Character>;
std::map<GLchar, Character> * font_AirbusPFD = new std::map<GLchar, Character>;

char* pluginPath = new char[255];
char* resPath = new char[255];

float * N1_1;
float * N1_2;

void setupContainer();

XPLMDataRef DR_n1;
XPLMDataRef DR_n2;
XPLMDataRef DR_egt;
XPLMDataRef DR_fuelFlow;
XPLMDataRef DR_oilPress;
XPLMDataRef DR_oilTemp;

XPLMDataRef DR_fuelTank;

XPLMDataRef DR_pitch_pilot;
XPLMDataRef DR_hdg_pilot;
XPLMDataRef DR_roll_pilot;
XPLMDataRef DR_cas_pilot;
XPLMDataRef DR_acc_pilot;
XPLMDataRef DR_alt_pilot;
XPLMDataRef DR_radio_alt_pilot;
XPLMDataRef DR_vs_pilot;

XPLMDataRef DR_pitch_copilot;
XPLMDataRef DR_hdg_copilot;
XPLMDataRef DR_roll_copilot;
XPLMDataRef DR_cas_copilot;
XPLMDataRef DR_acc_copilot;
XPLMDataRef DR_alt_copilot;
XPLMDataRef DR_radio_alt_copilot;
XPLMDataRef DR_vs_copilot;

XPLMDataRef DR_lat;
XPLMDataRef DR_lon;

float n1_data[2];
float n2_data[2];
float egt_data[2];
float fuelFlow_data[2];
float oilPress_data[2];
float oilTemp_data[2];

float fuelTank_data[3];
float totalFuel;

float hdg_data[2];
float pitch_data[2];
float roll_data[2];
float cas_data[2];
float acc_data[2];
float alt_data[2];
float radioAlt_data[2];
float vs_data[2];
float gps_data_lat[1];
float gps_data_lon[1];

#if IBM
#include <windows.h>
BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#endif

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc)
{
	strcpy(outName, "AvionicsManager Test");
	strcpy(outSig, "camp.aircraft.avionics");
	strcpy(outDesc, "This Plugin manages the displays in the cockpit of my CSeries/A220.");

	XPLMRegisterDrawCallback(render, xplm_Phase_Gauges, 1, NULL);
	XPLMGetPluginInfo(XPLMGetMyID(), NULL, pluginPath, NULL, NULL);

	int i; //Last position in the char* pluginPath
		   //Delete the last entries to "move up" in the directory (7 for "win.xpl", 1 for "\", and 3 for "x64")
	for (i = 0; i < 255; i++) {
		if (pluginPath[i] == 0) break;
	}
	for (int j = i; i > j - 1 - 7 - 3; i--) {
		pluginPath[i] = 0;
	}
	strcpy(resPath, pluginPath);
	strcat(resPath, "res\\");
	strcpy(Utils::resourcePath, resPath);

	char* tmpPath = new char[255];
	strcpy(tmpPath, resPath);
	strcat(tmpPath, "ECAMa.ttf");
	Utils::LoadFont(tmpPath, 32, *font_AirbusMCDUa);
	Utils::testFont = font_AirbusMCDUa;

	strcpy(tmpPath, resPath);
	strcat(tmpPath, "AirbusPFD.ttf");
	Utils::LoadFont(tmpPath, 32, *font_AirbusPFD);

	GLint tx_button;
	strcpy(tmpPath, resPath);
	strcat(tmpPath, "Button_default.png");
	Utils::LoadTexturePNG(&tx_button, tmpPath);

	displays[0] = new Display(0, 1 - 564.0f / 2048, 768.0f / 2048, 564.0f / 2048, 768, 564);
	displays[1] = new Display(768.0f / 2048, 1 - 564.0f / 2048, 768.0f / 2048, 564.0f / 2048, 768, 564);
	displays[2] = new Display(768.0f / 2048, 1 - 564.0f / 1024, 768.0f / 2048, 564.0f / 2048, 768, 564);
	displays[3] = new Display(0, 1 - 564.0f / 1024, 768.0f / 2048, 564.0f / 2048, 768, 564);

	DR_n1 = XPLMFindDataRef("sim/cockpit2/engine/indicators/N1_percent");
	DR_n2 = XPLMFindDataRef("sim/cockpit2/engine/indicators/N2_percent");
	DR_egt = XPLMFindDataRef("sim/cockpit2/engine/indicators/EGT_deg_C");
	DR_fuelFlow = XPLMFindDataRef("sim/cockpit2/engine/indicators/fuel_flow_kg_sec");
	DR_oilPress = XPLMFindDataRef("sim/cockpit2/engine/indicators/oil_pressure_psi");
	DR_oilTemp = XPLMFindDataRef("sim/cockpit2/engine/indicators/oil_temperature_deg_C");

	DR_fuelTank = XPLMFindDataRef("sim/cockpit2/fuel/fuel_quantity");

	DR_hdg_pilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/heading_AHARS_deg_mag_pilot");
	DR_pitch_pilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/pitch_AHARS_deg_pilot");
	DR_roll_pilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/roll_AHARS_deg_pilot");
	DR_cas_pilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/airspeed_kts_pilot");
	DR_acc_pilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/airspeed_acceleration_kts_sec_pilot");
	DR_alt_pilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/altitude_ft_pilot");
	DR_vs_pilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/vvi_fpm_pilot");
	DR_radio_alt_pilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/radio_altimeter_height_ft_pilot");

	DR_hdg_copilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/heading_AHARS_deg_mag_copilot");
	DR_pitch_copilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/pitch_AHARS_deg_copilot");
	DR_roll_copilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/roll_AHARS_deg_copilot");
	DR_cas_copilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/airspeed_kts_copilot");
	DR_acc_copilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/airspeed_acceleration_kts_sec_copilot");
	DR_alt_copilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/altitude_ft_copilot");
	DR_vs_copilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/vvi_fpm_copilot");
	DR_radio_alt_copilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/radio_altimeter_height_ft_copilot");

	DR_lat = XPLMFindDataRef("sim/flightmodel/position/latitude");
	DR_lon = XPLMFindDataRef("sim/flightmodel/position/longitude");

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//PFD Pilot:
	int pfdGyro_width = 768;
	int pfdGyro_height = 352;

	PFDGyro * gyro_pilot = new PFDGyro();
	gyro_pilot->setBounds(0, 0, pfdGyro_width, pfdGyro_height);
	gyro_pilot->setFont(font_AirbusPFD);
	gyro_pilot->setDataSources(&(pitch_data[0]), &(roll_data[0]), &(hdg_data[0]));

	PFDIndicatorCAS * cas_pilot = new PFDIndicatorCAS();
	cas_pilot->setBounds((int)(0.17f * pfdGyro_width), (int)(0.2f * pfdGyro_height), (int)(0.1f * pfdGyro_width), (int)(0.6f * pfdGyro_height));
	cas_pilot->setFont(font_AirbusPFD);
	cas_pilot->setDataSources(&(cas_data[0]), &(acc_data[0]));

	PFDIndicatorAltitude * alt_pilot = new PFDIndicatorAltitude();
	alt_pilot->setBounds((int)(0.73f * pfdGyro_width), (int)(0.2f * pfdGyro_height), (int)(0.1f * pfdGyro_width), (int)(0.6f * pfdGyro_height));
	alt_pilot->setFont(font_AirbusPFD);
	alt_pilot->setDataSources(&(alt_data[0]), &(radioAlt_data[0]), &(vs_data[0]));

	PFD * pfd_pilot = new PFD();
	pfd_pilot->setBounds(0, 212, 768, 352);
	pfd_pilot->gyro = gyro_pilot;
	pfd_pilot->speedIndicator = cas_pilot;
	pfd_pilot->altIndicator = alt_pilot;

	displays[0]->addElement(pfd_pilot);

	NavRose * navRosePilot = new NavRose();
	navRosePilot->setHdgData(&(hdg_data[0]));
	navRosePilot->setBounds(256, -44, 256, 256);
	navRosePilot->setFont(font_AirbusPFD);
	displays[0]->addElement(navRosePilot);

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//PFD Copilot:
	PFDGyro * gyro_copilot = new PFDGyro();
	gyro_copilot->setBounds(0, 0, pfdGyro_width, pfdGyro_height);
	gyro_copilot->setFont(font_AirbusPFD);
	gyro_copilot->setDataSources(&(pitch_data[1]), &(roll_data[1]), &(hdg_data[1]));

	PFDIndicatorCAS * cas_copilot = new PFDIndicatorCAS();
	cas_copilot->setBounds((int)(0.17f * pfdGyro_width), (int)(0.2f * pfdGyro_height), (int)(0.1f * pfdGyro_width), (int)(0.6f * pfdGyro_height));
	cas_copilot->setFont(font_AirbusPFD);
	cas_copilot->setDataSources(&(cas_data[1]), &(acc_data[1]));

	PFDIndicatorAltitude * alt_copilot = new PFDIndicatorAltitude();
	alt_copilot->setBounds((int)(0.73f * pfdGyro_width), (int)(0.2f * pfdGyro_height), (int)(0.1f * pfdGyro_width), (int)(0.6f * pfdGyro_height));
	alt_copilot->setFont(font_AirbusPFD);
	alt_copilot->setDataSources(&(alt_data[1]), &(radioAlt_data[1]), &(vs_data[1]));

	PFD * pfd_copilot = new PFD();
	pfd_copilot->setBounds(0, 212, 768, 352);
	pfd_copilot->gyro = gyro_copilot;
	pfd_copilot->speedIndicator = cas_copilot;
	pfd_copilot->altIndicator = alt_copilot;

	displays[2]->addElement(pfd_copilot);


	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//Container setup:
	setupContainer();
	
	return 1;
}
PLUGIN_API void XPluginStop(void)
{
	XPLMUnregisterDrawCallback(render, xplm_Phase_Gauges, 1, NULL);
}
PLUGIN_API void XPluginDisable(void)
{
}
PLUGIN_API int XPluginEnable(void)
{
	return 1;
}
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID	inFromWho, int inMessage, void * inParam)
{
}

int render(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon) {
	switch (inPhase)
	{
	case xplm_Phase_Gauges:
		//Load necessary datarefs:
		XPLMGetDatavf(DR_n1, n1_data, NULL, 2);
		XPLMGetDatavf(DR_n2, n2_data, NULL, 2);
		XPLMGetDatavf(DR_egt, egt_data, NULL, 2);
		XPLMGetDatavf(DR_fuelFlow, fuelFlow_data, NULL, 2);
		fuelFlow_data[0] *= 3600;
		fuelFlow_data[1] *= 3600;
		XPLMGetDatavf(DR_oilTemp, oilTemp_data, NULL, 2);
		XPLMGetDatavf(DR_oilPress, oilPress_data, NULL, 2);
		XPLMGetDatavf(DR_fuelTank, fuelTank_data, NULL, 3);
		totalFuel = fuelTank_data[0] + fuelTank_data[1] + fuelTank_data[2];

		hdg_data[0] = XPLMGetDataf(DR_hdg_pilot);
		pitch_data[0] = XPLMGetDataf(DR_pitch_pilot);
		roll_data[0] = XPLMGetDataf(DR_roll_pilot);
		cas_data[0] = XPLMGetDataf(DR_cas_pilot);
		acc_data[0] = XPLMGetDataf(DR_acc_pilot);
		alt_data[0] = XPLMGetDataf(DR_alt_pilot);
		vs_data[0] = XPLMGetDataf(DR_vs_pilot);
		radioAlt_data[0] = XPLMGetDataf(DR_radio_alt_pilot);

		hdg_data[1] = XPLMGetDataf(DR_hdg_copilot);
		pitch_data[1] = XPLMGetDataf(DR_pitch_copilot);
		roll_data[1] = XPLMGetDataf(DR_roll_copilot);
		cas_data[1] = XPLMGetDataf(DR_cas_copilot);
		acc_data[1] = XPLMGetDataf(DR_acc_copilot);
		alt_data[1] = XPLMGetDataf(DR_alt_copilot);
		vs_data[1] = XPLMGetDataf(DR_vs_copilot);
		radioAlt_data[1] = XPLMGetDataf(DR_radio_alt_copilot);

		gps_data_lat[0] = (float) XPLMGetDatad(DR_lat);
		gps_data_lon[0] = (float) XPLMGetDatad(DR_lon);

		XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 1, 0, 1, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glColor4f(1, 1, 1, 1);

		glBegin(GL_LINES);
			glVertex2f(0, 0.5f);
			glVertex2f(1, 0.5f);
			glVertex2f(0.5f, 0);
			glVertex2f(0.5f, 1);
		glEnd();

		for (Display * d : displays) {
			d->draw();
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		break;
	}

	return 1;
}

void setupContainer() {
	//Nav Page (index 0):
	char* tmpPath = new char[255];

	pages[0] = new Container();
	pages[0]->setBounds(0, 0, 384, 564);
	displays[1]->addElement(pages[0]);

	//XPLMDebugString("main.cpp: Loading airport data.\n");
	strcpy(tmpPath, resPath);
	//strcat(tmpPath, "ESGG_aptdat.txt");
	//strcat(tmpPath, "EDDL_aptdat.txt");
	//strcat(tmpPath, "EDDF.dat");
	strcat(tmpPath, "EHAM.dat");
	Airport * apt = new Airport();
	if (apt->loadAirport(tmpPath, "EHAM")) {
		XPLMDebugString("main.cpp: Successfully loaded Airport data for EHAM.\n");
	}

	AirportMap * map = new AirportMap();
	map->setAirport(apt);
	map->setBounds(0, 0, 384, 512);
	map->setScale(1852);
	map->setDataSource_hdg(&(hdg_data[0]));
	map->setDataSource_GPS(&(gps_data_lat[0]), &(gps_data_lon[0]));
	pages[0]->addElement(map);

	NavRose * navRoseND = new NavRose();
	navRoseND->setHdgData(&(hdg_data[0]));
	navRoseND->setBounds(0, 64, 384, 384);
	navRoseND->setFont(font_AirbusPFD);
	pages[0]->addElement(navRoseND);

	//General Info Page (index 1):
	pages[1] = new Container();
	pages[1]->setBounds(384, 0, 384, 564);
	displays[1]->addElement(pages[1]);

	//Engine Stats:
	Rect * rectEngine = new Rect();
	rectEngine->setBounds(0, 282, 192, 282);
	rectEngine->setColor3fv(Utils::COLOR_BLACK);
	rectEngine->setLineWidth(2);
	pages[1]->addElement(rectEngine);

	TextSimple * textN1 = new TextSimple(2);
	textN1->setText("N1");
	textN1->setColor(Utils::COLOR_GRAY);
	textN1->setFont(font_AirbusPFD);
	textN1->setSize(0.3f);
	textN1->setPosition(96 - textN1->getTextWidth() / 2, 474);
	pages[1]->addElement(textN1);

	TextSimple * textEGT = new TextSimple(3);
	textEGT->setText("EGT");
	textEGT->setColor(Utils::COLOR_GRAY);
	textEGT->setFont(font_AirbusPFD);
	textEGT->setSize(0.3f);
	textEGT->setPosition(96 - textEGT->getTextWidth() / 2, 398);
	pages[1]->addElement(textEGT);

	CircularProgressGauge * gauge_n1_1 = new CircularProgressGauge();
	gauge_n1_1->setBounds(6, 466, 84, 84);
	gauge_n1_1->setDataSource(&(n1_data[0]));
	gauge_n1_1->setMaximum(100);
	gauge_n1_1->setColorExceed(Utils::COLOR_ORANGE);
	gauge_n1_1->setColorCircle(Utils::COLOR_GRAY_DARK);
	gauge_n1_1->setFont(font_AirbusPFD);
	pages[1]->addElement(gauge_n1_1);

	CircularProgressGauge * gauge_n1_2 = new CircularProgressGauge();
	gauge_n1_2->setBounds(102, 466, 84, 84);
	gauge_n1_2->setDataSource(&(n1_data[1]));
	gauge_n1_2->setColorExceed(Utils::COLOR_ORANGE);
	gauge_n1_2->setColorCircle(Utils::COLOR_GRAY_DARK);
	gauge_n1_2->setMaximum(100);
	gauge_n1_2->setFont(font_AirbusPFD);
	pages[1]->addElement(gauge_n1_2);

	CircularProgressGauge * gauge_egt_1 = new CircularProgressGauge();
	gauge_egt_1->setBounds(6, 390, 84, 84);
	gauge_egt_1->setDataSource(&(egt_data[0]));
	gauge_egt_1->setColorCircle(Utils::COLOR_GRAY_DARK);
	gauge_egt_1->setMaximum(1100);
	gauge_egt_1->configureMarker(-(float)M_PI * 1.125f, 0, Utils::COLOR_YELLOW);
	gauge_egt_1->setFont(font_AirbusPFD);
	gauge_egt_1->setNumberFormat(4, 0);
	pages[1]->addElement(gauge_egt_1);

	CircularProgressGauge * gauge_egt_2 = new CircularProgressGauge();
	gauge_egt_2->setBounds(102, 390, 84, 84);
	gauge_egt_2->setDataSource(&(egt_data[1]));
	gauge_egt_2->setColorCircle(Utils::COLOR_GRAY_DARK);
	gauge_egt_2->setMaximum(1100);
	gauge_egt_2->configureMarker(-(float)M_PI * 1.125f, 0, Utils::COLOR_YELLOW);
	gauge_egt_2->setFont(font_AirbusPFD);
	gauge_egt_2->setNumberFormat(4, 0);
	pages[1]->addElement(gauge_egt_2);

	TextSimple * textN2 = new TextSimple();
	textN2->setText("N2");
	textN2->setColor(Utils::COLOR_GRAY);
	textN2->setFont(font_AirbusPFD);
	textN2->setSize(0.3f);
	textN2->setPosition(96 - textN2->getTextWidth() / 2, 370);
	pages[1]->addElement(textN2);

	TextSimpleData * textN2_1 = new TextSimpleData();
	textN2_1->setPosition(32, 370);
	textN2_1->setDataSource(&(n2_data[0]));
	textN2_1->alignRight = false;
	textN2_1->setSize(0.375f);
	textN2_1->setDecimals(1);
	textN2_1->setFont(font_AirbusPFD);
	pages[1]->addElement(textN2_1);

	TextSimpleData * textN2_2 = new TextSimpleData();
	textN2_2->setPosition(160, 370);
	textN2_2->setDataSource(&(n2_data[1]));
	textN2_2->alignRight = true;
	textN2_2->setSize(0.375f);
	textN2_2->setDecimals(1);
	textN2_2->setFont(font_AirbusPFD);
	pages[1]->addElement(textN2_2);

	TextSimple * textFF = new TextSimple();
	textFF->setText("FF (KPH)");
	textFF->setColor(Utils::COLOR_GRAY);
	textFF->setFont(font_AirbusPFD);
	textFF->setSize(0.3f);
	textFF->setPosition(96 - textFF->getTextWidth() / 2, 353);
	pages[1]->addElement(textFF);

	TextSimpleData * textFF_1 = new TextSimpleData();
	textFF_1->setPosition(32, 353);
	textFF_1->setDataSource(&(fuelFlow_data[0]));
	textFF_1->alignRight = false;
	textFF_1->setSize(0.375f);
	textFF_1->setDecimals(0);
	textFF_1->setFont(font_AirbusPFD);
	pages[1]->addElement(textFF_1);

	TextSimpleData * textFF_2 = new TextSimpleData();
	textFF_2->setPosition(160, 353);
	textFF_2->setDataSource(&(fuelFlow_data[1]));
	textFF_2->alignRight = true;
	textFF_2->setSize(0.375f);
	textFF_2->setDecimals(0);
	textFF_2->setFont(font_AirbusPFD);
	pages[1]->addElement(textFF_2);

	TextSimple * textOT = new TextSimple();
	textOT->setText("OIL TEMP");
	textOT->setColor(Utils::COLOR_GRAY);
	textOT->setFont(font_AirbusPFD);
	textOT->setSize(0.3f);
	textOT->setPosition(96 - textOT->getTextWidth() / 2, 336);
	pages[1]->addElement(textOT);

	TextSimpleData * textOT_1 = new TextSimpleData();
	textOT_1->setPosition(32, 336);
	textOT_1->setDataSource(&(oilTemp_data[0]));
	textOT_1->alignRight = false;
	textOT_1->setSize(0.375f);
	textOT_1->setDecimals(0);
	textOT_1->setFont(font_AirbusPFD);
	pages[1]->addElement(textOT_1);

	TextSimpleData * textOT_2 = new TextSimpleData();
	textOT_2->setPosition(160, 336);
	textOT_2->setDataSource(&(oilTemp_data[1]));
	textOT_2->alignRight = true;
	textOT_2->setSize(0.375f);
	textOT_2->setDecimals(0);
	textOT_2->setFont(font_AirbusPFD);
	pages[1]->addElement(textOT_2);

	TextSimple * textOP = new TextSimple();
	textOP->setText("OIL PRESS");
	textOP->setColor(Utils::COLOR_GRAY);
	textOP->setFont(font_AirbusPFD);
	textOP->setSize(0.3f);
	textOP->setPosition(96 - textOP->getTextWidth() / 2, 321);
	pages[1]->addElement(textOP);

	TextSimpleData * textOP_1 = new TextSimpleData();
	textOP_1->setPosition(32, 321);
	textOP_1->setDataSource(&(oilPress_data[0]));
	textOP_1->alignRight = false;
	textOP_1->setSize(0.375f);
	textOP_1->setDecimals(0);
	textOP_1->setFont(font_AirbusPFD);
	pages[1]->addElement(textOP_1);

	TextSimpleData * textOP_2 = new TextSimpleData();
	textOP_2->setPosition(160, 321);
	textOP_2->setDataSource(&(oilPress_data[1]));
	textOP_2->alignRight = true;
	textOP_2->setSize(0.375f);
	textOP_2->setDecimals(0);
	textOP_2->setFont(font_AirbusPFD);
	pages[1]->addElement(textOP_2);

	//Fuel info:
	Rect * rectFuel = new Rect();
	rectFuel->setBounds(0, 218, 192, 64);
	rectFuel->setColor3fv(Utils::COLOR_BLACK);
	rectFuel->setLineWidth(2);
	pages[1]->addElement(rectFuel);

	TextSimple * textFuel1 = new TextSimple();
	textFuel1->setText("TOTAL FUEL");
	textFuel1->setColor(Utils::COLOR_GRAY);
	textFuel1->setFont(font_AirbusPFD);
	textFuel1->setSize(0.425f);
	textFuel1->setPosition(14, 260);
	pages[1]->addElement(textFuel1);

	TextSimple * textFuel2 = new TextSimple();
	textFuel2->setText("(KG)");
	textFuel2->setColor(Utils::COLOR_GRAY);
	textFuel2->setFont(font_AirbusPFD);
	textFuel2->setSize(0.3f);
	textFuel2->setPosition(100, 260);
	pages[1]->addElement(textFuel2);

	Rect * rectTotalFuel = new Rect();
	rectTotalFuel->setBounds(128, 256, 58, 18);
	rectTotalFuel->setColor3fv(Utils::COLOR_BLACK);
	rectTotalFuel->setLineWidth(2);
	pages[1]->addElement(rectTotalFuel);

	TextSimpleData * textFuel_T = new TextSimpleData();
	textFuel_T->setPosition(184, 260);
	textFuel_T->setDataSource(&totalFuel);
	textFuel_T->alignRight = true;
	textFuel_T->setSize(0.425f);
	textFuel_T->setDecimals(0);
	textFuel_T->setFont(font_AirbusPFD);
	pages[1]->addElement(textFuel_T);

	TextSimpleData * textFuel_L = new TextSimpleData();
	textFuel_L->setPosition(56, 232);
	textFuel_L->setDataSource(&(fuelTank_data[0]));
	textFuel_L->alignRight = true;
	textFuel_L->setSize(0.425f);
	textFuel_L->setDecimals(0);
	textFuel_L->setFont(font_AirbusPFD);
	pages[1]->addElement(textFuel_L);

	TextSimpleData * textFuel_C = new TextSimpleData();
	textFuel_C->setPosition(116, 232);
	textFuel_C->setDataSource(&(fuelTank_data[1]));
	textFuel_C->alignRight = true;
	textFuel_C->setSize(0.425f);
	textFuel_C->setDecimals(0);
	textFuel_C->setFont(font_AirbusPFD);
	pages[1]->addElement(textFuel_C);

	TextSimpleData * textFuel_R = new TextSimpleData();
	textFuel_R->setPosition(176, 232);
	textFuel_R->setDataSource(&(fuelTank_data[2]));
	textFuel_R->alignRight = true;
	textFuel_R->setSize(0.425f);
	textFuel_R->setDecimals(0);
	textFuel_R->setFont(font_AirbusPFD);
	pages[1]->addElement(textFuel_R);

	//Warnings and status:
	Rect * rectStatus = new Rect();
	rectStatus->setBounds(192, 218, 192, 346);
	rectStatus->setColor3fv(Utils::COLOR_BLACK);
	rectStatus->setLineWidth(2);
	pages[1]->addElement(rectStatus);

	//Cabin status:
	Rect * rectCabin = new Rect();
	rectCabin->setBounds(0, 108, 192, 110);
	rectCabin->setColor3fv(Utils::COLOR_BLACK);
	rectCabin->setLineWidth(2);
	pages[1]->addElement(rectCabin);

	//Trim status:
	Rect * rectTrim = new Rect();
	rectTrim->setBounds(192, 108, 192, 110);
	rectTrim->setColor3fv(Utils::COLOR_BLACK);
	rectTrim->setLineWidth(2);
	pages[1]->addElement(rectTrim);

	//FLT CTRL (index 2):
	pages[2] = new Container();
	pages[2]->setBounds(0, 0, 384, 564);


}