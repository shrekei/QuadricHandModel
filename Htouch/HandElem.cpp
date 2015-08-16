#include "HandElem.h"
#include "global.h"
#include <cctype>
#include <algorithm>
using namespace std;

void Quadric::SetModelViewMtx(GLdouble *mtx)
{
	memcpy(m_pModelView, mtx, 16 * sizeof(GLdouble));
	bmu::Vector4 arrModelElem4[4];
	for (int i = 0; i < 4; i++)
	{
		arrModelElem4[i].x = (GLdouble)(mtx[i * 4 + 0]);
		arrModelElem4[i].y = (GLdouble)(mtx[i * 4 + 1]);
		arrModelElem4[i].z = (GLdouble)(mtx[i * 4 + 2]);
		arrModelElem4[i].w = (GLdouble)(mtx[i * 4 + 3]);
	}
	bmu::Matrix44 mtxMVL(arrModelElem4[0], arrModelElem4[1], arrModelElem4[2], arrModelElem4[3]);
	m_mtxModelView = mtxMVL;
	mtxMVL.Transpose();
	InverseTransformation(mtxMVL, mtxMVL);
	bmu::Matrix44 mtxMVR = mtxMVL;
	mtxMVL.Transpose();

	bmu::Matrix44 mtxTmp;
	mtxTmp.Dot(Q, mtxMVR);
	GQ.Dot(mtxMVL, mtxTmp);

	// update the clipping planes
	if (m_bIsClipped == true)
	{
		mtxTmp.Dot(PL1,mtxMVR);
		GPL1.Dot(mtxMVL, mtxTmp);
		mtxTmp.Dot(PL2,mtxMVR);
		GPL2.Dot(mtxMVL, mtxTmp);
	}
	
	// update the elements for occlusion test
	bmu::Vector3 v1(GQ.a.x, GQ.a.y, GQ.a.z);
	bmu::Vector3 v2(GQ.b.x, GQ.b.y, GQ.b.z);
	bmu::Vector3 v3(GQ.c.x, GQ.c.y, GQ.c.z);
	bmu::Vector3 v4(GQ.d.x, GQ.d.y, GQ.d.z);
	A = bmu::Matrix33(v1, v2, v3);
	b = v4;
	c = GQ.d.w;
	C = bmu::Matrix33(c * v1 - b.x * b, c * v2 - b.y * b, c * v3 - b.z * b);

	// update the oriented bounding box
	bmu::Matrix44 mtxModelView = m_mtxModelView;
	mtxModelView.Transpose();
	for (int i = 0; i < 8; i++)
	{
		mtxModelView.Transform(m_vOBB[i], m_vOBBG[i]);
	}
}

vector<Intsct> Quadric::CheckIntscts(bmu::Vector3 vRay)
{
	bmu::Vector3 vTmp3;
	double r1, r2;
	
	vector<Intsct> vecTmp;
	A.Transform(vRay, vTmp3);
	double det0 = (b.Dot(vRay)) * (b.Dot(vRay)) - vTmp3.Dot(vRay) * c;
	if (det0 > 0)
	{
		r1 = (-1 * 2 * b.Dot(vRay) + 2 * sqrt(det0)) / (2 * c);
		r2 = (-1 * 2 * b.Dot(vRay) - 2 * sqrt(det0)) / (2 * c);	

		// clipping plane test
		if (m_bIsClipped == true)
		{
			bmu::Vector4 vR1(vRay.x, vRay.y, 1, r1);
			bmu::Vector4 vR2(vRay.x, vRay.y, 1, r2);
			bmu::Vector4 vTmp41, vTmp42;

			GPL1.Transform(vR1, vTmp41);
			GPL2.Transform(vR1, vTmp42);
			double det1 = (vR1.Dot(vTmp41)) * (vR1.Dot(vTmp42));
			GPL1.Transform(vR2, vTmp41);
			GPL2.Transform(vR2, vTmp42);
			double det2 = (vR2.Dot(vTmp41)) * (vR2.Dot(vTmp42));

			if (det1 < 0)
			{
				vecTmp.push_back(Intsct(this, m_nLabel, false, 1 / r1));
			}
			if (det2 < 0)
			{
				vecTmp.push_back(Intsct(this, m_nLabel, false, 1 / r2));
			}
		}
		else
		{
			vecTmp.push_back(Intsct(this, m_nLabel, false, 1 / r1));
			vecTmp.push_back(Intsct(this, m_nLabel, false, 1 / r2));
		}
	}
	else if (det0 == 0)
	{
		r1 = -1 * 2 * b.Dot(vRay) / (2 * c);
		
		// clipping plane test
		if (m_bIsClipped == true)
		{
			bmu::Vector4 vR1(vRay.x, vRay.y, 1, r1);
			bmu::Vector4 vTmp41, vTmp42;

			GPL1.Transform(vR1, vTmp41);
			GPL2.Transform(vR1, vTmp42);
			double det1 = (vR1.Dot(vTmp41)) * (vR1.Dot(vTmp42));
			if (det1 < 0)
			{
				vecTmp.push_back(Intsct(this, m_nLabel, true, 1 / r1));
			}
		}
		else
		{
			vecTmp.push_back(Intsct(this, m_nLabel, true, 1 / r1));
		}
	}
	return vecTmp;
}

