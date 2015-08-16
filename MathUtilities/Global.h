#pragma once

// commonly used quantities
const double RADTODEG =  57.2958;	// 360 / (2 * PI)
const double PI = 3.14159265359;

// the maximum values
const double _DOUBLE_MAX = 1.7e+308;
const double _DOUBLE_MIN = -1.7e+308;
const short _SHORT_MAX = 32767;
const short _SHORT_MIN = -32768;
const int _INT_MIN = -2147483648;
const int _INT_MAX = 2147483647;

// the machine zero problem
const double EPSILON_FLOAT = 1e-2;	// float has only 6 significant digits, we thus use a conservative EPSILON value
const double EPSILON_DOUBLE = 1e-7;

inline bool IsMachineZero(double x)
{
	double abs_x = x >= 0 ? x : -x;
	return abs_x <= EPSILON_DOUBLE;
}
