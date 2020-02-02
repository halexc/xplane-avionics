#include "Utils.h"

#include "XPLMUtilities.h"
#include "XPLMGraphics.h"

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>
#include <cmath>

static GLuint VAO, VBO;
static int shaderProgram;
static bool initFT = false;

std::map<GLchar, Character> * Utils::testFont = new std::map<GLchar, Character>;
char * Utils::resourcePath = new char[255];

float Utils::COLOR_RED_BRIGHT[3] = { 1 , 0 , 0 };
float Utils::COLOR_RED[3] = { .75f , 0 , 0 };
float Utils::COLOR_RED_DARK[3] = { .5625f , 0 , 0 };
float Utils::COLOR_GREEN_BRIGHT[3] = { 0 , 1 , 0 };
float Utils::COLOR_GREEN[3] = { 0 , .6875f , 0 };
float Utils::COLOR_GREEN_DARK[3] = { 0 , .375f , 0 };
float Utils::COLOR_BLUE_BRIGHT[3] = { 0 , 0.625f , 1 };
float Utils::COLOR_BLUE[3] = { 0 , 0 , 1 };
float Utils::COLOR_BLUE_DARK[3] = { 0 , 0 , .5f };
float Utils::COLOR_YELLOW[3] = { 1.0f , .9375f , 0.0f };
float Utils::COLOR_YELLOW_DARK[3] = { .9375f , .75f , 0 };
float Utils::COLOR_ORANGE[3] = { 1 , .625f , 0 };
float Utils::COLOR_BROWN[3] = { .5f , .25f , 0 };
float Utils::COLOR_BROWN_DARK[3] = { .25f , .125f , 0 };
float Utils::COLOR_TURQUOISE_BRIGHT[3] = { 0 , 1 , 1 };
float Utils::COLOR_TURQUOISE[3] = { 0 , .8125f , .8125f };
float Utils::COLOR_TEAL[3] = { 0 , .5f , .5f };
float Utils::COLOR_FUCHSIA[3] = { 1 , 0 , 1 };
float Utils::COLOR_PURPLE[3] = { .6875f , 0 , .6875f };

float Utils::COLOR_WHITE[3] = { 1 , 1 , 1 };
float Utils::COLOR_GRAY_LIGHT[3] = { .75f , .75f , .75f };
float Utils::COLOR_GRAY[3] = { .5f , .5f , .5f };
float Utils::COLOR_GRAY_DARK[3] = { .25f , .25f , .25f };
float Utils::COLOR_BLACK[3] = { 0 , 0 , 0 };

//std::map<GLchar, Character> Characters;

const char * vertexShaderSource =	"#version 330 core\n"
									"layout(location = 0) in vec4 vertex;\n"
									"out vec2 TexCoords;\n"
									"uniform mat4 projection;\n"
									"void main()\n"
									"{\n"
									"    gl_Position = vec4(vertex.xy, 0, 1);\n"
									"    TexCoords = vertex.zw;\n"
									"}\n\0";

const char * fragShaderSource = "#version 330 core\n"
								"in vec2 TexCoords;\n"
								"out vec4 color;\n"
								"uniform sampler2D text;\n"
								"uniform vec3 textColor;\n"
								"void main()\n"
								"{\n"
								"    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
								"    color = vec4(textColor, 1.0)*sampled;\n"
								"}\n\0";

bool Utils::InitFBO(GLuint * fb, GLint * texId, GLsizei width, GLsizei height, GLint format)
{
	GLint framebuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);
	//Creating the render target:
	//Creating the framebuffer:
	glGenFramebuffers(1, fb);
	glBindFramebuffer(GL_FRAMEBUFFER, *fb);

	//Creating the texture:
	XPLMGenerateTextureNumbers(texId, 1);
	XPLMBindTexture2d(*texId, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
	//Filtering:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Configure framebuffer:
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *texId, 0);
	//Setup draw buffers:
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		XPLMDebugString("Utils.cpp: Error when setting up framebuffer.");
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	return true;

}

