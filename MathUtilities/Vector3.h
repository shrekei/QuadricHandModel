#pragma once
#include <math.h>
#include <stdio.h>

namespace bmu
{
class Vector3
{
public:
	Vector3() : x(0.0), y(0.0), z(0.0)							{}
	Vector3(double x0, double y0, double z0)	: x(x0), y(y0), z(z0)		{}	
	Vector3(const Vector3 &rv);
	Vector3& operator=(const Vector3 &rv);
	bool operator==(const Vector3 &rv);	
	bool operator!=(const Vector3 &rv);		
	
	//useful for bounding box
	bool operator<(const Vector3 &rv) const;
	~Vector3()											{}

public:
	void Set(double x0, double y0, double z0)						{x=x0; y=y0; z=z0;}
	void Zero()											{x = y = z = 0.0f;}

public:
	// Algebra
	void Add(const Vector3 a)						{x += a.x; y += a.y; z += a.z; }
	void Add(const Vector3 a,const Vector3 b)			{x = a.x+b.x; y = a.y + b.y; z = a.z + b.z;}
	void Subtract(const Vector3 a)						{x -= a.x; y-=a.y; z-=a.z;}
	void Subtract(const Vector3 a,const Vector3 b)			{x = a.x - b.x; y = a.y - b.y; z = a.z - b.z;}
	void Scale(double s)								{x *= s; y *= s; z *= s;}
	void Scale(const Vector3 a,double s)					{x = a.x * s; y = a.y * s; z = a.z*s;}

	double Dot(const Vector3 a) const					{return x*a.x+y*a.y+z*a.z;}
	void Cross(const Vector3 a,const Vector3 b)			{x=a.y*b.z-a.z*b.y; y=a.z*b.x-a.x*b.z; z=a.x*b.y-a.y*b.x;}

	void Normalize()								{if (sqrtf(x*x+y*y+z*z) == 0) return; double s=1.0f/sqrtf(x*x+y*y+z*z); x*=s; y*=s; z*=s;}
	void Negate()									{x=-x; y=-y; z=-z;}

	double Mag() const								{return sqrtf(x*x+y*y+z*z);}
	double Mag2() const								{return x*x+y*y+z*z;}

	double Dist2(const Vector3 a) const					{return (x-a.x)*(x-a.x)+(y-a.y)*(y-a.y)+(z-a.z)*(z-a.z);}

	void Lerp(double t,const Vector3 a,const Vector3 b)		{Scale(a,1.0f-t); x+=b.x*t; y+=b.y*t; z+=b.z*t;}

	// Misc functions
	void Print(char *name=0) const						{if(name) printf("%s=",name); printf("{%lf,%lf,%lf}\n", x, y, z);}

	double &operator[](int i)							{return(((double*)this)[i]);}

	// Static vectors
	static Vector3 XAXIS,YAXIS,ZAXIS;
	static Vector3 ORIGIN;

public:
	double x, y, z;
};
}

inline bmu::Vector3 operator+(const bmu::Vector3& lv, const bmu::Vector3& rv)
{
	return bmu::Vector3(lv.x + rv.x, lv.y + rv.y, lv.z + rv.z);
}

inline bmu::Vector3 operator-(const bmu::Vector3& lv, const bmu::Vector3& rv)
{
	return bmu::Vector3(lv.x - rv.x, lv.y - rv.y, lv.z - rv.z);
}

inline bmu::Vector3 operator*(const double scale, const bmu::Vector3& rv)
{
	return bmu::Vector3(scale * rv.x, scale * rv.y, scale * rv.z);
}

inline bmu::Vector3 operator*(const bmu::Vector3& lv, const double scale)
{
	return bmu::Vector3(scale * lv.x, scale * lv.y, scale * lv.z);
}