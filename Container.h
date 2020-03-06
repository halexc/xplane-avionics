#pragma once
#include "DisplayElementFBO.h"
#include "Clickable.h"

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include "Utils.h"

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

#include <vector>

class Container :
	public DisplayElementFBO, public Clickable
{
public:
	/** Container()
	 *
	 *  Default constructor. Before drawing a container object, set the bounds accordingly.
	 */
	Container();
	Container(int resX, int resY);
	~Container();

	/** void drawTex()
	 *
	 *  Renders the texture by drawing all contained elements. Any DisplayElementFBO objects
	 *  have their drawTex() function called first.
	 */
	virtual void drawTex();

	/** void update()
	 *  
	 *  Additionally to calling the containers own update function, also all elements in this
	 *  container are updated.
	 */
	virtual void update();

	/** void addElement(DisplayElement * element)
	 *  
	 *  Adds a DisplayElement to the Container object. During draw() and update() callbacks,
	 *  the corresponding function of each element is called.
	 */
	void addElement(DisplayElement * element);

	/** std::vector<DisplayElement*> getElements()
	 *
	 *  Returns a vector containing all currently added elements.
	 */
	std::vector<DisplayElement*> getElements();

	virtual void onClick(XPLMMouseStatus status, float mx, float my);
	virtual void onHover(float mx, float my);

	/** void setBounds(int x, int y, int width, int height)
	 *
	 *  This function sets the bounds of a DisplayElement by setting its coordinates and 
	 *  dimensions. The coordinates should be between 0 and the x and y resolution, 
	 *  respectively, of a display. This also implicitly sets the resolution to width by height.
	 */
	void setBounds(int x, int y, int width, int height);

	/** void setBounds(int x, int y, int width, int height, int resX, int resY)
	 *
	 *  This function sets the bounds of a DisplayElement by setting its coordinates and 
	 *  dimensions. The coordinates should be between 0 and the x and y resolution, 
	 *  respectively, of a display. Use this function to explicitly set the resolution of the 
	 *  texture.
	 */
	void setBounds(int x, int y, int width, int height, int resX, int resY);

private:
	int resX, resY;

	std::vector<DisplayElement*> elements;
};