bool Quadric::InverseTransformation(bmu::Matrix44 &m, bmu::Matrix44 &n)
{	
	bmu::Matrix44 mtxTmp1, mtxTmp2;
	mtxTmp1 = m;
	mtxTmp1.Transpose();
	double subx, suby, subz;
	subx=Det22( mtxTmp1.b.y, mtxTmp1.b.z, mtxTmp1.c.y, mtxTmp1.c.z );
	suby=Det22( mtxTmp1.b.x, mtxTmp1.b.z, mtxTmp1.c.x, mtxTmp1.c.z );
	subz=Det22( mtxTmp1.b.x, mtxTmp1.b.y, mtxTmp1.c.x, mtxTmp1.c.y );
	
	double det = mtxTmp1.a.x * subx - mtxTmp1.a.y * suby + mtxTmp1.a.z * subz; // Determinant
	if(det == 0.0)
	{
		return false;
	}
	det=1.0 / det;		// Inverse of the determinant
	
	mtxTmp2.a.x = subx * det;
	mtxTmp2.b.x = -suby * det;
	mtxTmp2.c.x = subz * det;
	mtxTmp2.d.x = -(mtxTmp1.d.x * mtxTmp2.a.x + mtxTmp1.d.y * mtxTmp2.b.x + mtxTmp1.d.z * mtxTmp2.c.x);
	
	subx = Det22( mtxTmp1.a.y, mtxTmp1.a.z, mtxTmp1.c.y, mtxTmp1.c.z );
	suby = Det22( mtxTmp1.a.x, mtxTmp1.a.z, mtxTmp1.c.x, mtxTmp1.c.z );
	subz = Det22( mtxTmp1.a.x, mtxTmp1.a.y, mtxTmp1.c.x, mtxTmp1.c.y );
	
	mtxTmp2.a.y = -subx * det;
	mtxTmp2.b.y = suby * det;
	mtxTmp2.c.y = -subz * det;
	mtxTmp2.d.y = -(mtxTmp1.d.x * mtxTmp2.a.y + mtxTmp1.d.y * mtxTmp2.b.y + mtxTmp1.d.z * mtxTmp2.c.y);
	
	subx = Det22( mtxTmp1.a.y, mtxTmp1.a.z, mtxTmp1.b.y, mtxTmp1.b.z );
	suby = Det22( mtxTmp1.a.x, mtxTmp1.a.z, mtxTmp1.b.x, mtxTmp1.b.z );
	subz = Det22( mtxTmp1.a.x, mtxTmp1.a.y, mtxTmp1.b.x, mtxTmp1.b.y );
	
	mtxTmp2.a.z = subx * det;
	mtxTmp2.b.z = -suby * det;
	mtxTmp2.c.z = subz * det;
	mtxTmp2.d.z = -(mtxTmp1.d.x * mtxTmp2.a.z + mtxTmp1.d.y * mtxTmp2.b.z + mtxTmp1.d.z * mtxTmp2.c.z);
	
	mtxTmp2.Transpose();
	mtxTmp2.d.x = mtxTmp2.d.y = mtxTmp2.d.z = 0;
	mtxTmp2.d.w = 1;
	n = mtxTmp2;

	return true;
}

