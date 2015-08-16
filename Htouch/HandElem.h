#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <glut.h>
#include "Tree.h"
#include "Tokenizer.h"
#include "../MathUtilities/Matrix44.h"
#include "../MathUtilities/Matrix33.h"

class Quadric;
// Intscts stores the information of the intersections between a ray and a clipped quadric surface
class Intsct
{
public:
	Intsct(void) {}
	Intsct(Quadric* pQuadric, int nLabel, bool bIsTangent, double fDepth) : m_pQuadric(pQuadric), m_nLabel(nLabel), m_bIsTangent(bIsTangent), m_fDepth(fDepth) {}
	void Set(Quadric* pQuadric, int nLabel, bool bIsTangent, double fDepth) { m_pQuadric = pQuadric; m_nLabel = nLabel; m_bIsTangent = bIsTangent; m_fDepth = fDepth;}
	bool IsTangent(void) const {return m_bIsTangent;}
	double GetDepth(void) const {return m_fDepth;}
	int GetLabel(void) const {return m_nLabel;}
	Quadric* GetQuadric(void) { return m_pQuadric;}

private:
	double m_fDepth;
	int m_nLabel;
	bool m_bIsTangent; 
	Quadric* m_pQuadric;
};

class Quadric
{
public:
	Quadric(void) {}
	Quadric(bmu::Matrix44 q) {Q = q; m_bIsClipped = false;}
	Quadric(bmu::Matrix44 q, bmu::Matrix44 pl1, bmu::Matrix44 pl2) {Q = q; PL1 = pl1; PL2 = pl2; m_bIsClipped = true;}
	void SetModelViewMtx(GLdouble *mtx);
	virtual void Draw(GLUquadricObj *quad_obj) {}
	virtual void DrawOBB(GLUquadricObj *quad_obj);
	bool IsClipped(void) {return m_bIsClipped;}
	std::vector<Intsct> CheckIntscts(bmu::Vector3 vRay);
	void SetLabel(int nLabel) {m_nLabel = nLabel;}
	int GetLabel(void) {return m_nLabel;}
	const bmu::Vector4* GetOBB(void) const {return m_vOBBG;}
	bmu::Vector3 GetTranslate(void) const {return m_vTranslate;} 
	bmu::Matrix33 GetConic(void) const {return C;}
	virtual void PrintType(void) {}

protected:
	inline double Det22(double ax, double ay, double bx ,double by) {return ax * by - ay * bx;}
	bool InverseTransformation(bmu::Matrix44 &m, bmu::Matrix44 &n);

protected:
	bmu::Matrix44 m_mtxModelView;
	GLdouble m_pModelView[16];
	bmu::Vector3 m_vTranslate;

protected:
	bmu::Vector4 m_vOBB[8], m_vOBBG[8];
	bmu::Matrix44 Q, PL1, PL2;
	bmu::Matrix44 GQ, GPL1, GPL2;

protected:
	bmu::Matrix33 A;
	bmu::Matrix33 C;
	bmu::Vector3 b;
	double c;
	int m_nLabel;
	bool m_bIsClipped;
};

// The normalized cylinder is assumed to be located along the y axis
// with bottom at y = 0
class Cylinder : public Quadric
{
public:
	Cylinder(bmu::Vector3 p, double a, double b, double h);
	virtual void Draw(GLUquadricObj *quad_obj);
	virtual void PrintType(void) { printf("Cylinder: %d\n", m_nLabel);}

private:
	double m_fRadiusX, m_fRadiusZ, m_fHeight;
};

// The normalized cone is assumed to be located along the y axis
// with bottom at y = 0.
class Cone : public Quadric
{
public:
	Cone(bmu::Vector3 p, double rb, double rt, double h);
	virtual void Draw(GLUquadricObj *quad_obj);
	virtual void PrintType(void) { printf("Cone: %d\n", m_nLabel);}

private:
	double m_fRadiusBottom, m_fRadiusTop, m_fHeight;
};

// The normalized ellipsoid is assumed to be located at the origin
// and aligned with the x, y, z axis 
class Ellipsoid : public Quadric
{
public:
	Ellipsoid(bmu::Vector3 p, double rx, double ry, double rz);
	virtual void Draw(GLUquadricObj *quad_obj);
	virtual void PrintType(void) { printf("Ellipsoid: %d\n", m_nLabel);}

private:
	double m_fRadiusX, m_fRadiusY, m_fRadiusZ;
};

// Implementation of the normalized sphere
// center located at the origin
class Sphere : public Quadric
{
public:
	Sphere(bmu::Vector3 p, double r);
	virtual void Draw(GLUquadricObj *quad_obj);
	virtual void PrintType(void) { printf("Sphere: %d\n", m_nLabel);}

private:
	double m_fRadius;
};

// A hand element is composed of one or a few quadrics
class HandElem  
{
public:
	HandElem(std::string strName);
	HandElem(const HandElem &);				//copy construction function
	virtual ~HandElem();
	HandElem&	operator=(const HandElem &);	//= overload  
	bool		operator==(const HandElem &);		//== overload  
	bool		operator!=(const HandElem &);		//!=overload 
	
public:
	virtual void ReadInfo(Tokenizer &sfp);
	virtual void PrintInfo(void);
	virtual void Draw(GLUquadricObj *quad_obj);
	void SetLabel(int nLabel) {m_nLabel = nLabel;}
	int GetLabel(void) {return m_nLabel;}

	friend class Tree<HandElem>;
	friend class Hand;

public:
	// !! temporarily for test purpose
	// !! need revision in the future
	void SetPose(bmu::Vector3 vPose) {m_vPose = vPose; Clip3();}
	void SetOffset(bmu::Vector3 vOffset) {m_vOffset = vOffset;}
	bmu::Vector3 GetOffset(void) {return m_vOffset;}
	
public:	
	void Update(void);

public:
	bmu::Vector3 GetPose(void) { return m_vPose;}
	std::vector<Quadric *> &GetQuadrics(void) {return m_vecQuadrics;}

protected:
	std::vector<Quadric *> m_vecQuadrics;

protected:
	std::string m_strName;
	int m_nLabel;
	bmu::Vector3 m_vOffset;
	bmu::Vector3 m_vRotMin, m_vRotMax;
	bmu::Vector3 m_vPose;
	bmu::Vector3 m_vScale;
	
public:
	static std::string s_strOffset;
	static std::string s_strRotMin;
	static std::string s_strRotMax;
	static std::string s_strPose;
	static std::string s_strScale;

	static std::string s_strCylinder;
	static std::string s_strCone;
	static std::string s_strEllipsoid;
	static std::string s_strSphere;

	static std::string s_strElem;
	static std::string s_strLabel;

protected:
	inline void Clip(double min, double max, double &x);
	void Clip3(void);
};

inline void HandElem::Clip(double min, double max, double &x)
{
	// if min == max, we assume that its not initialized
	//!! this need to be modified later on
	if (min < max)
	{
		if (x < min)
			x = min;
		if (x > max)
			x = max;
	}
}