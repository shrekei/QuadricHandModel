#pragma once
#include "Global.h"
#include "Hand.h"
#include <cxcore.h>
#include <highgui.h>
#include <cv.h>
#include "../MathUtilities/Vector3.h"

class Pixel
{
public:
	Pixel(void);
	Pixel(cv::Point2f  ptCenter);
	
public:
	cv::Point2f		Center;			// 2D center in the image plane
	Intsct			NearIntsct;		// nearest intersection point
	bmu::Vector3	Norm;			
	bool			IsForegrd;		// foreground indicator
	bool			IsContour;		// contour point indicator
	bool			IsSample;		// indicator to sparsely sample the whole contour
	double			Depth;			// depth value
};

////////////////////////////////////////////////////////////////////////////////
//******************************HandProjector.h *********************************//
// The image plane manages a 2D array of pixels. In the 3D hand model projection 
// procedure, it stores the contours, silhouette and depth for the sampling points
// on the 3D hand model. We assume that the image plane is in a normalized camera
// , and this means it is located at z = 1; and the focal point is located at the
// origin
////////////////////////////////////////////////////////////////////////////////
class HandProjector
{
	friend void OnMouse( int event, int x, int y, int flags, void* param );
public:
	HandProjector(void);
	~HandProjector(void);
	void Init(void);
	void Reset(void);
	void SetTSize(cv::Size sPanel);
	Pixel & operator() (int y, int x);
	cv::Mat&	GetContour(void)		{ return m_mtxContour; }
	cv::Mat&	GetSilhouette(void)		{ return m_mtxSilhouette; }
	cv::Mat&	GetDepthImg(void)		{ return m_mtxDepthImg; }
	void		GetMinMaxDepth(double &fMin, double&fMax) {fMin = m_fMinDepth; fMax = m_fMaxDepth; }

public:
	void CalcProjection(Hand* pHand);
	void CalcIntscts(Hand *pHand);

private:
	void GenerateContour(void);

private:
	Pixel **	m_pHandProjector;
	cv::Size	m_sPanel;
	double		m_fDx, m_fDy;

private:
	cv::Mat		m_mtxContour;
	cv::Mat		m_mtxSilhouette;
	cv::Mat		m_mtxDepthImg;
	double		m_fMinDepth, m_fMaxDepth;
	std::vector<cv::Point>	m_vecEdgePoints;
};