void Quadric::DrawOBB(GLUquadricObj *quad_obj)
{
	// show the bounding vertices
#ifdef HTOUCH_DEBUG
	for (int i = 0; i < 8; i++)
	{
		glPushMatrix();
		glTranslatef(m_vOBBG[i].x, m_vOBBG[i].y, m_vOBBG[i].z);
		gluSphere(quad_obj, 0.1, 20, 20);
		glPopMatrix();
	}
#endif
}

Cylinder::Cylinder(bmu::Vector3 p, double a, double b, double h)
{
	m_bIsClipped = true;
	m_vTranslate = p;
	m_fRadiusX = a;
	m_fRadiusZ = b;
	m_fHeight = h;
	Q = bmu::Matrix44(1 / (a * a), 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 1 / (b * b), 0,
		0, 0, 0, -1);
	PL1 = bmu::Matrix44(0, 0, 0, 0,
		0, 0, 0, 0.5,
		0, 0, 0, 0,
		0, 0.5, 0, -h);
	PL2 = bmu::Matrix44(0, 0, 0, 0,
		0, 0, 0, 0.5,
		0, 0, 0, 0,
		0, 0.5, 0, 0);

	// obtain the bounding box, from min x-y-z to max x-y-z
	// increase by the order of x-z-y
	m_vOBB[0] = bmu::Vector4(-a, 0, -b, 1);
	m_vOBB[1] = bmu::Vector4(a, 0, -b, 1);
	m_vOBB[2] = bmu::Vector4(-a, 0, b, 1);
	m_vOBB[3] = bmu::Vector4(a, 0, b, 1);
	m_vOBB[4] = bmu::Vector4(-a, h, -b, 1);
	m_vOBB[5] = bmu::Vector4(a, h, -b, 1);
	m_vOBB[6] = bmu::Vector4(-a, h, b, 1);
	m_vOBB[7] = bmu::Vector4(a, h, b, 1);
}

void Cylinder::Draw(GLUquadricObj *quad_obj)
{
	glPushMatrix();
	glLoadMatrixd(m_pModelView);
	glTranslatef(0, 1.0 * m_fHeight / 2, 0);
	glScalef(m_fRadiusX, m_fHeight, m_fRadiusZ);
	glRotatef(90, -1, 0, 0);
	glTranslatef(0, 0, -0.5);
	gluCylinder(quad_obj, 1, 1, 1, 20, 20);
	glPopMatrix();

	DrawOBB(quad_obj);
}

Cone::Cone(bmu::Vector3 p, double rb, double rt, double h)
{
	double s, y0;
	m_bIsClipped = true;
	m_vTranslate = p;
	m_fRadiusBottom = rb;
	m_fRadiusTop = rt;
	m_fHeight = h;
	s = fabs(rt - rb) / h;

	if (rb > rt)
		y0 = rb / s;
	else
		y0 = -rb / s;

	Q = bmu::Matrix44(1, 0, 0, 0,
		0, -1 * s * s, 0, s * s * y0,
		0, 0, 1, 0,
		0, s * s * y0, 0, -1 * s * s * y0 * y0);
	PL1 = bmu::Matrix44(0, 0, 0, 0,
		0, 0, 0, 0.5,
		0, 0, 0, 0,
		0, 0.5, 0, -h);
	PL2 = bmu::Matrix44(0, 0, 0, 0,
		0, 0, 0, 0.5,
		0, 0, 0, 0,
		0, 0.5, 0, 0);

	// obtain the bounding box, from min x-y-z to max x-y-z
	// increase by the order of x-z-y
	// the OBB for a cone is defined by the maximum of the radius
	// of the top surface and the bottom surface
	double fOBBR = max(rb, rt);
	m_vOBB[0] = bmu::Vector4(-fOBBR, 0, -fOBBR, 1);
	m_vOBB[1] = bmu::Vector4(fOBBR, 0, -fOBBR, 1);
	m_vOBB[2] = bmu::Vector4(-fOBBR, 0, fOBBR, 1);
	m_vOBB[3] = bmu::Vector4(fOBBR, 0, fOBBR, 1);
	m_vOBB[4] = bmu::Vector4(-fOBBR, h, -fOBBR, 1);
	m_vOBB[5] = bmu::Vector4(fOBBR, h, -fOBBR, 1);
	m_vOBB[6] = bmu::Vector4(-fOBBR, h, fOBBR, 1);
	m_vOBB[7] = bmu::Vector4(fOBBR, h, fOBBR, 1);
}

