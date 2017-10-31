/* --Animation2D.cpp-- */
#include "Animation2D.h"

using namespace std;

Animation2D::Animation2D()
{
}

Animation2D::Animation2D(float tX, float tY, float sX, float sY, float aStep, int s)
{
	translateX = tX;
	translateY = tY;
	scaleX = sX;
	scaleY = sY;
	angularStep = aStep;
	status = s;
}

Animation2D::~Animation2D()
{
}

float Animation2D::GetTranslateX()
{
	return this->translateX;
}

float Animation2D::GetTranslateY()
{
	return translateY;
}

float Animation2D::GetScaleX()
{
	return scaleX;
}

float Animation2D::GetScaleY()
{
	return scaleY;
}

float Animation2D::GetAngularStep()
{
	return angularStep;
}

int Animation2D::GetStatus()
{
	return status;
}
