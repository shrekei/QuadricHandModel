#include "stdafx.h"
#include "Matrix33.h"

using namespace bmu;

Matrix33 Matrix33::IDENTITY(1.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,0.0f,1.0f);

Matrix33::Matrix33(double ax,double ay,double az,
	double bx,double by,double bz,
	double cx,double cy,double cz)
{
	a.Set(ax,ay,az);
	b.Set(bx,by,bz);
	c.Set(cx,cy,cz);
}

////////////////////////////////////////////////////////////////////////////////
Matrix33& Matrix33::operator=(const Matrix33 &rm)
{
	if (this != &rm)
	{
		a = rm.a;
		b = rm.b;
		c = rm.c;
	}
	return *this;
}
////////////////////////////////////////////////////////////////////////////////
void Matrix33::Identity()
{
	a.Set(1.0f, 0.0f, 0.0f);
	b.Set(0.0f, 1.0f, 0.0f);
	c.Set(0.0f, 0.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////
// Dot
////////////////////////////////////////////////////////////////////////////////
// we can see that each vector occupy one row
void Matrix33::Dot(const Matrix33 &m,const Matrix33 &n)
{
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
}

////////////////////////////////////////////////////////////////////////////////
// Transform
////////////////////////////////////////////////////////////////////////////////
void Matrix33::Transform(const Vector3 &in,Vector3 &out) const
{
	double x = a.x * in.x + a.y * in.y + a.z * in.z;
	double y = b.x * in.x + b.y * in.y + b.z * in.z;
	double z = c.x * in.x + c.y * in.y + c.z * in.z;
	out.Set(x,y,z);
}

////////////////////////////////////////////////////////////////////////////////
// MakeRotate
////////////////////////////////////////////////////////////////////////////////

void Matrix33::MakeRotateX(double t)
{
	double cost=cos(t);
	double sint=sin(t);
	
	a.Set(1.0f, 0.0f, 0.0f);
	b.Set(0.0f, cost, sint);
	c.Set(0.0f,-sint, cost);
}

////////////////////////////////////////////////////////////////////////////////

void Matrix33::MakeRotateY(double t)
{
	double cost=cos(t);
	double sint=sin(t);
	
	a.Set(cost, 0.0f,-sint);
	b.Set(0.0f, 1.0f, 0.0f);
	c.Set(sint, 0.0f, cost);
}

////////////////////////////////////////////////////////////////////////////////

void Matrix33::MakeRotateZ(double t) 
{
	double cost=cos(t);
	double sint=sin(t);
	
	a.Set( cost, sint, 0.0f);
	b.Set(-sint, cost, 0.0f);
	c.Set( 0.0f, 0.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////

void Matrix33::MakeRotateUnitAxis(const Vector3 &v,double t) 
{
	double cost=cos(t);
	double sint=sin(t);
	double omc=1.0f-cost;
	
	a.x=omc*v.x*v.x+cost;
	b.y=omc*v.y*v.y+cost;
	c.z=omc*v.z*v.z+cost;
	a.y=omc*v.x*v.y+sint*v.z;
	b.x=omc*v.x*v.y-sint*v.z;
	a.z=omc*v.x*v.z-sint*v.y;
	c.x=omc*v.x*v.z+sint*v.y;
	b.z=omc*v.y*v.z+sint*v.x;
	c.y=omc*v.y*v.z-sint*v.x;
}

////////////////////////////////////////////////////////////////////////////////
// Euler angles
////////////////////////////////////////////////////////////////////////////////

void Matrix33::FromEulers(double ang1, double ang2, double ang3, int order)
{
	double sx=sin(ang1),cx=cos(ang1);
	double sy=sin(ang2),cy=cos(ang2);
	double sz=sin(ang3),cz=cos(ang3);
	
	switch(order) {
	case EULER_XYZ:
		a.Set(cy*cz,          cy*sz,         -sy);
		b.Set(sx*sy*cz-cx*sz, sx*sy*sz+cx*cz, sx*cy);
		c.Set(cx*sy*cz+sx*sz, cx*sy*sz-sx*cz, cx*cy);
		break;
	case EULER_XZY:
		a.Set( cz*cy,           sz,    -cz*sy);
		b.Set(-cx*sz*cy+sx*sy,  cx*cz,  cx*sz*sy+sx*cy);
		c.Set( sx*sz*cy+cx*sy, -sx*cz, -sx*sz*sy+cx*cy);
		break;
	case EULER_YXZ:
		a.Set( cy*cz-sy*sx*sz, cy*sz+sy*sx*cz, -sy*cx);
		b.Set(-cx*sz,          cx*cz,           sx);
		c.Set( sy*cz+cy*sx*sz, sy*sz-cy*sx*cz,  cy*cx); 
		break;
	case EULER_YZX:
		a.Set(cy*cz, cy*sz*cx+sy*sx, cy*sz*sx-sy*cx);
		b.Set(  -sz, cz*cx,          cz*sx);
		c.Set(sy*cz, sy*sz*cx-cy*sx, sy*sz*sx+cy*cx);
		break;
	case EULER_ZXY:
		a.Set( cz*cy+sz*sx*sy, sz*cx, -cz*sy+sz*sx*cy);
		b.Set(-sz*cy+cz*sx*sy, cz*cx,  sz*sy+cz*sx*cy);
		c.Set( cx*sy,         -sx,     cx*cy);
		break;
	case EULER_ZYX:
		a.Set( cz*cy, sz*cx+cz*sy*sx, sz*sx-cz*sy*cx);
		b.Set(-sz*cy, cz*cx-sz*sy*sx, cz*sx+sz*sy*cx);
		c.Set( sy,   -cy*sx,          cy*cx);
		break;
	default:
		printf("ERROR: Matrix44::FromEulers()- Bad order specified\n");
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////

void Matrix33::ToEulers(double &ang1, double &ang2, double &ang3,int order)
{
	switch(order) 
	{
	case EULER_XYZ:
		ang1 = atan2(b.z,c.z);
		ang2 = asin(-a.z);
		ang3 = atan2(a.y,a.x);
		break;
	case EULER_XZY:
		ang1 = atan2(-c.y,b.y);
		ang2 = atan2(-a.z,a.x);
		ang3 = asin(a.y);
		break;
	case EULER_YXZ:
		ang1 = asin(b.z);
		ang2 = atan2(-a.z,c.z);
		ang3 = atan2(-b.x,b.y);
		break;
	case EULER_YZX:
		ang1 = atan2(b.z,b.y);
		ang2 = atan2(c.x,a.x);
		ang3 = asin(-b.x);
		break;
	case EULER_ZXY:
		ang1 = asin(-c.y);
		ang2 = atan2(c.x,c.z);
		ang3 = atan2(a.y,b.y);
		break;
	case EULER_ZYX:
		ang1 = atan2(-c.y,c.z);
		ang2 = asin(c.x);
		ang3 = atan2(-b.x,a.x);
		break;
	default:
		printf("ERROR: Matrix33::ToEulers()- Bad order specified\n");
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Scale
////////////////////////////////////////////////////////////////////////////////

void Matrix33::MakeScale(double x,double y,double z) 
{
	a.Set(x,0.0f,0.0f);
	b.Set(0.0f,y,0.0f);
	c.Set(0.0f,0.0f,z);
}

////////////////////////////////////////////////////////////////////////////////
// Inverse, Transpose
////////////////////////////////////////////////////////////////////////////////

inline double Det22(double ax,double ay,double bx,double by) {return ax*by-ay*bx;}

bool Matrix33::Inverse()
{
	double subx, suby, subz;
	subx=Det22( b.y, b.z, c.y, c.z );
	suby=Det22( b.x, b.z, c.x, c.z );
	subz=Det22( b.x, b.y, c.x, c.y );
	
	double Det = a.x*subx - a.y*suby + a.z*subz; // Determinant
	if(Det==0.0f)
	{
		printf("ERROR: Matrix44::Inverse()- Matrix not invertible\n");
		Identity();
		return false;
	}
	Det=1.0f / Det;		// Inverse of the determinant
	
	//use companion matrix to calculate the inverse matrix
	Matrix33 Temp;
	Temp.a.x = subx*Det;
	Temp.b.x = -suby*Det;
	Temp.c.x = subz*Det;
	
	subx = Det22( a.y, a.z, c.y, c.z );
	suby = Det22( a.x, a.z, c.x, c.z );
	subz = Det22( a.x, a.y, c.x, c.y );
	
	Temp.a.y = -subx*Det;
	Temp.b.y = suby*Det;
	Temp.c.y = -subz*Det;
	
	subx = Det22( a.y, a.z, b.y, b.z );
	suby = Det22( a.x, a.z, b.x, b.z );
	subz = Det22( a.x, a.y, b.x, b.y );
	
	Temp.a.z = subx*Det;
	Temp.b.z = -suby*Det;
	Temp.c.z = subz*Det;
	
	(*this)=Temp;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//!! not verified
void Matrix33::FastInverse() {
	Matrix33 tmp;
	
	tmp.a.Set(a.x,b.x,c.x);
	tmp.b.Set(a.y,b.y,c.y);
	tmp.c.Set(a.z,b.z,c.z);
	
	(*this)=tmp;
}

////////////////////////////////////////////////////////////////////////////////

void Matrix33::Transpose() {
	double t;
	t=b.x; b.x=a.y; a.y=t;
	t=c.x; c.x=a.z; a.z=t;
	t=c.y; c.y=b.z; b.z=t;
}


////////////////////////////////////////////////////////////////////////////////
// Misc functions
////////////////////////////////////////////////////////////////////////////////

double Matrix33::Determinant() const {
	return a.x*b.y*c.z+a.y*b.z*c.x+a.z*b.x*c.y-a.x*b.z*c.y-a.y*b.x*c.z-a.z*b.y*c.x;
}

////////////////////////////////////////////////////////////////////////////////

void Matrix33::Print(const char *s) const {
	if(s) printf("%s={\n",s);
	else printf("matrix={\n");
	a.Print();
	b.Print();
	c.Print();
	printf("}\n");
}
