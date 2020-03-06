#include "DisplayElementFBO.h"

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "XPLMDefs.h"
#include "XPLMDisplay.h"

#include <vector>

#pragma once
class Display
{
public:

	/** Display()
	 *
	 *  Default constructor. Bounds and resolution still have to be set before drawing.
	 */
	Display();

	/** Display(float x, float y, float width, float height)
	*
	*  Constructor. The position (x, y) denotes the lower left corner texture position on the panel 
	*  texture, bound to the interval [0,1] in both directions. The other corners are given by width 
	*  and height. The texture resolution of the display is set to width * 2048 by height * 2048 by 
	*  default. Note that this only influences the resolution of the texture bound to the Display 
	*  object's framebuffer object, not the actual panel resolution which xPlane uses.
	*/
	Display(float x, float y, float width, float height);

	/** Display(float x, float y, float width, float height, int pixelsX, int pixelsY)
	 *
	 *  Constructor. Works just as Display(float x, float y, float width, float height), with the
	 *  exception that the texture resolution is set manually using pixelsXand pixelsY. Note that 
	 *  this only influences the resolution of the texture bound to the Display object's framebuffer
	 *  object, not the actual panel resolution which xPlane uses.
	 */
	Display(float x, float y, float width, float height, int pixelsX, int pixelsY);

	~Display();

	/** void setBounds(float x, float y, float width, float height)
	 *
	 *  Manually sets the bounds of the Display object on the panel texture. Remember to set the 
	 *  bounds according to your projection matrix. This function does NOT modify the resolution.
	 */
	void setBounds(float x, float y, float width, float height);

	/** void getBounds(float * x, float * y, float * width, float * height)
	 *
	 *  Get the bounds of the display on the panel texture. If you are only interested in certain
	 *  values, pass nullpointers for the rest.
	 */
	void getBounds(float * x, float * y, float * width, float * height);

	/** void setResolution(int pixelsX, int pixelsY)
	 *
	 *  Manually sets the resolution of the framebuffer texture. This also leads to the framebuffer 
	 *  object being recreated during the next draw callback. Note that this only influences the 
	 *  resolution of the texture bound to the Display object's framebuffer object, not the actual
	 *  panel resolution which xPlane uses.
	 */
	void setResolution(int pixelsX, int pixelsY);

	/** void getResolution(int * pixelsX, int * pixelsY)
	 *
	 *  Get the resolution of the framebuffer texture. If you are only interested in certain
	 *  values, pass nullpointers for the rest.
	 */
	void getResolution(int * pixelsX, int * pixelsY);

	/** void addElement(DisplayElement* de)
	 *
	 *  Adds a DisplayElement to the Display object. During draw() and update() callbacks, the 
	 *  corresponding function of each element is called. If the object is a Clickable type, 
	 *  then also the onClick and onHover functions are called on such events.
	 */
	void addElement(DisplayElement* de);

	/** std::vector<DisplayElement*> getElements()
	 *
	 *  Returns a vector with all elements currently added to the Display.
	 */
	std::vector<DisplayElement*> getElements();

	void onClick(XPLMMouseStatus status, float mx, float my);
	void onHover(float mx, float my);

	void draw();
	void update();

private:

	float texX, texY, texW, texH;
	int resX, resY;
	
	bool initialized = false;

	GLint oldFramebuffer;
	GLuint framebuffer;
	GLint texture;

	std::vector<DisplayElement*> elements;

	void init();

};

