// Camera.cpp: implementation of the Camera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Htouch.h"
#include "Camera.h"

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glut.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Camera::Camera()
{
	Reset();
}

Camera::~Camera()
{

}

void Camera::Update() 
{
}

void Camera::Init()
{
	FOV=70.0f;
	Aspect=1.33f;
	NearClip=0.1f;
	FarClip=1000.0f;

	Distance=100.0f;
	Azimuth=0.0f;
	Incline=0.0f;
	x = y = 0;
}

void Camera::Reset()
{
	FOV=0.0f;
	Aspect=0.0f;
	NearClip=0.0f;
	FarClip=0.0f;

	Distance=0.0f;
	Azimuth=0.0f;
	Incline=0.0f;
	x = y = 0;
}

void Camera::Draw()
{
	// Tell GL we are going to adjust the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set perspective projection
	gluPerspective(FOV, Aspect, NearClip, FarClip);

	// Place Camera
	glTranslatef(x, y, -Distance);
	glRotatef(Incline,1.0f,0.0f,0.0f);
	glRotatef(Azimuth,0.0f,1.0f,0.0f);

	// Return to modelview matrix mode
	glMatrixMode(GL_MODELVIEW);
}