void Cone::Draw(GLUquadricObj *quad_obj)
{
	glPushMatrix();
	glLoadMatrixd(m_pModelView);
	glTranslatef(0, 1.0 * m_fHeight / 2, 0);
	glRotatef(90, -1, 0, 0);
	glTranslatef(0, 0, -1.0 * m_fHeight / 2);
	gluCylinder(quad_obj, m_fRadiusBottom, m_fRadiusTop, m_fHeight, 20, 20);
	glPopMatrix();

	DrawOBB(quad_obj);
}

Ellipsoid::Ellipsoid(bmu::Vector3 p, double rx, double ry, double rz)
{
	m_bIsClipped = false;
	m_vTranslate = p;
	m_fRadiusX = rx;
	m_fRadiusY = ry;
	m_fRadiusZ = rz;
	Q = bmu::Matrix44(1 / (rx * rx), 0, 0, 0,
		0, 1 / (ry * ry), 0, 0,
		0, 0, 1 / (rz * rz), 0,
		0, 0, 0, -1);

	// obtain the bounding box, from min x-y-z to max x-y-z
	// increase by the order of x-z-y
	m_vOBB[0] = bmu::Vector4(-rx, -ry, -rz, 1);
	m_vOBB[1] = bmu::Vector4(rx, -ry, -rz, 1);
	m_vOBB[2] = bmu::Vector4(-rx, -ry, rz, 1);
	m_vOBB[3] = bmu::Vector4(rx, -ry, rz, 1);
	m_vOBB[4] = bmu::Vector4(-rx, ry, -rz, 1);
	m_vOBB[5] = bmu::Vector4(rx, ry, -rz, 1);
	m_vOBB[6] = bmu::Vector4(-rx, ry, rz, 1);
	m_vOBB[7] = bmu::Vector4(rx, ry, rz, 1);
}

void Ellipsoid::Draw(GLUquadricObj *quad_obj)
{
	glPushMatrix();
	glLoadMatrixd(m_pModelView);	
	glScalef(m_fRadiusX, m_fRadiusY, m_fRadiusZ);
	gluSphere(quad_obj, 1, 20, 20);
	glPopMatrix();

	DrawOBB(quad_obj);
}

Sphere::Sphere(bmu::Vector3 p, double r)
{
	m_bIsClipped = false;
	m_vTranslate = p;
	m_fRadius = r;
	Q = bmu::Matrix44(1 / (r * r), 0, 0, 0,
		0, 1 / (r * r), 0, 0,
		0, 0, 1 / (r * r), 0,
		0, 0, 0, -1);

	// obtain the bounding box, from min x-y-z to max x-y-z
	// increase by the order of x-z-y
	m_vOBB[0] = bmu::Vector4(-r, -r, -r, 1);
	m_vOBB[1] = bmu::Vector4(r, -r, -r, 1);
	m_vOBB[2] = bmu::Vector4(-r, -r, r, 1);
	m_vOBB[3] = bmu::Vector4(r, -r, r, 1);
	m_vOBB[4] = bmu::Vector4(-r, r, -r, 1);
	m_vOBB[5] = bmu::Vector4(r, r, -r, 1);
	m_vOBB[6] = bmu::Vector4(-r, r, r, 1);
	m_vOBB[7] = bmu::Vector4(r, r, r, 1);
}

