#include "StdAfx.h"
#include "Quaternion.h"

using namespace bmu;

Quaternion::Quaternion(void)
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Quaternion::Quaternion(double wi, double xi, double yi, double zi)
{
	w = wi;
	x = xi;
	y = yi;
	z = zi;
}

Quaternion::Quaternion(double v[4])
{
	w = v[0];
	x = v[1];
	y = v[2];
	z = v[3];
}

Quaternion::Quaternion(double angle, Vector3 vAxis)
{ 
	double fHalfAng = angle * 0.5f;
	double fSinHalfAng = sin(fHalfAng);

	w = cos(fHalfAng);
	x = fSinHalfAng * vAxis.x;
	y = fSinHalfAng * vAxis.y;
	z = fSinHalfAng * vAxis.z;
}

Quaternion::Quaternion(const Quaternion& q)
{
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
} 

#ifdef SHOEMAKE
Quaternion::Quaternion(double e[3], int order)
{
	EulerAngles ea;
	ea.x = e[0];
	ea.y = e[1];
	ea.z = e[2];
	ea.w = order;

	Quat q = Eul_ToQuat(ea);

	x = q.x;
	y = q.y; 
	z = q.z;
	w = q.w;
}
#endif

Quaternion::~Quaternion()
{
}

Quaternion Quaternion::operator=(const Quaternion& q)
{
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
	return (*this);
}

Quaternion Quaternion::operator+(const Quaternion& q)
{
	return Quaternion(w+q.w, x+q.x, y+q.y, z+q.z);
}

Quaternion Quaternion::operator-(const Quaternion& q)
{
	return Quaternion(w-q.w, x-q.x, y-q.y, z-q.z);
}

Quaternion Quaternion::operator*(const Quaternion& q)
{
	return Quaternion(
		w*q.w - x*q.x - y*q.y - z*q.z, 
		w*q.x + x*q.w + y*q.z - z*q.y,                          
		w*q.y + y*q.w + z*q.x - x*q.z,
		w*q.z + z*q.w + x*q.y - y*q.x);
}

Quaternion Quaternion::operator/(Quaternion& q)
{
	return ((*this) * (q.Inverse()));
}

Quaternion& Quaternion::operator+=(const Quaternion& q)
{
	w += q.w;
	x += q.x;
	y += q.y;
	z += q.z;
	return (*this);
}

Quaternion& Quaternion::operator-=(const Quaternion& q)
{
	w -= q.w;
	x -= q.x;
	y -= q.y;
	z -= q.z;
	return (*this);
}

Quaternion& Quaternion::operator*=(const Quaternion& q)
{
	double w_val = w*q.w - x*q.x - y*q.y - z*q.z;
	double x_val = w*q.x + x*q.w + y*q.z - z*q.y; 
	double y_val = w*q.y + y*q.w + z*q.x - x*q.z;
	double z_val = w*q.z + z*q.w + x*q.y - y*q.x; 

	w = w_val;
	x = x_val;
	y = y_val;
	z = z_val;

	return (*this);
}

Quaternion& Quaternion::operator /= (Quaternion& q)
{
	(*this) = (*this)*q.Inverse();
	return (*this);
}

bool Quaternion::operator!=(const Quaternion& q)
{
	return (w!=q.w || x!=q.x || y!=q.y || z!=q.z) ? true : false;
}

bool Quaternion::operator==(const Quaternion& q)
{
	return (w==q.w && x==q.x && y==q.y && z==q.z) ? true : false;
}  

double Quaternion::Norm()
{
	return (w*w + x*x + y*y + z*z);  
}

double Quaternion::Magnitude()
{
	return sqrt(Norm());
}

Quaternion Quaternion::Scale(double  s)
{
	return Quaternion(w*s, x*s, y*s, z*s);
}

Quaternion Quaternion::Inverse()
{
	return Conjugate().Scale(1 / Norm());
}

Quaternion Quaternion::Conjugate()
{
	return Quaternion(w, -x, -y, -z);
}

Quaternion Quaternion::UnitQuaternion()
{
	return (*this).Scale(1/(*this).Magnitude());
}

void Quaternion::Rotate(Vector3 &v)
{
	Quaternion  qv(0, v[0], v[1], v[2]);
	Quaternion  qm = (*this) * qv * (*this).Inverse();

	v[0] = qm.x;
	v[1] = qm.y;
	v[2] = qm.z;  
}

#ifdef SHOEMAKE
void Quaternion::toEuler(double e[3], int order)
{
	Quat q;

	q.w = 0;
	q.x = e[0];
	q.y = e[1];
	q.z = e[2];

	EulerAngles ea = Eul_FromQuat(q, order);

	w = ea.w;
	x = ea.x;
	y = ea.y;
	z = ea.z;
}
#endif 
