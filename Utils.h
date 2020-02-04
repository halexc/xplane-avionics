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

	//Bright red color: R=1, G=0, B=0
	static float COLOR_RED_BRIGHT[3];

	//Red color: R=0.75, G=0, B=0
	static float COLOR_RED[3];

	//Dark red color: R=0.5625, G=0, B=0
	static float COLOR_RED_DARK[3];

	//Bright green color: R=0, G=1, B=0
	static float COLOR_GREEN_BRIGHT[3];

	//Green color: R=0, G=0.6875, B=0
	static float COLOR_GREEN[3];

	//Dark green color: R=0, G=0.375, B=0
	static float COLOR_GREEN_DARK[3];

	//Bright blue color: R=0, G= 0.625, B=1
	static float COLOR_BLUE_BRIGHT[3];

	//Blue color: R=0, G=0, B=1
	static float COLOR_BLUE[3];

	//Dark blue color: R=0, G=0, B=0.5
	static float COLOR_BLUE_DARK[3];

	//Yellow color: R=1, G=0.9375, B=0
	static float COLOR_YELLOW[3];

	//Dark yellow color: R=0.9375, G=0.75, B=0
	static float COLOR_YELLOW_DARK[3];

	//Orange color: R=1, G= 0.625, B=0
	static float COLOR_ORANGE[3];

	//Brown color: R=0.5, G=0.25, B=0
	static float COLOR_BROWN[3];

	//Dark brown color: R=0.25, G=0.125, B=0
	static float COLOR_BROWN_DARK[3];

	//Bright turquoise color: R=0, G=1, B=1
	static float COLOR_TURQUOISE_BRIGHT[3];

	//Turquoise color: R=0, G=0.8125, B=0.8125
	static float COLOR_TURQUOISE[3];

	//Teal color: R=0, G=0.5, B=0.5
	static float COLOR_TEAL[3];

	//Fuchsia color: R=1, G=0, B=1
	static float COLOR_FUCHSIA[3];

	//Purple color: R=0.6875, G=0, B=0.6875
	static float COLOR_PURPLE[3];

	//White color: R=1, G=1, B=1
	static float COLOR_WHITE[3];

	//Light gray color: R=0.75, G=0.75, B=0.75
	static float COLOR_GRAY_LIGHT[3];

	//Gray color: R=0.5, G=0.5, B=0.5
	static float COLOR_GRAY[3];

	//Dark gray color: R=0.25, G=0.25, B=0.25
	static float COLOR_GRAY_DARK[3];

	//Black color: R=0, G=0, B=0
	static float COLOR_BLACK[3];

	//Framebuffer utility functions:
	static bool InitFBO(GLuint * fb, GLint * texId, GLsizei width, GLsizei height, GLint format);

	//Font utility functions (using FreeType):

	/**	void InitFT();
	*	
	*	Initialize the shaders and framebuffer objects neccessary to load
	*	a FreeType font. This function is also automatically called upon 
	*	using the LoadFont(...) function if it has not been called before.
	*/
	static void InitFT();

	/**	void LoadFont(const char * path, int size, std::map<GLchar, Character> & Characters)
	*	
	*	Parameters:
	*	- const char * path: File path to the TTF file as c-string
	*	- int size: Size of the render target. Increasing this increase
	*	  the resolution of the drawn characters but also their size
	*	- std::map<GLchar, Character> & Characters: font target
	*
	*	Use:
	*	This function loads the TTF file at the path and generates a
	*	texture for each of the 128 ASCII characters and stores them
	*	in the '&Characters' parameter.
	*/
	static void LoadFont(const char * path, int size, std::map<GLchar, Character> & Characters);

	/** void RenderText(const char * string, const std::map<GLchar, Character> & Characters, float x, float y, float size, float color[3] = COLOR_WHITE);
	*
	*	Parameters:
	*	- const char * string: Text to be rendered (as c-string)
	*	- const std::map<GLchar, Character> & Characters: font
	*	- float x: x coordinate at which the first character is drawn
	*	  (in OpenGL space)
	*	- float y: y coordinate at which the text baseline is. Some
	*	  characters may be drawn partially under this line.
	*	  (in OpenGL space)
	*	- float size: Scaling factor
	*	- float color[3]: RGB color in which the text is drawn as 
	*	  float vector. Default is white (R=1, G=1, B=1).
	*
	*	Use:
	*	This function draws the passed string using the specified font
	*	in OpenGL space beginning at the specified position (x, y).
	*/
	static void RenderText(const char * string, const std::map<GLchar, Character> & Characters,
								float x, float y, float size, float color[3] = COLOR_WHITE);

	/** void RenderTextAngle(const char * string, const std::map<GLchar, Character> & Characters, float x, float y, float size, float angle, float color[3] = COLOR_WHITE);
	*	
	*	Parameters:
	*	- const char * string: Text to be rendered (as c-string)
	*	- const std::map<GLchar, Character> & Characters: font
	*	- float x: x coordinate at which the first character is drawn
	*	  (in OpenGL space).
	*	- float y: y coordinate at which the text baseline is (in 
	*	  OpenGL space).
	*	- float size: Scaling factor
	*	- float angle: Angle at which the text is drawn
	*	- float color[3]: RGB color in which the text is drawn as
	*	  float vector. Default is white (R=1, G=1, B=1).
	*
	*	Use:
	*	This function draws the passed string using the specified font
	*	in OpenGL space beginning at the specified position (x, y).
	*	Instead of drawing the text horizontally, the text is drawn
	*	at an angle.
	*/
	static void RenderTextAngle(const char * string, const std::map<GLchar, Character> & Characters,
								float x, float y, float size, float angle, float color[3] = COLOR_WHITE);

	/**	float GetTextWidth(const char * string, const std::map<GLchar, Character> & Characters, float scale)
	*	
	*	Parameters:
	*	- const char * string: Text to determine the OpenGL width of
	*	  (as c-string)
	*	- const std::map<GLchar, Character> & Characters: font
	*	- float scale: Scaling factor
	*
	*	Return value:
	*	- float: Text width
	*
	*	Use:
	*	This function can be used to calculate the OpenGL width of
	*	the specified string when rendered with the specified font
	*	and scale. This function comes in handy for alignment
	*	purposes.
	*/
	static float GetTextWidth(const char * string, const std::map<GLchar, Character> & Characters, float scale);

	//Texture utility functions

	/**	void LoadTexturePNG(int * inTexID, const char * path)
	*
	*	Parameters:
	*	- int * inTexID: Pointer to where the texture ID is to be
	*	stored.
	*	- const char * path: File path to the PNG file (as c-string)
	*
	*	Use:
	*	This function can be used to load a PNG file into an OpenGL
	*	texture.
	*/
	static void LoadTexturePNG(int * inTexID, const char * path);

	//Drawing assistance functions:

	/**	void DrawCircle(float r, float x, float y, float color[3], float width = 1, int segments = 32, bool smooth = true)
	*	
	*	Parameters:
	*	- float r: Cirlce radius
	*	- float x: x coordinate of the center (in OpenGL space)
	*	- float y: y coordinate of the center (in OpenGL space)
	*	- float color[3]: Color of the circle as float vector
	*	- float width: Width of the line
	*	- int segments: Number of segments to approximate the circle
	*	- bool smooth: Use anti-aliasing?
	*
	*	Use:
	*	This function draws an approximated circle by drawing 'segments'
	*	many lines.
	*/
	static void DrawCircle(float r, float x, float y, float color[3], float width = 1, int segments = 32, bool smooth = true);

	/**	void DrawPartCircle(float r, float x, float y, float startAngle, float endAngle, float color[3], float width = 1, int segments = 32, bool smooth = true)
	*
	*	Parameters:
	*	- float r: Cirlce radius
	*	- float x: x coordinate of the center (in OpenGL space)
	*	- float y: y coordinate of the center (in OpenGL space)
	*	- float startAngle: rad at which the arc starts
	*	- float endAngle: rad at which the arc ends
	*	- float color[3]: Color of the circle as float vector
	*	- float width: Width of the line
	*	- int segments: Number of segments to approximate the circle
	*	- bool smooth: Use anti-aliasing?
	*
	*	Use:
	*	This function draws an approximated arc between the specified
	*	angles. Here, the number of segments does not reflect the lines
	*	used to draw the arc but the number of segments a full circle 
	*	would have. The angles are in radians and go CCW, where 0 
	*	represents the fully right position on the circle.
	*/
	static void DrawPartCircle(float r, float x, float y, float startAngle, float endAngle, float color[3], float width = 1, int segments = 32, bool smooth = true);

	/**	void DrawCircle(float r, float x, float y, float color[3], float width = 1, int segments = 32, bool smooth = true)
	*
	*	Parameters:
	*	- float r: Cirlce radius
	*	- float x: x coordinate of the center (in OpenGL space)
	*	- float y: y coordinate of the center (in OpenGL space)
	*	- float color[3]: Color of the circle as float vector
	*	- int segments: Number of segments to approximate the circle
	*
	*	Use:
	*	This function draws an approximated circle by drawing 'segments'
	*	many triangles. The circle is thus filled.
	*/
	static void DrawCircleFilled(float r, float x, float y, float color[3], int segments = 32);

	/**	void DrawPartCircle(float r, float x, float y, float startAngle, float endAngle, float color[3], float width = 1, int segments = 32, bool smooth = true)
	*
	*	Parameters:
	*	- float r: Cirlce radius
	*	- float x: x coordinate of the center (in OpenGL space)
	*	- float y: y coordinate of the center (in OpenGL space)
	*	- float startAngle: rad at which the arc starts
	*	- float endAngle: rad at which the arc ends
	*	- float color[3]: Color of the circle as float vector
	*	- int segments: Number of segments to approximate the circle
	*
	*	Use:
	*	This function draws an approximated arc between the specified
	*	angles. Here, the number of segments does not reflect the lines
	*	used to draw the arc but the number of segments a full circle
	*	would have. The angles are in radians and go CCW, where 0
	*	represents the fully right position on the circle. The arc is
	*	filled along the center and not between the ends.
	*/
	static void DrawPartCircleFilled(float r, float x, float y, float startAngle, float endAngle, float color[3], int segments = 32);

	/** void DrawLine(float x1, float y1, float x2, float y2, float color[3] = COLOR_WHITE, float width = 1, bool smooth = true)
	*
	*	Parameters:
	*	- float x1: Starting x coordinate (in OpenGL space)
	*	- float y1: Starting y coordinate (in OpenGL space)
	*	- float x2: Ending x coordinate (in OpenGL space)
	*	- float y2: Ending y coordinate (in OpenGL space)
	*	- float color[3]: Color of the line
	*	- float width: Width of the line
	*	- bool smooth: Use anti-aliasing?
	*
	*	Use:
	*	Use this function to quickly draw a line between the start
	*	(x1, y1) to the end (x2, y2).
	*/
	static void DrawLine(float x1, float y1, float x2, float y2, float color[3] = COLOR_WHITE, float width = 1, bool smooth = true);

	/** void DrawLine(float x1, float y1, float x2, float y2, float color[3] = COLOR_WHITE, float width = 1, bool smooth = true)
	*
	*	Parameters:
	*	- float xs: x coordinates (in OpenGL space)
	*	- float ys: y coordinates (in OpenGL space)
	*	- int n: Number of vertices in xs/ys
	*	- float color[3]: Color of the line
	*	- float width: Width of the line
	*	- bool smooth: Use anti-aliasing?
	*
	*	Use:
	*	This function can be used to draw multiple lines more
	*	efficiently than by using DrawLine(...). The ends of
	*	each line should be passed in pairs of 2 such that:
	*	xs[i/2], ys[i/2] represent the starting points, and
	*	xs[i/2 + 1], ys[i/2 + 1] represent the starting points
	*	of each line for 0 <= i < n. Thus, n should be even, as
	*	the coordinates come in pairs of 2.
	*/
	static void DrawLines(float *xs, float *ys, int n, float color[3] = COLOR_WHITE, float width = 1, bool smooth = true);

	/** void DrawLine(float x1, float y1, float x2, float y2, float color[3] = COLOR_WHITE, float width = 1, bool smooth = true)
	*
	*	Parameters:
	*	- float xs: x coordinates (in OpenGL space)
	*	- float ys: y coordinates (in OpenGL space)
	*	- int n: Number of vertices in xs/ys
	*	- float color[3]: Color of the line
	*	- float width: Width of the line
	*	- bool smooth: Use anti-aliasing?
	*
	*	Use:
	*	This function can be used to draw multiple lines which
	*	are connected but do not neccessarily form a loop more
	*	efficiently than by using DrawLine(...).
	*	The lines form a path and may even form a loop if
	*	xs[0] == xs[n-1] and ys[0] == ys[n-1].
	*/
	static void DrawLinesConnected(float * xs, float * ys, int n, float color[3] = COLOR_WHITE, float width = 1, bool smooth = true);

	//Math utility functions

	/**	void calcRotatedPoint(float px, float py, float rad, float * qx, float * qy, float cx = 0.0f, float cy = 0.0f)
	*
	*	Parameters:
	*	- float px: Original x coordinate
	*	- float py: Original y coordinate
	*	- float rad: Angle to rotate by in radians
	*	- float * qx: Target to store the rotated x coordinate
	*	- float * qy: Target to store the rotated y coordinate
	*	- float cx: Center of rotation, x coordinate
	*	- float cy: Center of rotation, y coordinate
	*
	*	Use:
	*	This function can be used to rotate a point (px, py) by the
	*	angle 'rad' around the point (cx, cy).
	*/
	static void calcRotatedPoint(float px, float py, float rad, float * qx, float * qy, float cx = 0.0f, float cy = 0.0f);

	/** float calcGreatCircleDistance(float lat1, float lon1, float lat2, float lon2, float r = 6371000)
	*
	*	Parameters:
	*	- float lat1: Latitude of the origin
	*	- float lon1: Longitude of the origin
	*	- float lat2: Latitude of the target 
	*	- float lon2: Longitude of the target
	*	- float r: Radius (default: Earth radius)
	*	
	*	Return value:
	*	- float: Great circle distance
	*
	*	Use:
	*	This function calculates the great circle distance from
	*	(lat1, lon1) to (lat2, lon2) on a globe with radius 'r'
	*/
	static float calcGreatCircleDistance(float lat1, float lon1, float lat2, float lon2, float r = 6371000);

	static std::map<GLchar, Character> * testFont;
	static char * resourcePath;

private:

};