void Sphere::Draw(GLUquadricObj *quad_obj)
{
	glPushMatrix();
	glLoadMatrixd(m_pModelView);	
	glScalef(m_fRadius, m_fRadius, m_fRadius);
	gluSphere(quad_obj, 1, 20, 20);
	glPopMatrix();

	DrawOBB(quad_obj);
}

string HandElem::s_strOffset = "offset";
string HandElem::s_strRotMin = "rotmin";
string HandElem::s_strRotMax = "rotmax";
string HandElem::s_strPose = "pose";
string HandElem::s_strScale = "scale";
string HandElem::s_strCylinder = "cylinder";
string HandElem::s_strCone = "cone";
string HandElem::s_strEllipsoid = "ellipsoid";
string HandElem::s_strSphere = "sphere";
string HandElem::s_strElem = "elem";
string HandElem::s_strLabel = "label";

HandElem::HandElem(string strName)
{
	m_strName = strName;
	m_vScale.Set(1, 1, 1);
	m_vecQuadrics.clear();
}

HandElem::~HandElem()
{
	for (vector<Quadric *>::const_iterator it = m_vecQuadrics.begin();
		it != m_vecQuadrics.end(); it++)
	{
		delete *it;
	}
}

HandElem::HandElem(const HandElem &rj)
{
	m_strName = rj.m_strName;
	m_vOffset = rj.m_vOffset;
	m_vRotMin = rj.m_vRotMin;
	m_vRotMax = rj.m_vRotMax;
	m_vPose = rj.m_vPose;
	m_vScale = rj.m_vScale;
	m_vecQuadrics = rj.m_vecQuadrics;
}

HandElem& HandElem::operator=(const HandElem &rj)
{
	if (this != &rj)
	{
		m_strName = rj.m_strName;
		m_vOffset = rj.m_vOffset;
		m_vRotMin = rj.m_vRotMin;
		m_vRotMax = rj.m_vRotMax;
		m_vPose = rj.m_vPose;
		m_vScale = rj.m_vScale;
		m_vecQuadrics = rj.m_vecQuadrics;
	}
	return *this;
}

