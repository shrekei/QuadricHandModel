#include "StdAfx.h"
#include "EulerAngles.h"
#include "Global.h"
#include <math.h>

//---------------the following two functions conforom to "LEFT-HANDED positive sign convention"--------------//

// for the conversion between Euler angles and rotation matrix, refer to the 
// Z-Y-X convention in "http://en.wikipedia.org/wiki/Euler_angles"
bmu::Matrix33 Euler2MatrixXYZ(bmu::Vector3 v)
{
	double s1, s2, s3, c1, c2, c3;
	s3 = sin(v.x); c3 = cos(v.x);
	s2 = sin(v.y); c2 = cos(v.y);
	s1 = sin(v.z); c1 = cos(v.z);

	bmu::Matrix33 m(c1 * c2, c1 * s2 * s3 - s1 * c3, s1 * s3 + c1 * s2 * c3,
		s1 * c2, s1 * s2 * s3 + c1 * c3, s1 * s2 * c3 - c1 * s3,
		-s2, c2 * s3, c2 * c3);
//	m.Transpose();
	return m;
}

bmu::Vector3 CalcEulerAnglesXYZ(bmu::Matrix33 m)
{
	// note here the denominator may be machine ZERO, this will cause numerical instability
	// we need to take special measures for the case FABS(m.c.x) ~ 1 and FABS(m.c.x) ~ 0
	if (!IsMachineZero(fabs(m.c.x) - 1))
	{
		double ang1[2];
		double ang2[2];
		double ang3[2];
		ang2[0] = -asin(m.c.x); 		// rotation respect to y
		double cos20 = cos(ang2[0]);

		// note that atan2 return the angle within the interval [-PI, PI]
		ang1[0] = atan2(m.c.y / cos20, m.c.z / cos20); 	// rotation respect to x
		ang3[0] = atan2(m.b.x / cos20, m.a.x / cos20);	// rotation respect to z

		ang2[1] = PI - ang2[0]; 		// rotation respect to y
		double cos21 = cos(ang2[1]);
		ang1[1] = atan2(m.c.y / cos21, m.c.z / cos21); 	// rotation respect to x
		ang3[1] = atan2(m.b.x / cos21, m.a.x / cos21);	// rotation respect to z

		// according to the properties of trigonometric function, we know that within the interval [0, 2 * PI]
		// 2. sin(theta) = sin(3 * PI - theta) for theta > PI
		// 3. sin(theta) = sin(PI - theta) for theta < PI
		double fError = _DOUBLE_MAX;
		double ang10, ang20, ang30;
		for (int i = 0; i <= 1; i++)
		{
			double sin1 = sin(ang1[i]);
			double cos1 = cos(ang1[i]);
			double sin2 = sin(ang2[i]);
			double cos2 = cos(ang2[i]);
			double sin3 = sin(ang3[i]);
			double cos3 = cos(ang3[i]);
			double m11, m12, m13,m21, m22, m23, m31, m32, m33;
			m11 = cos2 * cos3 - m.a.x;
			m12 = -cos1 * sin3 + sin1 * sin2 * cos3 - m.a.y;
			m13 = sin1 * sin3 + cos1 * sin2 * cos3 - m.a.z;
			m21 = cos2 * sin3 - m.b.x;
			m22 = cos1 * cos3 + sin1 * sin2 * sin3 - m.b.y;
			m23 = -sin1 * cos3 + cos1 * sin2 * sin3 - m.b.z;
			m31 = -sin2 - m.c.x;
			m32 = sin1 * cos2 - m.c.y;
			m33 = cos1 * cos2 - m.c.z;
			double error = fabs(m11) + fabs(m12) + fabs(m13) + fabs(m21) + fabs(m22) + fabs(m23) + fabs(m31) + fabs(m32) + fabs(m33);
			if (error < fError)
			{
				fError = error;
				ang10 = ang1[i];
				ang20 = ang2[i];
				ang30 = ang3[i];
			}
		}
		return bmu::Vector3(ang10, ang20, ang30);
	}
	else
	{
		double ang1, ang2, ang3;
		if (IsMachineZero(m.c.x - 1))
		{
			ang2 = PI / 2;
			ang1 = 0;
			ang3 = atan2(m.a.y, m.a.z); 
		}
		else if (IsMachineZero(m.c.x + 1))
		{
			ang2 = PI * 1.5;
			ang1 = 0;
			ang3 = atan2(-m.a.y,-m.a.z);
		}
		return bmu::Vector3(ang1, ang2, ang3);
	}
}

bmu::Vector3 CalcEulerAnglesXYZWithRef(bmu::Matrix33 m, 
	bmu::Vector3 vRef)
{
	if (!IsMachineZero(fabs(m.c.x) - 1))
	{
		bmu::Vector3 vPose1, vPose2;

		vPose1.y = -asin(m.c.x); 		// rotation respect to y
		double cos20 = cos(vPose1.y);

		// note that atan2 return the angle within the interval [-PI, PI]
		vPose1.x = atan2(m.c.y / cos20, m.c.z / cos20); 	// rotation respect to x
		vPose1.z = atan2(m.b.x / cos20, m.a.x / cos20);	// rotation respect to z

		vPose2.y = PI - vPose1.y; 		// rotation respect to y
		double cos21 = cos(vPose2.y);
		vPose2.x = atan2(m.c.y / cos21, m.c.z / cos21); 	// rotation respect to x
		vPose2.z = atan2(m.b.x / cos21, m.a.x / cos21);	// rotation respect to z

		return vPose1.Dist2(vRef) < vPose2.Dist2(vRef) ? vPose1 : vPose2;
	}
	else
	{
		double ang1, ang2, ang3;
		if (IsMachineZero(m.c.x - 1))
		{
			ang2 = PI / 2;
			ang1 = 0;
			ang3 = atan2(m.a.y, m.a.z); 
		}
		else if (IsMachineZero(m.c.x + 1))
		{
			ang2 = PI * 1.5;
			ang1 = 0;
			ang3 = atan2(-m.a.y,-m.a.z);
		}
		return bmu::Vector3(ang1, ang2, ang3);
	}
}
