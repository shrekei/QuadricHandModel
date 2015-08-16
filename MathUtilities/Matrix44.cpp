// Matrix44.cpp: implementation of the Matrix44 class.

#include "Stdafx.h"
#include "Matrix44.h"
#include "Vector3.h"

using namespace bmu;

Matrix44 Matrix44::IDENTITY(1.0f,0.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,1.0f);

Matrix44::Matrix44(double ax,double ay,double az,double aw,
	double bx,double by,double bz,double bw,
	double cx,double cy,double cz,double cw,
	double dx,double dy,double dz,double dw)
{
	a.Set(ax,ay,az,aw);
	b.Set(bx,by,bz,bw);
	c.Set(cx,cy,cz,cw);
	d.Set(dx,dy,dz,dw);
}

Matrix44::Matrix44(	GLdouble m[16])
{
	a.Set(m[0],m[1],m[2],m[3]);
	b.Set(m[4],m[5],m[6],m[7]);
	c.Set(m[8],m[9],m[10],m[11]);
	d.Set(m[12],m[13],m[14],m[15]);
}
////////////////////////////////////////////////////////////////////////////////
Matrix44& Matrix44::operator=(const Matrix44 &rm)
{
	if (this != &rm)
	{
		a = rm.a;
		b = rm.b;
		c = rm.c;
		d = rm.d;
	}
	return *this;
}
////////////////////////////////////////////////////////////////////////////////
void Matrix44::Identity() {
	a.Set(1.0f, 0.0f, 0.0f, 0.0f);
	b.Set(0.0f, 1.0f, 0.0f, 0.0f);
	c.Set(0.0f, 0.0f, 1.0f, 0.0f);
	d.Set(0.0f, 0.0f, 0.0f, 1.0f);
}

void Matrix44::Zero()
{
	a.Set(0.0f, 0.0f, 0.0f, 0.0f);
	b.Set(0.0f, 0.0f, 0.0f, 0.0f);
	c.Set(0.0f, 0.0f, 0.0f, 0.0f);
	d.Set(0.0f, 0.0f, 0.0f, 0.0f);
}
////////////////////////////////////////////////////////////////////////////////
// Dot
////////////////////////////////////////////////////////////////////////////////
// we can see that each vector occupy one row
void Matrix44::Dot(const Matrix44 &m,const Matrix44 &n)
{
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
}

////////////////////////////////////////////////////////////////////////////////
// Transform
////////////////////////////////////////////////////////////////////////////////

void Matrix44::Transform(const Vector4 &in,Vector4 &out) const
{
	double x = a.x * in.x + a.y * in.y + a.z * in.z + a.w * in.w;
	double y = b.x * in.x + b.y * in.y + b.z * in.z + b.w * in.w;
	double z = c.x * in.x + c.y * in.y + c.z * in.z + c.w * in.w;
	double w = d.x * in.x + d.y * in.y + d.z * in.z + d.w * in.w;
	out.Set(x,y,z,w);
}

////////////////////////////////////////////////////////////////////////////////

void Matrix44::Transpose()
{
	double t;
	t=b.x; b.x=a.y; a.y=t;
	t=c.x; c.x=a.z; a.z=t;
	t=d.x; d.x=a.w; a.w=t;

	t=c.y; c.y=b.z; b.z=t;
	t=d.y; d.y=b.w; b.w=t;
	t=d.z; d.z=c.w; c.w=t;
}

////////////////////////////////////////////////////////////////////////////////
// Viewing
////////////////////////////////////////////////////////////////////////////////

void Matrix44::LookAt(const Vector4 &from,const Vector4 &to)
{
	d=from;
	c.Subtract(d,to);
	c.Normalize();
	a.Set(c.z,0.0f,-c.x);
	a.Normalize();			// BUG: this can blow up
	b.Cross(c,a);
}

////////////////////////////////////////////////////////////////////////////////
//!! not verified
/*
void Matrix44::PolarView(double dist,double azm,double inc,double twst) {
	FromEulers(-inc,azm,twst,EULER_ZXY);
	d.Scale(c,dist);
}*/

////////////////////////////////////////////////////////////////////////////////

void Matrix44::Print(const char *s) const 
{
	if(s) printf("%s={\n",s);
	else printf("matrix={\n");
	a.Print();
	b.Print();
	c.Print();
	d.Print();
	printf("}\n");
}

// the following fast inverse method is only valid for matrix consists of rotation and translation, the idea is based on the following three facts:
// 1. The inverse of a translation matrix is the translation matrix with the opposite signs on each of the translation components.
// 2. The inverse of a rotation matrix is the rotation matrix's transpose.
// 3. The inverse of a matrix product is the product of the inverse matrices ordered in reverse.
void Matrix44::FastInverse()
{
	Vector3 u, v, w, t;
	u.x = a.x; u.y = b.x; u.z = c.x;
	v.x = a.y; v.y = b.y; v.z = c.y;
	w.x = a.z; w.y = b.z; w.z = c.z;
	t.x = a.w; t.y = b.w; t.z = c.w;

	a.x = u.x; a.y = u.y; a.z = u.z; a.w = -1 * u.Dot(t);
	b.x = v.x; b.y = v.y; b.z = v.z; b.w = -1 * v.Dot(t);
	c.x = w.x; c.y = w.y; c.z = w.z; c.w = -1 * w.Dot(t);
	d = Vector4(0, 0, 0, 1);
}