#pragma once

#include "XPLMDefs.h"
#include "XPLMDisplay.h"
class Clickable
{
public:
	virtual ~Clickable() {}

	virtual void onClick(XPLMMouseStatus status, float mx, float my) = 0;
	virtual void onHover(float mx, float my) = 0;
};

