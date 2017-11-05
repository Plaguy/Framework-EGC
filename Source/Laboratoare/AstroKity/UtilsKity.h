#pragma once
#include <cmath>

namespace UtilsKity
{
	float VariateFactor(float factor, float time, int status);
	int GetStatus(float factor, float min, float max, int status);
	float UniformTransformFactor(float factor1, float time, float f1min, float f1max, float f2min, float f2max);
	float MovePlayer(float factor, float time, float min, float max);

}