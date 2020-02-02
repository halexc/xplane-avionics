#pragma once
#include "DisplayElementFBO.h"
#include "PFDGyro.h"
#include "PFDIndicatorAltitude.h"
#include "PFDIndicatorCAS.h"

class PFD :
	public DisplayElementFBO
{
public:
	PFD();
	~PFD();

	void drawTex();
	void update();

	PFDGyro * gyro;
	PFDIndicatorAltitude * altIndicator;
	PFDIndicatorCAS * speedIndicator;
};

