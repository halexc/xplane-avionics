#pragma once

#include "XPLMUtilities.h"
#include "XPLMGraphics.h"

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "SOIL2/SOIL2.h"

#include <map>

struct Character {
	XPLMTextureID TexID;
	GLuint width, height;
	GLuint bearingX, bearingY;
	GLuint advance;

};

class Utils
{
public:

	static float COLOR_RED_BRIGHT[3];
	static float COLOR_RED[3];
	static float COLOR_RED_DARK[3];
	static float COLOR_GREEN_BRIGHT[3];
	static float COLOR_GREEN[3];
	static float COLOR_GREEN_DARK[3];
	static float COLOR_BLUE_BRIGHT[3];
	static float COLOR_BLUE[3];
	static float COLOR_BLUE_DARK[3];
	static float COLOR_YELLOW[3];
	static float COLOR_YELLOW_DARK[3];
	static float COLOR_ORANGE[3];
	static float COLOR_BROWN[3];
	static float COLOR_BROWN_DARK[3];
	static float COLOR_TURQUOISE_BRIGHT[3];
	static float COLOR_TURQUOISE[3];
	static float COLOR_TEAL[3];
	static float COLOR_FUCHSIA[3];
	static float COLOR_PURPLE[3];

	static float COLOR_WHITE[3];
	static float COLOR_GRAY_LIGHT[3];
	static float COLOR_GRAY[3];
	static float COLOR_GRAY_DARK[3];
	static float COLOR_BLACK[3];

	//Framebuffer utility functions:
	static bool InitFBO(GLuint * fb, GLint * texId, GLsizei width, GLsizei height, GLint format);

	//Font utility functions (using FreeType):
	static void InitFT();
	static void LoadFont(const char * path, int size, std::map<GLchar, Character> & Characters);
	static void RenderText(const char * string, const std::map<GLchar, Character> & Characters,
								float x, float y, float size, float color[3] = COLOR_WHITE);
	static void RenderTextAngle(const char * string, const std::map<GLchar, Character> & Characters,
								float x, float y, float size, float angle, float color[3] = COLOR_WHITE);
	static float GetTextWidth(const char * string, const std::map<GLchar, Character> & Characters, float scale);
	static void LoadTexturePNG(int * inTexID, const char * path);

	//Drawing assistance functions:
	static void DrawCircle(float r, float x, float y, float color[3], float width = 1, int segments = 32, bool smooth = true);
	static void DrawPartCircle(float r, float x, float y, float startAngle, float endAngle, float color[3], float width = 1, int segments = 32, bool smooth = true);
	static void DrawCircleFilled(float r, float x, float y, float color[3], int segments = 32);
	static void DrawPartCircleFilled(float r, float x, float y, float startAngle, float endAngle, float color[3], int segments = 32);
	static void DrawLine(float x1, float y1, float x2, float y2, float color[3] = COLOR_WHITE, float width = 1, bool smooth = true);
	static void DrawLines(float *xs, float *ys, int n, float color[3] = COLOR_WHITE, float width = 1, bool smooth = true);
	static void DrawLinesConnected(float * xs, float * ys, int n, float color[3] = COLOR_WHITE, float width = 1, bool smooth = true);

	//Math utility functions
	static void calcRotatedPoint(float px, float py, float rad, float * qx, float * qy, float cx = 0.0f, float cy = 0.0f);
	static float calcGreatCircleDistance(float lat1, float lon1, float lat2, float lon2, float r = 6371000);

	static std::map<GLchar, Character> * testFont;
	static char * resourcePath;

private:

};