void Utils::InitFT() {
	GLenum err = glewInit();
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		XPLMDebugString("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
	}

	int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		XPLMDebugString("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		XPLMDebugString("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	XPLMDebugString("Utils: Shader program successfully constructed!\n");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	

	initFT = true;
}
void Utils::LoadFont(const char * path, int size, std::map<GLchar, Character> & Characters) {

	if (!initFT) InitFT();
	if (!&Characters) {
		XPLMDebugString("Utils: CRITICAL ERROR: FONT POINTER EMPTY");
		return;
	}
	FT_Library ft_lib;
	if (FT_Init_FreeType(&ft_lib)) {
		XPLMDebugString("Utils: Could not init FreeType library. \n");
	}
	//XPLMDebugString("ACFUtils: Library initialized... Now loading font from path: ");
	//XPLMDebugString(path);
	//XPLMDebugString(". \n");
	FT_Face ft_face;
	if (FT_New_Face(ft_lib, path, 0, &ft_face)) {
		XPLMDebugString("Utils: Failed to load FreeType font. \n");
	}
	//XPLMDebugString("ACFUtils: Font successfully loaded... Beginning rendering of glyphs now! \n");
	FT_Set_Pixel_Sizes(ft_face, 0, size);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//XPLMDebugString("ACFUtils: Pixel storing mode for unpacking set to 1. \n");

	for (GLubyte c = 0; c < 128; c++) {
		if (FT_Load_Char(ft_face, c, FT_LOAD_RENDER)) {
			XPLMDebugString("Utils: Failed to load FreeType Glyph");
		}
		//XPLMDebugString("ACFUtils: Character loaded... Generating texture accordingly. \n");
		XPLMTextureID texture;
		XPLMGenerateTextureNumbers(&texture, 1);
		XPLMBindTexture2d(texture, 0);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			ft_face->glyph->bitmap.width,
			ft_face->glyph->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			ft_face->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		Character character = {
			texture,
			ft_face->glyph->bitmap.width, ft_face->glyph->bitmap.rows,
			ft_face->glyph->bitmap_left, ft_face->glyph->bitmap_top,
			ft_face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));

		//char * msg = new char[255];
		//strcpy(msg, "Utils: Texture generated with texture number ");
		//strcat(msg, std::to_string(texture).c_str());
		//strcat(msg, "\n");
		//XPLMDebugString(msg);

	}
	//XPLMDebugString("Utils: All characters loaded and textures generated. \n");
	FT_Done_Face(ft_face);
	FT_Done_FreeType(ft_lib);
	//XPLMDebugString("ACFUtils: FreeType concluded properly. \n");
}

void Utils::RenderText(const char * string, const std::map<GLchar, Character> & Characters,
	float x, float y, float scale, float color[3])
{
	if (!&Characters) {
		XPLMDebugString("Utils: No font loaded! \n");
		return;
	}
	if (!initFT) InitFT();

	glColor4f(color[0], color[1], color[2], 1.0f);

	for (const char * i = string; *i; i++) {
		Character c = Characters.at(*i);

		GLfloat xpos = x + c.bearingX * scale;
		GLfloat ypos = y - (c.height - c.bearingY) * scale;
		GLfloat w = c.width * scale;
		GLfloat h = c.height * scale;

		XPLMBindTexture2d(c.TexID, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 1);		glVertex2f(xpos, ypos);
			glTexCoord2f(0, 0);		glVertex2f(xpos, ypos + h);
			glTexCoord2f(1, 0);		glVertex2f(xpos + w, ypos + h);
			glTexCoord2f(1, 1);		glVertex2f(xpos + w, ypos);
		glEnd();
		x += (c.advance / 64.0f) * scale;
	}

	glColor4f(1, 1, 1, 1);
}

void Utils::RenderTextAngle(const char * string, const std::map<GLchar, Character>& Characters, float x, float y, float size, float angle, float color[3])
{
	if (!&Characters) {
		XPLMDebugString("Utils: No font loaded! \n");
		return;
	}
	if (!initFT) InitFT();

	glColor4f(color[0], color[1], color[2], 1.0f);

	for (const char * i = string; *i; i++) {
		Character c = Characters.at(*i);

		GLfloat xpos = x + c.bearingX * size;
		GLfloat ypos = y - (c.height - c.bearingY) * size;
		GLfloat w = c.width * size;
		GLfloat h = c.height * size;
		float diag = sqrtf(w*w + h*h);
		float diag_a = asinf(h/diag);

		XPLMBindTexture2d(c.TexID, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 1);		glVertex2f(xpos, ypos);
			glTexCoord2f(0, 0);		glVertex2f(xpos + h * cosf(M_PI_2 + angle), ypos + h * sinf(M_PI_2 + angle));
			glTexCoord2f(1, 0);		glVertex2f(xpos + diag * cosf(diag_a + angle), ypos + diag * sinf(diag_a + angle));
			glTexCoord2f(1, 1);		glVertex2f(xpos + w * cosf(angle), ypos + w * sinf(angle));
		glEnd();

		x += cosf(angle) * (c.advance / 64.0f) * size;
		y += sinf(angle) * (c.advance / 64.0f) * size;
	}

	glColor4f(1, 1, 1, 1);
}

