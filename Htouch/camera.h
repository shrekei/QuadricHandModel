#pragma once

class Camera  
{
public:
	Camera();
	virtual ~Camera();
	
public:
	void Init(void);
	void Reset(void);
	void Draw(void);
	void Update(void);
	
	// Access functions
	void SetAspect(double a)		{Aspect=a;}
	void SetDistance(double d)	{Distance=d;}
	void SetAzimuth(double a)	{Azimuth=a;}
	void SetIncline(double i)	{Incline=i;}
	void SetTranslation(double px, double py) {x += px; y += py;}
	
	double GetDistance()			{return Distance;}
	double GetAzimuth()			{return Azimuth;}
	double GetIncline()			{return Incline;}
	
private:
	// Perspective controls
	double FOV;		// Field of View Angle
	double Aspect;	// Aspect Ratio
	double NearClip;	// Near clipping plane distance
	double FarClip;	// Far clipping plane distance
	
	// Polar controls
	double Distance;	// Distance of the camera eye position to the origin
	double Azimuth;	// Rotation of the camera eye position around the Y axis
	double Incline;	// Angle of the camera eye position over the XZ plane
	double x, y;
};