// Matrix44.h: interface for the Matrix44 class.
#pragma once
#include <glut.h>
#include "Vector4.h"

namespace bmu
{
class Matrix44 {
public:
	Matrix44()										{Identity();}
	Matrix44(double ax,double ay,double az,double aw,
		double bx,double by,double bz,double bw,
		double cx,double cy,double cz,double cw,
		double dx,double dy,double dz, double dw);
	Matrix44(GLdouble m[16]);
	Matrix44(Vector4 a0, Vector4 b0, Vector4 c0, Vector4 d0) : a(a0), b(b0), c(c0), d(d0) {}
	Matrix44(const Matrix44 &rm) : a(rm.a), b(rm.b), c(rm.c), d(rm.d) {}
	Matrix44& operator=(const Matrix44 &);
	
	void Identity();
	void Zero();
	
	// Matrix Operation
	void Dot(const Matrix44 &m,const Matrix44 &n);
	void Transform(const Vector4 &in,Vector4 &out) const;			
	void Transpose();
	void FastInverse();	// valid only for the transformation matrix which consists only the translation and rotation
	
	// Viewing
	void LookAt(const Vector4 &from,const Vector4 &to);
	void PolarView(double dist,double azm,double inc,double twst=0);
	
	// Misc functions
	void Print(const char *s=0) const;
	Vector4 &operator[](int i)						{return *((Vector4*)&(((double*)this)[i*4]));}	// yuck!
	operator double*()								{return (double*)this;}
	
	// Static matrices
	static Matrix44 IDENTITY;
	
public:
	Vector4 a;
	Vector4 b; 
	Vector4 c;
	Vector4 d;
};
}

inline bmu::Matrix44 operator+(const bmu::Matrix44& m, const bmu::Matrix44& n)
{
	// the method is clumsy here because of the strange definition of the "+" and "-" for the Vector4 class
	bmu::Vector4 a, b, c, d;
	double x, y, z, w;
	x = m.a.x + n.a.x; y = m.a.y + n.a.y; z = m.a.z + n.a.z ; w = m.a.w + n.a.w;
	a.Set(x, y, z, w);
	x = m.b.x + n.b.x; y = m.b.y + n.b.y; z = m.b.z + n.b.z ; w = m.b.w + n.b.w;
	b.Set(x, y, z, w);
	x = m.c.x + n.c.x; y = m.c.y + n.c.y; z = m.c.z + n.c.z ; w = m.c.w + n.c.w;
	c.Set(x, y, z, w);
	x = m.d.x + n.d.x; y = m.d.y + n.d.y; z = m.d.z + n.d.z ; w = m.d.w + n.d.w;
	d.Set(x, y, z, w);
	
	return bmu::Matrix44(a, b, c, d);
}

inline bmu::Matrix44 operator-(const bmu::Matrix44 &m, const bmu::Matrix44 &n)
{
	// the method is clumsy here because of the strange definition of the "+", "-" and "*" for the Vector4 class
	bmu::Vector4 a, b, c, d;
	double x, y, z, w;
	x = m.a.x - n.a.x; y = m.a.y - n.a.y; z = m.a.z - n.a.z ; w = m.a.w - n.a.w;
	a.Set(x, y, z, w);
	x = m.b.x - n.b.x; y = m.b.y - n.b.y; z = m.b.z - n.b.z ; w = m.b.w - n.b.w;
	b.Set(x, y, z, w);
	x = m.c.x - n.c.x; y = m.c.y - n.c.y; z = m.c.z - n.c.z ; w = m.c.w - n.c.w;
	c.Set(x, y, z, w);
	x = m.d.x - n.d.x; y = m.d.y - n.d.y; z = m.d.z - n.d.z ; w = m.d.w - n.d.w;
	d.Set(x, y, z, w);
	
	return bmu::Matrix44(a, b, c, d);
}

