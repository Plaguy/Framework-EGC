/* --Utils.h--
This will deal with math that doesn't belong in the AstroKity source
and with the colision detection mechanism 
*/
#include "UtilsKity.h"

float UtilsKity::UpdateFactor(float factor, float time, int status)
{
	if (status == 0)
		factor += time;
	if (status == 1)
		factor -= time;
	return factor;
}

int UtilsKity::GetStatus(float factor, float min, float max, int status)
{
	//Returns 0 if factor is smaller than min
	//Returns 1 if factor is grater than max
	if (factor <= min)
		return 0;
	if (factor >= max)
		return 1;
	return status;
}