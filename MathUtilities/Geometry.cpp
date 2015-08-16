#include "Geometry.h"
#include <math.h>
#include <stdio.h>

// Heron's formula, inputs are the side lengths of the triangle
double CalcTriangleArea(double a, double b, double c)
{
	// check whether the input lengths are legal
	if(((a + b) > c) && ((a + c) > b) && ((b + c) > a))
	{
		double p = 0.5 * (a + b + c);
		return sqrt(p*(p - a) * (p - b) * (p - c));
	}
	else
	{
		printf("Wrong side lengths!\n");
		return 0.0;
	}
}