inline bmu::Matrix44 operator*(const double scale, const bmu::Matrix44 &m)
{
	bmu::Vector4 a, b, c, d;
	double x, y, z, w;
	x = scale * m.a.x; y = scale * m.a.y; z = scale * m.a.z ; w = scale * m.a.w;
	a.Set(x, y, z, w);
	x = scale * m.b.x; y = scale * m.b.y; z = scale * m.b.z ; w = scale * m.b.w;
	b.Set(x, y, z, w);
	x = scale * m.c.x; y = scale * m.c.y; z = scale * m.c.z ; w = scale * m.c.w;
	c.Set(x, y, z, w);
	x = scale * m.d.x; y = scale * m.d.y; z = scale * m.d.z ; w = scale * m.d.w;
	d.Set(x, y, z, w);
	return bmu::Matrix44(a, b, c, d);
}

inline bmu::Matrix44 operator*(const bmu::Matrix44 &m, const double scale)
{
	bmu::Vector4 a, b, c, d;
	double x, y, z, w;
	x = scale * m.a.x; y = scale * m.a.y; z = scale * m.a.z ; w = scale * m.a.w;
	a.Set(x, y, z, w);
	x = scale * m.b.x; y = scale * m.b.y; z = scale * m.b.z ; w = scale * m.b.w;
	b.Set(x, y, z, w);
	x = scale * m.c.x; y = scale * m.c.y; z = scale * m.c.z ; w = scale * m.c.w;
	c.Set(x, y, z, w);
	x = scale * m.d.x; y = scale * m.d.y; z = scale * m.d.z ; w = scale * m.d.w;
	d.Set(x, y, z, w);
	return bmu::Matrix44(a, b, c, d);
}

inline bmu::Matrix44 operator*(const bmu::Matrix44 &m, const bmu::Matrix44 &n)
{
	bmu::Vector4 a, b, c, d;
	double ax = m.a.x * n.a.x + m.a.y * n.b.x + m.a.z * n.c.x + m.a.w * n.d.x;
	double ay = m.a.x * n.a.y + m.a.y * n.b.y + m.a.z * n.c.y + m.a.w * n.d.y;
	double az = m.a.x * n.a.z + m.a.y * n.b.z + m.a.z * n.c.z + m.a.w * n.d.z;
	double aw = m.a.x * n.a.w + m.a.y * n.b.w + m.a.z * n.c.w + m.a.w * n.d.w;
	
	double bx = m.b.x * n.a.x + m.b.y * n.b.x + m.b.z * n.c.x + m.b.w * n.d.x;
	double by = m.b.x * n.a.y + m.b.y * n.b.y + m.b.z * n.c.y + m.b.w * n.d.y;
	double bz = m.b.x * n.a.z + m.b.y * n.b.z + m.b.z * n.c.z + m.b.w * n.d.z;
	double bw = m.b.x * n.a.w + m.b.y * n.b.w + m.b.z * n.c.w + m.b.w * n.d.w;
	
	double cx = m.c.x * n.a.x + m.c.y * n.b.x + m.c.z * n.c.x + m.c.w * n.d.x;
	double cy = m.c.x * n.a.y + m.c.y * n.b.y + m.c.z * n.c.y + m.c.w * n.d.y;
	double cz = m.c.x * n.a.z + m.c.y * n.b.z + m.c.z * n.c.z + m.c.w * n.d.z;
	double cw = m.c.x * n.a.w + m.c.y * n.b.w + m.c.z * n.c.w + m.c.w * n.d.w;
	
	double dx = m.d.x * n.a.x + m.d.y * n.b.x + m.d.z * n.c.x + m.d.w * n.d.x;
	double dy = m.d.x * n.a.y + m.d.y * n.b.y + m.d.z * n.c.y + m.d.w * n.d.y;
	double dz = m.d.x * n.a.z + m.d.y * n.b.z + m.d.z * n.c.z + m.d.w * n.d.z;
	double dw = m.d.x * n.a.w + m.d.y * n.b.w + m.d.z * n.c.w + m.d.w * n.d.w;
	
	a.Set(ax,ay,az,aw);
	b.Set(bx,by,bz,bw);
	c.Set(cx,cy,cz,cw);
	d.Set(dx,dy,dz,dw);
	return bmu::Matrix44(a, b, c, d);
}