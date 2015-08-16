#include "StdAfx.h"
#include "Vector4.h"

using namespace bmu;

Vector4 Vector4::XAXIS(1.0f,0.0f,0.0f);
Vector4 Vector4::YAXIS(0.0f,1.0f,0.0f);
Vector4 Vector4::ZAXIS(0.0f,0.0f,1.0f);
Vector4 Vector4::ORIGIN(0.0f,0.0f,0.0f);

Vector4::Vector4(const Vector4 &rv)
{
	x = rv.x;
	y = rv.y;
	z = rv.z;
	w = rv.w;
}

Vector4& Vector4::operator=(const Vector4 &rv)
{
	if (this != &rv)
	{
		x = rv.x;
		y = rv.y;
		z = rv.z;
		w = rv.w;
	}
	return *this;
}

bool Vector4::operator==(const Vector4 &rv)
{
	if (x == rv.x &&
		y == rv.y &&
		z == rv.z && 
		w == rv.w)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector4::operator!=(const Vector4 &rv)
{
	return !(operator==(rv));
}

bool Vector4::operator<(const Vector4 &rv) const
{
	double norm = x * x + y * y + z * z;
	double rv_norm = rv.x * rv.x + rv.y * rv.y + rv.z * rv.z;
	return (norm < rv_norm);
}
