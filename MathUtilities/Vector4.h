#pragma once

#include <math.h>
#include <stdio.h>

namespace bmu
{
class Vector4  
{
public:
	Vector4() : x(0.0), y(0.0), z(0.0), w(1.0)										{}
	Vector4(double x0, double y0, double z0)	: x(x0), y(y0), z(z0), w(1.0)			{}	
	Vector4(double x0, double y0 ,double z0, double w0)	: x(x0), y(y0), z(z0), w(w0)	{}	
	Vector4(const Vector4 &);
	Vector4& operator=(const Vector4 &);
	bool operator==(const Vector4 &);	
	bool operator!=(const Vector4 &);		
	
	//useful for bounding box
	bool operator<(const Vector4 &rv) const;
	~Vector4()											{}

public:
	void Set(double x0, double y0, double z0)						{x=x0; y=y0; z=z0; w = 1.0;}
	void Set(double x0, double y0, double z0, double w0)			{x=x0; y=y0; z=z0; w=w0;}
	void Zero()													{x = y = z = 0.0; w = 1.0;}

public:
	// Algebra
	void Add(const Vector4 a)							{x += a.x; y += a.y; z += a.z; w = 1.0;}
	void Add(const Vector4 a,const Vector4 b)			{x = a.x+b.x; y = a.y + b.y; z = a.z + b.z; w = 1.0;}
	void Subtract(const Vector4 a)						{x -= a.x; y-=a.y; z-=a.z; w = 1.0;}
	void Subtract(const Vector4 a,const Vector4 b)		{x = a.x - b.x; y = a.y - b.y; z = a.z - b.z; w = 1.0;}
	void Scale(double s)									{x *= s; y *= s; z *= s; w = 1.0; }
	void Scale(const Vector4 a,double s)					{x = a.x * s; y = a.y * s; z = a.z*s; w = 1.0;}

	double Dot(const Vector4 a) const					{return x * a.x + y * a.y + z * a.z + w * a.w;}
	//!! not verified
	void Cross(const Vector4 a,const Vector4 b)			{x=a.y*b.z-a.z*b.y; y=a.z*b.x-a.x*b.z; z=a.x*b.y-a.y*b.x;}

	void Normalize()									{double s=1.0f/sqrtf(x*x+y*y+z*z); x*=s; y*=s; z*=s;}
	void Negate()										{x=-x; y=-y; z=-z;}

	double Mag() const									{return sqrt(x*x+y*y+z*z);}
	double Mag2() const									{return x*x+y*y+z*z;}

	double Dist2(const Vector4 a) const					{return (x-a.x)*(x-a.x)+(y-a.y)*(y-a.y)+(z-a.z)*(z-a.z);}

	void Lerp(double t,const Vector4 a,const Vector4 b)	{Scale(a,1.0f-t); x+=b.x*t; y+=b.y*t; z+=b.z*t;}

	// Misc functions
	void Print(char *name=0) const						{if(name) printf("%s=",name); printf("{%lf,%lf,%lf, %lf}\n", x, y, z, w);}

	double &operator[](int i)							{return(((double*)this)[i]);}

	// Static vectors
	static Vector4 XAXIS,YAXIS,ZAXIS;
	static Vector4 ORIGIN;

public:
	double x, y, z, w;
};
}

inline bmu::Vector4 operator+(const bmu::Vector4 &lv, const bmu::Vector4 &rv)
{
	return bmu::Vector4(lv.x + rv.x, lv.y + rv.y, lv.z + rv.z, 1.0);
}

inline bmu::Vector4 operator-(const bmu::Vector4 &lv, const bmu::Vector4 &rv)
{
	return bmu::Vector4(lv.x - rv.x, lv.y - rv.y, lv.z - rv.z, 1.0);
}

inline bmu::Vector4 operator*(const double scale, const bmu::Vector4& rv)
{
	return bmu::Vector4(scale * rv.x, scale * rv.y, scale * rv.z, 1.0);
}

inline bmu::Vector4 operator*(const bmu::Vector4& lv, const double scale)
{
	return bmu::Vector4(scale * lv.x, scale * lv.y, scale * lv.z, 1.0);
}