#pragma once
#include <iostream>
#include <math.h>
#include "Vector3.h"

namespace bmu
{
class Quaternion
{
public:
	//Quaternion
	// -default constructor
	// -creates a new quaternion with all parts equal to zero
	Quaternion(void);

	//Quaternion
	// -constructor
	// -parametes : w, x, y, z elements of the quaternion
	// -creates a new quaternion based on the elements passed in
	Quaternion(double wi, double xi, double yi, double zi);

	//Quaternion
	// -constructor
	// -parameters : 4D vector
	// -creates a new quaternion based on the elements passed in
	Quaternion(double v[4]);
	
	//Quaternion
	// -constructor
	// -parameters : rotation angle, rotation axis
	// -creates a new quaternion based on the elements passed in
	Quaternion(double angle, Vector3 vAxis);

	//Quaternion
	// -copy constructor
	// -parameters : const quaternion q
	// -creates a new quaternion based on the quaternion passed in
	Quaternion(const Quaternion &q); 

#ifdef SHOEMAKE
	//Quaternion
	// -constructor
	// -parameters : yaw, pitch, and roll of an Euler angle
	// -creates a new quaternion based on the Euler elements passed in
	// -used with Shoemakes code
	Quaternion(double e[3], int order);
#endif  

	//~Quaternion
	// -default destructor
	~Quaternion();

	//operator=
	// -parameters : q1- Quaternion object
	// -return values : Quaternion
	// -when called on quaternion q2 sets q2 to be an object of  q3 
	Quaternion operator=(const Quaternion &q);

	//operator+
	// -parameters : q1 - Quaternion object
	// -return value : Quaternion 
	// -when called on quaternion q2 adds q1 + q2 and returns the sum in a new quaternion
	Quaternion operator+(const Quaternion &q);

	//operator-
	// -parameters : q1- Quaternion object
	// -return values : Quaternion 
	// -when called on q1 subtracts q1 - q2 and returns the difference as a new quaternion
	Quaternion operator-(const Quaternion &q);

	//operator*
	// -parameters : q1 - Quaternion object
	// -return values : Quaternion 
	// -when called on a quaternion q2, multiplies q2 *q1  and returns the product in a new quaternion 
	Quaternion operator*(const Quaternion &q);

	//operator/
	// -parameters : q1 and q2- Quaternion objects
	// -return values : Quaternion 
	// -divide q1 by q2 and returns the quotient as q1 
	Quaternion operator/(Quaternion &q);

	//operator+=
	// -parameters : q1- Quaternion object
	// -return values : Quaternion 
	// -when called on quaternion q3 adds q1 and q3 and returns the sum as q3 
	Quaternion& operator+=(const Quaternion &q);

	//operator-=
	// -parameters : q1- Quaternion object
	// -return values : Quaternion 
	// -when called on quaternion q3, subtracts q1 from q3 and returns the difference as q3
	Quaternion& operator-=(const Quaternion &q);

	//operator*=
	// -parameters : q1- Quaternion object
	// -return values : Quaternion 
	// -when called on quaternion q3, multiplies q3 by q1 and returns the product as q3
	Quaternion& operator*=(const Quaternion &q);

	//operator/=
	// -parameters : q1- Quaternion object
	// -return values : quaternion
	// -when called on quaternion q3, divides q3 by q1 and returns the quotient as q3
	Quaternion& operator/=(Quaternion &q);

	//operator<<
	// -parameters : ostream o, quaternion q
	// -return values :
	// -prints out a quaternion by it's components
	friend inline std::ostream& operator<<(std::ostream& output, const Quaternion &q)
	{
		output << "[" << q.w << ", " << "(" << q.x << ", " << q.y << ", " << q.z << ")]";
		return output; 
	}

	//operator!=
	// -parameters : q1 and q2- Quaternion objects
	// -return value : bool
	// -determines if q1 and q2 and equal
	bool operator!=(const Quaternion &q);

	//operator==
	// -parameters : q1 and q2- Quaternion objects
	// -return value : bool
	// -determines if q1 and q2 and equal
	bool operator == (const Quaternion &q);  

	//other methods: norm, inverse, conjugate, toEuler

	//norm
	// -parameters : none
	// -return value : double
	// -when called on a quaternion object q, returns the norm of q
	double Norm();

	//magnitude
	// -parameters : none
	// -return value : double
	// -when called on a quaternion object q, returns the magnitude q
	double Magnitude();

	//scale
	// -parameters :  s- a value to scale q1 by
	// -return value: quaternion
	// -returns the original quaternion with each part, w,x,y,z, multiplied by some scalar s
	Quaternion Scale(double s);

	//inverse
	// -parameters : none
	// -return value : quaternion
	// -when called on a quaternion object q, returns the inverse of q
	Quaternion Inverse();

	//conjugate
	// -parameters : none
	// -return value : quaternion
	// -when called on a quaternion object q, returns the conjugate of q
	Quaternion Conjugate();

	//UnitQuaternion
	// -parameters : none
	// -return value : quaternion
	// -when called on quaterion q, takes q and returns the unit quaternion of q
	Quaternion UnitQuaternion();

	// -parameters : 3D vector of type double
	// -return value : void
	// -when given a  3D vector, v, rotates v by the quaternion
	void Rotate(Vector3 &v);

#ifdef SHOEMAKE
	// -parameters : empty 3D vector, rotation order
	// -return : void
	// - converts this quaternion into Euler angles
	void toEuler(double e[3], int order);
#endif

private:
	// [w, (x, y, z)]
	double w, x, y, z;
};

}