float Utils::GetTextWidth(const char * string, const std::map<GLchar, Character> & Characters, float scale)
{
	if (!&Characters) {
		XPLMDebugString("Utils: No font loaded! \n");
		return 0;
	}
	float res = 0;
	for (const char * i = string; *i; i++) {
		Character c = Characters.at(*i);
		res += (c.advance / 64.0f) * scale;
	}
	return res;
}

void Utils::LoadTexturePNG(int * inTexID, const char * path)
{
	XPLMGenerateTextureNumbers(inTexID, 1);
	*inTexID = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, *inTexID, SOIL_FLAG_MIPMAPS);
}

void Utils::DrawCircle(float r, float x, float y, float color[3], float width, int segments, bool smooth)
{
	GLboolean smooth_old;
	glGetBooleanv(GL_LINE_SMOOTH, &smooth_old);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	if (smooth_old != smooth)
	{
		if (smooth) {
			glEnable(GL_LINE_SMOOTH);
		}
		else glDisable(GL_LINE_SMOOTH);
	}

	GLfloat width_old;
	glGetFloatv(GL_LINE_WIDTH, &width_old);

	XPLMBindTexture2d(0, 0);
	glColor4f(color[0], color[1], color[2], 1.0f);
	glLineWidth(width);

	float xpos = x + r;
	float ypos = y;

	for (int i = 1; i <= segments; i++) {
		float nextX = x + r * std::cosf(i * (2 * (float) M_PI / segments));
		float nextY = y + r * std::sinf(i * (2 * (float) M_PI / segments));

		glBegin(GL_LINES);
			glVertex2f(xpos, ypos);
			glVertex2f(nextX, nextY);
		glEnd();

		xpos = nextX;
		ypos = nextY;
	}
	if (smooth_old != smooth) 
	{
		if (smooth_old) {
			glEnable(GL_LINE_SMOOTH);
		}
		else glDisable(GL_LINE_SMOOTH);
	}
	glLineWidth(width_old);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void Utils::DrawPartCircle(float r, float x, float y, float startAngle, float endAngle, float color[3], float width, int segments, bool smooth)
{
	GLboolean smooth_old;
	glGetBooleanv(GL_LINE_SMOOTH, &smooth_old);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	if (smooth_old != smooth)
	{
		if (smooth) {
			glEnable(GL_LINE_SMOOTH);
		}
		else glDisable(GL_LINE_SMOOTH);
	}

	GLfloat width_old;
	glGetFloatv(GL_LINE_WIDTH, &width_old);

	XPLMBindTexture2d(0, 0);
	glColor4f(color[0], color[1], color[2], 1.0f);
	glLineWidth(width);

	float xpos = x + r * std::cosf(startAngle);
	float ypos = y + r * std::sinf(startAngle);

	for (int i = 1; startAngle + i * (2 * (float) M_PI / segments) < endAngle; i++) {
		float nextX = x + r * std::cosf(startAngle + i * (2 * (float) M_PI / segments));
		float nextY = y + r * std::sinf(startAngle + i * (2 * (float) M_PI / segments));

		glBegin(GL_LINES);
			glVertex2f(xpos, ypos);
			glVertex2f(nextX, nextY);
		glEnd();

		xpos = nextX;
		ypos = nextY;
	}
	glBegin(GL_LINES);
		glVertex2f(xpos, ypos);
		glVertex2f(x + r * std::cosf(endAngle), y + r * std::sinf(endAngle));
	glEnd();

	if (smooth_old != smooth)
	{
		if (smooth_old) {
			glEnable(GL_LINE_SMOOTH);
		}
		else glDisable(GL_LINE_SMOOTH);
	}
	glLineWidth(width_old);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void Utils::DrawCircleFilled(float r, float x, float y, float color[3], int segments)
{
	XPLMBindTexture2d(0, 0);
	glColor4f(color[0], color[1], color[2], 1.0f);

	float xpos = x + r;
	float ypos = y;

	for (int i = 1; i <= segments; i++) {
		float nextX = x + r * std::cosf(i * (2 * (float)M_PI / segments));
		float nextY = y + r * std::sinf(i * (2 * (float)M_PI / segments));

		glBegin(GL_TRIANGLES);
			glVertex2f(x, y);
			glVertex2f(nextX, nextY);
			glVertex2f(xpos, ypos);
		glEnd();

		xpos = nextX;
		ypos = nextY;
	}

	glColor3f(1.0f, 1.0f, 1.0f);
}

void Utils::DrawPartCircleFilled(float r, float x, float y, float startAngle, float endAngle, float color[3], int segments)
{
	XPLMBindTexture2d(0, 0);
	glColor4f(color[0], color[1], color[2], 1.0f);

	float xpos = x + r * std::cosf(startAngle);
	float ypos = y + r * std::sinf(startAngle);

	for (int i = 1; startAngle + i * (2 * (float)M_PI / segments) < endAngle; i++) {
		float nextX = x + r * std::cosf(startAngle + i * (2 * (float)M_PI / segments));
		float nextY = y + r * std::sinf(startAngle + i * (2 * (float)M_PI / segments));

		glBegin(GL_TRIANGLES);
			glVertex2f(xpos, ypos);
			glVertex2f(x, y);
			glVertex2f(nextX, nextY);
		glEnd();

		xpos = nextX;
		ypos = nextY;
	}
	glBegin(GL_TRIANGLES);
		glVertex2f(xpos, ypos);
		glVertex2f(x, y);
		glVertex2f(x + r * std::cosf(endAngle), y + r * std::sinf(endAngle));
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
}

void Utils::DrawLine(float x1, float y1, float x2, float y2, float color[3], float width, bool smooth)
{
	XPLMBindTexture2d(0, 0);
	GLboolean smooth_old;
	glGetBooleanv(GL_LINE_SMOOTH, &smooth_old);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	if (smooth_old != smooth)
	{
		if (smooth) {
			glEnable(GL_LINE_SMOOTH);
		}
		else glDisable(GL_LINE_SMOOTH);
	}

	GLfloat width_old;
	glGetFloatv(GL_LINE_WIDTH, &width_old);

	glLineWidth(width);
	glColor4f(color[0], color[1], color[2], 1.0f);
	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();

	if (smooth_old != smooth)
	{
		if (smooth_old) {
			glEnable(GL_LINE_SMOOTH);
		}
		else glDisable(GL_LINE_SMOOTH);
	}
	glLineWidth(width_old);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void Utils::DrawLines(float * xs, float * ys, int n, float color[3], float width, bool smooth)
{
	XPLMBindTexture2d(0, 0);
	GLboolean smooth_old;
	glGetBooleanv(GL_LINE_SMOOTH, &smooth_old);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	if (smooth_old != smooth)
	{
		if (smooth) {
			glEnable(GL_LINE_SMOOTH);
		}
		else glDisable(GL_LINE_SMOOTH);
	}

	GLfloat width_old;
	glGetFloatv(GL_LINE_WIDTH, &width_old);

	glLineWidth(width);
	glColor4f(color[0], color[1], color[2], 1.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < n; i++) {
		glVertex2f(xs[i], ys[i]);
	}
	glEnd();

	if (smooth_old != smooth)
	{
		if (smooth_old) {
			glEnable(GL_LINE_SMOOTH);
		}
		else glDisable(GL_LINE_SMOOTH);
	}
	glLineWidth(width_old);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void Utils::DrawLinesConnected(float * xs, float * ys, int n, float color[3], float width, bool smooth)
{
	XPLMBindTexture2d(0, 0);
	GLboolean smooth_old;
	glGetBooleanv(GL_LINE_SMOOTH, &smooth_old);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	if (smooth_old != smooth)
	{
		if (smooth) {
			glEnable(GL_LINE_SMOOTH);
		}
		else glDisable(GL_LINE_SMOOTH);
	}

	GLfloat width_old;
	glGetFloatv(GL_LINE_WIDTH, &width_old);

	glLineWidth(width);
	glColor4f(color[0], color[1], color[2], 1.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < n; i++) {
		glVertex2f(xs[i], ys[i]);
	}
	glEnd();

	if (smooth_old != smooth)
	{
		if (smooth_old) {
			glEnable(GL_LINE_SMOOTH);
		}
		else glDisable(GL_LINE_SMOOTH);
	}
	glLineWidth(width_old);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void Utils::calcRotatedPoint(float px, float py, float rad, float * qx, float * qy, float cx, float cy)
{
	*qx = cos(rad) * (px - cx) - sin(rad) * (py - cy) + cx;
	*qy = sin(rad) * (px - cx) + cos(rad) * (py - cy) + cy;
}

float Utils::calcGreatCircleDistance(float lat1, float lon1, float lat2, float lon2, float r)
{
	float lat1_rad = lat1 * M_PI / 180;
	float lat2_rad = lat2 * M_PI / 180;
	float deltaLat_rad = (lat2 - lat1) * M_PI / 180;
	float deltaLon_rad = (lon2 - lon1) * M_PI / 180;

	float a = sinf(deltaLat_rad / 2) * sinf(deltaLat_rad / 2) +
		cosf(lat1_rad) * cosf(lat2_rad) * sinf(deltaLon_rad / 2) * sinf(deltaLon_rad / 2);

	return 2 * r * atan2f(sqrtf(a), sqrtf(1-a));
}
