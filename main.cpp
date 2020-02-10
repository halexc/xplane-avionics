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
#include "AirportMap.h"
#include "NavRose.h"

static int render(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon);
static Display * displays[4];

std::map<GLchar, Character> * font_AirbusMCDUa = new std::map<GLchar, Character>;
std::map<GLchar, Character> * font_AirbusPFD = new std::map<GLchar, Character>;

char* pluginPath = new char[255];
char* resPath = new char[255];

float * N1_1;
float * N1_2;

XPLMDataRef DR_n1;
XPLMDataRef DR_egt;

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
float egt_data[2];
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
	DR_egt = XPLMFindDataRef("sim/cockpit2/engine/indicators/EGT_deg_C");

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
	//MFD Pilot:
	//Empty page:

	//Nav Page:
	
	Container * navPage = new Container();
	navPage->setBounds(0, 0, 384, 564);
	displays[1]->addElement(navPage);
	
	//XPLMDebugString("main.cpp: Loading airport data.\n");
	strcpy(tmpPath, resPath);
	//strcat(tmpPath, "ESGG_aptdat.txt");
	strcat(tmpPath, "EDDL_aptdat.txt");
	//strcat(tmpPath, "EDDF.dat");
	Airport * apt = new Airport();
	if (apt->loadAirport(tmpPath, "EDDL")) {
		XPLMDebugString("main.cpp: Successfully loaded Airport data for EDDL.\n");
	}

	AirportMap * map = new AirportMap();
	map->setAirport(apt);
	map->setBounds(0, 0, 384, 512);
	map->setScale(1852);
	map->setDataSource_hdg(&(hdg_data[0]));
	map->setDataSource_GPS(&(gps_data_lat[0]), &(gps_data_lon[0]));
	navPage->addElement(map);

	NavRose * navRoseND = new NavRose();
	navRoseND->setHdgData(&(hdg_data[0]));
	navRoseND->setBounds(0, 64, 384, 384);
	navRoseND->setFont(font_AirbusPFD);
	navPage->addElement(navRoseND);

	//General Info Page:
	Container * infoPage = new Container();
	infoPage->setBounds(384, 0, 384, 564);
	displays[1]->addElement(infoPage);
		//Engine Stats:
	TextSimple * textN1 = new TextSimple(2);
	textN1->setText("N1");
	textN1->setFont(font_AirbusPFD);
	textN1->setSize(0.375f);
	textN1->setPosition(96 - textN1->getTextWidth()/2, 444);
	infoPage->addElement(textN1);

	TextBox * textTest = new TextBox();
	textTest->setText("TEST\nThis is a test\nfor the TextBox");
	textTest->setFont(font_AirbusMCDUa);
	textTest->setTextSize(0.375f);
	textTest->setBounds(40, 120, 256, 120);
	textTest->setBorderSize(2.0f);
	infoPage->addElement(textTest);

	TextSimple * textEGT = new TextSimple(3);
	textEGT->setText("EGT");
	textEGT->setFont(font_AirbusPFD);
	textEGT->setSize(0.375f);
	textEGT->setPosition(96 - textEGT->getTextWidth() / 2, 368);
	infoPage->addElement(textEGT);
	
	CircularProgressGauge * gauge_n1_1 = new CircularProgressGauge();
	gauge_n1_1->setBounds(6, 436, 84, 84);
	gauge_n1_1->setDataSource(&(n1_data[0]));
	gauge_n1_1->setMaximum(100);
	gauge_n1_1->setColorExceed(Utils::COLOR_ORANGE);
	gauge_n1_1->setFont(font_AirbusPFD);
	infoPage->addElement(gauge_n1_1);

	CircularProgressGauge * gauge_n1_2 = new CircularProgressGauge();
	gauge_n1_2->setBounds(102, 436, 84, 84);
	gauge_n1_2->setDataSource(&(n1_data[1]));
	gauge_n1_2->setColorExceed(Utils::COLOR_ORANGE);
	gauge_n1_2->setMaximum(100);
	gauge_n1_2->setFont(font_AirbusPFD);
	infoPage->addElement(gauge_n1_2);

	CircularProgressGauge * gauge_egt_1 = new CircularProgressGauge();
	gauge_egt_1->setBounds(6, 360, 84, 84);
	gauge_egt_1->setDataSource(&(egt_data[0]));
	gauge_egt_1->setMaximum(1100);
	gauge_egt_1->configureMarker(-(float)M_PI * 1.125f, 0, Utils::COLOR_YELLOW);
	gauge_egt_1->setFont(font_AirbusPFD);
	gauge_egt_1->setNumberFormat(4, 0);
	infoPage->addElement(gauge_egt_1);

	CircularProgressGauge * gauge_egt_2 = new CircularProgressGauge();
	gauge_egt_2->setBounds(102, 360, 84, 84);
	gauge_egt_2->setDataSource(&(egt_data[1]));
	gauge_egt_2->setMaximum(1100);
	gauge_egt_2->configureMarker(-(float)M_PI * 1.125f, 0, Utils::COLOR_YELLOW);
	gauge_egt_2->setFont(font_AirbusPFD);
	gauge_egt_2->setNumberFormat(4, 0);
	infoPage->addElement(gauge_egt_2);
	
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
		XPLMGetDatavf(DR_egt, egt_data, NULL, 2);
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