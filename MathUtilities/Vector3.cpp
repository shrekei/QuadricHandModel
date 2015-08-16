#include "StdAfx.h"
#include "Vector3.h"

using namespace bmu;

Vector3 Vector3::XAXIS(1.0f,0.0f,0.0f);
Vector3 Vector3::YAXIS(0.0f,1.0f,0.0f);
Vector3 Vector3::ZAXIS(0.0f,0.0f,1.0f);
Vector3 Vector3::ORIGIN(0.0f,0.0f,0.0f);

Vector3::Vector3(const Vector3 &rv)
{
	x = rv.x;
	y = rv.y;
	z = rv.z;
}

Vector3& Vector3::operator=(const Vector3 &rv)
{
	if (this != &rv)
	{
		x = rv.x;
		y = rv.y;
		z = rv.z;
	}
	return *this;
}

bool Vector3::operator==(const Vector3 &rv)
{
	if (x == rv.x &&
		y == rv.y &&
		z == rv.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Vector3::operator!=(const Vector3 &rv)
{
	return !(operator==(rv));
}


bool Vector3::operator<(const Vector3 &rv) const
{
	double norm = x * x + y * y + z * z;
	double rv_norm = rv.x * rv.x + rv.y * rv.y + rv.z * rv.z;
	return (norm < rv_norm);
}
