#pragma once
#include "Vector3.h"

namespace bmu
{
class Matrix33
{
public:
	Matrix33()										{Identity();}
	~Matrix33()										{}
	Matrix33(double ax,double ay,double az,
		double bx,double by,double bz,
		double cx,double cy,double cz);
	Matrix33(Vector3 a0, Vector3 b0, Vector3 c0) : a(a0), b(b0), c(c0) {}
	Matrix33(const Matrix33 &rm) : a(rm.a), b(rm.b), c(rm.c) {}
	Matrix33& operator=(const Matrix33 &);
	
	void Identity();
	
	// Dot
	void Dot(const Matrix33 &m,const Matrix33 &n);	// this = m (dot) n
	
	// Transform
	void Transform(const Vector3 &in,Vector3 &out) const;
	
	// MakeRotate (NOTE: t is an angle in RADIANS)
	void MakeRotateX(double t);
	void MakeRotateY(double t);
	void MakeRotateZ(double t);
	void MakeRotateUnitAxis(const Vector3 &v,double t);	// v must be normalized
	
	// Scale
	void MakeScale(double x,double y,double z);
	void MakeScale(const Vector3 &v)				{MakeScale(v.x,v.y,v.z);}
	void MakeScale(double s)							{MakeScale(s,s,s);}
	
	// Euler angles
	enum {EULER_XYZ,EULER_XZY,EULER_YXZ,EULER_YZX,EULER_ZXY,EULER_ZYX};
	void FromEulers(double ang1, double ang2, double ang3, int order);
	void ToEulers(double &ang1, double &ang2, double &ang3, int order);
	
	// Inversion
	bool Inverse();									// Full inverse (expensive)
	void FastInverse();								// Only works on ORTHONORMAL matrices
	void Transpose();								// Only modifies 3x3 portion (doesn't change d)
		
	// Misc functions
	double Determinant() const;
	void Print(const char *s=0) const;
	Vector3 &operator[](int i)						{return *((Vector3*)&(((double*)this)[i*3]));}	// yuck!
	operator double*()								{return (double*)this;}
	
	// Static matrices
	static Matrix33 IDENTITY;
	
public:
	Vector3 a;
	Vector3 b; 
	Vector3 c;
};
}

inline bmu::Matrix33 operator+(const bmu::Matrix33& lv, const bmu::Matrix33& rv)
{
	return bmu::Matrix33(lv.a + rv.a, lv.b + rv.b, lv.c + rv.c);
}

inline bmu::Matrix33 operator-(const bmu::Matrix33& lv, const bmu::Matrix33& rv)
{
	return bmu::Matrix33(lv.a - rv.a, lv.b - rv.b, lv.c - rv.c);
}

inline bmu::Matrix33 operator*(const double scale, const bmu::Matrix33& rv)
{
	return bmu::Matrix33(scale * rv.a, scale * rv.b, scale * rv.c);
}

inline bmu::Matrix33 operator*(const bmu::Matrix33& lv, const double scale)
{
	return bmu::Matrix33(scale * lv.a, scale * lv.b, scale * lv.c);
}

inline bmu::Matrix33 operator*(const bmu::Matrix33 &m, const bmu::Matrix33 &n)
{
	bmu::Vector3 a, b, c;
	double ax = m.a.x * n.a.x + m.a.y * n.b.x + m.a.z * n.c.x;
	double ay = m.a.x * n.a.y + m.a.y * n.b.y + m.a.z * n.c.y;
	double az = m.a.x * n.a.z + m.a.y * n.b.z + m.a.z * n.c.z;

	double bx = m.b.x * n.a.x + m.b.y * n.b.x + m.b.z * n.c.x;
	double by = m.b.x * n.a.y + m.b.y * n.b.y + m.b.z * n.c.y;
	double bz = m.b.x * n.a.z + m.b.y * n.b.z + m.b.z * n.c.z;

	double cx = m.c.x * n.a.x + m.c.y * n.b.x + m.c.z * n.c.x;
	double cy = m.c.x * n.a.y + m.c.y * n.b.y + m.c.z * n.c.y;
	double cz = m.c.x * n.a.z + m.c.y * n.b.z + m.c.z * n.c.z;
	a.Set(ax,ay,az);
	b.Set(bx,by,bz);
	c.Set(cx,cy,cz);
	return bmu::Matrix33(a, b, c);
}