/* --Utils.h--
This will deal with math that doesn't belong in the AstroKity source
and with the colision detection mechanism 
*/
#include "UtilsKity.h"

float UtilsKity::VariateFactor(float factor, float time, int status)
{
	/*
	@param factor - transformation parameter
	@param time - transformation oscilation step
	@param status - control parameter for the way of the oscilation step
	@return factor - transformation parameter after the oscilation step 
	*/
	if (status == 0)
		factor += time;
	if (status == 1)
		factor -= time;
	return factor;
}

int UtilsKity::GetStatus(float factor, float min, float max, int status)
{
	/*
	@param factor - transformation parameter
	@param min - minimal value for the transformation parameter
	@param max - maximum value for the transformation parameter
	@param status - control parameter for the way of the oscilation step
	@return 0 if factor is smaller than min
	@return 1 if factor is grater than max
	*/
	if (factor <= min)
		return 0;
	if (factor >= max)
		return 1;
	return status;
}

float UtilsKity::UniformTransformFactor(float factor1, float time, float f1min, float f1max, \
	float f2min, float f2max)
{
	/*
	@param factor1 - transformation parameter; taken as reference
	@param time - transformation oscilation step of the transformation parameter taken as reference
	@param f1min - minimal value of the transformation parameter taken as reference
	@param f1max - maximum value of the transformation parameter taken as reference
	@param f2min - minimal value of the transformation parameter whom the uniform transformation 
	               will be applied to.
	@param f2max - maximum value of the transformation parameter whom the uniform transformation 
	               will be applied to.
	@return the transformation oscilation step of the transformation parameter whom the transformation
	        will be applied to.
	*/
	return ((time * abs(f2max - f2min)) / abs(f1max - f1min));
}

float UtilsKity::MovePlayer(float factor, float time, float min, float max)
{
	/*
	@param factor - transformation parameter
	@param time - transformation oscilation step
	@param min - minimal value of the transformation parameter
	@param max - maximum value of the transformation parameter
	*/
	if (min > max)
	{
		if (factor > min || factor < max)
			return factor;
		else
			return factor -= time;
	}
	else
	{
		if (factor > max || factor < min)
			return factor;
		else
			return factor += time;
	}
}