bool HandElem::operator==(const HandElem &rj)	
{
	if (m_strName == rj.m_strName &&
		m_vOffset == rj.m_vOffset &&
		m_vRotMin == rj.m_vRotMin &&
		m_vRotMax == rj.m_vRotMax &&
		m_vPose == rj.m_vPose &&
		m_vScale == rj.m_vScale &&
		m_vecQuadrics == rj.m_vecQuadrics)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool HandElem::operator!=(const HandElem &rj)
{
	return !(operator==(rj));
}

void HandElem::ReadInfo(Tokenizer &sfp)
{
	double x, y, z;

	string strTokenName;
	while (((strTokenName = sfp.CheckToken()) != s_strElem) &&
		strTokenName != "}")
	{
		// get the token name and change to lower case
		strTokenName = sfp.GetToken();
		transform(strTokenName.begin(), strTokenName.end(), strTokenName.begin(), tolower);
		
		//!! the format of the model file must ensure that the label is read first
		if (strTokenName == s_strLabel)
		{
			int nLabel = (int)(sfp.GetDouble());
			m_nLabel = nLabel;
		}
		else if (strTokenName == s_strOffset)
		{			
			x = sfp.GetDouble();
			y = sfp.GetDouble();
			z = sfp.GetDouble();
			m_vOffset.Set(x, y, z);
		}
		else if (strTokenName == s_strRotMin)
		{
			x = sfp.GetDouble();
			y = sfp.GetDouble();
			z = sfp.GetDouble();
			m_vRotMin.Set(x, y, z);
		}
		else if (strTokenName == s_strRotMax)
		{
			x = sfp.GetDouble();
			y = sfp.GetDouble();
			z = sfp.GetDouble();
			m_vRotMax.Set(x, y, z);
		}
		else if (strTokenName == s_strPose)
		{
			x = sfp.GetDouble();
			y = sfp.GetDouble();
			z = sfp.GetDouble();
			m_vPose.Set(x, y, z);
		}
		else if (strTokenName == s_strScale)
		{
			x = sfp.GetDouble();
			y = sfp.GetDouble();
			z = sfp.GetDouble();
			m_vScale.Set(x, y, z);
		}
		else if (strTokenName == s_strCylinder)
		{
			x = sfp.GetDouble();
			y = sfp.GetDouble();
			z = sfp.GetDouble();
			bmu::Vector3 p(x, y, z);
			double a, b, h;
			a = sfp.GetDouble();
			b = sfp.GetDouble();
			h = sfp.GetDouble();

			Quadric *pQdc = new Cylinder(p, a, b, h);
			pQdc->SetLabel(m_nLabel);
			m_vecQuadrics.push_back(pQdc);
		}
		else if (strTokenName == s_strCone)
		{
			x = sfp.GetDouble();
			y = sfp.GetDouble();
			z = sfp.GetDouble();
			bmu::Vector3 p(x, y, z);
			double rb, rt, h;
			rb = sfp.GetDouble();
			rt = sfp.GetDouble();
			h = sfp.GetDouble();

			Quadric *pQdc = new Cone(p, rb, rt, h);
			pQdc->SetLabel(m_nLabel);
			m_vecQuadrics.push_back(pQdc);
		}
		else if (strTokenName == s_strEllipsoid)
		{
			x = sfp.GetDouble();
			y = sfp.GetDouble();
			z = sfp.GetDouble();
			bmu::Vector3 p(x, y, z);
			double rx, ry, rz;
			rx = sfp.GetDouble();
			ry = sfp.GetDouble();
			rz = sfp.GetDouble();

			Quadric *pQdc = new Ellipsoid(p, rx, ry, rz);
			pQdc->SetLabel(m_nLabel);
			m_vecQuadrics.push_back(pQdc);
		}
		else if (strTokenName == s_strSphere)
		{
			x = sfp.GetDouble();
			y = sfp.GetDouble();
			z = sfp.GetDouble();
			bmu::Vector3 p(x, y, z);
			double r;
			r = sfp.GetDouble();

			Quadric *pQdc = new Sphere(p, r);
			pQdc->SetLabel(m_nLabel);
			m_vecQuadrics.push_back(pQdc);
		}
	}
}

void HandElem::Draw(GLUquadricObj *quad_obj)
{
	for (vector<Quadric *>::const_iterator it = m_vecQuadrics.begin();
		it != m_vecQuadrics.end(); it++)
	{
		(*it)->Draw(quad_obj);
	}
}

void HandElem::Update(void)
{
	glTranslatef(m_vOffset.x, m_vOffset.y, m_vOffset.z);
	glRotatef(m_vPose.x * RADTODEG, 1, 0, 0);
	glRotatef(m_vPose.y * RADTODEG, 0, 1, 0);
	glRotatef(m_vPose.z * RADTODEG, 0, 0, 1);

	for (vector<Quadric *>::const_iterator it = m_vecQuadrics.begin();
		it != m_vecQuadrics.end(); it++)
	{
		glPushMatrix();
		bmu::Vector3 p = (*it)->GetTranslate();
		glTranslatef(p.x, p.y, p.z);
		GLdouble arrModelView[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, arrModelView);
		(*it)->SetModelViewMtx(arrModelView);
		glPopMatrix();
	}
}

void HandElem::PrintInfo(void)
{
	cout << m_strName << endl;
}

void HandElem::Clip3(void)
{
	Clip(m_vRotMin.x, m_vRotMax.x, m_vPose.x);
	Clip(m_vRotMin.y, m_vRotMax.y, m_vPose.y);
	Clip(m_vRotMin.z, m_vRotMax.z, m_vPose.z);
}
